/*---------------------------------------------------------------------------*/
/* File:        ExtractEngine.h                                              */
/* Created:     Wed, 05 Oct 2005 17:35:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Sat, 18 Nov 2017 by https://github.com/datadiode             */
/*---------------------------------------------------------------------------*/
/* Revision:    3813                                                         */
/* Updated:     Mon, 14 Mar 2016 05:05:17 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/*---------------------------------------------------------------------------*/
/* Revision:    1629                                                         */
/* Updated:     Mon, 22 Mar 2010 11:15:14 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#ifndef _EXTRACTENGINE_H_INCLUDED_
#define _EXTRACTENGINE_H_INCLUDED_

class CSfxExtractEngine:
	public IArchiveExtractCallback,
#ifdef SFX_CRYPTO
	public ICryptoGetTextPassword,
#endif // SFX_CRYPTO
	public CMyUnknownImp
{
public:
	MY_UNKNOWN_IMP1(ICryptoGetTextPassword)

	// IProgress
	STDMETHOD(SetTotal)(UInt64 size);
	STDMETHOD(SetCompleted)(const UInt64 *completeValue);

	// IExtractCallback
	STDMETHOD(GetStream)(UInt32 index, ISequentialOutStream **outStream, Int32 askExtractMode);
	STDMETHOD(PrepareOperation)(Int32 askExtractMode);
	STDMETHOD(SetOperationResult)(Int32 resultEOperationResult);

	HANDLE	GetExtractThread() { return m_hExtractThread; };
	Int32	GetErrorCode() { return m_ErrorCode; };
	STDMETHOD(SayInternalError)( Int32 errc, DWORD dwLastError );

#ifdef SFX_CRYPTO
	STDMETHOD(CryptoGetTextPassword)(BSTR *password);
#endif // SFX_CRYPTO

	CSfxExtractEngine()
		: m_hExtractThread(NULL)
		, m_ErrorCode(NArchive::NExtract::NOperationResult::kOK)
	{
		m_outFileStreamSpec = NULL;
#ifdef _SFX_USE_EXTRACT_MASK
		SetIndices(NULL,0);
#endif // _SFX_USE_EXTRACT_MASK
	};
	void SetDirectory( LPCWSTR lpwszDirectory ) { m_directoryPath = lpwszDirectory; m_directoryPath += L'\\'; };
#ifdef _SFX_USE_EXTRACT_MASK
	void SetIndices( UInt32 * indices, UInt32 num ) { m_indices = indices; m_numIndices = num; }
#endif // _SFX_USE_EXTRACT_MASK

private:
	
	CSfxStringU				m_directoryPath;
	BOOL					m_extractMode;
	COutFileStream *		m_outFileStreamSpec;
	CMyComPtr<ISequentialOutStream> m_outFileStream;
	CSfxStringU				m_diskFilePath;
	HANDLE					m_hExtractThread;
	Int32					m_ErrorCode;
#ifdef _SFX_USE_EXTRACT_MASK
	UInt32 *				m_indices;
	UInt32					m_numIndices;
#endif // _SFX_USE_EXTRACT_MASK

	struct CProcessedFileInfo
	{
		FILETIME	UTCLastWriteTime;
		BOOL		IsDirectory;
		UInt32		Attributes;
	} m_processedFileInfo;

public:
	static HRESULT WINAPI ExtractThread( CSfxExtractEngine * pThis );
	HRESULT Extract( LPCWSTR lpwszFolderName );
};

#endif // _EXTRACTENGINE_H_INCLUDED_
