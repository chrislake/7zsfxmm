/*---------------------------------------------------------------------------*/
/* File:        execute.cpp                                                  */
/* Created:     Fri, 05 Feb 2016 10:45:46 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Tue, 31 Oct 2017 by https://github.com/datadiode             */
/*---------------------------------------------------------------------------*/
/* Revision:    37                                                           */
/* Updated:     Sat, 12 Mar 2016 11:24:03 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "7zSfxModInt.h"

void GetConfigDirectory( CSfxStringU & result )
{
	LPCWSTR lpwszValue;
	result.Empty();
	if( (lpwszValue = GetTextConfigValue( CFG_DIRECTORY)) != NULL )
	{
		result = lpwszValue;
		ReplaceVariablesEx( result );
		if( result[result.Len()-1] != L'\\' && result[result.Len()-1] != L'/' )
			result += L'\\';
	}
}

#ifdef _SFX_USE_PREFIX_WAITALL
DWORD Parent_ExecuteSfxWaitAll( LPCWSTR lpwszApp, LPCWSTR lpwszCmdLine, int flags )
{
	CSfxStringU sfxPath;
	CSfxStringU executeString;
	LoadQuotedString( ::GetCommandLine(), sfxPath );
	executeString = L"\"" + sfxPath + L"\" -" + CMDLINE_SFXWAITALL + L':' + (WCHAR)(flags+'0') + L' ' +
		L'\"' + lpwszApp + L"\" " + lpwszCmdLine;
	STARTUPINFO	si;
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);
	::GetStartupInfo(&si);
	if( ::CreateProcess(NULL, (LPWSTR)(LPCWSTR)executeString,NULL,NULL,TRUE,
		CREATE_BREAKAWAY_FROM_JOB|CREATE_SUSPENDED,NULL,(LPCTSTR)extractPath,&si,&pi) == FALSE )
		return ::GetLastError();
	bool fWaitProcess = true;
	HANDLE hJob = NULL;
	HANDLE hIocp = NULL;
	if( (hJob = ::CreateJobObject( NULL, NULL))!= NULL &&
		AssignProcessToJobObject( hJob,pi.hProcess ) != FALSE &&
		(hIocp = CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 1, 0 )) != NULL )
	{
		JOBOBJECT_ASSOCIATE_COMPLETION_PORT	iop;
		iop.CompletionKey = (PVOID)1;
		iop.CompletionPort = hIocp;
		SetInformationJobObject( hJob, JobObjectAssociateCompletionPortInformation, &iop, sizeof(iop) );
		::ResumeThread( pi.hThread );
		// Wait for events
		DWORD dwBytes;
		ULONG_PTR completionkey;
		LPOVERLAPPED ove;
		while( GetQueuedCompletionStatus( hIocp, &dwBytes, &completionkey, &ove, INFINITE) != FALSE )
		{
			if( dwBytes == JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO )
			{
				fWaitProcess = false;
				break;
			}
		}
	}
	if( fWaitProcess != false )
	{
		::ResumeThread( pi.hThread );
		::WaitForSingleObject( pi.hProcess, INFINITE );
	}
	::CloseHandle( pi.hThread );
	DWORD dwExitCode;
	if( ::GetExitCodeProcess( pi.hProcess, &dwExitCode ) == FALSE )
		dwExitCode = ::GetLastError();
	::CloseHandle( pi.hProcess );
	if( hIocp != NULL ) :: CloseHandle( hIocp );
	if( hJob != NULL ) ::CloseHandle( hJob );
	return dwExitCode;
}

DWORD Child_ExecuteSfxWaitAll( LPCWSTR lpwszCmdLine )
{
	DWORD dwFlags = 0;
	if( lpwszCmdLine[0] == L':' && lpwszCmdLine[1] >= L'0' && lpwszCmdLine[1] <= L'9' )
	{
		dwFlags=StringToLong( lpwszCmdLine+1 );
		while( ((unsigned)*lpwszCmdLine) > L' ' ) lpwszCmdLine++;
		SKIP_WHITESPACES_W( lpwszCmdLine);
	}
	if( SfxExecute( lpwszCmdLine, dwFlags, NULL ) == FALSE )
		return GetLastError();
	return ERROR_SUCCESS;
}
#endif // _SFX_USE_PREFIX_WAITALL

#ifdef _SFX_USE_PREFIX_PLATFORM
#if defined(_WIN64) && defined(_M_X64)
	/* exclude x86 (i386) prefixes */
	BOOL SfxPrepareExecute( int nPlatform )
	{
		switch( nPlatform )
		{
			case SFX_EXECUTE_PLATFORM_ANY:
			case SFX_EXECUTE_PLATFORM_AMD64:
				return TRUE;
		}
		return FALSE;
	}
#elif defined(_WIN32) && defined(_M_IX86)
	typedef BOOL (WINAPI * Wow64DisableWow64FsRedirection_Proc)( __out  PVOID *OldValue );
	typedef BOOL (WINAPI * Wow64RevertWow64FsRedirection_Proc)( __in  PVOID OldValue );

	static BOOL SfxExecute_fFsRedirectionSwitched = FALSE;
	static PVOID SfxExecute_pFsRedirectionState = NULL;

	void SfxFinalizeExecute()
	{
		if( SfxExecute_fFsRedirectionSwitched != FALSE )
		{
			Wow64RevertWow64FsRedirection_Proc pfnWow64RevertWow64FsRedirection = 
				(Wow64RevertWow64FsRedirection_Proc)::GetProcAddress( hKernel32, "Wow64RevertWow64FsRedirection" );
			if( pfnWow64RevertWow64FsRedirection != NULL )
			{
				pfnWow64RevertWow64FsRedirection( SfxExecute_pFsRedirectionState );
				SfxExecute_fFsRedirectionSwitched = FALSE;
			}
		}
	}

	void SfxDisableWow64FsRedirection()
	{
		SfxExecute_fFsRedirectionSwitched = FALSE;
		Wow64DisableWow64FsRedirection_Proc pfnWow64DisableWow64FsRedirection = 
			(Wow64DisableWow64FsRedirection_Proc)::GetProcAddress( hKernel32, "Wow64DisableWow64FsRedirection" );
		if( pfnWow64DisableWow64FsRedirection != NULL )
		{
			pfnWow64DisableWow64FsRedirection( &SfxExecute_pFsRedirectionState );
			SfxExecute_fFsRedirectionSwitched = TRUE;
		}
	}

	BOOL SfxPrepareExecute( int nPlatform )
	{
		if( nPlatform == SFX_EXECUTE_PLATFORM_ANY )
			return TRUE;
		DWORD dwPlatform = GetPlatform();
		if( dwPlatform == PROCESSOR_ARCHITECTURE_AMD64 && nPlatform == SFX_EXECUTE_PLATFORM_AMD64 )
		{
			SfxDisableWow64FsRedirection();
			return TRUE;
		}
		if( dwPlatform == PROCESSOR_ARCHITECTURE_INTEL && nPlatform == SFX_EXECUTE_PLATFORM_I386 )
			return TRUE;
		return FALSE;
	}
#endif
#endif // _SFX_USE_PREFIX_PLATFORM

BOOL SfxExecute( LPCWSTR lpwszCmdLine, DWORD dwFlags, LPCWSTR lpwszDirectory )
{
	CSfxStringU filePath;
	CSfxStringU fileParams;
	SHELLEXECUTEINFO execInfo;
	memset( &execInfo, 0, sizeof(execInfo) );
	execInfo.cbSize = sizeof(execInfo);
	execInfo.lpDirectory = lpwszDirectory;

	execInfo.fMask = 
		SEE_MASK_NOCLOSEPROCESS |
		SEE_MASK_FLAG_DDEWAIT |
		SEE_MASK_DOENVSUBST |
		SEE_MASK_FLAG_NO_UI;
	execInfo.nShow = SW_SHOWDEFAULT;

	if( (dwFlags&SFXEXEC_HIDCON) != 0 )
	{
		// use hidcon
		execInfo.nShow = SW_HIDE;
		execInfo.fMask |= SEE_MASK_NO_CONSOLE;
	}
	if( (dwFlags&SFXEXEC_RUNAS) != 0 )
		execInfo.lpVerb = L"runas";
	if( (dwFlags&SFXEXEC_NOWAIT) == 0 )
		execInfo.fMask |= SEE_MASK_NOASYNC;

	fileParams = LoadQuotedString( lpwszCmdLine, filePath );
	if( filePath.Len() == 0 )
		return TRUE;
	execInfo.lpFile = filePath;
	execInfo.lpParameters = fileParams;
	if( ::ShellExecuteEx( &execInfo ) != FALSE )
	{
		if( (dwFlags&SFXEXEC_NOWAIT) == 0 )
			::WaitForSingleObject( execInfo.hProcess, INFINITE );
		::CloseHandle( execInfo.hProcess );
		return TRUE;
	}
	return FALSE;
}

BOOL ExecuteConfigProgram( CSfxStringU& ustrConfigString, LPCWSTR lpwszCurrentFolder, bool fUseExecuteFile, CSfxStringU& ustrDirPrefix, LPCWSTR cmdline )
{
	DWORD dwExecFlags = 0;
	int		ShortcutDefault = -1;
	int		DeleteDefault = -1;
	CSfxStringU	ustrRunProgram;
	LPCWSTR lpwszValue;
	CSfxCurrentDirectory currentFolder(lpwszCurrentFolder);

#ifdef _SFX_USE_PREFIX_PLATFORM
	int	nPlatform = SFX_EXECUTE_PLATFORM_ANY;
#endif // _SFX_USE_PREFIX_PLATFORM
#ifdef _SFX_USE_PREFIX_WAITALL
	bool	fUseWaitAll = false;
#endif // _SFX_USE_PREFIX_WAITALL

	ReplaceVariablesEx( ustrConfigString );
	lpwszValue = ustrConfigString;

	while( 1 )
	{
		LPCWSTR lpwszTmp;
#ifdef _SFX_USE_PREFIX_WAITALL
		if( (lpwszTmp = CheckPrefix( lpwszValue, L"waitall", CPF_NONE )) != NULL )
		{
			lpwszValue = lpwszTmp;
			fUseWaitAll = true;
			continue;
		}
#endif // _SFX_USE_PREFIX_WAITALL
#ifdef _SFX_USE_PREFIX_HIDCON
		if( (lpwszTmp = CheckPrefix( lpwszValue, L"hidcon", CPF_NONE )) != NULL )
		{
			lpwszValue = lpwszTmp;
			dwExecFlags |= SFXEXEC_HIDCON;
			continue;
		}
#endif // _SFX_USE_PREFIX_HIDCON
#ifdef _SFX_USE_PREFIX_NOWAIT
		if( (lpwszTmp = CheckPrefix( lpwszValue, L"nowait", CPF_NONE )) != NULL )
		{
			lpwszValue = lpwszTmp;
			// work only with InstallPath
			if( fUseInstallPath != false ) dwExecFlags |= SFXEXEC_NOWAIT;
			continue;
		}
#endif // _SFX_USE_PREFIX_NOWAIT
#ifdef _SFX_USE_PREFIX_FORCENOWAIT
		if( (lpwszTmp = CheckPrefix( lpwszValue, L"forcenowait", CPF_NONE )) != NULL )
		{
			lpwszValue = lpwszTmp;
			dwExecFlags |= SFXEXEC_NOWAIT;
			continue;
		}
#endif // _SFX_USE_PREFIX_FORCENOWAIT
		if( (lpwszTmp = CheckPrefix( lpwszValue, L"fm", CPF_NUMBER )) != NULL )
		{
			if( FinishMessage == -1 ) FinishMessage = StringToLong( lpwszValue+2 );
			lpwszValue = lpwszTmp;
			continue;
		}
		if( (lpwszTmp = CheckPrefix( lpwszValue, L"shc", CPF_SFX_INDEX )) != NULL )
		{
			ShortcutDefault = lpwszValue[3]-L'0';
			lpwszValue = lpwszTmp;
			continue;
		}
		if( (lpwszTmp = CheckPrefix( lpwszValue, L"del", CPF_SFX_INDEX )) != NULL )
		{
			DeleteDefault = lpwszValue[3]-L'0';
			lpwszValue = lpwszTmp;
			continue;
		}
#ifdef _SFX_USE_PREFIX_PLATFORM
		if( (lpwszTmp = IsPrefixPlatformI386(lpwszValue)) != NULL )
		{
			lpwszValue = lpwszTmp;
			nPlatform = SFX_EXECUTE_PLATFORM_I386;
			continue;
		}
		if( (lpwszTmp = IsPrefixPlatformAmd64(lpwszValue)) != NULL )
		{
			lpwszValue = lpwszTmp;
			nPlatform = SFX_EXECUTE_PLATFORM_AMD64;
			continue;
		}
#endif // _SFX_USE_PREFIX_PLATFORM
		break;
	}

	if( fUseExecuteFile != false )
	{
		if( *lpwszValue != L'\"' )
		{
			ustrRunProgram = L"\""; ustrRunProgram += lpwszValue; ustrRunProgram += L"\"";
		}
		else
			ustrRunProgram = lpwszValue;
		if( (lpwszValue = GetTextConfigValue( CFG_EXECUTEPARAMETERS )) != NULL )
		{
			ustrRunProgram += L" "; ustrRunProgram += lpwszValue;
		}
	}
	else
	{
		// 'RunProgram' or 'AutoInstall'
		ustrRunProgram = ustrDirPrefix; ustrRunProgram += lpwszValue;
	}

	CSfxStringU filePath;
	CSfxStringU fileParams = LoadQuotedString( ustrRunProgram, filePath );
	ReplaceVariablesEx( filePath );

//
//
#ifdef _SFX_USE_TEST
	if( nTestModeType == 0 )
	{
#endif // _SFX_USE_TEST
#ifdef _SFX_USE_PREFIX_PLATFORM
		if( SfxPrepareExecute( nPlatform ) != FALSE )
		{
#endif // _SFX_USE_PREFIX_PLATFORM
			if( *cmdline != L'\0' )
			{
				fileParams += cmdline;
				while( *cmdline != L'\0' ) cmdline++;
			}
			ReplaceVariablesEx( fileParams );
			while( 1 )
			{
#ifdef _SFX_USE_PREFIX_WAITALL
				if( fUseWaitAll == false )
				{
#endif // _SFX_USE_PREFIX_WAITALL
					CSfxStringU strExecuteString = L"\""+filePath + L"\" " + fileParams;
					if( SfxExecute( strExecuteString, dwExecFlags, lpwszCurrentFolder ) != FALSE )
						break;
#ifdef _SFX_USE_PREFIX_WAITALL
				}
				else
				{
					DWORD dwExitCode = Parent_ExecuteSfxWaitAll( filePath, fileParams, dwExecFlags );
					if( dwExitCode == ERROR_SUCCESS )
						break;
					::SetLastError( dwExitCode );
				}
#endif // _SFX_USE_PREFIX_WAITALL
				if( (MiscFlags&MISCFLAGS_NO_CANCELLED) == FALSE || ::GetLastError() != ERROR_CANCELLED )
					SfxErrorDialog( TRUE, ERR_EXECUTE, (LPCWSTR)ustrRunProgram );
				SfxCleanup();
				return ERRC_EXECUTE;
			}
#ifdef _SFX_USE_PREFIX_PLATFORM
			SfxFinalizeExecute();
		}

#endif // _SFX_USE_PREFIX_PLATFORM
#ifdef _SFX_USE_TEST
	}
#endif // _SFX_USE_TEST

	return TRUE;
}

BOOL ExecuteBatch( LPCWSTR lpwszKey, LPCWSTR lpwszCurrentFolder, LPCWSTR lpwszBatch, CSfxStringU& ustrDirPrefix, LPCWSTR cmdline )
{
	LPCWSTR lpwszValue;
	int		nExecuteIndex = 0;
	CSfxStringU	executeName = lpwszKey;
	executeName += *lpwszBatch;
	while( 1 )
	{
		if( (lpwszValue = GetTextConfigValue( executeName, &nExecuteIndex )) == NULL )
		{
			lpwszBatch++;
			if( (*lpwszBatch >= L'0' && *lpwszBatch <= L'9') ||
				(*lpwszBatch >= L'a' && *lpwszBatch <= L'z') ||
				(*lpwszBatch >= L'A' && *lpwszBatch <= L'Z') )
			{
				executeName = lpwszKey;
				executeName += *lpwszBatch;
				nExecuteIndex = 0;
				continue;
			}
			break;
		}
		ExecuteConfigProgram( CSfxStringU(lpwszValue), lpwszCurrentFolder, false, ustrDirPrefix, cmdline );
		cmdline = L"";
		ustrDirPrefix.Empty();
		nExecuteIndex++;
	}

	return TRUE;
}
