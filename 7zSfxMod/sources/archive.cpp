/*---------------------------------------------------------------------------*/
/* File:        archive.cpp                                                  */
/* Created:     Sat, 05 Mar 2016 22:41:49 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Sat, 18 Nov 2017 by https://github.com/datadiode             */
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

STDMETHODIMP CSfxArchive::CPassword::SetTotal(const UInt64 * /*files*/, const UInt64 * /*bytes*/)
{
	return S_OK;
}

STDMETHODIMP CSfxArchive::CPassword::SetCompleted(const UInt64 * /*files*/, const UInt64 * /*bytes*/)
{
	return S_OK;
}

HRESULT CSfxArchive::CPassword::GetTextPassword(BSTR *password)
{
	if( IsDefined() || ((MiscFlags & MISCFLAGS_LATE_PASSWORD) == 0 && ShowDialog()) )
		return StringToBstr(Get(), password);
	return E_FAIL;
}

BOOL CSfxArchive::CPassword::ShowDialog()
{
	CSfxDialog_Password	passwdDialog;
	LPCWSTR pwd_text = lpwszPasswordText ? lpwszPasswordText : GetLanguageString(STR_PASSWORD_TEXT);
	if( passwdDialog.Show( SD_OKCANCEL, lpwszPasswordTitle, pwd_text, hwndExtractDlg ) )
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

	IMAGE_DOS_HEADER mz;
	IMAGE_NT_HEADERS nt;
	UInt32 processedSize;
	UInt64 newPosition;
	if (FAILED(m_stream->Seek(0, STREAM_SEEK_SET, NULL)))
		return false;
	if (FAILED(m_stream->Read(&mz, sizeof mz, &processedSize)) || (processedSize != sizeof mz))
		return false;
	if (FAILED(m_stream->Seek(mz.e_lfanew, STREAM_SEEK_SET, &newPosition)))
		return false;
	if (FAILED(m_stream->Read(&nt, sizeof nt, &processedSize)) || (processedSize != sizeof nt))
		return false;
	if (nt.Signature != IMAGE_NT_SIGNATURE)
		return false;
	if (nt.FileHeader.SizeOfOptionalHeader != sizeof nt.OptionalHeader)
		return false;
	for (WORD i = 0; i < nt.FileHeader.NumberOfSections; ++i)
	{
		IMAGE_SECTION_HEADER sh;
		if (FAILED(m_stream->Read(&sh, sizeof sh, &processedSize)) || (processedSize != sizeof sh))
			return false;
		UInt64 endPosition = sh.PointerToRawData + sh.SizeOfRawData;
		if (newPosition < endPosition)
			newPosition = endPosition;
	}
	if (FAILED(m_stream->Seek(newPosition, STREAM_SEEK_SET, NULL)))
		return false;
#ifdef SFX_CRYPTO
	CSfxArchive::CPassword * passwordCallback = new CSfxArchive::CPassword;
	result = m_handler->Open( m_stream, NULL, passwordCallback );
#else
	result = m_handler->Open( m_stream, NULL, NULL );
#endif // SFX_CRYPTO
	if( result == S_OK )
		return true;
	return false;
}
