/*---------------------------------------------------------------------------*/
/* File:        main.cpp                                                     */
/* Created:     Fri, 29 Jul 2005 03:23:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Sat, 10 Feb 2018 by https://github.com/datadiode             */
/*---------------------------------------------------------------------------*/
/* Revision:    3901                                                         */
/* Updated:     Sat, 02 Apr 2016 06:31:33 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/*---------------------------------------------------------------------------*/
/* Revision:    1798                                                         */
/* Updated:		Wed, 30 Jun 2010 09:24:36 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Fix: extraction folder was not set as default				 */
/*				for external programs										 */
/*---------------------------------------------------------------------------*/
/* Revision:    1794                                                         */
/* Updated:     Sat, 26 Jun 2010 04:23:10 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Execution of external programs was rewritten to support		 */
/*				'wait all childs' of the executed process					 */
/*				new execution prefix 'waitall'								 */
/*---------------------------------------------------------------------------*/
/* Revision:    1774                                                         */
/* Updated:     Sun, 06 Jun 2010 08:51:48 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Add warning stuff											 */
/*				(WarningTitle, MiscFlags, -mf command line switch)			 */
/*---------------------------------------------------------------------------*/
/* Revision:    1698                                                         */
/* Updated:     Mon, 22 Mar 2010 11:26:58 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "7zSfxModInt.h"
#include "ExtractEngine.h"
#include "archive.h"
#include "../C/Lzma86.h"
#include "VersionData.h"
#include "vs_version.h"

using NWindows::NFile::NIO::CInFile;
using NWindows::NFile::NIO::COutFile;

void DeleteSFX( LPCWSTR moduleName );

CSfxStringU SfxMultiByteToUnicodeString( CSfxStringA const &srcString, UINT codePage );
CSfxStringA SfxUnicodeStringToMultiByte( CSfxStringU const &srcString, UINT codePage );

char const kSignatureConfigStart[] = ";!@Install@!UTF-8!";
char const kSignatureConfigEnd[] = ";!@InstallEnd@!";

LPCWSTR	lpwszErrorTitle;
#ifdef _SFX_USE_WARNINGS
	LPCWSTR		lpwszWarningTitle;
	CSfxStringU	strWarningTitle;
#endif // _SFX_USE_WARNINGS
LPCWSTR	lpwszTitle;
LPCWSTR lpwszExtractTitle;
LPCWSTR lpwszExtractPathTitle;
LPCWSTR lpwszExtractPathText;
LPCWSTR lpwszCancelPrompt;
LPCWSTR	lpwszCancelText;
LPCWSTR	lpwszExtractDialogText;
#ifdef SFX_CRYPTO
	LPCWSTR	lpwszPasswordTitle = NULL;
	LPCWSTR	lpwszPasswordText = NULL;
#endif // SFX_CRYPTO
#ifdef _SFX_USE_VOLUME_NAME_STYLE
	int nVolumeNameStyle=0;
#endif // _SFX_USE_VOLUME_NAME_STYLE

int		GUIMode = 0;
int		GUIFlags = -1;
int		MiscFlags = 0;
int		ExtractDialogWidth = 300;
int		ExtractPathWidth = 300;
int		OverwriteMode = OVERWRITE_MODE_ALL;
int		OverwriteFlags = 0;
int		FinishMessage = -1;
#ifdef _SFX_USE_BEGINPROMPTTIMEOUT
	int		BeginPromptTimeout = 0;
#endif // _SFX_USE_BEGINPROMPTTIMEOUT
CSfxStringU	extractRoot;
CSfxStringU	extractPath;
CSfxStringU strSfxFolder;
CSfxStringU strSfxName;
CSfxStringU	strErrorTitle;
CSfxStringU	strTitle;
#ifdef _SFX_USE_PREFIX_PLATFORM
	CSfxStringU	strOSPlatform;
#endif // _SFX_USE_PREFIX_PLATFORM
CSfxArchive	gSfxArchive;

bool	fUseInstallPath;

CSfxStringU	strModulePathName;
CObjectVector<CTextConfigPair> gConfig;

HMODULE	hKernel32;

CObjectVector<CTextConfigPair> Variables;
bool fUseBackward = false;

#ifndef CSIDL_COMMON_DOCUMENTS
	#define CSIDL_COMMON_DOCUMENTS          0x002e        // All Users\Documents
#endif

static void SfxInit()
{
	hKernel32 = ::GetModuleHandleW( L"kernel32" );
	InitCommonControls();
	CrcGenerateTable();

#ifdef _SFX_USE_LANG
	GetUILanguage();
#endif // _SFX_USE_LANG

	lpwszErrorTitle = GetLanguageString( STR_ERROR_TITLE );
#ifdef _SFX_USE_WARNINGS
	lpwszWarningTitle = GetLanguageString( STR_WARNING_TITLE );
#endif // _SFX_USE_WARNINGS
	lpwszTitle = GetLanguageString( STR_TITLE );
	lpwszExtractTitle = GetLanguageString( STR_EXTRACT_TITLE );
	lpwszExtractPathTitle = GetLanguageString( STR_EXTRACT_PATH_TITLE );
	lpwszExtractPathText = GetLanguageString( STR_EXTRACT_PATH_TEXT );
	lpwszCancelPrompt = GetLanguageString( STR_CANCEL_PROMPT );
	lpwszCancelText = lpwszExtractDialogText = NULL;

	WCHAR	wszPath[MAX_PATH];
	WCHAR	wszName[_countof(SPECIAL_FOLDER_FORMAT) + 32];
	for( int i = 0; i < 0x40; i++ )
	{
		if( SHGetSpecialFolderPath(NULL, wszPath, i, FALSE) )
		{
			static struct {
				int		nFolder;
				LPCWSTR	lpwszName;
			} const EnvAliases[] = {
				{ CSIDL_COMMON_DESKTOPDIRECTORY,	L"CommonDesktop" },
				{ CSIDL_COMMON_DOCUMENTS,			L"CommonDocuments" },
				{ CSIDL_DESKTOPDIRECTORY,			L"UserDesktop" },
				{ CSIDL_PERSONAL,					L"MyDocuments" },
				{ CSIDL_PERSONAL,					L"MyDocs" }
			};
			wsprintf( wszName, SPECIAL_FOLDER_FORMAT, i );
			SfxAddEnvironmentVarWithAlias( wszName, wszPath );
			for( int j = 0; j < _countof(EnvAliases); j++ )
			{
				if( EnvAliases[j].nFolder == i )
					SfxAddEnvironmentVar( EnvAliases[j].lpwszName, wszPath );
			}
		}
	}
}

static void ReplaceVariables( CSfxStringU& str )
{
	ExpandEnvironmentStrings( str );
	ReplaceWithArchivePath( str, strSfxFolder );
	ReplaceWithArchiveName( str, strSfxName );
#ifdef _SFX_USE_PREFIX_PLATFORM
	ReplaceWithPlatform( str );
#endif // _SFX_USE_PREFIX_PLATFORM
	ExpandEnvironmentStrings( str );
}

void ReplaceVariablesEx( CSfxStringU& str )
{
	// ExpandEnvironmentStrings( str );
	ReplaceWithExtractPath( str, extractRoot );
	ReplaceVariables( str );
}

static void SetEnvironment()
{
	for( unsigned i = 0; i < Variables.Size(); i++ )
	{
		CSfxStringU val = Variables[i].String;
		ReplaceVariablesEx( val );
		::SetEnvironmentVariable( Variables[i].ID, val );
	}
}

static LPCWSTR UpdateFlagsCommon( LPCWSTR lpwszText, int * pnValue )
{
	while( (*lpwszText >= L'0' && *lpwszText <= L'9') || *lpwszText == L'+' || *lpwszText == L'-' )
	{
		int nValue = StringToLong( lpwszText );
		// skip to next
		while( *lpwszText == L'-' || *lpwszText == L'+' ) lpwszText++;
		while( *lpwszText >= L'0' && *lpwszText <= L'9' ) lpwszText++;
		if( nValue > 0 )
			pnValue[0] |= nValue;
		else
			pnValue[0] &= (~(0-nValue));
	}

	return lpwszText;
}

static LPCWSTR UpdateGUIFlags( LPCWSTR lpwszText )
{
	if( GUIFlags == -1 || (*lpwszText != L'+' && *lpwszText != L'-') )
		GUIFlags = 0;

	return UpdateFlagsCommon( lpwszText, &GUIFlags );
}

static LPCWSTR UpdateMiscFlags( LPCWSTR lpwszText )
{
	if( *lpwszText != L'+' && *lpwszText != L'-' )
		MiscFlags = 0;

	return UpdateFlagsCommon( lpwszText, &MiscFlags );
}

static LPCWSTR UpdateOverwriteMode( LPCWSTR lpwszText )
{
	int nValue = OverwriteMode | OverwriteFlags;
	if( *lpwszText != L'+' && *lpwszText != L'-' )
		nValue = OVERWRITE_MODE_ALL;
	LPCWSTR lpwszRet = UpdateFlagsCommon( lpwszText, &nValue );
	if( (nValue&OVERWRITE_MODE_MASK) <= OVERWRITE_MODE_MAX )
		OverwriteMode = nValue & OVERWRITE_MODE_MASK;
	OverwriteFlags = nValue & (~OVERWRITE_MODE_MASK);
	return lpwszRet;
}

static void ReplaceVariableInShortcut( CSfxStringU &strShortcut, CSfxStringU const &strVarName, CSfxStringU const &strVarValue )
{
	int nVarNameLength = strVarName.Len();
	for( unsigned i = 0; i < strShortcut.Len(); i++ )
	{
		if( strShortcut[i] == L'%' &&
			_wcsnicmp( strShortcut.Ptr( i + 1 ), strVarName, nVarNameLength ) == 0 &&
			strShortcut[i + nVarNameLength + 1] == L'%' )
		{
			// var found
			strShortcut.Delete( i, nVarNameLength + 2 );
			strShortcut.Insert( i, strVarValue );
		}
	}
}

static LPCWSTR IsSfxSwitch( LPCWSTR lpwszCommandLine, LPCWSTR lpwszSwitch )
{
	SKIP_WHITESPACES_W(lpwszCommandLine);
	if( *lpwszCommandLine == L'-' || *lpwszCommandLine == L'/')
	{
		lpwszCommandLine = IsSubString( lpwszCommandLine + 1, lpwszSwitch );
		if( lpwszCommandLine && (unsigned(*lpwszCommandLine) <= L' ' || *lpwszCommandLine == L':') )
		{
			return lpwszCommandLine;
		}
	}
	return NULL;
}

static void SetConfigVariables()
{
	LPCWSTR	lpwszValue;
	int from;

	// Main title
	if( (lpwszValue = GetTextConfigValue( CFG_TITLE )) != NULL )
	{
		strErrorTitle = lpwszValue; strErrorTitle += GetLanguageString( STR_ERROR_SUFFIX );
		lpwszErrorTitle = strErrorTitle;
#ifdef _SFX_USE_WARNINGS
		strWarningTitle = lpwszValue; strWarningTitle += GetLanguageString( STR_WARNING_SUFFIX );
		lpwszWarningTitle = strWarningTitle;
#endif // _SFX_USE_WARNINGS
		lpwszTitle = lpwszValue;
	}
	// Update error title
	if( (lpwszValue = GetTextConfigValue( CFG_ERRORTITLE )) != NULL )
		lpwszErrorTitle = lpwszValue;
#ifdef _SFX_USE_WARNINGS
	// Update warnings title
	if( (lpwszValue = GetTextConfigValue( CFG_WARNINGTITLE )) != NULL )
		lpwszWarningTitle = lpwszValue;
#endif // _SFX_USE_WARNINGS
	// Extract title
	if( (lpwszValue = GetTextConfigValue( CFG_EXTRACT_TITLE )) != NULL )
		lpwszExtractTitle = lpwszValue;
	// Load GUIMode & 7-Zip > 4.42 'Progress'
	if( (lpwszValue = GetTextConfigValue( CFG_PROGRESS)) != NULL && _wcsicmp( lpwszValue, L"no" ) == 0 )
		GUIMode = GUIMODE_HIDDEN;
	if( (lpwszValue = GetTextConfigValue( CFG_GUIMODE )) != NULL )
	{
		if( lpwszValue[0] >= L'0' && lpwszValue[0] <= L'0'+GUIMODE_MAX )
			GUIMode = lpwszValue[0] - L'0';
	}
	// Load OverwriteMode
	if( (lpwszValue = GetTextConfigValue( CFG_OVERWRITE_MODE)) != NULL )
	{
		UpdateOverwriteMode( lpwszValue );
	}
	// Load GUIFlags
	from = 0;
	while( (lpwszValue = GetTextConfigValue( CFG_GUIFLAGS, &from )) != NULL )
	{
		from++;
		UpdateGUIFlags( lpwszValue );
	}
	// Load MiscFlags
	from = 0;
	while( (lpwszValue = GetTextConfigValue( CFG_MISCFLAGS, &from )) != NULL )
	{
		from++;
		UpdateMiscFlags( lpwszValue );
	}

	lpwszCancelText = GetTextConfigValue( CFG_EXTRACT_CANCELTEXT );
	lpwszExtractDialogText = GetTextConfigValue( CFG_EXTRACT_DIALOGTEXT );
	if( (lpwszValue = GetTextConfigValue( CFG_EXTRACT_DIALOGWIDTH)) != NULL )
		ExtractDialogWidth = StringToLong( lpwszValue );
	if( (lpwszValue = GetTextConfigValue( CFG_EXTRACTPATH_WIDTH)) != NULL )
		ExtractPathWidth = StringToLong( lpwszValue );

	// Extract path strings
	if( (lpwszValue = GetTextConfigValue(CFG_EXTRACT_PATH_TITLE)) != NULL )
		lpwszExtractPathTitle = lpwszValue;
	if( (lpwszValue = GetTextConfigValue(CFG_EXTRACT_PATH_TEXT)) != NULL )
		lpwszExtractPathText = lpwszValue;

	// Cancel prompt text
	if( (lpwszValue = GetTextConfigValue(CFG_CANCEL_PROMPT)) != NULL )
		lpwszCancelPrompt = lpwszValue;

#ifdef SFX_CRYPTO
	if( (lpwszValue = GetTextConfigValue(CFG_PASSWORD_TITLE)) != NULL )
		lpwszPasswordTitle = lpwszValue;
	else
		lpwszPasswordTitle = lpwszTitle;
	if( (lpwszValue = GetTextConfigValue(CFG_PASSWORD_TEXT)) != NULL )
		lpwszPasswordText = lpwszValue;
	else
		lpwszPasswordText = GetLanguageString( STR_PASSWORD_TEXT );
#endif // SFX_CRYPTO
#ifdef _SFX_USE_VOLUME_NAME_STYLE
	if( (lpwszValue = GetTextConfigValue( CFG_VOLUME_NAME_STYLE )) != NULL )
	{
		if( lpwszValue[0] >= L'0' && lpwszValue[0] <= L'9' )
			nVolumeNameStyle = lpwszValue[0] - L'0';
	}
#endif // _SFX_USE_VOLUME_NAME_STYLE
}

static void PostExecute_Shortcut( LPCWSTR lpwszValue )
{
	CSfxStringU strShortcut = lpwszValue;
	ReplaceWithExtractPath( strShortcut, extractRoot );
	ReplaceWithArchivePath( strShortcut, strSfxFolder );
	ReplaceWithArchiveName( strShortcut, strSfxName );
#ifdef _SFX_USE_PREFIX_PLATFORM
	ReplaceWithPlatform( strShortcut );
#endif // _SFX_USE_PREFIX_PLATFORM
	for( unsigned i = 0; i < Variables.Size(); i++ )
	{
		CSfxStringU varValue = MyGetEnvironmentVariable( Variables[i].ID );
		ExpandEnvironmentStrings( varValue );
		ReplaceWithExtractPath( varValue, extractRoot );
		ReplaceWithArchivePath( varValue, strSfxFolder );
		ReplaceWithArchiveName( varValue, strSfxName );
#ifdef _SFX_USE_PREFIX_PLATFORM
		ReplaceWithPlatform( varValue );
#endif // _SFX_USE_PREFIX_PLATFORM

		ReplaceVariableInShortcut( strShortcut, Variables[i].ID, varValue );
	}
	CreateShortcut( strShortcut );
}

static void PostExecute_Delete( LPCWSTR lpwszValue )
{
	CSfxStringU tmp = lpwszValue;
	ReplaceVariablesEx( tmp );
	DeleteFileOrDirectoryAlways( tmp );
}

typedef void (* POST_EXECUTE_PROC)( LPCWSTR lpwszParamName );

static void ProcessPostExecuteSub(	POST_EXECUTE_PROC pfnPostExecute,
									LPCWSTR lpwszParamPrefix,
									LPCWSTR lpwszBatchIndexes,
									int nUseDefault )
{
		CSfxStringU	ustrParamName;
		LPCWSTR	lpwszValue;
		LPCWSTR p1 = lpwszBatchIndexes;
		bool	fBatchExecuted = false;
		int nIndex;
		while( (*p1 >= L'0' && *p1 <= L'9') ||
			   (*p1 >= L'a' && *p1 <= L'z') ||
			   (*p1 >= L'A' && *p1 <= L'Z') ||
			   (lpwszBatchIndexes == p1 && nUseDefault == -1) )
		{
			ustrParamName = lpwszParamPrefix;
			ustrParamName += *p1;
			nIndex = 0;
			p1++;

			while( (lpwszValue = GetTextConfigValue( ustrParamName, &nIndex)) != NULL )
			{
				pfnPostExecute( lpwszValue );
				nIndex++;
				fBatchExecuted = true;
			}
		}
		switch( nUseDefault )
		{
		case 0:
			break;
		case 1:
			ProcessPostExecuteSub( pfnPostExecute, lpwszParamPrefix, L"\0", -1 );
			break;
		default:
			if( *lpwszBatchIndexes == L'\0' && nUseDefault == -1 )
				break;
			if( fBatchExecuted == false && *lpwszBatchIndexes != L'\0' )
				ProcessPostExecuteSub( pfnPostExecute, lpwszParamPrefix, L"\0",  -1 );
		}
}

static void ShowSfxVersion()
{
	GUIFlags = 0;
	CSfxDialog_Version dlg;
	extern unsigned int g_NumCodecs;
	extern const CCodecInfo *g_Codecs[];
	CSfxStringU ustrVersion = GetLanguageString( STR_SFXVERSION );
	unsigned int ki;
	for( ki = 0; ki < g_NumCodecs; ki++ )
	{
		if( ki != 0 )
			ustrVersion += L", ";
		ustrVersion.AddAscii( g_Codecs[ki]->Name );
	}
#ifdef SFX_VOLUMES
	if( ki != 0 )
		ustrVersion += L", ";
	ustrVersion += L"Volumes";
#endif // SFX_VOLUMES
	WCHAR tmp[128];
	wsprintf( tmp, L" \n\t%X - %03X - %03X - %03X - %03X",
				SFXBUILD_OPTIONS1, SFXBUILD_OPTIONS2, SFXBUILD_OPTIONS3, SFXBUILD_OPTIONS4, SFXBUILD_OPTIONS5 );
	ustrVersion += tmp;
	ustrVersion += L"\n";
	dlg.Show( SD_OK|SD_ICONINFORMATION, lpwszTitle, ustrVersion );
}

void SfxCleanup()
{
	if( fUseInstallPath == false )
		DeleteFileOrDirectoryAlways( extractRoot );
}

static BOOL CALLBACK SfxEnumResNameProc(HMODULE hModule, LPCWSTR lpType, LPWSTR lpName, LONG_PTR lParam)
{
	HRSRC const hFindRes = FindResource(hModule, lpName, lpType);
	HGLOBAL const hLoadRes = LoadResource(hModule, hFindRes);
	const Byte *inBuffer = static_cast<const Byte *>(LockResource(hLoadRes));
	UInt32 inSize32 = SizeofResource(hModule, hFindRes);
	CBuffer<Byte> outBuffer;
	if (lpType == RT_MANIFEST)
	{
		lpName = L"7ZSfxMod.exe.manifest";
	}
	else if (*inBuffer <= 0xE0)
	{
		size_t inSize = inSize32;
		UInt64 outSize64;
		if (Lzma86_GetUnpackSize(inBuffer, inSize, &outSize64) != 0)
			return FALSE; // data error
		size_t outSize = static_cast<UInt32>(outSize64);
		if (outSize != outSize64)
			return FALSE; // Unpack size is too big
		outBuffer.Alloc(outSize);
		if (Lzma86_Decode(outBuffer, &outSize, inBuffer, &inSize) != 0)
			return FALSE; // Decode error
		if (outSize != static_cast<UInt32>(outSize64))
			return FALSE; // incorrect processed size
		inBuffer = outBuffer;
		inSize32 = static_cast<UInt32>(outSize);
	}
	if (lParam == 0)
		return LoadAndParseConfig(inBuffer, inSize32) == 0;
	CSfxStringU OutFileName = reinterpret_cast<LPWSTR>(lParam);
	OutFileName += L"\\";
	OutFileName += lpName;
	COutFile OutFile;
	if (!OutFile.Open(OutFileName, CREATE_ALWAYS))
		return FALSE;
	UInt32 processedSize;
	if (!OutFile.Write(inBuffer, inSize32, processedSize) || (processedSize != inSize32))
		return FALSE;
	return TRUE;
}

static BOOL ExtractResources(HMODULE hModule, LPWSTR lpType, LPCWSTR lpRoot)
{
	CreateFolderTree(lpRoot);
	return EnumResourceNamesW(hModule, lpType, SfxEnumResNameProc, reinterpret_cast<LONG_PTR>(lpRoot));
}

static BOOL LoadAndParseConfig(HMODULE hModule)
{
	return EnumResourceNamesW(hModule, RT_HTML, SfxEnumResNameProc, 0);
}

static bool CreateSelfExtractor(LPCWSTR strModulePathName, LPCWSTR lpwszValue)
{
	CSfxStringU OutFileName;
	SKIP_WHITESPACES_W(lpwszValue);
	lpwszValue = LoadQuotedString(lpwszValue, OutFileName);
	COutFile OutFile;
	UInt32 processedSize;
	UInt64 newPosition;
	if (!CopyFile(strModulePathName, OutFileName, FALSE))
		return false;
	HANDLE hUpdate = BeginUpdateResourceW(OutFileName, FALSE);
	if (hUpdate == NULL)
		return false;
	BOOL fDiscard = TRUE;
	BOOL fSuccess = TRUE;
	// Add resources as specified
	LPCWSTR lpwszAhead;
	while (LPWSTR lpType =
		(lpwszAhead = IsSfxSwitch(lpwszValue, L"config"))	!= NULL ? RT_HTML :
		(lpwszAhead = IsSfxSwitch(lpwszValue, L"adjunct"))	!= NULL ? RT_RCDATA :
		(lpwszAhead = IsSfxSwitch(lpwszValue, L"manifest"))	!= NULL ? RT_MANIFEST :
		(lpwszAhead = IsSfxSwitch(lpwszValue, L"icon"))		!= NULL ? RT_GROUP_ICON :
		NULL)
	{
		CSfxStringU InFileName;
		SKIP_WHITESPACES_W(lpwszAhead);
		lpwszValue = LoadQuotedString(lpwszAhead, InFileName);
		CInFile InFile;
		UInt64 fileSize;
		if (!InFile.Open(InFileName) || !InFile.GetLength(fileSize))
			return false;
		CBuffer<Byte> inBuffer(static_cast<size_t>(fileSize));
		UInt32 inSize = static_cast<UInt32>(fileSize);
		if (inSize != fileSize)
			return false; // File is too big
		if (!InFile.Read(inBuffer, inSize, processedSize) || (processedSize != inSize))
			return false;
		CBuffer<Byte> outBuffer;
		LPVOID lpData = inBuffer;
		LPCWSTR lpName = MAKEINTRESOURCE(1);
		if (lpType == RT_GROUP_ICON)
		{
			struct IconHeader
			{
				WORD idReserved;
				WORD idType;
				WORD idCount;
			};
			struct IconFileRecord
			{
				BYTE bWidth;
				BYTE bHeight;
				BYTE bColorCount;
				BYTE bReserved;
				WORD wPlanes;
				WORD wBitCount;
				DWORD dwBytesInRes;
				DWORD dwOffset;
			};
			struct IconGroupRecord
			{
				BYTE bWidth;
				BYTE bHeight;
				BYTE bColorCount;
				BYTE bReserved;
				WORD wPlanes;
				WORD wBitCount;
				BYTE dwBytesInRes[sizeof(DWORD)];
				WORD nId;
			};
			// Limit the size and color depth of the icon as specified
			LONG lMaxWidth = LONG_MAX;
			UINT uMaxDepth = UINT_MAX;
			UINT limits[3];
			if (swscanf(lpwszValue, L"%ux%ux%u", &limits[0], &limits[1], &limits[2]) == 3)
			{
				// Assign to lMaxWidth the value which occurs at least twice
				// Assign to uMaxDepth whatever value remains
				UINT i = limits[1] == limits[2] ? 0 : limits[0] == limits[2] ? 1 : limits[0] == limits[1] ? 2 : 3;
				if (i < _countof(limits))
				{
					uMaxDepth = limits[i];
					lMaxWidth = limits[!i];
				}
				// Consume the extra argument
				lpwszValue = LoadQuotedString(lpwszValue, InFileName);
			}
			IconHeader const *const qHeader = reinterpret_cast<IconHeader *>(static_cast<Byte *>(inBuffer));
			IconFileRecord const *qEntry = reinterpret_cast<IconFileRecord const *>(qHeader + 1);
			WORD const n = qHeader->idCount;
			UInt32 outSize = sizeof(IconHeader) + n * sizeof(IconGroupRecord);
			outBuffer.Alloc(outSize);
			IconHeader *const pHeader = reinterpret_cast<IconHeader *>(&outBuffer[0]);
			IconGroupRecord *pEntry = reinterpret_cast<IconGroupRecord *>(pHeader + 1);
			*pHeader = *qHeader;
			pHeader->idCount = 0;
			outSize = sizeof(IconHeader);
			WORD nId = 1;
			for (WORD i = 0; i < n; ++i)
			{
				BITMAPINFOHEADER *pBIH = reinterpret_cast<BITMAPINFOHEADER *>(&inBuffer[qEntry->dwOffset]);
				if (pBIH->biWidth <= lMaxWidth && pBIH->biBitCount <= uMaxDepth)
				{
					++pHeader->idCount;
					outSize += sizeof(IconGroupRecord);
					memcpy(pEntry, qEntry, offsetof(IconGroupRecord, nId));
					pEntry->nId = nId;
					pEntry->wPlanes = pBIH->biPlanes;
					pEntry->wBitCount = pBIH->biBitCount;
					++nId;
					++pEntry;
				}
				++qEntry;
			}
			// Update RT_GROUP_ICON *before* RT_ICON so as to not end up in a mess!
			BOOL fUpdate = UpdateResourceW(hUpdate, lpType, lpName, 0, pHeader, outSize);
			(fUpdate ? fDiscard : fSuccess) = FALSE;
			pEntry = reinterpret_cast<IconGroupRecord *>(pHeader + 1);
			qEntry = reinterpret_cast<IconFileRecord const *>(qHeader + 1);
			nId = 1;
			for (WORD i = 0; i < n; ++i)
			{
				BITMAPINFOHEADER *pBIH = reinterpret_cast<BITMAPINFOHEADER *>(&inBuffer[qEntry->dwOffset]);
				if (pBIH->biWidth <= lMaxWidth && pBIH->biBitCount <= uMaxDepth)
				{
					BOOL fUpdate = UpdateResourceW(hUpdate, RT_ICON, MAKEINTRESOURCE(nId), 0, pBIH, qEntry->dwBytesInRes);
					(fUpdate ? fDiscard : fSuccess) = FALSE;
					++nId;
					++pEntry;
				}
				++qEntry;
			}
			continue; // Update of RT_GROUP_ICON has already happened above!
		}
		else if (lpType != RT_MANIFEST)
		{
			// we allocate 105% of original size for output buffer
			UInt64 outSize64 = fileSize / 20 * 21 + (1 << 16);
			size_t outSize = static_cast<UInt32>(outSize64);
			if (outSize != outSize64)
				return false; // File is too big
			outBuffer.Alloc(outSize);
			UInt32 dict = 1 << 24;
			while (UInt32 smaller = dict >> 1)
			{
				if (smaller < inSize)
					break;
				dict = smaller;
			}
			int res = Lzma86_Encode(outBuffer, &outSize, inBuffer, inSize, 5, dict, SZ_FILTER_AUTO);
			if (res != 0)
				return false; // Encode error
			inSize = static_cast<UInt32>(outSize);
			lpData = outBuffer;
			lpName = InFileName.Ptr() + InFileName.ReverseFind_PathSepar() + 1;
		}
		BOOL fUpdate = UpdateResourceW(hUpdate, lpType, lpName, 0, lpData, inSize);
		(fUpdate ? fDiscard : fSuccess) = FALSE;
	}
	// Patch version resource
	static WCHAR const Version[]			= L"*Version";			// Shortcut to set all version fields at once
	static WCHAR const FileVersion[]		= L"FileVersion";
	static WCHAR const ProductVersion[]		= L"ProductVersion";
	static WCHAR const CompanyName[]		= L"CompanyName";
	static WCHAR const FileDescription[]	= L"FileDescription";
	static WCHAR const InternalName[]		= L"InternalName";
	static WCHAR const LegalCopyright[]		= L"LegalCopyright";
	static WCHAR const OriginalFilename[]	= L"OriginalFilename";
	static WCHAR const PrivateBuild[]		= L"PrivateBuild";
	static WCHAR const SpecialBuild[]		= L"SpecialBuild";
	static WCHAR const ProductName[]		= L"ProductName";
	file_ver_data fvd;
	if (CVersionData const *const pvd = CVersionData::Load())
	{
		if (VS_FIXEDFILEINFO const *const pVffInfo =
			reinterpret_cast<const VS_FIXEDFILEINFO *>(pvd->Data()))
		{
			fvd.m_fxi = *pVffInfo;
			fvd.m_fxi.dwFileFlags = 0;
		}
		if (CVersionData const *const pvdStringFileInfo = pvd->Find(L"StringFileInfo"))
		{
			if (CVersionData const *const pvdLanguage = pvdStringFileInfo->First())
			{
				CVersionData const *const qvdLanguage = pvdStringFileInfo->Next();
				CVersionData const *pvdAssignment = reinterpret_cast<CVersionData const *>(pvdLanguage->Data());
				while (pvdAssignment < qvdLanguage)
				{
					if (_wcsicmp(pvdAssignment->szKey, PrivateBuild) != 0)
					{
						fvd.addTwostr(pvdAssignment->szKey, pvdAssignment->Data());
					}
					pvdAssignment = pvdAssignment->Next();
				}
			}
		}
	}
	bool bPatchVersionResource = false;
	while (LPCWSTR lpKey =
		(lpwszAhead = IsSfxSwitch(lpwszValue, &Version[true]))		!= NULL	? MAKEINTRESOURCE(offsetof(file_ver_data, m_fxi.dwFileVersionLS)) :
		(lpwszAhead = IsSfxSwitch(lpwszValue, L"FileVersionNo"))	!= NULL	? MAKEINTRESOURCE(offsetof(file_ver_data, m_fxi.dwFileVersionMS)) :
		(lpwszAhead = IsSfxSwitch(lpwszValue, L"ProductVersionNo"))	!= NULL	? MAKEINTRESOURCE(offsetof(file_ver_data, m_fxi.dwProductVersionMS)) :
		(lpwszAhead = IsSfxSwitch(lpwszValue, FileVersion))			!= NULL	? FileVersion :
		(lpwszAhead = IsSfxSwitch(lpwszValue, ProductVersion))		!= NULL	? ProductVersion :
		(lpwszAhead = IsSfxSwitch(lpwszValue, CompanyName))			!= NULL	? CompanyName :
		(lpwszAhead = IsSfxSwitch(lpwszValue, FileDescription))		!= NULL	? FileDescription :
		(lpwszAhead = IsSfxSwitch(lpwszValue, InternalName))		!= NULL	? InternalName :
		(lpwszAhead = IsSfxSwitch(lpwszValue, LegalCopyright))		!= NULL	? LegalCopyright :
		(lpwszAhead = IsSfxSwitch(lpwszValue, OriginalFilename))	!= NULL	? OriginalFilename :
		(lpwszAhead = IsSfxSwitch(lpwszValue, PrivateBuild))		!= NULL	? PrivateBuild :
		(lpwszAhead = IsSfxSwitch(lpwszValue, SpecialBuild))		!= NULL	? SpecialBuild :
		(lpwszAhead = IsSfxSwitch(lpwszValue, ProductName))			!= NULL	? ProductName :
		NULL)
	{
		CSfxStringU Value;
		SKIP_WHITESPACES_W(lpwszAhead);
		lpwszValue = LoadQuotedString(lpwszAhead, Value);
		if (IS_INTRESOURCE(lpKey))
		{
			LPBYTE p = reinterpret_cast<LPBYTE>(&fvd) + reinterpret_cast<WORD>(lpKey);
			memset(p, 0, 8);
			swscanf(Value, L"%hu.%hu.%hu.%hu", p + 2, p + 0, p + 6, p + 4);
			switch (reinterpret_cast<WORD>(lpKey))
			{
			case offsetof(file_ver_data, m_fxi.dwFileVersionLS):
				fvd.m_fxi.dwFileVersionMS = fvd.m_fxi.dwFileVersionLS;
				fvd.m_fxi.dwFileVersionLS = fvd.m_fxi.dwProductVersionMS;
				fvd.m_fxi.dwProductVersionMS = fvd.m_fxi.dwFileVersionMS;
				fvd.m_fxi.dwProductVersionLS = fvd.m_fxi.dwFileVersionLS;
				lpKey = &Version[false];
				break;
			case offsetof(file_ver_data, m_fxi.dwFileVersionMS):
				lpKey = FileVersion;
				break;
			case offsetof(file_ver_data, m_fxi.dwProductVersionMS):
				lpKey = ProductVersion;
				break;
			}
		}
		fvd.addTwostr(lpKey, Value);
		bPatchVersionResource = true;
	}
	if (bPatchVersionResource)
	{
		BYTE buf[8192];
		if (UInt32 cb = fvd.makeVersionResource(buf, sizeof buf))
		{
			BOOL fUpdate = UpdateResourceW(hUpdate, RT_VERSION, MAKEINTRESOURCE(VS_VERSION_INFO), 0, buf, cb);
			(fUpdate ? fDiscard : fSuccess) = FALSE;
		}
	}
	if (!EndUpdateResourceW(hUpdate, fDiscard) || !fSuccess)
		return false;
	// Append archives as specified
	if (!OutFile.Open(OutFileName, OPEN_EXISTING) || !OutFile.SeekToEnd(newPosition))
		return false;
	while (*lpwszValue)
	{
		CSfxStringU InFileName;
		lpwszValue = LoadQuotedString(lpwszValue, InFileName);
		CInFile InFile;
		if (!InFile.Open(InFileName))
			return false;
		do
		{
			BYTE buf[8192];
			UInt32 remainingSize;
			if (!InFile.Read(buf, sizeof buf, remainingSize))
				return false;
			if (!OutFile.Write(buf, remainingSize, processedSize) || (processedSize != remainingSize))
				return false;
		} while (processedSize != 0);
	}
	return true;
}

static int Run()
{
#ifdef _DEBUG
	DWORD dwStartTime = ::GetTickCount();
#endif // -DEBUG
	int ShortcutDefault = -1;
	int DeleteDefault = -1;
	int SelfDelete = -1;
	LPCWSTR lpwszValue;
	// Command line
	CSfxStringU tmpstr;
	CSfxStringU	SfxConfigFilename;
	CSfxStringU strCmdLine;

	SfxInit();

	strCmdLine = LoadQuotedString( ::GetCommandLine(), tmpstr );
	ReplaceHexChars( strCmdLine );
	LPCWSTR str = strCmdLine;
#ifdef _SFX_USE_ENVIRONMENT_VARS
	SfxAddEnvironmentVarWithAlias( SFX_ENV_VAR_MODULE_PLATFORM, PLATFORM_NAME_W );
	SfxAddEnvironmentVarWithAlias( SFX_ENV_VAR_SYSTEM_PLATFORM, GetPlatformName() );
	SfxAddEnvironmentVarWithAlias( SFX_ENV_VAR_CMDLINE_0, ::GetCommandLine() );
#endif // _SFX_USE_ENVIRONMENT_VARS

#ifdef _SFX_USE_LANG
	#ifdef _SFX_USE_ENVIRONMENT_VARS
		tmpstr.ReleaseBuf_SetEnd( wsprintf( tmpstr.GetBuf(32), L"%d", idSfxLang ) );
		SfxAddEnvironmentVarWithAlias( SFX_ENV_VAR_SYSTEM_LANGUAGE, tmpstr );
	#endif // _SFX_USE_ENVIRONMENT_VARS

	if( (lpwszValue = IsSfxSwitch( str, CMDLINE_SFXLANG )) != NULL && *lpwszValue == L':' )
	{
		long idLang=StringToLong( lpwszValue+1 );
		if( idLang > 0 && idLang <= 0xffff )
			idSfxLang = (LANGID)idLang;
		str++;
		while( ((unsigned)*str) > L' ' ) str++;
	}
#endif // _SFX_USE_LANG

	if( IsSfxSwitch( str, CMDLINE_SFXVERSION ) != NULL )
	{
		ShowSfxVersion();
		return ERRC_NONE;
	}

#ifdef _SFX_USE_PREFIX_WAITALL
	if( (lpwszValue = IsSfxSwitch(str, CMDLINE_SFXWAITALL)) != NULL )
	{
		return Child_ExecuteSfxWaitAll( lpwszValue );
	}
#endif // _SFX_USE_PREFIX_WAITALL

#ifdef _SFX_USE_ELEVATION
	bool fInElevation = false;
	if( (lpwszValue = IsSfxSwitch(str, CMDLINE_SFXELEVATION)) != NULL )
	{
		fInElevation = true;
		str = lpwszValue;
	}
#endif // _SFX_USE_ELEVATION

	if( (lpwszValue = IsSfxSwitch( str, CMDLINE_SFXTEST )) != NULL )
#ifdef _SFX_USE_TEST
	{
		if( lpwszValue[0] != L':' )
			return ERRC_SFXTEST;
		switch( MyCharLower_Ascii( lpwszValue[1] ) )
		{
		case L'd':
			// dialogs test
			if( lpwszValue[1] == L'D' )
			{
				if( lpwszValue[2] != L':' )
					return ERRC_SFXTEST;
				lpwszValue += 3;
				while( ((unsigned)*lpwszValue) > L' ' && *lpwszValue != L':' )
				{
					TSD_Flags += *lpwszValue++;
				}
				if( TSD_Flags.IsEmpty() )
					return ERRC_SFXTEST;
			}
			else
				lpwszValue += 2;
			TSD_ExtractTimeout = 0;
			if( *lpwszValue == L':' )
			{
				TSD_ExtractTimeout=StringToLong( lpwszValue+1 );
			}
			if( TSD_ExtractTimeout > 3600 || TSD_ExtractTimeout == 0 )
					TSD_ExtractTimeout = 10;
			nTestModeType = TMT_DIALOGS;
			break;
		case L'a':
			nTestModeType = TMT_ARCHIVE;
			break;
		case L'c':
			if( lpwszValue[2] == L'c' )
				nTestModeType = TMT_CHECK_CONFIG;
			else
				nTestModeType = TMT_DIALOGS_TO_STDOUT;
			break;
		case L'o':
			if( lpwszValue[2] != L':' )
				return 0;
			switch( lpwszValue[3] )
			{
			case '1':
				return SFXBUILD_OPTIONS1;
			case '2':
				return SFXBUILD_OPTIONS2;
			case '3':
				return SFXBUILD_OPTIONS3;
			case '4':
				return SFXBUILD_OPTIONS4;
			case '5':
				return SFXBUILD_OPTIONS5;
			default:
				return 0;
			}
		case L'v':
			return 0x4000 | VERSION_REVISION;
		default:
			return ERRC_SFXTEST;
		}

		while( ((unsigned)*lpwszValue) > L' ' ) lpwszValue++;
		str = lpwszValue;

		if( (lpwszValue = IsSfxSwitch( str,CMDLINE_SFXCONFIG )) != NULL )
		{
			if( *lpwszValue == L':' ) lpwszValue++;
			SKIP_WHITESPACES_W( lpwszValue );
			tmpstr.Empty();
			str = LoadQuotedString( lpwszValue, tmpstr );
			if( nTestModeType != TMT_ARCHIVE )
			{
				strModulePathName = tmpstr;
				CInFile InFile;
				UInt64 fileSize;
				if (!InFile.Open(tmpstr) || !InFile.GetLength(fileSize))
					return ERRC_SFXTEST;
				CBuffer<Byte> inBuffer(static_cast<size_t>(fileSize));
				UInt32 inSize = static_cast<UInt32>(fileSize);
				if (inSize != fileSize)
					return ERRC_SFXTEST; // File is too big
				UInt32 processedSize;
				if (!InFile.Read(inBuffer, inSize, processedSize) || (processedSize != inSize))
					return ERRC_SFXTEST;
				if (LoadAndParseConfig(inBuffer, inSize) != 0)
					return ERRC_SFXTEST;
			}
		}
	}
#else
	{
		return ERRC_SFXTEST;
	}
#endif // _SFX_USE_TEST

#ifdef _DEBUG
	if( strModulePathName.IsEmpty() )
	{
		strModulePathName = L"S:\\tmp\\setup_button_x86_x64.exe";
		strModulePathName = L"S:\\tmp\\Aida64AE.exe";
		strModulePathName = L"C:\\tmp\\vars.exe";
		hRsrcModule = LoadLibraryEx(strModulePathName, NULL, LOAD_LIBRARY_AS_DATAFILE);
	}
#else
	strModulePathName.ReleaseBuf_SetEnd(
		::GetModuleFileName( NULL, strModulePathName.GetBuf(MAX_PATH), MAX_PATH ) );
	if( strModulePathName.IsEmpty() )
	{
		SfxErrorDialog( TRUE, ERR_MODULEPATHNAME );
		return ERRC_GET_PATHNAME;
	}
#endif

	if( (lpwszValue = IsSfxSwitch( str, CMDLINE_SFXCREATE )) != NULL )
	{
		if (!CreateSelfExtractor( strModulePathName, lpwszValue ))
		{
			// Existing errmsg for CMDLINE_SFXCONFIG seems reasonable here
			SfxErrorDialog( FALSE, ERR_WRITE_CONFIG );
			return ERRC_CONFIG_DATA;
		}
		return ERRC_NONE;
	}

#ifdef _SFX_USE_TEST
	if( nTestModeType == 0 && (lpwszValue = IsSfxSwitch( str,CMDLINE_SFXCONFIG )) != NULL )
#else
	if( (lpwszValue = IsSfxSwitch( str, CMDLINE_SFXCONFIG )) != NULL )
#endif // _SFX_USE_TEST
	{
		SKIP_WHITESPACES_W( lpwszValue );
		lpwszValue = LoadQuotedString(lpwszValue, extractRoot);
		if (!ExtractResources(hRsrcModule, RT_RCDATA, extractRoot + L"\\adjunct") &&
				GetLastError() != ERROR_RESOURCE_TYPE_NOT_FOUND ||
			!ExtractResources(hRsrcModule, RT_HTML, extractRoot + L"\\config") &&
				GetLastError() != ERROR_RESOURCE_TYPE_NOT_FOUND ||
			!ExtractResources(hRsrcModule, RT_MANIFEST, extractRoot + L"\\manifest") &&
				GetLastError() != ERROR_RESOURCE_TYPE_NOT_FOUND)
		{
			SfxErrorDialog( FALSE, ERR_WRITE_CONFIG );
			return ERRC_CONFIG_DATA;
		}
		return ERRC_NONE;
	}

	strSfxFolder = strModulePathName;
	strSfxName = strModulePathName;
	int nPos = strModulePathName.ReverseFind_PathSepar();
	if( nPos >= 0 )
	{
		strSfxFolder.ReleaseBuf_SetEnd( nPos );
		strSfxName = strModulePathName.Ptr( nPos + 1 );
		strTitle = strModulePathName.Ptr( nPos + 1 );
		// Sanity check the title string
		if( strTitle.Find( L'.' ) >= 64 )
		{
			SfxErrorDialog( FALSE, ERR_READ_CONFIG );
			return ERRC_CONFIG_DATA;
		}
		if( (nPos = strTitle.ReverseFind( L'.' )) > 0 )
			strTitle.ReleaseBuf_SetEnd( nPos );
		strErrorTitle = strTitle;
		strErrorTitle += GetLanguageString( STR_ERROR_SUFFIX );
		lpwszTitle = strTitle;
		lpwszErrorTitle = strErrorTitle;
#ifdef SFX_CRYPTO
		lpwszPasswordTitle = lpwszTitle;
#endif // SFX_CRYPTO
	}
#ifdef _SFX_USE_PREFIX_PLATFORM
	strOSPlatform = GetPlatformName();
#endif // _SFX_USE_PREFIX_PLATFORM

	if( gSfxArchive.Init(strModulePathName) == false )
	{
		SfxErrorDialog( TRUE, ERR_OPEN_ARCHIVE, (LPCWSTR)strModulePathName );
		return ERRC_OPEN_ARCHIVE;
	}

	// Read SFX config
	LoadAndParseConfig(hRsrcModule);

#ifdef _SFX_USE_TEST
	if( nTestModeType == TMT_CHECK_CONFIG )
		return ERRC_NONE;
#endif // _SFX_USE_TEST
	//
	LANGSTRING * pLangString = SfxLangStrings;
	while( pLangString->id != 0 )
	{
		WCHAR wszVarName[sizeof(SFX_STRING_FORMAT)/sizeof(WCHAR)+32];
		wsprintf( wszVarName, SFX_STRING_FORMAT, pLangString->id );
		SfxAddEnvironmentVar( wszVarName, GetLanguageString(pLangString->id) );
		pLangString++;
	}

	// parse command line
	if( (str=ParseCommandLineParameters(str)) == NULL )
		return ERR_CONFIG_CMDLINE;
	SetConfigVariables();

	gSfxArchive.Open();

#ifdef _SFX_USE_ELEVATION
// Check elevation
	if( fInElevation == false && (MiscFlags & MISCFLAGS_ELEVATE) != 0 && IsRunAsAdmin() == FALSE )
	{
		CSfxStringU sfxPath;
		CSfxStringU executeString;
		CSfxStringU strCmdLine = LoadQuotedString( ::GetCommandLine(), sfxPath );
		executeString = L"\"" + sfxPath + L"\" -" + CMDLINE_SFXELEVATION + L' ' + strCmdLine;
		::SetProcessWorkingSetSize( ::GetCurrentProcess(), (SIZE_T)-1, (SIZE_T)-1 );
		if( SfxExecute( executeString, SFXEXEC_RUNAS, NULL ) == FALSE )
			return ERRC_ELEVATE;
		return ERRC_NONE;
	}
#endif // _SFX_USE_ELEVATION

	// extra environment variables
	int		nIndex = 0;
	while( (lpwszValue = GetTextConfigValue( CFG_SETENVIRONMENT, &nIndex )) != NULL )
	{
		CSfxStringU strEnvName = lpwszValue;
		nPos = strEnvName.Find( L'=' );
		if( nPos <= 0 )
			break;
		strEnvName.ReleaseBuf_SetEnd( nPos );
		SfxAddEnvironmentVar( strEnvName, lpwszValue + nPos + 1 );
		nIndex++;
	}

	SetEnvironment();

	if( GUIFlags == -1 )
		GUIFlags = 0;

	if( gSfxArchive.GetShowHelp() )
	{
		if( (lpwszValue = GetTextConfigValue(CFG_HELP_TEXT)) == NULL )
				lpwszValue = GetLanguageString( STR_DEFAULT_HELP_TEXT );
		CSfxDialog_HelpText	dlg;
		dlg.Show( SD_OK|SD_ICONINFORMATION, lpwszTitle, lpwszValue );
		return ERRC_NONE;
	}

	if( gSfxArchive.GetAssumeYes() )
		GUIFlags &= ~GUIFLAGS_CONFIRM_CANCEL;

	CoInitializeEx( NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );

	// Path to extract
	if( (lpwszValue = GetTextConfigValue(CFG_INSTALLPATH)) != NULL )
	{
		extractPath = lpwszValue;
		fUseInstallPath = true;
	}

	ReplaceVariables( extractPath );

#ifdef _SFX_USE_BEGINPROMPTTIMEOUT
	if( (lpwszValue = GetTextConfigValue( CFG_BEGINPROMPTTIMEOUT)) != NULL )
		BeginPromptTimeout = StringToLong( lpwszValue );
#endif // _SFX_USE_BEGINPROMPTTIMEOUT

#ifdef _SFX_USE_TEST
	if( TSD_Flags.IsEmpty() == false )
	{
		return TestSfxDialogs();
	}
	if( nTestModeType == TMT_DIALOGS_TO_STDOUT )
	{
		return TestSfxDialogsToStdout();
	}
#endif // _SFX_USE_TEST

#ifdef _SFX_USE_CONFIG_EARLY_EXECUTE
	if( GetTextConfigValue(CFG_EXECUTE_ON_LOAD) != NULL && gSfxArchive.GetNoRun() == false )
	{
		CSfxStringU ustrDirPrefix;
		ExecuteBatch( CFG_EXECUTE_ON_LOAD, strSfxFolder, L"", ustrDirPrefix, L"" );
	}
#endif // _SFX_USE_CONFIG_EARLY_EXECUTE

#ifdef _DEBUG
	DWORD dwTime = ::GetTickCount() - dwStartTime;
	CHAR msg[128];
	wsprintfA( msg, "Time: %u msec", dwTime );
	::MessageBoxA( NULL, msg, "Time", MB_OK );
#endif // -DEBUG
	// Begin prompt
Loc_BeginPrompt:
	if( (lpwszValue = GetTextConfigValue(CFG_BEGINPROMPT)) != NULL )
	{
		if( gSfxArchive.GetAssumeYes() == false )
		{
			if( SfxBeginPrompt( lpwszTitle, lpwszValue ) == FALSE )
			{
				return ERRC_CANCELED;
			}
			if( GetKeyState(VK_SHIFT)&0x8000 )
			{
				gSfxArchive.SetAutoInstall(true);
				gSfxArchive.SetAssumeYes(true);
			}
#ifdef _SFX_USE_BEGINPROMPTTIMEOUT
			if( CSfxDialog::IsTimedOut() )
				GUIFlags &= ~GUIFLAGS_EXTRACT_PATH2;
#endif // _SFX_USE_BEGINPROMPTTIMEOUT
		}
	}

	// check autoinstall
	CSfxStringU	executeName;
	if( gSfxArchive.GetAutoInstall() )
	{
		LPCWSTR p1 = gSfxArchive.GetBatchInstall();
		do {
			executeName = CFG_AUTOINSTALL;
			executeName += *p1;
			if( GetTextConfigValue( executeName ) == NULL )
			{
				// auto install command not found
				SfxErrorDialog( FALSE, ERR_AUTOINSTALL_NOTFOUND, executeName.Ptr() );
				return ERRC_BAD_AUTOINSTALL;
			}
			p1++;
		} while( (*p1 >= L'0' && *p1 <= L'9') ||
				 (*p1 >= L'a' && *p1 <= L'z') ||
				 (*p1 >= L'A' && *p1 <= L'Z') );
		executeName = CFG_AUTOINSTALL;
	}

	bool fUseExecuteFile = false;
	// check for 'ExecuteFile' (only if no 'autoinstall' commands)
	if( executeName.IsEmpty() && GetTextConfigValue( CFG_EXECUTEFILE ) != NULL )
	{
		executeName = CFG_EXECUTEFILE;
		fUseExecuteFile = true;
	}
	// if no 'ExecuteFile' - try 'RunProgram'
	if( executeName.IsEmpty() && GetTextConfigValue( CFG_RUNPROGRAM ) != NULL )
		executeName = CFG_RUNPROGRAM;

	// Path to extract
	if( gSfxArchive.GetAutoInstall() == false && gSfxArchive.GetAssumeYes() == false &&
			(GUIFlags&(GUIFLAGS_EXTRACT_PATH2|GUIFLAGS_EXTRACT_PATH1)) == GUIFLAGS_EXTRACT_PATH2 &&
				SfxExtractPathDialog( lpwszExtractPathTitle, lpwszExtractPathText ) == FALSE )
	{
		if( fUseBackward )
			goto Loc_BeginPrompt;
		return ERRC_CANCELED;
	}

	if( unsigned len = extractPath.Len() )
	{
		--len;
		if( extractPath[len] == L'\\' || extractPath[len] == L'/' )
			extractPath.ReleaseBuf_SetEnd( len );
	}

	if( extractPath.IsEmpty() || extractPath.Find( L"%%T" ) != -1 )
	{
		extractRoot = CreateTempName( L"7ZipSfx.%03x" );
		fUseInstallPath = false;
	}
	else
	{
		extractRoot = extractPath;
		fUseInstallPath = true;
	}
	ReplaceVariablesEx( extractPath );
	if( extractPath.IsEmpty() )
		extractPath = extractRoot;

	if( gSfxArchive.GetAssumeYes() )
		MiscFlags |= MISCFLAGS_NO_CHECK_DISK_FREE | MISCFLAGS_NO_CHECK_RAM;

	ExtractResources(hRsrcModule, RT_RCDATA, extractRoot);

#ifdef _SFX_USE_TEST
	HRESULT hrStatus;
	if( TSD_ExtractTimeout == 0 )
		hrStatus = ExtractArchive( extractPath );
	else
	{
		if( ExtractDialog(NULL) )
			hrStatus = S_OK;
		else
			hrStatus = E_FAIL;
	}
#else
	HRESULT hrStatus = ExtractArchive( extractPath );
#endif // _SFX_USE_TEST

	// delete temporary folder
	if( hrStatus != S_OK )
	{
		SfxCleanup();
		return ERRC_EXTRACT;
	}

	SetEnvironment();

	if( gSfxArchive.GetNoRun() == false || fUseInstallPath == false )
	{
		// execute program
		CSfxStringU	ustrDirPrefix;
		CSfxStringU	ustrRunProgram;
		if( gSfxArchive.GetAutoInstall() == false )
			GetConfigDirectory( ustrDirPrefix );

		if( executeName.IsEmpty() )
		{
			if( fUseInstallPath == false )
			{
				lpwszValue = L"setup.exe";
				ustrRunProgram = extractPath + L"\\" + lpwszValue;
				if( ::GetFileAttributes( ustrRunProgram ) == (DWORD)-1 )
				{
					// nothing to execute
					SfxCleanup();
					SfxErrorDialog( FALSE, ERR_NO_SETUP_EXE );
					return ERRC_NOTHING_TO_EXECUTE;
				}
				ExecuteConfigProgram( lpwszValue, extractPath, fUseExecuteFile, ustrDirPrefix, str );
			}
		}
		else
		{
			ExecuteBatch( executeName, extractPath, gSfxArchive.GetBatchInstall(), ustrDirPrefix, str );
		}

#ifdef _SFX_USE_TEST
		if( nTestModeType == 0 )
		{
#endif // _SFX_USE_TEST
		// create shortcuts
#define SH_TEST
		SetEnvironment();
		ProcessPostExecuteSub( PostExecute_Shortcut, CFG_SHORTCUT, gSfxArchive.GetBatchInstall(), ShortcutDefault );

		::SetCurrentDirectory( strSfxFolder );
		ProcessPostExecuteSub( PostExecute_Delete, CFG_DELETE, gSfxArchive.GetBatchInstall(), DeleteDefault );

		SfxCleanup();
#ifdef _SFX_USE_TEST
		}
#endif // _SFX_USE_TEST
	}
	if( FinishMessage == -1 && gSfxArchive.GetAssumeYes() == false )
	{
		FinishMessage = 1;
	}
	if( FinishMessage > 0 && (lpwszValue = GetTextConfigValue( CFG_FINISHMESSAGE )) != NULL )
	{
		if( FinishMessage > FINISHMESSAGE_MAX_TIMEOUT )
			FinishMessage = FINISHMESSAGE_MAX_TIMEOUT;
		CSfxDialog_FinishMessage	dlg;
		dlg.Show( SD_OK|SD_ICONINFORMATION, lpwszTitle, lpwszValue );
	}

	// self delete
#ifdef _SFX_USE_TEST
	if( gSfxArchive.GetNoRun() == false && nTestModeType == 0 )
#else
	if( gSfxArchive.GetNoRun() == false )
#endif // _SFX_USE_TEST
	{
		switch( SelfDelete )
		{
		case 0:
			break;
		case 1:
			DeleteSFX( strModulePathName );
			break;
		default:
			if( (lpwszValue = GetTextConfigValue(CFG_SELFDELETE)) != NULL && lpwszValue[0] == L'1' )
				DeleteSFX( strModulePathName );
			break;
		}
	}

	return ERRC_NONE;
}

void DeleteSFX( LPCWSTR moduleName )
{
	if( moduleName[1] == L':' && (moduleName[2] == L'\\' || moduleName[2] == L'/') )
	{
		WCHAR szRoot[4];
		szRoot[0] = moduleName[0];
		szRoot[1] = L':'; szRoot[2] = L'\\'; szRoot[3] = L'\0';
		if( ::GetDriveType(szRoot) != DRIVE_FIXED )
			return;
	}
	else
	{
		return;
	}

	CSfxStringU tmpPath = CreateTempName( L"7ZSfx%03x.cmd");

	HANDLE hfCmd = ::CreateFile( tmpPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hfCmd != INVALID_HANDLE_VALUE )
	{
		CSfxStringU cmdBody;
		cmdBody = L":Repeat\r\n";
		cmdBody += L"del \""; cmdBody += moduleName; cmdBody += L"\"\r\n";
		cmdBody += L"if exist \""; cmdBody += moduleName; cmdBody += L"\" goto Repeat\r\n";
		cmdBody += L"del \""; cmdBody += tmpPath; cmdBody += L"\"\r\n";
		DWORD dwWrited;
		BOOL fWrited = WriteFile( hfCmd, SfxUnicodeStringToMultiByte(cmdBody, CP_OEMCP), cmdBody.Len(), &dwWrited, NULL );
		::CloseHandle( hfCmd );
		if( fWrited == FALSE || dwWrited != (DWORD)cmdBody.Len() )
		{
			DeleteFileOrDirectoryAlways( tmpPath );
			return;
		}
		SetFileAttributes( moduleName, 0 );
		// execute cmd
		ShellExecute( NULL, L"open", tmpPath, NULL, NULL, SW_HIDE );
	}
}

#include <new.h>

static int sfx_new_handler(size_t)
{
	FatalAppExitA(0, "Could not allocate memory");
	return 0;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR , int)
{
	int ret = 0;
	_set_new_handler(sfx_new_handler);
	_set_new_mode(1);
	__try
	{
		hRsrcModule = hInstance;
		ret = Run();
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		char msg[256];
		wsprintfA(msg, "An exception of type 0x%08lX has occurred", GetExceptionCode());
		FatalAppExitA(0, msg);
	}
	return ret;
}
