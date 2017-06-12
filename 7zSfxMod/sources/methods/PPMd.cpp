/*---------------------------------------------------------------------------*/
/* File:        PPMd.cpp                                                     */
/* Created:     Thu, 03 May 2007 00:31:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Mon, 22 Mar 2010 11:27:36 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    1055                                                         */
/*---------------------------------------------------------------------------*/
/* Revision:    1055                                                         */
/* Updated:     Mon, 22 Mar 2010 11:27:36 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"

#ifdef COMPRESS_PPMD
	#include "7zip\Compress\PPMDDecoder.cpp"
	#include "7zip\Compress\PPMDRegister.cpp"
	#include "7zip\Common\CWrappers.cpp"
	#include "..\C\Ppmd7.c"
	#include "..\C\Ppmd7Dec.c"
#endif