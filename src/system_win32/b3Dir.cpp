/*
**
**	$Filename:	b3Dir.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - directory routines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3SystemInclude.h"
#include "blz3/system/b3Dir.h"
#include <sys/types.h>
#include <sys/stat.h>	
#include <io.h>
#include <direct.h>

/*************************************************************************
**                                                                      **
**                        b3Dir implementation                          **
**                                                                      **
*************************************************************************/

b3_path_type b3Dir::b3Exists (const char *Name)
{
	struct _stat status;
	b3Path       aux;

	if ((strlen(Name) == 2) && (Name[1] == ':'))
	{
		aux.b3Format("%s\\",Name);
		Name = aux;
	}

	if (_stat(Name,&status) != 0)
	{
		return B3_NOT_EXISTANT;
	}

	if (S_ISDIR(status.st_mode))
	{
		return B3_TYPE_DIR;
	}
	return B3_TYPE_FILE;
}

b3_bool b3Dir::b3ChDir (const char *Name)
{
	return (chdir(Name)==0);
}

b3_bool b3Dir::b3ChkDir (const char *Name)
{
	return b3Exists(Name) == B3_TYPE_DIR;
}

b3_bool b3Dir::b3MkDir (const char *Name)
{
	b3_bool result;

	if (b3ChkDir(Name))
	{
		return true;
	}
	result = (_mkdir(Name) == 0);
	if (result)
	{
/*
		BMode (Name,
			S_IRUSR|S_IWUSR|S_IXUSR|
			S_IRGRP|S_IXGRP|
			S_IROTH|S_IXOTH);
*/
//		b3PrintF (B3LOG_FULL,"### CLASS: b3Dir  # b3MkDir(%s)\n",Name);
	}
	return result;
}

void b3Dir::b3CreateBasePath(
	const char *base,
	const char *dir)
{
	b3Path parent;

	if (strcmp(base,dir) != 0)
	{
		b3ParentName     (dir, parent);
		b3CreateBasePath (base,parent);
	}
	b3MkDir (dir);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Dir  # b3MkDir(%s)\n",dir);
}

void b3Dir::b3CreateFullPath(const char *dir)
{
	b3Path parent;

	if (strlen(dir) > 3)
	{
		b3ParentName (dir, parent);
		b3CreateFullPath (parent);
	}
	b3MkDir (dir);
	b3PrintF (B3LOG_FULL,"### CLASS: b3Dir  # b3MkDir(%s)\n",dir);
}

// Be sure that a path exists. If many diorectory
// instances are missing create the full path!
b3_bool b3Dir::b3CreatePath(
	const char *base,
	const char *dir)
{
	b3_size  len;
	b3_bool  result = false;

	if (base != null)
	{
		len = strlen(base);
		if (strncmp(base,dir,len) == 0)
		{
			try
			{
				b3CreateBasePath(base,dir);
				result = true;
			}
			catch(...)
			{
				b3PrintF (B3LOG_FULL,
					"### CLASS: b3Dir  # "
					"Error in b3CreateBasePath(%s,%s);\n",
					base,dir);
			}
		}
		else
		{
			b3PrintF (B3LOG_FULL,
				"### CLASS: b3Dir  # "
				"Error in b3CreatePath(%s,%s) - dir is not subdir of base;\n",
				base,dir);
		}
	}
	else
	{
		try
		{
			b3CreateFullPath(dir);
			result = true;
		}
		catch(...)
		{
				b3PrintF (B3LOG_FULL,
					"### CLASS: b3Dir  # "
					"Error in b3CreateFullPath(s);\n",
					dir);
		}
	}
	return result;
}

/*************************************************************************
**                                                                      **
**                        routines                                      **
**                                                                      **
*************************************************************************/

b3DirSystem::b3DirSystem()
{
	handle = INVALID_HANDLE_VALUE;
}

b3Dir::b3Dir()
{
	type   = B3_NOT_EXISTANT;
	b3Empty();
}

// This constructor opens a dir entry directly when
// instantiated
b3Dir::b3Dir (const char *path)
{
	if (!b3OpenDir(path))
	{
		B3_THROW(b3DirException,B3_DIR_NOT_FOUND);
	}
}

// The destructor deinitializes the dir entry if necessary
b3Dir::~b3Dir()
{
	b3CloseDir();
}

// Open a directory list
b3_bool b3Dir::b3OpenDir (const char *path)
{
	b3Path name;

	b3LinkFileName (name,path,"*.*");
	handle = FindFirstFile (name,&entry);
	if (handle != INVALID_HANDLE_VALUE)
	{
		if (entry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			type = B3_TYPE_DIR;
		}
		else 
		{
			type = B3_TYPE_FILE;
		}
	}
	else
	{
		type = B3_NOT_EXISTANT;
	}
	return (handle != INVALID_HANDLE_VALUE);
}

b3_path_type b3Dir::b3DirNext (char *name)
{
	b3_path_type local_type;
	b3_bool loop;

	do
	{
		loop = false;
		local_type = type;
		if (local_type != B3_NOT_EXISTANT)
		{
			// remove "." and ".." (means keep in loop)
			// These directories exists in every case
			if (local_type == B3_TYPE_DIR)
			{
				if (strcmp(entry.cFileName,"..") == 0) loop = true;
				if (strcmp(entry.cFileName,".")  == 0) loop = true;
			}
			strcpy (name,entry.cFileName);
			if (FindNextFile (handle,&entry) == 0)
			{
				type = B3_NOT_EXISTANT;
			}
			else
			{
				// Set flag for file/directory entry
				type = entry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ?
					B3_TYPE_DIR : B3_TYPE_FILE;
			}
		}
	}
	while (loop);
	return local_type;
}

void b3Dir::b3CloseDir ()
{
	if (handle != INVALID_HANDLE_VALUE)
	{
		FindClose (handle);
		handle = INVALID_HANDLE_VALUE;
	}
}

/*************************************************************************
**                                                                      **
**                        filename handling routines                    **
**                                                                      **
*************************************************************************/

void b3Path::b3Empty()
{
	m_Path[0] = 0;
}

// Non static one...
void b3Path::b3Correct()
{
	b3Correct(m_Path);
}

// Static input=output
void b3Path::b3Correct(char *input)
{
	b3Correct(input,input);
}

// Non static one...
void b3Path::b3Correct(const char *input)
{
	b3Correct(input,m_Path);
}

// Static filename correction
void b3Path::b3Correct(const char *input,char *output)
{
	b3_size i,len;

	B3_ASSERT(input != null);
	B3_ASSERT(output != null);
	len = strlen(input);
	for (i = 0;i < len;i++)
	{
		output[i] = (input[i] == '/' ? '\\' : input[i]);
	}
	output[len] = 0;
}

// Non static one...
void b3Path::b3LinkFileName(
	const char *param_path,
	const char *param_name)
{
	b3LinkFileName(m_Path,param_path,param_name);
}

// link a name to an existing file path to create a
// new full qualified filepath
void b3Path::b3LinkFileName(
	      char *full,
	const char *path,
	const char *name)
{
	char    nFullPath[_MAX_DIR];
	b3_size i,len;

	B3_ASSERT(full != null);

#if 0
	char    nDrive[_MAX_DRIVE];
	char    nPathOfPath[_MAX_DIR];
	char    nNameOfPath[_MAX_FNAME + _MAX_EXT];
	char    nExtOfPath[_MAX_EXT];
	char    nPathOfName[_MAX_DIR];
	char    nNameOfName[_MAX_FNAME + _MAX_EXT];
	char    nExtOfName[_MAX_EXT];

	nFullPath[0] = 0;
	if (path != null)
	{
		_splitpath (path,nDrive,nPathOfPath,nNameOfPath,nExtOfPath);
		strncat(nNameOfPath,nExtOfPath, sizeof(nNameOfPath));
	}
	else
	{
		nNameOfPath[0] = 0;
	}

	if (name != null)
	{
		_splitpath (name,null,nPathOfName,nNameOfName,nExtOfName);
		strncat(nNameOfName,nExtOfName, sizeof(nNameOfName));
	}

	if (strlen(nPathOfName) > 0)
	{
		snprintf(nFullPath,sizeof(nFullPath),"%s%s\\%s%s",nPathOfPath,nNameOfPath,nPathOfName);
	}
	else
	{
		snprintf(nFullPath,sizeof(nFullPath),"%s%s",nPathOfPath,nNameOfPath);
	}

	_makepath (full,nDrive,nFullPath,nNameOfName,"");
#else
	if (path != null)
	{
		if (name != null)
		{
			snprintf(nFullPath, sizeof(nFullPath), "%s\\%s", path, name);
		}
		else
		{
			strncpy(nFullPath, path, sizeof(nFullPath));
		}
	}
	else
	{
		nFullPath[0] = 0;
	}
	strcpy (full, nFullPath);
#endif
	b3RemoveDelimiter(full);

	// convert '/' to '\'
	len = strlen(full);
	for (i = 0;i < len;i++)
	{
		if (full[i] == '/')
		{
			full[i] = '\\';
		}
	}
}

// Non static one...
void b3Path::b3SplitFileName(
	char *param_path,
	char *param_name)
{
	b3SplitFileName(m_Path,param_path,param_name);
}

// This routine splits a full qualified filename into
// 1. directory name in "path"
// 2a to a filename in "name" if "uncorrected" is a file
// 2b to a filename of zero length if "uncorrected" is a directory
void b3Path::b3SplitFileName(
	const char *uncorrected,
	      char *path,
	      char *name)
{
	char full[B3_FILESTRINGLEN];
	char nDrive[_MAX_DRIVE];
	char nPath[_MAX_DIR];
	char nName[_MAX_FNAME];
	char nExt[_MAX_EXT];
#ifdef CONVERT_SLASH
	long i;
#endif

	if (uncorrected == null)
	{
		return;
	}

		/* convert '/' to '\' */
#ifdef CONVERT_SLASH
	for (i=0;i<strlen(uncorrected);i++)
	{
		if (uncorrected[i] == '/') full[i] = '\\';
		else full[i] = uncorrected[i];
	}
	full[i] = 0;
#else
	strcpy (full,uncorrected);
#endif

		/* "full" is already directory */
	if (b3Dir::b3Exists(full) == B3_TYPE_DIR)
	{
		if (path) strcpy(path,full);
		if (name) name[0] = 0;
		return;
	}

		/* split into path and file */
	_splitpath (full,nDrive,nPath,nName,nExt);
	if (path != null)
	{
		snprintf (path,B3_FILESTRINGLEN,"%s%s",nDrive,nPath);
		b3RemoveDelimiter(path);
	}
	if (name != null)
	{
		snprintf (name,B3_FILESTRINGLEN,"%s%s",nName,nExt);
	}
}

// Non static one...
void b3Path::b3ParentName()
{
	b3ParentName(m_Path);
}

// Source and destination are the same (static one)
void b3Path::b3ParentName(char *param_path)
{
	b3ParentName(param_path,param_path);
}

// Static one...
void b3Path::b3ParentName(const char *param_path)
{
	b3ParentName(param_path,m_Path);
}

// Get the parent directory of a directory or file
void b3Path::b3ParentName(
	const char *file,
	      char *parent)
{
	char     actDir[B3_FILESTRINGLEN];
	b3_size  i, len;

	ASSERT((file != null) && (parent != null));

	// Source is a file so split directory first
	if (b3Dir::b3Exists(file) == B3_TYPE_FILE)
	{
		b3SplitFileName (file,actDir,null);
	}
	else
	{
		strcpy (actDir,file);
	}

	// Remove trailing "/" or "\"
	len = strlen(actDir);
	for (i = len - 1;i >= 0;i--) switch(actDir[i])
	{
		case '/' :
		case '\\' :
			actDir[i] = 0;
		case ':' :
			i = 0;
			break;

		default :
			actDir[i] = 0;
			break;
	}

	// If parent is only a drive letter append trailing "\"
	if ((strlen(actDir) == 2) && (actDir[1] == ':'))
	{
		actDir[2] = '\\';
		actDir[3] = 0;
	}

	// Copy to destination if available else
	// overwrite source
	strcpy (parent != null ? parent : (char *)file,actDir);
}

// Non static one...
void b3Path::b3RemoveExt()
{
	b3RemoveExt(m_Path);
}

// Static one
void b3Path::b3RemoveExt(char *input)
{
	b3RemoveExt(input,input);
}

// Non static one...
void b3Path::b3RemoveExt(const char *input)
{
	b3RemoveExt(input,m_Path);
}

// Remove extension of a file. This routine is needed
// for creating a new extension.
void b3Path::b3RemoveExt(const char *name,char *output)
{
	char    actPath[B3_FILESTRINGLEN];
	char    actName[B3_FILESTRINGLEN];
	b3_size i = 0;

	ASSERT((name != null) && (output != null));
	b3SplitFileName (name,actPath,actName);
	while(actName[i] != 0)
	{
		if (actName[i] == '.')
		{
			actName[i] = 0;
		}
		else
		{
			i++;
		}
	}
	b3LinkFileName (output,actPath,actName);
}

// Non static one...
void b3Path::b3ExtractExt()
{
	b3ExtractExt(m_Path);
}

// Static one
void b3Path::b3ExtractExt(char *input)
{
	b3ExtractExt(input,input);
}

// Non static one...
void b3Path::b3ExtractExt(const char *input)
{
	b3ExtractExt(input,m_Path);
}

// Extract an extension from a filename;
void b3Path::b3ExtractExt(const char *filename,char *ext)
{
	char     actName[B3_FILESTRINGLEN];
	b3_size  i, len;

	b3SplitFileName (filename,null,actName);
	len = strlen(actName);
	for (i = len - 1;i >= 0;i--)
	{
		if (actName[i] == '.')
		{
			strcpy(ext,&actName[i+1]);
			return;
		}
	}
	ext[0] = 0;
}

void b3Path::b3RemoveDelimiter(char *name)
{
	b3_offset i, len;

	len = (b3_offset)strlen(name);
	for (i = len - 1;i >= 0;i--)
	{
		if ((name[i] == '/') || (name[i] == '\\')) len--;
		else i = -1;
	}
	name[len] = 0;
}

void b3Path::b3Format(const char *format,...)
{
	va_list  argptr;

	va_start (argptr,format);
	vsnprintf(m_Path,sizeof(m_Path),format,argptr);
	va_end   (argptr);
}

void b3Path::b3Append(const char *ext)
{
	b3_size i,k=0;

	for (i = strlen(m_Path);i < sizeof(m_Path);i++)
	{
		m_Path[i] = ext[k];
		if (ext[k] == 0)
		{
			return;
		}
		k++;
	}
}

/*************************************************************************
**                                                                      **
**                        file dialog routines                          **
**                                                                      **
*************************************************************************/

b3_bool b3Path::b3FileDialog(
	const char *default_name,
	const char *default_ext,
	const char *file_filter,
	char       *file_name,
	bool        is_open)
{
	char      aux[B3_FILESTRINGLEN];
	char      suggest[B3_FILESTRINGLEN];
	b3_bool   result;
	b3_size   i, len;
	CWinApp  *app = AfxGetApp();

	// Make filename ready for use...
	len = strlen(default_name);
	if (len >= sizeof(aux))
	{
		len = sizeof(aux) - 1;
	}
	for (i = 0;i < len;i++)
	{
		aux[i] = (default_name[i] == '/' ? '\\' : default_name[i]);
	}
	aux[len] = 0;

	if (b3Dir::b3Exists(aux) == B3_TYPE_DIR)
	{
		// Suggest directory pattern
		snprintf(suggest,sizeof(suggest),"%s\\*.*",aux);
	}
	else
	{
		strcpy(suggest,aux);
	}

	CFileDialog   filedlg(
		is_open, // Use file save dialog
		default_ext, // default extension
		suggest, // file name suggestion to save
		is_open ? OFN_HIDEREADONLY : OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,	// flags
		file_filter, // File extension filter
		app->m_pMainWnd); // parent

	// Ask for name...
	result = (filedlg.DoModal() == IDOK);
	if (result)
	{
		strcpy(file_name,filedlg.GetPathName());
	}
#if 0
	else
	{
		int error = CommDlgExtendedError();
	}
#endif
	return result;
}

b3_bool b3Path::b3OpenDialog(
	const char *default_name,
	const char *default_ext,
	const char *file_filter,
	char       *file_name)
{
	return b3FileDialog(default_name,default_ext,file_filter,file_name,true);
}

b3_bool b3Path::b3SaveDialog(
	const char *default_name,
	const char *default_ext,
	const char *file_filter,
	char       *file_name)
{
	return b3FileDialog(default_name,default_ext,file_filter,file_name,false);
}

/*************************************************************************
**                                                                      **
**                        b3Folder implementation for                   **
**                        selecting a folder.                           **
**                                                                      **
*************************************************************************/

b3Folder::b3Folder(CWnd *parent)
{
	info.hwndOwner = parent->m_hWnd;
	b3Empty();
}

b3_bool b3Folder::b3SelectFolder(CString &path,const int title_id,const char *root)
{
	CString title;

	title.LoadString(title_id);
	return b3SelectFolder(path,title,root);
}

ITEMIDLIST *b3Folder::b3ConvertItemIDList(const char *szPath)
{
	ITEMIDLIST    *pidl = null;
	LPSHELLFOLDER  pDesktopFolder;
	OLECHAR        olePath[MAX_PATH];
	ULONG          chEaten;
	ULONG          dwAttributes;
	HRESULT        hr;

	if (szPath == null)
	{
		return null;
	}

	//
	// Get a pointer to the Desktop's IShellFolder interface.
	//
	if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	{
		//
		// IShellFolder::ParseDisplayName requires the file name be in
		// Unicode.
		//
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1,
		olePath, MAX_PATH);

		//
		// Convert the path to an ITEMIDLIST.
		//
		hr = pDesktopFolder->ParseDisplayName(
			NULL,
			NULL,
			olePath,
			&chEaten,
			&pidl,
			&dwAttributes);
		if (FAILED(hr))
		{
			// Handle error.
		}

		//
		// pidl now contains a pointer to an ITEMIDLIST for .\readme.txt.
		// This ITEMIDLIST needs to be freed using the IMalloc allocator
		// returned from SHGetMalloc().
		//

		//release the desktop folder object
		pDesktopFolder->Release();
	}
	return pidl;
}
 


b3_bool b3Folder::b3SelectFolder(CString &param,const char *title,const char *root)
{
	ITEMIDLIST *result;
	ITEMIDLIST *root_id;
	LPMALLOC    pMalloc;
	b3_size     i, len;

	b3Format("%s",param);
	len = strlen(m_Path);
	for (i = 0;i < len;i++)
	{
		if (m_Path[i] == '/')
		{
			m_Path[i] = '\\';
		}
	}

	root_id = b3ConvertItemIDList(root);
	info.pidlRoot       = root_id;
	info.pszDisplayName = m_Path;
	info.lpszTitle      = title;
	info.ulFlags        = 0;
	info.lpfn           = b3Callback;
	info.lParam         = (LPARAM)this;
	info.iImage         = 0;
	result = ::SHBrowseForFolder(&info);
	if (result != null)
	{
		if (SUCCEEDED(::SHGetPathFromIDList(result, m_Path)))
		{
			param = m_Path;
		}

		//Retrieve a pointer to the shell's IMalloc interface
		if (SUCCEEDED(::SHGetMalloc(&pMalloc)))
		{
			// free the PIDL that SHBrowseForFolder returned to us.
			pMalloc->Free(result);
			// release the shell's IMalloc interface
			(void)pMalloc->Release();
		}
	}

	if (root_id != null)
	{
		if (SUCCEEDED(::SHGetMalloc(&pMalloc)))
		{
			// free the PIDL that SHBrowseForFolder returned to us.
			pMalloc->Free(root_id);
			// release the shell's IMalloc interface
			(void)pMalloc->Release();
		}
	}
	return result != null;
}

int __stdcall b3Folder::b3Callback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	b3Folder *pbff = (b3Folder *)lpData;
	char     *ptr;

	if (uMsg == BFFM_INITIALIZED)
	{
		ptr = (char *)pbff;
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE,(LPARAM)ptr);
	}

	return 0;
}

