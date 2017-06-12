/*---------------------------------------------------------------------------*/
/* File:        stdafx.h                                                     */
/* Created:     Sun, 29 Oct 2006 13:32:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Sun, 06 Mar 2016 20:17:50 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    3417                                                         */
/*---------------------------------------------------------------------------*/
/* Revision:    1240                                                         */
/* Updated:     Mon, 22 Mar 2010 11:33:37 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#ifndef _STDAFX_H_INCLUDED_
#define _STDAFX_H_INCLUDED_

#include "config.h"

#if !defined(UNICODE) || !defined(_UNICODE)
	#error "UNICODE and _UNICODE must be defined"
#endif // UNICODE && _UNICODE

#define _MSC_VER_MIN	1400
#define _MSC_VER_MAX	1600

#if defined(_MSC_VER) && (_MSC_VER < _MSC_VER_MIN || (_MSC_VER > _MSC_VER_MAX && _MSC_VER != 1900))
	#pragma message( "Warning: This MS compiler is not tested" )
	#undef _MSC_VER_OK
#else
	#define _MSC_VER_OK	
#endif // _MSC_VER

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500
#define _RICHEDIT_VER	0x0200

#if defined(_MSC_VER) && _MSC_VER == 1200
	#pragma warning(disable:4068)
#endif // _MSC_VER && _MSC_VER == 1200
#include <windows.h>
#if defined(_MSC_VER) && _MSC_VER == 1200
	#pragma warning(default:4068)
#endif // _MSC_VER && _MSC_VER == 1200

#if defined(_MSC_VER) && _MSC_VER == 1900
	#pragma warning(disable:28159)
//	#pragma warning(disable:28251)
#endif // _MSC_VER && _MSC_VER == 1900

#include <commctrl.h>
#include <shlobj.h>
#include <shellapi.h>
#include <initguid.h>
#include <richedit.h>


#include "Windows/PropVariant.h"
#include "Common/Common.h"
#include "7Zip/Common/FileStreams.h"
#include "7Zip/Common/StreamObjects.h"
#include "7Zip/Common/RegisterCodec.h"
#include "7Zip/Archive/IArchive.h"
#include "Common/Wildcard.h"

extern "C" {
	#include "../C/7zcrc.h"
}

#ifndef MUI_LANGUAGE_ID
	#define MUI_LANGUAGE_ID                     0x4      // Use traditional language ID convention
#endif // MUI_LANGUAGE_ID

#ifdef SFX_CRYPTO
	#include "../C/Sha256.h"
#endif // SFX_CRYPTO

#endif // _STDAFX_H_INCLUDED_
