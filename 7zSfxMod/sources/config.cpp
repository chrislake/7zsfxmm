/*---------------------------------------------------------------------------*/
/* File:        config.cpp                                                   */
/* Created:     Tue, 23 Feb 2010 05:32:49 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Sat, 18 Nov 2017 by https://github.com/datadiode             */
/*---------------------------------------------------------------------------*/
/* Revision:    2209                                                         */
/* Updated:     Fri, 11 Mar 2016 16:45:19 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/*---------------------------------------------------------------------------*/
/* Revision:    28                                                           */
/* Updated:     Mon, 22 Mar 2010 11:11:30 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "7zSfxModInt.h"

#ifdef SFX_CRYPTO
#	include "7zip/Crypto/MyAes.cpp"
#	include "7zip/Crypto/7zAes.cpp"
#	include "7zip/Crypto/7zAesRegister.cpp"
#endif
#ifdef COMPRESS_BCJ
#	include "7zip/Compress/BcjRegister.cpp"
#endif
#ifdef COMPRESS_BCJ2
#	include "7zip/Compress/Bcj2Register.cpp"
#endif
#ifdef COMPRESS_COPY
#	include "7zip/Compress/CopyRegister.cpp"
#endif
#ifdef COMPRESS_DEFLATE
#	include "7zip/Compress/DeflateRegister.cpp"
#endif
#ifdef COMPRESS_DELTA
#	include "7zip\Compress\DeltaFilter.cpp"
#endif
#ifdef COMPRESS_LZMA
#	include "7zip/Compress/LzmaRegister.cpp"
#endif
#ifdef COMPRESS_LZMA2
#	include "7zip/Compress/Lzma2Register.cpp"
#endif
#ifdef COMPRESS_PPMD
#	include "7zip/Compress/PpmdRegister.cpp"
#endif
