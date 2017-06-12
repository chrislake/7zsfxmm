/*---------------------------------------------------------------------------*/
/* File:        Delta.cpp                                                    */
/* Created:     Sat, 02 May 2015 00:30:23 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Sat, 02 May 2015 01:20:52 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    1                                                            */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"

#ifdef COMPRESS_DELTA
	#include "7zip\Compress\DeltaFilter.cpp"
	extern "C" {
		#include "../C/Delta.c"
	}
#endif