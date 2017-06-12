/*---------------------------------------------------------------------------*/
/* File:        LZMA.CPP                                                     */
/* Created:     Thu, 03 May 2007 10:26:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Mon, 22 Mar 2010 11:23:15 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    1055                                                         */
/*---------------------------------------------------------------------------*/
/* Revision:    1055                                                         */
/* Updated:     Mon, 22 Mar 2010 11:23:15 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"

#ifdef COMPRESS_LZMA
	#include "7zip\Compress\LZMADecoder.cpp"
	#include "7zip\Compress\LZMARegister.cpp"

	extern "C" {
		#include "../C/LzmaDec.c"
	}
#endif