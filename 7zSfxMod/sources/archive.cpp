/*---------------------------------------------------------------------------*/
/* File:        archive.cpp                                                  */
/* Created:     Sat, 05 Mar 2016 22:41:49 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Tue, 31 Oct 2017 by https://github.com/datadiode             */
/*---------------------------------------------------------------------------*/
/* Revision:    27                                                           */
/* Updated:     Fri, 01 Apr 2016 19:54:56 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "7zSfxModInt.h"

#ifdef SFX_CRYPTO
	bool		CSfxArchive::CPassword::m_fPasswordDefined = false;
	CSfxStringU	CSfxArchive::CPassword::m_strPassword;
	int			CSfxArchive::CPassword::m_nAttempts = 0;

	STDMETHODIMP CSfxArchive::CPassword::SetTotal(const UInt64 *files, const UInt64 *bytes) { return S_OK; }
	STDMETHODIMP CSfxArchive::CPassword::SetCompleted(const UInt64 *files, const UInt64 *bytes) { return S_OK; }
	HRESULT CSfxArchive::CPassword::GetTextPassword(BSTR *password)
	{
		if( IsDefined() != false || ((MiscFlags&MISCFLAGS_LATE_PASSWORD) == 0 && ShowDialog() != FALSE) )
			return StringToBstr(Get(), password);
		return E_FAIL;
	}
	BOOL CSfxArchive::CPassword::ShowDialog()
	{
		CSfxDialog_Password	passwdDialog;
		LPCWSTR pwd_text = (lpwszPasswordText != NULL) ? lpwszPasswordText : GetLanguageString(STR_PASSWORD_TEXT);
		if( passwdDialog.Show( SD_OKCANCEL, lpwszPasswordTitle, pwd_text, hwndExtractDlg ) != FALSE )
		{
			Set( passwdDialog.GetPassword() );
			return TRUE;
		}
		m_nAttempts++;
		return FALSE;
	}
#endif // SFX_CRYPTO

CSfxArchive::CSfxArchive()
{
	m_streamSpec = new CSfxInStream;
	m_stream = m_streamSpec;
	m_handler = new NArchive::N7z::CHandler;
	m_lpwszBatchInstall = L"\0";
	m_fUseAutoInstall = m_fAssumeYes = m_fNoRun = false;
}

UInt32 CSfxArchive::GetItemsCount()
{
	UInt32 num = 0;
	m_handler->GetNumberOfItems(&num);
	return num;
}

bool CSfxArchive::Init( LPCWSTR lpwszFileName )
{
	if( GetItemsCount() == 0 && m_streamSpec->GetStreamsCount() == 0 )
	{
		m_streamSpec->Seek( 0, STREAM_SEEK_SET, NULL );
		if( m_streamSpec->Open(lpwszFileName) == false )
			return false;
	}

	return true;
}

bool CSfxArchive::Open()
{
	HRESULT	result;

	if( GetItemsCount() > 0 )
		return true;

	m_stream->Seek( 0, STREAM_SEEK_SET, NULL );
#ifdef SFX_CRYPTO
	CSfxArchive::CPassword * passwordCallback = new CSfxArchive::CPassword;
	result = m_handler->Open( m_stream, &kMaxCheckStartPosition, passwordCallback );
#else
	result = m_handler->Open( m_stream, &kMaxCheckStartPosition, NULL );
#endif // SFX_CRYPTO
	if( result == S_OK )
		return true;
	return false;
}

#ifdef _SFX_USE_COMPRESSED_CONFIG
bool CSfxArchive::LoadAndParseCompressedConfig()
{
	if( (MiscFlags&MISCFLAGS_NO_COMPRESSED_CONFIG) != 0 || CSfxArchive::CPassword::GetAttempts() > 0 || Open() == false )
		return false;
	UInt32 i, j, num_items = GetItemsCount();
	UInt32 * indices = new UInt32[num_items];
	for( i = j = 0; i < num_items; i++ )
	{
		if( IsSfxApiConfig(i) != false )
			indices[j++] = i;
	}
	if( j == 0 )
	{
		delete indices;
		return false;
	}
	CSfxArchive::CCompressedConfig * config = new CSfxArchive::CCompressedConfig;
	CMyComPtr<IArchiveExtractCallback>	callback(config);
	SetOperationBegin();
	if( m_handler->Extract( indices, j, false, callback ) != S_OK || config->GetSize() == 0 )
	{
		delete indices;
		return false;
	}
	delete indices;
	return true;
}
#endif // _SFX_USE_COMPRESSED_CONFIG