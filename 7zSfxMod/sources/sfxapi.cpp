/*---------------------------------------------------------------------------*/
/* File:        sfxapi.cpp                                                   */
/* Created:     Sat, 06 Feb 2016 23:19:41 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Sat, 02 Apr 2016 06:35:57 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    56                                                           */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"

#if defined(_SFX_USE_SFXAPI) || defined(_SFX_USE_COMPRESSED_CONFIG)
#include "7zSfxModInt.h"
#include "ExtractEngine.h"

/* Part 1: Common for SfxAPI & compressed configs */
static bool IsSfxApiCommon( UInt32 index, const char * mask )
{
	NWindows::NCOM::CPropVariant prop;
	if( gSfxArchive.GetProperty(index, kpidPath, &prop) == S_OK && prop.vt == VT_BSTR )
	{
		CSfxStringU name = prop.bstrVal;
		CSfxStringU ustrMask = mask;
#ifdef _DEBUG
		if( name.Compare(L"Aida64AE.sfx.config") == 0 )
			DoesWildcardMatchName(ustrMask,name);
#endif // _DEBUG
		return DoesWildcardMatchName(ustrMask,name);
	}
	return false;
}

#endif // defined(_SFX_USE_SFXAPI) || defined(_SFX_USE_COMPRESSED_CONFIG)

/* Part 2: Compressed configs */
#ifdef _SFX_USE_COMPRESSED_CONFIG
bool IsSfxApiConfig( UInt32 index )
{
	if( IsSfxApiCommon(index, "*" SFXAPI_CONFIG_FILENAME) ||
		IsSfxApiCommon(index, "*" SFXAPI_CONFIG_FILENAME ".*") )
		return true;
	return false;
}

STDMETHODIMP CSfxArchive::CCompressedConfig::GetStream(UInt32 index, ISequentialOutStream **outStream, Int32 askExtractMode)
{
	if( askExtractMode == NArchive::NExtract::NAskMode::kExtract && IsSfxApiConfig(index) )
	{
		CMyComPtr<ISequentialOutStream> outStreamLoc(m_outStreamSpec);
		m_outStream = outStreamLoc;
		*outStream = outStreamLoc.Detach();
	}
	return S_OK;
}

STDMETHODIMP CSfxArchive::CCompressedConfig::SetOperationResult( Int32 resultEOperationResult )
{
	if( resultEOperationResult != NArchive::NExtract::NOperationResult::kOK )
		return E_FAIL;

	if( m_outStreamSpec->GetSize() > 0 )
	{
		CBufInStream * inStreamLoc = new CBufInStream;
		CMyComPtr<IInStream> inStream(inStreamLoc);
		inStreamLoc->Init( m_outStreamSpec->GetBuffer(), m_outStreamSpec->GetSize() );
		LoadAndParseConfig(inStream,true);
		m_outStreamSpec->Init();
	}
	return S_OK;
}

STDMETHODIMP CSfxArchive::CCompressedConfig::SetCompleted(const UInt64 *completeValue)
{
	if( gSfxArchive.GetOperationDuration() > 1000 )
		gSfxArchive.ShowPrepareDialog();
	return S_OK;
}
#endif // _SFX_USE_COMPRESSED_CONFIG

#ifdef _SFX_USE_SFXAPI
bool IsSfxApiHelpers( UInt32 index )
{
	if( IsSfxApiCommon(index, "*" SFXAPI_HELPERS_NAME ) ||
		IsSfxApiCommon(index, "*" SFXAPI_HELPERS_NAME ".*") ||
		IsSfxApiCommon(index, "*" SFXAPI_HELPERS_NAME "\\*") ||
		IsSfxApiCommon(index, "*" SFXAPI_HELPERS_NAME "/*") )
		return true;
	return false;
}

bool CSfxAPI::Init()
{
	if( m_strTmpDir.IsEmpty() == false )
		return true;
	if( (MiscFlags&MISCFLAGS_NO_SFXAPI) != 0 )
	{
		m_strTmpDir = strSfxFolder;
		return true;
	}
	m_strTmpDir = CreateTempName( L"7ZipSfx.%03x" );
	if( CreateFolderTree(m_strTmpDir) == false )
		return false;
#ifdef _SFX_USE_ENVIRONMENT_VARS
	SfxAddEnvironmentVarInternal( SFX_ENV_VAR_APIPATH, GetDirectory(), false );
#endif // _SFX_USE_ENVIRONMENT_VARS
	return Extract();
}

bool CSfxAPI::Free()
{
	::SetCurrentDirectory( strSfxFolder );
	if( m_strTmpDir.IsEmpty() == false && m_strTmpDir != strSfxFolder )
		DeleteFileOrDirectoryAlways( m_strTmpDir );
	m_strTmpDir.Empty();
	return true;
}

bool CSfxAPI::Extract()
{
	if( (MiscFlags&MISCFLAGS_NO_SFXAPI) == 0 || CSfxArchive::CPassword::GetAttempts() > 0 || gSfxArchive.Open() == false )
		return false;
	UInt32 i, j, num_items = gSfxArchive.GetItemsCount();
	UInt32 * indices = new UInt32[num_items];
	for( i = j = 0; i < num_items; i++ )
	{
		if( IsSfxApiHelpers(i) != false )
			indices[j++] = i;
	}
	if( j == 0 )
	{
		delete indices;
		return false;
	}
	CSfxExtractEngine * helpers = new CSfxExtractEngine;
	CMyComPtr<IArchiveExtractCallback>	callback(helpers);
	helpers->SetDirectory( GetDirectory() );
	if( gSfxArchive.GetHandler()->Extract( indices, j, false, callback ) != S_OK )
	{
		delete indices;
		return false;
	}
	delete indices;
	return true;
}
#endif // _SFX_USE_SFXAPI
