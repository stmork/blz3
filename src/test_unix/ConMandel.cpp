// ConMandel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ConMandel.h"

#include "blz3/b3Main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		char *args[6];

		args[0] = argv[0];
		args[1] = "-1";
		args[2] = "2.2";
		args[3] = "-1";
		args[4] = "1";
		args[5] = "256";

		// TODO: code your application's behavior here.
		nRetCode = _Blizzard3Main(sizeof(args)/sizeof(char *),args);
	}

	return nRetCode;
}


