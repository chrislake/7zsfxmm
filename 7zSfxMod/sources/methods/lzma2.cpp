/*---------------------------------------------------------------------------*/
/* File:        lzma2.cpp                                                    */
/* Created:     Thu, 03 May 2007 10:26:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Mon, 22 Mar 2010 11:26:07 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    1055                                                         */
/*---------------------------------------------------------------------------*/
/* Revision:    1055                                                         */
/* Updated:     Mon, 22 Mar 2010 11:26:07 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"

#ifdef COMPRESS_LZMA2
	#include "7zip\Compress\LZMA2Decoder.cpp"
	#include "7zip\Compress\LZMA2Register.cpp"

	extern "C" {
		#ifndef COMPRESS_LZMA
			#include "../C/LzmaDec.c"
		#endif
		#include "../C/Lzma2Dec.c"
	}
#endif