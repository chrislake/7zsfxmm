/*---------------------------------------------------------------------------*/
/* File:        7zSfxModInt.h                                                */
/* Created:     Wed, 25 Jul 2007 09:54:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Sat, 10 Feb 2018 by https://github.com/datadiode             */
/*---------------------------------------------------------------------------*/
/* Revision:    3174                                                         */
/* Updated:     Fri, 01 Apr 2016 20:42:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/*---------------------------------------------------------------------------*/
/* Revision:    1641                                                         */
/* Updated:     Fri, 20 Jan 2012 22:47:56 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Add 'safe_SHBrowseForFolder' that ignore                     */
/*              RPC_S_SERVER_UNAVAILABLE exception on x64 platform           */
/*---------------------------------------------------------------------------*/
/* Revision:    1067                                                         */
/* Updated:     Sat, 26 Jun 2010 04:22:23 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New prefix 'waitall' stuf                                    */
/*---------------------------------------------------------------------------*/
/* Revision:    1047                                                         */
/* Updated:     Sun, 06 Jun 2010 07:47:34 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Add warnings stuff                                           */
/*---------------------------------------------------------------------------*/
/* Revision:    972                                                          */
/* Updated:     Mon, 22 Mar 2010 11:08:53 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#ifndef _7ZSFXMODINT_H_INCLUDED_
#define _7ZSFXMODINT_H_INCLUDED_

#include "7zSfxMod.h"
#include "langstrs.h"
#include "resource.h"
#include "version.h"
#include "7Zip/Archive/7z/7zHandler.h"
#include "strings.h"

#define SKIP_WHITESPACES_W(str) 	while( *str != L'\0' && unsigned(*str) <= L' ' ) str++;

class CSfxExtractEngine;

int GetWindowStringU( HWND hwnd, CSfxStringU &result );

#include "SfxDialogs.h"
#include "archive.h"

struct CTextConfigPair
{
	CSfxStringU ID;
	CSfxStringU String;
};

namespace SfxErrors
{
	enum
	{
		sePropVariant1 = 100,
		sePropVariant2,
		sePropVariant3,
		seAnti,
		seCreateFolder,
		seOverwrite,
		seCreateFile,
#ifdef SFX_CRYPTO
		seNoPassword,
#endif // SFX_CRYPTO
	};
}

extern char const kSignatureConfigStart[];
extern char const kSignatureConfigEnd[];

extern LPCWSTR	lpwszTitle;
extern LPCWSTR	lpwszErrorTitle;
extern LPCWSTR	lpwszCancelText;
extern LPCWSTR	lpwszExtractTitle;
extern LPCWSTR	lpwszExtractPathTitle;
extern LPCWSTR	lpwszExtractPathText;
extern LPCWSTR	lpwszExtractDialogText;
extern LPCWSTR	lpwszCancelPrompt;
extern int		GUIMode;
extern int		GUIFlags;
extern int		MiscFlags;
extern LANGID	idSfxLang;
extern int		ExtractDialogWidth;
extern int		ExtractPathWidth;
extern HMODULE	hRsrcModule;
extern CSfxDialog_Extract *pwndExtractDialog;
extern HWND		hwndExtractDlg;
extern int		FinishMessage;
extern bool		fUseBackward;
#ifdef _SFX_USE_BEGINPROMPTTIMEOUT
	extern int		BeginPromptTimeout;
#endif // _SFX_USE_BEGINPROMPTTIMEOUT
#ifdef SFX_CRYPTO
	extern LPCWSTR	lpwszPasswordTitle;
	extern LPCWSTR	lpwszPasswordText;
#endif // SFX_CRYPTO
#ifdef _SFX_USE_VOLUME_NAME_STYLE
	extern int nVolumeNameStyle;
#endif // _SFX_USE_VOLUME_NAME_STYLE


#ifdef _SFX_USE_PREFIX_PLATFORM
	extern CSfxStringU	strOSPlatform;
#endif // _SFX_USE_PREFIX_PLATFORM
extern CSfxStringU	strModulePathName;
extern CSfxStringU	extractPath;
extern CSfxStringU	strSfxFolder;
extern int			OverwriteMode, OverwriteFlags;

// ExtractDlg
extern HWND		hwndExtractDlg;
extern BOOL		fCancelExtract;

extern CObjectVector<CTextConfigPair> Variables;
extern HMODULE	hKernel32;
extern bool	fUseInstallPath;
extern CObjectVector<CTextConfigPair> gConfig;
extern CSfxArchive	gSfxArchive;

#ifdef _SFX_USE_WARNINGS
	extern LPCWSTR lpwszWarningTitle;
	INT_PTR ShowSfxWarningDialog( LPCWSTR lpwszMessage );
#endif // _SFX_USE_WARNINGS
void	ShowSfxErrorDialog( LPCWSTR lpwszMessage );
void	SfxErrorDialog( BOOL fUseLastError, UINT idFormat, ... );
BOOL	DeleteFileOrDirectoryAlways( LPCWSTR lpwszPathName );
BOOL	SfxCreateDirectory( LPCWSTR lpwszPath );
int		LoadAndParseConfig( Byte const *buffer, UInt32 numBytesInBuffer );
HRESULT ExtractArchive( const CSfxStringU &folderName );
LPCWSTR ParseCommandLineParameters(LPCWSTR);

BOOL	CreateFolderTree( LPCWSTR lpwszPath );
BOOL	CreateShortcut( LPCTSTR lpszShortcutData );
void	DisableXPStyles( HWND hwnd );
void	GetDlgItemRect( HWND hwndDlg, int nIDItem, LPRECT rc );
void	ReplaceWithExtractPath( CSfxStringU& str, CSfxStringU &extractPath );
void	ReplaceWithArchivePath( CSfxStringU& str, CSfxStringU &archivePath );
void	ReplaceWithArchiveName( CSfxStringU& str, CSfxStringU &archiveName );
void	HookForExtractPath( HWND hwnd, BOOL fFromBeginPrompt );

bool	GetTextConfig( const CSfxStringA &string, bool fromCmdLine );
LPCWSTR GetTextConfigValue( LPCWSTR id, int * pFrom = NULL );
BOOL	SfxExtractPathDialog( LPCWSTR lpwszTitle, LPCWSTR lpwszText );
void	ExpandEnvironmentStrings( CSfxStringU & ustr );
int		GetOverwriteMode( LPCWSTR lpwszPath, FILETIME * fileTime );
BOOL	ReplaceVariablesInWindow( HWND hwnd );
BOOL	GetChildRect( HWND hwnd, LPRECT rc );
void	ReplaceHexChars( CSfxStringU& str );
#ifdef _SFX_USE_RTF_CONTROL
	HWND RecreateAsRichEdit( HWND hwndStatic );
#endif // _SFX_USE_RTF_CONTROL
#ifdef _SFX_USE_IMAGES
	BOOL SetDlgControlImage( HWND hwndControl );
#endif // _SFX_USE_IMAGES

#ifdef _SFX_USE_ENVIRONMENT_OLD_VARS
	void SfxAddEnvironmentVarInternal( LPCWSTR lpwszName, LPCWSTR lpwszValue, BOOL fUseAlias );
	#define SfxAddEnvironmentVar(a,b)			SfxAddEnvironmentVarInternal((a),(b),FALSE)
	#define SfxAddEnvironmentVarWithAlias(a,b)	SfxAddEnvironmentVarInternal((a),(b),TRUE)
#else
	void SfxAddEnvironmentVar( LPCWSTR lpwszName, LPCWSTR lpwszValue );
	#define SfxAddEnvironmentVarWithAlias	SfxAddEnvironmentVar
#endif // _SFX_USE_ENVIRONMENT_OLD_VARS

//long	StringToLong( LPCWSTR nptr );
#define StringToLong	_wtol

CSfxStringU SfxMultiByteToUnicodeString( const CSfxStringA &srcString, UINT codePage );
CSfxStringA SfxUnicodeStringToMultiByte( const CSfxStringU &srcString, UINT codePage );
LPCWSTR LoadQuotedString( LPCWSTR lpwszSrc, CSfxStringU & result );
BOOL SfxExecute( LPCWSTR lpwszCmdLine, DWORD dwFlags, LPCWSTR lpwszDirectory );
DWORD Child_ExecuteSfxWaitAll( LPCWSTR lpwszCmdLine );
BOOL ExecuteConfigProgram( LPCWSTR lpwszValue, LPCWSTR lpwszCurrentFolder, bool fUseExecuteFile, CSfxStringU& ustrDirPrefix, LPCWSTR cmdline );
BOOL ExecuteBatch( LPCWSTR lpwszKey, LPCWSTR lpwszCurrentFolder, LPCWSTR lpwszBatch, CSfxStringU& ustrDirPrefix, LPCWSTR cmdline );
DWORD GetPlatform();
LPCWSTR IsPrefixCurrentPlatform( LPCWSTR lpwszString );

INT_PTR ExtractDialog(CSfxExtractEngine *);

#define SFX_OM_ERROR		-1
#define SFX_OM_OVERWRITE	0
#define SFX_OM_SKIP			1

void SfxDialog_InitHooks();
UINT SfxDialog( LPCWSTR lpwszCaption, LPCWSTR lpwszText, UINT uType,
				WNDPROC WindowProc = NULL, HWND hwndOwner = NULL, int nIconFlag = GUIFLAGS_USEICON );
BOOL SfxBeginPrompt( LPCWSTR lpwszCaption, LPCWSTR lpwszText );
void SetFontFromDialog( HWND hwndTarged, HWND hwndDialog );
BOOL CancelInstall( HWND hwnd );
LPCWSTR IsSubString( LPCWSTR lpwszString, LPCWSTR lpwszSubString );
void GetConfigDirectory( CSfxStringU & result );
CSfxStringU CreateTempName( LPCWSTR lpwszFormat );

#define WM_7ZSFX_SETTOTAL		(WM_APP+1)
#define WM_7ZSFX_SETCOMPLETED	(WM_APP+2)

#define FINISHMESSAGE_MAX_TIMEOUT	999

#ifdef _SFX_USE_TEST
	
	extern	CSfxStringU	TSD_Flags;
	extern	DWORD		TSD_ExtractTimeout;
	extern	int			nTestModeType;

	int TestSfxDialogs();
	int TestSfxDialogsToStdout();
	void WriteStdoutChar( WCHAR wc );

#endif // _SFX_USE_TEST

BOOL IsRunAsAdmin();
void SfxCleanup();
void ReplaceVariablesEx( CSfxStringU& str );
CSfxStringU MyGetEnvironmentVariable( LPCWSTR lpwszName );
#ifdef _SFX_USE_LANG
	UINT GetUILanguage();
#endif // _SFX_USE_LANG
#ifdef _SFX_USE_PREFIX_PLATFORM
	BOOL SfxPrepareExecute( int nPlatform );
	void ReplaceWithPlatform( CSfxStringU& str );
	#if defined(_WIN64) && defined(_M_X64)
		#define SfxFinalizeExecute()
	#else
		void SfxFinalizeExecute();
	#endif // defined(_WIN64) && defined(_M_X64)
#endif // _SFX_USE_PREFIX_PLATFORM

void CreateConfigSignature(
#ifdef _SFX_USE_LANG
			DWORD dwLangId,
#endif // _SFX_USE_LANG
#ifdef _SFX_USE_CONFIG_PLATFORM
			LPCSTR lpszPlatformName,
#endif // _SFX_USE_CONFIG_PLATFORM
			CSfxStringA& strBegin, CSfxStringA& strEnd );


#define SFXEXEC_HIDCON		0x01
#define SFXEXEC_RUNAS		0x02
#define SFXEXEC_EXT_MASC	0x07
#define SFXEXEC_NOWAIT		0x10000

#ifdef _SFX_USE_PREFIX_PLATFORM
	#define SFX_EXECUTE_PLATFORM_ANY		0
	#define SFX_EXECUTE_PLATFORM_I386		1
	#define SFX_EXECUTE_PLATFORM_AMD64		2
#endif // _SFX_USE_PREFIX_PLATFORM

#if defined(_SFX_USE_PREFIX_PLATFORM) || defined(_SFX_USE_ENVIRONMENT_VARS)
	LPCWSTR GetPlatformName();
#endif // _SFX_USE_PREFIX_PLATFORM || _SFX_USE_ENVIRONMENT_VARS

#define CMDLINE_SFXWAITALL			L"sfxwaitall"
#define CMDLINE_SFXELEVATION		L"sfxelevation"

#if !defined(SFX_VOLUMES) && !defined(SFX_PROTECT)
	class CSfxInStream : public CInFileStream
	{
	public:
		UInt32 GetStreamsCount() { return 0; }
	};
#else
	#include "7zip/Archive/Common/MultiStream.h"
	class CSfxInStream : public CMultiStream
	{
	public:
#ifdef SFX_VOLUMES
		bool Open(LPCTSTR fileName);
		bool InitVolumes();
		IInStream * GetVolumesStream();
		UInt32 GetStreamsCount() { return Streams.Size(); };

	private:
		HRESULT OpenSubStream( LPCTSTR fileName, CSubStreamInfo * subStream );
#endif // SFX_VOLUMES
	};
#endif // !defined(SFX_VOLUMES) && !defined(SFX_PROTECT)

class CSfxCurrentDirectory
{
public:
	CSfxCurrentDirectory( LPCWSTR lpwszNewFolder );
	virtual ~CSfxCurrentDirectory();
protected:
	CSfxStringU	m_strDirectory;
};

#define	CPF_NONE		0
#define CPF_DIGIT		1
#define	CPF_NUMBER		2
#define CPF_SFX_INDEX	3
LPCWSTR CheckPrefix( LPCWSTR lpwszString, LPCWSTR lpwszPrefix, BOOL nCPFlags );
#ifdef _SFX_USE_PREFIX_PLATFORM
	LPCWSTR IsPrefixPlatformI386( LPCWSTR lpwszString );
	LPCWSTR IsPrefixPlatformAmd64( LPCWSTR lpwszString );
#endif // _SFX_USE_PREFIX_PLATFORM

#endif // _7ZSFXMODINT_H_INCLUDED_
