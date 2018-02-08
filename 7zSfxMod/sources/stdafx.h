/*---------------------------------------------------------------------------*/
/* File:        stdafx.h                                                     */
/* Created:     Sun, 29 Oct 2006 13:32:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Thu, 08 Feb 2018 by https://github.com/datadiode             */
/*---------------------------------------------------------------------------*/
/* Revision:    3417                                                         */
/* Updated:     Sun, 06 Mar 2016 20:17:50 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/*---------------------------------------------------------------------------*/
/* Revision:    1240                                                         */
/* Updated:     Mon, 22 Mar 2010 11:33:37 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#ifndef _STDAFX_H_INCLUDED_
#define _STDAFX_H_INCLUDED_

#include "config.h"

#define _NO_EXCEPTIONS
#define _HAS_EXCEPTIONS 0
#define try if (1,1)
#define catch(x) else if (1,1)

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT	0x0500
#define _RICHEDIT_VER	0x0200

#include <windows.h>

#include <commctrl.h>
#include <shlobj.h>
#include <shellapi.h>
#include <initguid.h>
#include <richedit.h>

#include <assert.h>
#include <wchar.h>

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
