/*
**
**	$Filename:	b3Matrix.h            
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork
**
**	Blizzard III - vector and matrix calculation proto types
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_MATRIX_H
#define B3_BASE_MATRIX_H

#include "blz3/b3Types.h"

b3_f64     b3Length             (b3_vector *vec);
b3_f64     b3Distance           (b3_vector *from,b3_vector *to);
b3_f64     b3Det2               (b3_f64 a,b3_f64 b,b3_f64 c,b3_f64 d);
b3_f64     b3Det3               (b3_vector *left,b3_vector *mid,b3_vector *right);
b3_f64     b3Det4               (b3_matrix *Matrix);
b3_matrix *b3MatrixUnit         (b3_matrix *Dst);
b3_matrix *b3MatrixInv          (b3_matrix *Src,b3_matrix *Dst);
b3_matrix *b3MatrixTrans        (b3_matrix *Src,b3_matrix *Dst);
b3_matrix *b3MatrixMove         (b3_matrix *Src,b3_matrix *Dst,b3_vector *Move);
b3_matrix *b3MatrixMoveNeg      (b3_matrix *Src,b3_matrix *Dst,b3_vector *MoveNeg);
b3_matrix *b3MatrixScale        (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_vector *Scale);
b3_matrix *b3MatrixRotX         (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 angle);
b3_matrix *b3MatrixRotY         (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 angle);
b3_matrix *b3MatrixRotZ         (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 angle);
b3_matrix *b3MatrixMirrorPoint  (b3_matrix *Src,b3_matrix *Dst,b3_vector *Center,b3_f64 scale);
b3_matrix *b3MatrixMirrorAxis   (b3_matrix *Src,b3_matrix *Dst,b3_line   *axis,  b3_f64 scale);
b3_matrix *b3MatrixMirrorPlane  (b3_matrix *Src,b3_matrix *Dst,b3_vector *base,b3_vector *uDir,b3_vector *vDir,b3_f64 scale);
b3_matrix *b3MatrixMMul         (b3_matrix *A,  b3_matrix *B,  b3_matrix *Result);
b3_matrix *b3MatrixSMul         (b3_matrix *Src,b3_matrix *Dst,b3_f64     factor);
b3_vector *b3MatrixVMul         (b3_matrix *Mat,b3_vector *Src,b3_vector *Dst,bool Use4D);
b3_matrix *b3MatrixMAdd         (b3_matrix *A,  b3_matrix *B,  b3_matrix *Result);
b3_matrix *b3MatrixRotVec       (b3_matrix *Src,b3_matrix *Dst,b3_line   *axis,b3_f64 angle);

#endif
