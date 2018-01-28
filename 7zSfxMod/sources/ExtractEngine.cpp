/*---------------------------------------------------------------------------*/
/* File:        ExtractEngine.cpp                                            */
/* Created:     Wed, 05 Oct 2005 07:36:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Sat, 18 Nov 2017 by https://github.com/datadiode             */
/*---------------------------------------------------------------------------*/
/* Revision:    3816                                                         */
/* Updated:     Thu, 17 Mar 2016 05:51:18 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/*---------------------------------------------------------------------------*/
/* Revision:    1706                                                         */
/* Updated:     Sun, 06 Jun 2010 08:51:01 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Add check free space on destination drive                    */
/*---------------------------------------------------------------------------*/
/* Revision:    1630                                                         */
/* Updated:     Mon, 22 Mar 2010 11:14:29 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "7zSfxModInt.h"
#include "ExtractEngine.h"

using namespace NWindows;


STDMETHODIMP CSfxExtractEngine::SetTotal( UInt64 size )
{
#ifdef _SFX_USE_CHECK_FREE_SPACE
	UInt64 ui64Free;
	if( (MiscFlags & MISCFLAGS_NO_CHECK_DISK_FREE) == 0 &&
			::GetDiskFreeSpaceEx( extractPath, (PULARGE_INTEGER)&ui64Free, NULL, NULL ) && ui64Free < size )
	{
		if( ShowSfxWarningDialog( GetLanguageString(STR_DISK_FREE_SPACE) ) != IDOK )
		{
			m_ErrorCode = SfxErrors::seCreateFile; // fake error code for prevent "Internal error" dialog
			return E_FAIL;
		}
		MiscFlags |= MISCFLAGS_NO_CHECK_DISK_FREE;
	}
#endif // _SFX_USE_CHECK_FREE_SPACE
	::SendMessage( hwndExtractDlg, WM_7ZSFX_SETTOTAL, 0, (LPARAM)&size );
	return S_OK;
}

STDMETHODIMP CSfxExtractEngine::SetCompleted( const UInt64 *completeValue )
{
	if( fCancelExtract )
		return E_ABORT;
	if( completeValue != NULL )
		::SendMessage( hwndExtractDlg, WM_7ZSFX_SETCOMPLETED, 0, (LPARAM)completeValue );
	return S_OK;
}

STDMETHODIMP CSfxExtractEngine::PrepareOperation( Int32 askExtractMode )
{
  m_extractMode = FALSE;
  
  if( askExtractMode == NArchive::NExtract::NAskMode::kExtract )
	  m_extractMode = TRUE;

  return S_OK;
}

STDMETHODIMP CSfxExtractEngine::SetOperationResult( Int32 resultEOperationResult )
{
	if( resultEOperationResult != NArchive::NExtract::NOperationResult::kOK )
	{
		m_ErrorCode = resultEOperationResult;
		return E_FAIL;
	}

	if( m_outFileStream != NULL )
		m_outFileStreamSpec->SetMTime( &m_processedFileInfo.UTCLastWriteTime );
	m_outFileStream.Release();
	if( m_extractMode )
		SetFileAttributes( m_diskFilePath, m_processedFileInfo.Attributes );
	return S_OK;
}

STDMETHODIMP CSfxExtractEngine::SayInternalError( Int32 errc, DWORD dwLastError )
{
	::SetLastError( dwLastError );
	if( errc == SfxErrors::seCreateFile )
		SfxErrorDialog( TRUE, ERR_CREATE_FILE, (LPCWSTR)m_diskFilePath );
	else
	{
		if( errc == SfxErrors::seOverwrite )
			SfxErrorDialog( TRUE, ERR_OVERWRITE, (LPCWSTR)m_diskFilePath );
	}
	return SetOperationResult( errc );
}

STDMETHODIMP CSfxExtractEngine::GetStream( UInt32 index, ISequentialOutStream **outStream, Int32 askExtractMode )
{
	if( fCancelExtract )
		return E_ABORT;
	*outStream = NULL;
	if( askExtractMode != NArchive::NExtract::NAskMode::kExtract )
		 return S_OK;

	m_outFileStream.Release();

	NCOM::CPropVariant propVariantName;
	RINOK( gSfxArchive.GetProperty(index, kpidPath, &propVariantName) );
	CSfxStringU fullPath;
	if( propVariantName.vt == VT_EMPTY || propVariantName.vt != VT_BSTR )
	{
		return SetOperationResult( SfxErrors::sePropVariant1 );
	}

	fullPath = propVariantName.bstrVal;
	m_diskFilePath = m_directoryPath + fullPath;

	NCOM::CPropVariant propVariant;
	RINOK(gSfxArchive.GetProperty(index, kpidAttrib, &propVariant));

	if (propVariant.vt == VT_EMPTY)
		m_processedFileInfo.Attributes = 0;
	else
	{
		if (propVariant.vt != VT_UI4)
			return SetOperationResult( SfxErrors::sePropVariant2 );
		m_processedFileInfo.Attributes = propVariant.ulVal;
	}

	RINOK(gSfxArchive.GetProperty(index, kpidIsDir, &propVariant));
	m_processedFileInfo.IsDirectory = (propVariant.boolVal != VARIANT_FALSE) ? TRUE : FALSE;

	/*
	BOOL isAnti = FALSE;
	{
		NCOM::CPropVariant propVariantTemp;
		RINOK(m_archiveHandler->GetProperty(index, kpidIsAnti,&propVariantTemp));
		if (propVariantTemp.vt == VT_BOOL)
			isAnti = (propVariantTemp.boolVal != VARIANT_FALSE) ? TRUE : FALSE;
	}
	if( isAnti )
	{
		return SetOperationResult( SfxErrors::seAnti );
	}*/

	RINOK(gSfxArchive.GetProperty(index, kpidMTime, &propVariant));
	SYSTEMTIME	currTime;
	switch( propVariant.vt )
	{
	case VT_EMPTY:
		::GetLocalTime( &currTime );
		::SystemTimeToFileTime( &currTime, &m_processedFileInfo.UTCLastWriteTime );
		break;
	case VT_FILETIME:
		m_processedFileInfo.UTCLastWriteTime = propVariant.filetime;
		break;
	default:
		return SetOperationResult( SfxErrors::sePropVariant3 );
	}

	if( m_processedFileInfo.IsDirectory )
	{
		if( CreateFolderTree( m_diskFilePath ) == FALSE )
			return SetOperationResult( SfxErrors::seCreateFolder );
		return S_OK;
	}
		
	switch( GetOverwriteMode( m_diskFilePath, &m_processedFileInfo.UTCLastWriteTime ) )
	{
	case SFX_OM_SKIP:
		return S_OK;
	case SFX_OM_ERROR:
		return SayInternalError( SfxErrors::seOverwrite, ::GetLastError() );
	}

	m_outFileStreamSpec = new COutFileStream;
	CMyComPtr<ISequentialOutStream> outStreamLoc(m_outFileStreamSpec);
	if( m_outFileStreamSpec->Create( m_diskFilePath, true ) == false )
	{
		DWORD dwLastError = ::GetLastError();
		CSfxStringU filePath = m_diskFilePath;
		int nPos = filePath.ReverseFind_PathSepar();
		if( nPos < 0 )
			return SayInternalError( SfxErrors::seCreateFile, dwLastError );
		filePath.ReleaseBuf_SetEnd( nPos );
		if( CreateFolderTree( filePath ) == FALSE )
			return SetOperationResult( SfxErrors::seCreateFolder );
		if( m_outFileStreamSpec->Create( m_diskFilePath, true ) == false )
			return SayInternalError( SfxErrors::seCreateFile, ::GetLastError() );
	}
	m_outFileStream = outStreamLoc;
	*outStream = outStreamLoc.Detach();

	return S_OK;
}

HRESULT CSfxExtractEngine::Extract(LPCWSTR lpwszFolderName)
{
	SetDirectory( lpwszFolderName );

	DWORD dwThreadId;
	m_ErrorCode = NArchive::NExtract::NOperationResult::kOK;
	m_hExtractThread = ::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CSfxExtractEngine::ExtractThread, this, CREATE_SUSPENDED, &dwThreadId );

	if (m_hExtractThread != NULL)
	{
		if (GUIMode != GUIMODE_HIDDEN)
			ExtractDialog(this);
		::ResumeThread(m_hExtractThread);
		::WaitForSingleObject(m_hExtractThread, INFINITE);
	}

	if( m_ErrorCode != NArchive::NExtract::NOperationResult::kOK )
	{
		switch( m_ErrorCode )
		{
		case NArchive::NExtract::NOperationResult::kUnsupportedMethod:
			SfxErrorDialog( FALSE, ERR_7Z_UNSUPPORTED_METHOD );
			break;
		case NArchive::NExtract::NOperationResult::kCRCError:
			SfxErrorDialog( FALSE, ERR_7Z_CRC_ERROR );
			break;
		case NArchive::NExtract::NOperationResult::kDataError:
			SfxErrorDialog( FALSE, ERR_7Z_DATA_ERROR );
			break;
		case SfxErrors::seCreateFolder:
		case SfxErrors::seCreateFile:
		case SfxErrors::seOverwrite:
#ifdef SFX_CRYPTO
		case SfxErrors::seNoPassword:
#endif // SFX_CRYPTO
			break;
		default:
			SfxErrorDialog( FALSE, ERR_7Z_INTERNAL_ERROR, m_ErrorCode );
		}
		return E_FAIL;
	}

	DWORD dwExitCode = 0;
	if( fCancelExtract == FALSE )
	{
		if( m_hExtractThread == NULL || ::GetExitCodeThread( m_hExtractThread, &dwExitCode ) == FALSE || dwExitCode != S_OK )
		{
			if( (dwExitCode&((FACILITY_WIN32 << 16) | 0x80000000)) != ((FACILITY_WIN32 << 16) | 0x80000000) )
				SfxErrorDialog( FALSE, ERR_7Z_EXTRACT_ERROR1, dwExitCode );
			else
			{
				if( (dwExitCode&0xffff) != ERROR_OUTOFMEMORY || MiscFlags != -1 )
				{
					::SetLastError( dwExitCode&0xffff );
					SfxErrorDialog( TRUE, ERR_7Z_EXTRACT_ERROR2, dwExitCode );
				}
			}
			return E_FAIL;
		}
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT WINAPI CSfxExtractEngine::ExtractThread(CSfxExtractEngine *pThis)
{
	HRESULT result = E_FAIL;
	UInt32 * indices = NULL;
	UInt32 num_indices = (UInt32)-1;
#ifdef _SFX_USE_EXTRACT_MASK
	if( pThis->m_numIndices > 0 && pThis->m_indices != NULL )
	{
		indices = pThis->m_indices;
		num_indices = pThis->m_numIndices;
	}
#endif // _SFX_USE_EXTRACT_MASK

	__try
	{
#ifdef _SFX_USE_TEST
		result = gSfxArchive.GetHandler()->Extract( indices, num_indices, nTestModeType == TMT_ARCHIVE ? true : false, pThis );
#else
		result = gSfxArchive.GetHandler()->Extract( indices, num_indices, 0, pThis );
#endif // _SFX_USE_TEST
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		char msg[256];
		wsprintfA(msg, "An exception of type 0x%08lX has occurred", GetExceptionCode());
		FatalAppExitA(0, msg);
	}
	if( GUIMode != GUIMODE_HIDDEN && hwndExtractDlg != NULL )
		::SendNotifyMessage( hwndExtractDlg, WM_COMMAND, SDC_BUTTON1, 0 );

	return result;
}

#ifdef SFX_CRYPTO
STDMETHODIMP CSfxExtractEngine::CryptoGetTextPassword(BSTR *password)
{
	if( CSfxArchive::CPassword::GetTextPassword(password) != S_OK )
		return SetOperationResult( SfxErrors::seNoPassword );
	return S_OK;
}
#endif // SFX_CRYPTO

#ifdef _SFX_USE_EXTRACT_MASK
	#ifdef _SFX_USE_PREFIX_PLATFORM
		void CleanupExtractMaskPlatform( LPCWSTR lpwszConfigName )
		{
			LPCWSTR lpwszValue;
			int from = 0;
			while( (lpwszValue = GetTextConfigValue( lpwszConfigName, &from )) != NULL )
			{
				CSfxStringU tmp = lpwszValue;
				ReplaceVariablesEx(tmp);
				if( (lpwszValue = IsPrefixCurrentPlatform((LPCWSTR)tmp)) == NULL )
					gConfig.Delete(from);
				else
				{
					gConfig[from].String = lpwszValue;
					from++;
				}
			}
		}
	#endif // _SFX_USE_PREFIX_PLATFORM

	BOOL IsExtractCadidate( UInt32 /*index*/, LPCWSTR lpwszMaskName )
	{
		if( MyStringCompareNoCase(lpwszMaskName,CFG_EXTRACTMASK_INCLUDE) != 0 )
			return FALSE;
		return TRUE;
	}
#endif // _SFX_USE_EXTRACT_MASK

HRESULT ExtractArchive( const CSfxStringU &folderName )
{
	HRESULT hr = S_OK;
#ifdef SFX_CRYPTO
	MiscFlags &= ~(MISCFLAGS_LATE_PASSWORD);
#endif // SFX_CRYPTO
	if( gSfxArchive.Open() == false )
	{
#ifdef SFX_CRYPTO
		SfxErrorDialog( FALSE,
			(CSfxArchive::CPassword::IsDefined() == false && CSfxArchive::CPassword::GetAttempts() == 0 ) ? ERR_NON7Z_ARCHIVE : ERR_7Z_DATA_ERROR );
#else
		SfxErrorDialog( FALSE, ERR_NON7Z_ARCHIVE );
#endif // SFX_CRYPTO
		return E_FAIL;
	}

#ifdef _SFX_USE_TEST
	if( nTestModeType != TMT_ARCHIVE && CreateFolderTree( folderName ) == FALSE )
		return E_FAIL;
#else
	if( CreateFolderTree( folderName ) == FALSE )
		return E_FAIL;
#endif // _SFX_USE_TEST

	CMyComPtr<CSfxExtractEngine> extractEngine = new CSfxExtractEngine;

#ifdef _SFX_USE_EXTRACT_MASK
	#ifdef _SFX_USE_PREFIX_PLATFORM
		CleanupExtractMaskPlatform( CFG_EXTRACTMASK_INCLUDE );
		CleanupExtractMaskPlatform( CFG_EXTRACTMASK_EXCLUDE );
	#endif // _SFX_USE_PREFIX_PLATFORM
	LPCWSTR lpwsz1stMask = CFG_EXTRACTMASK_EXCLUDE;
	LPCWSTR lpwsz2ndMask = CFG_EXTRACTMASK_INCLUDE;
	if( MiscFlags & MISCFLAGS_EXTRACTMASK_INVERT )
	{
		// Inverting: first 'include', late 'exclude'
		lpwsz1stMask = CFG_EXTRACTMASK_INCLUDE;
		lpwsz2ndMask  = CFG_EXTRACTMASK_EXCLUDE;
	}
	if( GetTextConfigValue(lpwsz2ndMask) == NULL )
	{
		CTextConfigPair defMask;
		defMask.ID = lpwsz2ndMask;
		defMask.String = L"*";
		gConfig.Add( defMask );
	}

	UInt32 i, j, num_items = gSfxArchive.GetItemsCount();
	UInt32 * indices = new UInt32[num_items];
	for( i = j = 0; i < num_items; i++ )
	{
		NWindows::NCOM::CPropVariant prop;
		if( gSfxArchive.GetProperty(i, kpidPath, &prop) != S_OK || prop.vt != VT_BSTR )
			continue;

		CSfxStringU pathName = prop.bstrVal;
		LPCWSTR lpwszValue;
		BOOL mask_founded = FALSE;

		int from = 0;
		while( mask_founded == FALSE && (lpwszValue = GetTextConfigValue( lpwsz1stMask, &from )) != NULL )
		{
			from++;
			if( DoesWildcardMatchName(CSfxStringU(lpwszValue),pathName) )
			{
				if( IsExtractCadidate(i,lpwsz1stMask) )
					indices[j++] = i;
				mask_founded = TRUE;
			}
		}

		from = 0;
		while( mask_founded == FALSE && (lpwszValue = GetTextConfigValue( lpwsz2ndMask, &from )) != NULL )
		{
			from++;
			if( DoesWildcardMatchName(CSfxStringU(lpwszValue),pathName) )
			{
				if( IsExtractCadidate(i,lpwsz2ndMask) )
					indices[j++] = i;
				mask_founded = TRUE;
			}
		}
		if( mask_founded == FALSE &&
				MyStringCompareNoCase(lpwsz2ndMask,CFG_EXTRACTMASK_EXCLUDE) == 0 &&
					IsExtractCadidate(i,CFG_EXTRACTMASK_INCLUDE) )
			indices[j++] = i;
	}
	extractEngine->SetIndices( indices, j );
	if( j > 0 )
		hr = extractEngine->Extract( folderName );
	delete indices;
#else
	hr = extractEngine->Extract( folderName );
#endif // _SFX_USE_EXTRACT_MASK

	return hr;
}
