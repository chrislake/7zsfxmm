/*---------------------------------------------------------------------------*/
/* File:        BCJ2.CPP                                                     */
/* Created:     Thu, 03 May 2007 10:42:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Tue, 29 Dec 2015 05:40:09 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    3162                                                         */
/*---------------------------------------------------------------------------*/
/* Revision:    1055                                                         */
/* Updated:     Mon, 22 Mar 2010 11:10:58 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"

#ifdef COMPRESS_BCJ2
	#include "7zip\Compress\BCJ2Coder.cpp"
	#include "7zip\Compress\BCJ2Register.cpp"
	extern "C" {
		#include "../C/Bcj2.c"
	}
#endif
