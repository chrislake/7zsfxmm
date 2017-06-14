/*---------------------------------------------------------------------------*/
/* File:        Exceptions.cpp                                               */
/* Created:     Mon, 15 Mar 2010 11:26:32 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Fri, 11 Mar 2016 16:53:18 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    2189                                                         */
/*---------------------------------------------------------------------------*/
/* Revision:    677                                                          */
/* Updated:     Fri, 20 Jan 2012 22:42:16 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Ignoring 'First-chance' exception RPC_S_SERVER_UNAVAILABLE   */
/*---------------------------------------------------------------------------*/
/* Revision:    75                                                           */
/* Updated:     Fri, 28 May 2010 22:41:25 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Update to support MSVC 16 (VS2010)                           */
/*---------------------------------------------------------------------------*/
/* Revision:    7                                                            */
/* Updated:     Mon, 22 Mar 2010 11:14:04 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "7zSfxModInt.h"

#ifdef _SFX_USE_CUSTOM_EXCEPTIONS_I386
	#undef	_SFX_USE_CUSTOM_EXCEPTIONS_I386
#endif // _SFX_USE_CUSTOM_EXCEPTIONS_I386

#ifdef _SFX_USE_CUSTOM_EXCEPTIONS_VECTORED_WIN64
	#undef	_SFX_USE_CUSTOM_EXCEPTIONS_VECTORED_WIN64
#endif // _SFX_USE_CUSTOM_EXCEPTIONS_VECTORED_WIN64

#if defined(_MSC_VER) && defined(_WIN32) && !defined(_DEBUG)
	#ifdef _SFX_USE_CUSTOM_EXCEPTIONS
		#undef _SFX_USE_CUSTOM_EXCEPTIONS
		#if defined(_M_IX86) && !defined(_WIN64) && defined(_MSC_VER_OK) && defined(_SFX_USE_CUSTOM_MSVCRT)
			// VC2005, x86, old msvcrt.dll from VC6
			#define _SFX_USE_CUSTOM_EXCEPTIONS_I386
			#define _SFX_USE_CUSTOM_EXCEPTIONS
		#endif // defined(_M_IX86) && !defined(_WIN64) && defined(_MSC_VER_OK) && defined(_SFX_USE_CUSTOM_MSVCRT)
		#ifdef _WIN64
			#define _SFX_USE_CUSTOM_EXCEPTIONS_VECTORED_WIN64
			#define _SFX_USE_CUSTOM_EXCEPTIONS
		#endif // _WIN64
	#endif // _SFX_USE_CUSTOM_EXCEPTIONS
#endif // defined(_MSC_VER) && defined(_WIN32) && !defined(_DEBUG)

#ifndef DBG_PRINTEXCEPTION_WIDE_C
	#define DBG_PRINTEXCEPTION_WIDE_C ((DWORD)0x4001000AL)
#endif

#ifdef _SFX_USE_CUSTOM_EXCEPTIONS
	UINT_PTR uSfxExceptionText = 0;
	typedef UINT_PTR (* GetSfxExceptionText)(void);
	#ifdef _SFX_USE_CUSTOM_EXCEPTIONS_VECTORED_WIN64
		DWORD dwFirstChanceException = 0;
	#endif // _SFX_USE_CUSTOM_EXCEPTIONS_VECTORED_WIN64
	void ReportException( PEXCEPTION_RECORD rec )
	{
		#ifdef _SFX_USE_CUSTOM_EXCEPTIONS_VECTORED_WIN64
			/* 0x0EEDFADE - first chance exception from Delphi library, may be raised by some auto-loaded DLLs into our process */
			if( rec->ExceptionCode == dwFirstChanceException ||
				rec->ExceptionCode == 0x0EEDFADE ||
				rec->ExceptionCode == DBG_PRINTEXCEPTION_C ||
				rec->ExceptionCode == DBG_PRINTEXCEPTION_WIDE_C)
				return;
		#endif // _SFX_USE_CUSTOM_EXCEPTIONS_VECTORED_WIN64
		if( uSfxExceptionText != 0 )
		{
			if( HIWORD(uSfxExceptionText) )
				uSfxExceptionText = ((GetSfxExceptionText)uSfxExceptionText)();
			ShowSfxErrorDialog( GetLanguageString(uSfxExceptionText) );
		}
		else
		{
			WCHAR buf[1024];
			LPCWSTR lpwszFormat = GetLanguageString( ERR_SFX_EXCEPTION );
			wsprintf( buf, lpwszFormat, rec->ExceptionCode, rec->ExceptionAddress );
			for( DWORD i = 0; i < rec->NumberParameters; i++ )
			{
				WCHAR buf2[128];
				wsprintf( buf2, L"\t0x%p\n", rec->ExceptionInformation[i] );
				lstrcat( buf, buf2 );
			}
			if( kSignatureConfigStart[0] != ';' )
				::MessageBoxW( NULL, buf, GetLanguageString(STR_ERROR_TITLE), MB_OK );
			else
				ShowSfxErrorDialog( buf );
		}
		SfxCleanup();
		ExitProcess( ERRC_EXCEPTION );
	}
#endif // _SFX_USE_CUSTOM_EXCEPTIONS

#ifdef _SFX_USE_CUSTOM_EXCEPTIONS_I386
	extern "C" void __cdecl __CxxFrameHandler3( PEXCEPTION_RECORD rec, void * frame, PCONTEXT context, void ** dispatch )
		{ ReportException( rec ); }

#endif // _SFX_USE_CUSTOM_EXCEPTIONS_I386

#ifdef _SFX_USE_CUSTOM_EXCEPTIONS_VECTORED_WIN64
	extern "C" WINBASEAPI __out PVOID WINAPI AddVectoredExceptionHandler(__in ULONG First,__in PVECTORED_EXCEPTION_HANDLER Handler);
	extern "C" WINBASEAPI ULONG WINAPI RemoveVectoredExceptionHandler(__in PVOID Handle);

	/* Fix Win XP/2003 x64 msvcrt.dll '__CxxFrameHandler3' link error */
	/* YumeYao msfn.org http://www.msfn.org/board/topic/101552-7z-sfx-modified-module-and-tools/page__view__findpost__p__938387 */
	/* Plastikat oszone.net http://forum.oszone.net/post-1661544-1155.html */
	extern "C" void *__CxxFrameHandler( PEXCEPTION_RECORD rec, void * frame, PCONTEXT context, void ** dispatch );
	extern "C" void __CxxFrameHandler3( PEXCEPTION_RECORD rec, void * frame, PCONTEXT context, void ** dispatch )
		{ __CxxFrameHandler( rec, frame, context, dispatch ); }

	class CCustomExceptions
	{
	public:
		CCustomExceptions();
		~CCustomExceptions();
		static LONG NTAPI ExceptionHandler( PEXCEPTION_POINTERS pExection );
	};

	static CCustomExceptions __cef;

	CCustomExceptions::CCustomExceptions() { AddVectoredExceptionHandler( 0, ExceptionHandler ); }
	CCustomExceptions::~CCustomExceptions() { RemoveVectoredExceptionHandler( ExceptionHandler ); }

	LONG NTAPI CCustomExceptions::ExceptionHandler( PEXCEPTION_POINTERS pException )
	{
		ReportException( pException->ExceptionRecord );
		return NULL;
	}

	LPITEMIDLIST safe_SHBrowseForFolder( LPBROWSEINFO lpbi )
	{
		dwFirstChanceException = RPC_S_SERVER_UNAVAILABLE;
		LPITEMIDLIST pidl = SHBrowseForFolder( lpbi );
		dwFirstChanceException = 0;
		return pidl;
	}
#endif // _SFX_USE_CUSTOM_EXCEPTIONS_VECTORED_WIN64