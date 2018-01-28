/*---------------------------------------------------------------------------*/
/* File:        cmdline.cpp                                                  */
/* Created:     Sun, 06 Mar 2016 01:44:14 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Sun, 28 Jan 2018 by https://github.com/datadiode             */
/*---------------------------------------------------------------------------*/
/* Revision:    12                                                           */
/* Updated:     Thu, 17 Mar 2016 06:22:46 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "7zSfxModInt.h"

#define IsCommandLineSwitch( lpwszCommandLine, lpwszSwitch )	\
	IsSubString( lpwszCommandLine + 1, lpwszSwitch )

LPCWSTR ParseConfigOverride( LPCWSTR lpwszCommandLine )
{
	static LPCWSTR const ConfigParams[] = {
		CFG_TITLE,
		CFG_ERRORTITLE,
#ifdef _SFX_USE_WARNINGS
		CFG_WARNINGTITLE,
#endif // _SFX_USE_WARNINGS
		CFG_GUIMODE,
		CFG_GUIFLAGS,
		CFG_MISCFLAGS,
		CFG_BEGINPROMPT,
		CFG_INSTALLPATH,
		CFG_EXTRACT_TITLE,
		CFG_EXTRACT_CANCELTEXT,
		CFG_EXTRACT_DIALOGTEXT,
		CFG_EXTRACT_DIALOGWIDTH,
		CFG_SELFDELETE,
		CFG_EXTRACT_PATH_TITLE,
		CFG_EXTRACT_PATH_TEXT,
		CFG_HELP_TEXT,
		CFG_OVERWRITE_MODE,
		CFG_CANCEL_PROMPT,
		CFG_EXTRACTPATH_WIDTH,
		CFG_FINISHMESSAGE,
		CFG_EXECUTEFILE,
		CFG_EXECUTEPARAMETERS,
		CFG_DIRECTORY,
		CFG_PROGRESS,
		CFG_SETENVIRONMENT,
#ifdef _SFX_USE_BEGINPROMPTTIMEOUT
		CFG_BEGINPROMPTTIMEOUT,
#endif // _SFX_USE_BEGINPROMPTTIMEOUT
#ifdef SFX_CRYPTO
		CFG_PASSWORD_TITLE,
		CFG_PASSWORD_TEXT,
#endif // SFX_CRYPTO
#ifdef _SFX_USE_VOLUME_NAME_STYLE
		CFG_VOLUME_NAME_STYLE,
#endif // _SFX_USE_VOLUME_NAME_STYLE
#ifdef _SFX_USE_CONFIG_EARLY_EXECUTE
		CFG_EXECUTE_ON_LOAD,
		CFG_PREEXTRACT,
#endif // _SFX_USE_CONFIG_EARLY_EXECUTE
#ifdef _SFX_USE_EXTRACT_MASK
		CFG_EXTRACTMASK_INCLUDE,
		CFG_EXTRACTMASK_EXCLUDE,
#endif // _SFX_USE_EXTRACT_MASK
		NULL
	};

	LPCWSTR const* pParams = ConfigParams;
	while( *pParams != NULL )
	{
		int const nLen = static_cast<int>(wcslen(*pParams));
		if( _wcsnicmp( lpwszCommandLine, *pParams, nLen ) == 0 && lpwszCommandLine[nLen] == L'=' )
		{
			CSfxStringU str = lpwszCommandLine;
			LPCWSTR p = str;
			bool inQuotes=false;
			while( *p )
			{
				if( unsigned(*p) <= L' ' && inQuotes == false )
					break;
				if( *p == L'\"' )
				{
					if( inQuotes == false )
						inQuotes = true;
					else
						inQuotes = false;
				}
				else
				{
					if( p[0] == L'\\' && p[1] == L'\"' )
						p++;
				}
				p++;
			}
			int nSwitchLen = (int)(p-(LPCWSTR)str);
			str.ReleaseBuf_SetEnd( nSwitchLen );
			CSfxStringA	aStr= SfxUnicodeStringToMultiByte( str, CP_UTF8 );
			if( GetTextConfig( aStr, true ) == false )
				return (LPCWSTR)1;
			return lpwszCommandLine + nSwitchLen;
		}
		pParams++;
	}
	return NULL;
}

void OverrideConfigParam( LPCWSTR lpwszName, LPCWSTR lpwszValue )
{
	CSfxStringU strParam = lpwszName;
	strParam += L"=\"";
	while( *lpwszValue > L' ' ) strParam += *lpwszValue++;
	strParam += L"\"";
	ParseConfigOverride( strParam );
}

LPCWSTR ParseCommandLineParameters(LPCWSTR str)
{
#ifdef _SFX_USE_ENVIRONMENT_VARS
	CSfxStringU	tmpstr;
	LPCWSTR lpwszCmdLineStart = str;
	LPCWSTR lpwszCmdLineEnd = str + wcslen(str);
	LPCWSTR lpwszCmdLine1End = NULL;
	SKIP_WHITESPACES_W( lpwszCmdLineStart );
#endif // _SFX_USE_ENVIRONMENT_VARS

	switch( 0 ) for ( ;; )
	{
		while( ((unsigned)*str) > L' ' ) str++;
	case 0: // first iteration starts here
		SKIP_WHITESPACES_W( str );
		if( str[0] != L'/' && str[0] != L'-' )
			break;
		// '-!'
		if( str[1] == L'!' )
		{
#ifdef _SFX_USE_ENVIRONMENT_VARS
			lpwszCmdLine1End = str;
#endif // _SFX_USE_ENVIRONMENT_VARS
			str += 2;
			break;
		}

		// 'ai', 'aiX, 'aiXXX'
		if( IsCommandLineSwitch( str, L"ai" ) )
		{
			// first check batch mode
			if( (str[3] >= L'0' && str[3] <= '9') ||
				(str[3] >= L'a' && str[3] <= 'z') ||
				(str[3] >= L'A' && str[3] <= 'Z') )
			{
				gSfxArchive.SetBatchInstall( str + 3 );
				gSfxArchive.SetAutoInstall(true);
				gSfxArchive.SetAssumeYes(true);
				continue;
			}
			// sample autoinstall
			if( ((unsigned)str[3]) <= L' ' )
			{
				gSfxArchive.SetBatchInstall( L"\0" );
				gSfxArchive.SetAutoInstall(true);
				gSfxArchive.SetAssumeYes(true);
				continue;
			}
			break;
		}

		// 'om' - Overwrite mode
		if( IsCommandLineSwitch( str, L"om" ) )
		{
			OverrideConfigParam( CFG_OVERWRITE_MODE, str + 3 );
			continue;
		}

		// 'gm' - GUI mode
		if( IsCommandLineSwitch( str, L"gm" ) )
		{
			if( str[3] >= L'0' && str[3] <= (L'0' + GUIMODE_MAX) && str[4] <= L' ' )
			{
				OverrideConfigParam( CFG_GUIMODE, str + 3 );
				continue;
			}
			break;
		}

		// 'gf' - GUI flags
		if( IsCommandLineSwitch( str, L"gf" ) )
		{
			OverrideConfigParam( CFG_GUIFLAGS, str + 3 );
			continue;
		}

		// 'mf' - Misc flags
		if( IsCommandLineSwitch( str, L"mf" ) )
		{
			OverrideConfigParam( CFG_MISCFLAGS, str + 3 );
			continue;
		}

		// 'sd' - self delete
		if( IsCommandLineSwitch( str, L"sd" ) )
		{
			if( (str[3] == L'0' || str[3] == L'1') && ((unsigned)str[4]) <= L' ' )
			{
				OverrideConfigParam( CFG_SELFDELETE,str+3 );
				continue;
			}
			break;
		}

		// 'nr' - no run
		if( IsCommandLineSwitch( str, L"nr" ) )
		{
			if( ((unsigned)str[3]) <= L' ' )
			{
				gSfxArchive.SetNoRun(true);
				continue;
			}
			break;
		}

		// 'fm' - finish message
		if( IsCommandLineSwitch( str, L"fm" ) )
		{
			if( str[3] >= L'0' && str[3] <= L'9' )
			{
				FinishMessage = StringToLong( str + 3 );
				continue;
			}
			break;
		}

#ifdef _SFX_USE_BEGINPROMPTTIMEOUT
		// 'bpt' - BeginPrompt timeout
		if( IsCommandLineSwitch( str, L"bpt" ) )
		{
			OverrideConfigParam( CFG_BEGINPROMPTTIMEOUT, str + 4 );
			continue;
		}
#endif // _SFX_USE_BEGINPROMPTTIMEOUT
#ifdef SFX_CRYPTO
		if( str[1] == L'p' || str[1] == L'P' )
		{
			CSfxStringU password;
			str = LoadQuotedString( str + 2, password ) - 1;
			CSfxArchive::CPassword::Set( password );
			continue;
		}
#endif // SFX_CRYPTO

		// assume 'yes'
		if( (str[1] == L'y' || str[1] == L'Y') && ((unsigned)str[2]) <= L' ' )
		{
			gSfxArchive.SetAssumeYes(true);
			continue;
		}

		if( (str[1] == L'?' || str[1] == L'h' || str[1] == L'H') && ((unsigned)str[2]) <= L' ' )
		{
			gSfxArchive.SetShowHelp(true);
			continue;
		}

		if( LPCWSTR pnext = ParseConfigOverride( str + 1 ) )
		{
			if( pnext == (LPCWSTR)1 )
				return NULL;
			str = pnext;
			continue;
		}
		break;
	}

#ifdef _SFX_USE_ENVIRONMENT_VARS
	if( lpwszCmdLine1End == NULL )
		lpwszCmdLine1End = str;
	int tmpstrLen = static_cast<int>(lpwszCmdLine1End - lpwszCmdLineStart);
	memcpy( tmpstr.GetBuf_SetEnd( tmpstrLen ), lpwszCmdLineStart, tmpstrLen * sizeof *lpwszCmdLineStart );
	tmpstr.Trim();
	SfxAddEnvironmentVarWithAlias( SFX_ENV_VAR_CMDLINE_1, tmpstr );
	tmpstrLen = static_cast<int>(lpwszCmdLineEnd - str);
	memcpy( tmpstr.GetBuf_SetEnd( tmpstrLen ), str, tmpstrLen * sizeof *str );
	tmpstr.Trim();
	SfxAddEnvironmentVarWithAlias( SFX_ENV_VAR_CMDLINE_2, tmpstr );
#endif // _SFX_USE_ENVIRONMENT_VARS

	return str;
}