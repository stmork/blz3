/*
**
**	$Filename:	b3TxSavePS.cc $
**	$Release:	Dortmund 2001 $
**	$Revision:	2.04 
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Saving a PostScript image
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

#include "b3TxSaveInfo.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2005/10/09 12:05:34  sm
**	- Changed to HDR image computation.
**
**	Revision 1.5  2005/06/16 10:15:39  smork
**	- Corrected PostScript handling.
**	
**	Revision 1.4  2005/06/02 07:45:44  smork
**	- Fixed RGB8 image saving in brt3.
**	- Added PostScript image save.
**	
**	Revision 1.3  2001/12/01 17:48:42  sm
**	- Added raytraced image saving
**	- Added texture search path configuration
**	- Always drawing fulcrum and view volume. The
**	  depth buffer problem persists
**	
**	Revision 1.2  2001/11/09 16:15:35  sm
**	- Image file encoder
**	- Performance meter for triangles / second added.
**	- Corrected Windows b3TimeSpan computation
**	
**	Revision 1.1  2001/11/08 19:31:33  sm
**	- Nasty CR/LF removal!
**	- Added TGA/RGB8/PostScript image saving.
**	- Hoping to win Peter H. for powerful MFC programming...
**	
**	
*/

/*************************************************************************
**                                                                      **
**                          PS                                          **
**                                                                      **
*************************************************************************/

class b3InfoEXR : protected b3TxSaveInfo
{
public:
	      b3InfoEXR(b3Tx *tx,const char *filename);
	     ~b3InfoEXR();
	void  b3Write();
};

b3InfoEXR::b3InfoEXR(b3Tx *tx,const char *filename) :
	b3TxSaveInfo(tx,filename,B3_TWRITE)
{
}

void b3InfoEXR::b3Write()
{
}

b3InfoEXR::~b3InfoEXR()
{
}

b3_result b3Tx::b3SaveEXR(const char *filename)
{
	b3PrintF(B3LOG_FULL, "Saving EXR: %s\n", filename);

	b3InfoEXR info(this,filename);
	info.b3Write();
	return B3_OK;
}
