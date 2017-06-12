/*---------------------------------------------------------------------------*/
/* File:        BCJ.CPP                                                      */
/* Created:     Thu, 03 May 2007 10:59:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Tue, 29 Dec 2015 05:33:42 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    3162                                                         */
/*---------------------------------------------------------------------------*/
/* Revision:    1055                                                         */
/* Updated:     Mon, 22 Mar 2010 11:10:21 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"

#ifdef COMPRESS_BCJ
	#include "7zip\Compress\BCJCoder.cpp"
	#include "7zip\Compress\BCJRegister.cpp"
	#include "7zip\Compress\BranchMisc.cpp"

	extern "C" {
		#include "../C/Bra86.c"
	}
#endif
