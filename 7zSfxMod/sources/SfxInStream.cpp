/*---------------------------------------------------------------------------*/
/* File:        SfxInStream.cpp                                              */
/* File:        Volumes.cpp                                                  */
/* Created:     Sat, 20 Nov 2010 11:58:35 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Tue, 29 Dec 2015 05:31:38 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    1865                                                         */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"

#if defined(SFX_VOLUMES) || defined(SFX_PROTECT)
#include "7zSfxModInt.h"

#ifdef SFX_VOLUMES
	#include "7zip\Archive\Common\MultiStream.cpp"

HRESULT CSfxInStream::OpenSubStream( LPCTSTR fileName, CSubStreamInfo * subStream )
{
	CInFileStream * streamSpec = new CInFileStream;
	if( !streamSpec->Open(fileName) )
	{
		delete streamSpec;
		return S_FALSE;
	}
	subStream->Stream = streamSpec;

	WIN32_FIND_DATA	fd;
	HANDLE hFind;
	UInt64 size = 0;
	if( (hFind = ::FindFirstFile( fileName, &fd )) != INVALID_HANDLE_VALUE )
	{
		((DWORD *)&subStream->Size)[0] = fd.nFileSizeLow;
		((DWORD *)&subStream->Size)[1] = fd.nFileSizeHigh;
		::FindClose( hFind );
		return S_OK;
	}
	delete streamSpec;
	return S_FALSE;
}

bool CSfxInStream::Open(LPCTSTR fileName)
{
	Streams.Clear();

	CSubStreamInfo stream;
	if( OpenSubStream( fileName, &stream ) != S_OK )
		return false;
	Streams.Add( stream );
	InitVolumes();
	CMultiStream::Init();
	return true;
}

static int ReverseFindOneOf( CSfxStringU& src, LPCWSTR chars )
{
	while( *chars != L'\0' )
	{
		int nPos = src.ReverseFind( *chars );
		if( nPos >= 0 )
			return nPos;
		chars++;
	}
	return -1;
}

// style 0: (name.7z.001.exe, name.7z.002, ...) || (name.001.exe, name.002, ...)
// style 1: name.exe, name.001, ...
static int GetVolumesName( CSfxStringU& baseName )
{
	baseName = strModulePathName;
	int nExtPos = ReverseFindOneOf( baseName, L".\\/" );
	if( nExtPos <= 0 || baseName[nExtPos] != L'.' )
		return -1;
	baseName.ReleaseBuffer(nExtPos);
#ifdef _SFX_USE_VOLUME_NAME_STYLE
	switch( nVolumeNameStyle )
	{
	case 1:
		{
			int nExtPos1 = ReverseFindOneOf( baseName, L".\\/" );
			if( nExtPos1 >= 0 && baseName[nExtPos1] == L'.' && (nExtPos-nExtPos1) == 4 /* ".00x" */ )
			{
				CSfxStringU tmp = baseName.Mid(nExtPos1+1,2);
				if( tmp == L"00" )
				{
					baseName.ReleaseBuffer(nExtPos1);
					return StringToLong(((const wchar_t *)baseName)+nExtPos1+1)+1;
				}
			}
			return 1;
		}
	default:
#endif // _SFX_USE_VOLUME_NAME_STYLE
		int nExtPos = ReverseFindOneOf( baseName, L".\\/" );
		if( nExtPos <= 0 || baseName[nExtPos] != L'.' )
			return -1;
		CSfxStringU tmp = baseName.Mid(nExtPos+1,2);
		if( tmp == L"00" )
		{
			baseName.ReleaseBuffer(nExtPos);
			return StringToLong(((const wchar_t *)baseName)+nExtPos+1)+1;
		}
#ifdef _SFX_USE_VOLUME_NAME_STYLE
	}
#endif // _SFX_USE_VOLUME_NAME_STYLE
		return -1;
}

bool CSfxInStream::InitVolumes()
{
	CSubStreamInfo	subStream;
	CSfxStringU baseName;
	int nStartVolume = GetVolumesName( baseName );

	if( nStartVolume == -1 )
		return false;

	for( int nVolume=nStartVolume; ; nVolume++ )
	{
		CSfxStringU strVolume;
		WCHAR		ccIndex[32];
		wsprintf( ccIndex, L".%03u", nVolume );
		strVolume = baseName + ccIndex;
		if( OpenSubStream( strVolume, &subStream ) == S_OK )
			Streams.Add( subStream );
		else
			break;
	}
	return true;
}
#endif // SFX_VOLUMES
#endif // SFX_VOLUMES || SFX_PROTECT
