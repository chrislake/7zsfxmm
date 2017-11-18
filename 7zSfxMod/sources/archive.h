/*---------------------------------------------------------------------------*/
/* File:        archive.h                                                    */
/* Created:     Sat, 05 Mar 2016 22:37:28 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Sat, 18 Nov 2017 by https://github.com/datadiode             */
/*---------------------------------------------------------------------------*/
/* Revision:    27                                                           */
/* Updated:     Fri, 01 Apr 2016 20:32:01 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/*---------------------------------------------------------------------------*/
#ifndef _SFXARCHIVE_H_INCLUDED_
#define _SFXARCHIVE_H_INCLUDED_

#include "7zSfxModInt.h"

class CSfxInStream;

class CSfxArchive
{
public:
/* CPassword callback */
#ifdef SFX_CRYPTO
	class CPassword:
		public IArchiveOpenCallback,
		public ICryptoGetTextPassword,
		public CMyUnknownImp
	{
	public:
		MY_UNKNOWN_IMP1(ICryptoGetTextPassword)
		INTERFACE_IArchiveOpenCallback(;)

		static void Set( LPCWSTR lpwszPassword ) { m_strPassword = lpwszPassword; m_fPasswordDefined = true; };
		static LPCWSTR Get() { return m_strPassword; };
		static BOOL ShowDialog();
		static bool IsDefined() { return m_fPasswordDefined; };
		static int GetAttempts() { return m_nAttempts; };

		STDMETHOD(CryptoGetTextPassword)(BSTR *password) { return GetTextPassword(password); };
		static HRESULT GetTextPassword(BSTR *password);

	private:
		static bool				m_fPasswordDefined;
		static CSfxStringU		m_strPassword;
		static int				m_nAttempts;
	};
#endif // SFX_CRYPTO

/* CCompressedConfig callback */
#ifdef _SFX_USE_COMPRESSED_CONFIG
	class CCompressedConfig:
		public IArchiveExtractCallback,
#ifdef SFX_CRYPTO
		public ICryptoGetTextPassword,
#endif // SFX_CRYPTO
		public CMyUnknownImp
	{
	public:
		CCompressedConfig() { m_outStreamSpec = new CDynBufSeqOutStream; };
		MY_UNKNOWN_IMP1(ICryptoGetTextPassword)

		// IProgress
		STDMETHOD(SetTotal)(UInt64 size) { return S_OK; };
		STDMETHOD(SetCompleted)(const UInt64 *completeValue);

		// IExtractCallback
		STDMETHOD(GetStream)(UInt32 index, ISequentialOutStream **outStream, Int32 askExtractMode);
		STDMETHOD(PrepareOperation)(Int32 askExtractMode) { return S_OK; };
		STDMETHOD(SetOperationResult)(Int32 resultEOperationResult);

#ifdef SFX_CRYPTO
		STDMETHOD(CryptoGetTextPassword)(BSTR *password) { return CSfxArchive::CPassword::GetTextPassword(password); };
#endif // SFX_CRYPTO
		const Byte * GetData() { return m_outStreamSpec->GetBuffer(); };
		size_t GetSize() { return m_outStreamSpec->GetSize(); };
	private:
		CDynBufSeqOutStream *	m_outStreamSpec;
		CMyComPtr<ISequentialOutStream>	m_outStream;
	};
#endif // _SFX_USE_COMPRESSED_CONFIG

public:
	CSfxArchive();
//	virtual ~CSfxArchive();

	bool Init( LPCWSTR lpwszFileName );
	bool Open();
	IInStream * GetStream() { return m_stream; };
	IInArchive * GetHandler() { return m_handler; };
	UInt32 GetItemsCount();
	HRESULT	GetProperty(UInt32 index, PROPID propID, PROPVARIANT *value) { return m_handler->GetProperty(index,propID,value); };
	void SetBatchInstall( LPCWSTR lpwszBatchInstall ) { m_lpwszBatchInstall = lpwszBatchInstall; };
	LPCWSTR GetBatchInstall() { return m_lpwszBatchInstall; };
	void SetAutoInstall( bool enable ) { m_fUseAutoInstall = enable; };
	bool GetAutoInstall() { return m_fUseAutoInstall; };
	void SetAssumeYes( bool enable ) { m_fAssumeYes = enable; };
	bool GetAssumeYes() { return m_fAssumeYes; };
	void SetNoRun( bool enable ) { m_fNoRun = enable; };
	bool GetNoRun() { return m_fNoRun; };
	void SetShowHelp( bool enable ) { m_fShowHelp = enable; };
	bool GetShowHelp() { return m_fShowHelp; };
#ifdef SFX_CRYPTO
	void SetPassword( LPCWSTR lpwszPassword ) { CSfxArchive::CPassword::Set(lpwszPassword); };
#endif // SFX_CRYPTO

private:
	CSfxInStream *			m_streamSpec;
	CMyComPtr<IInStream>	m_stream;
	CMyComPtr<IInArchive>	m_handler;

	// global
	LPCWSTR		m_lpwszBatchInstall;
	bool		m_fUseAutoInstall;
	bool		m_fAssumeYes;
	bool		m_fNoRun;
	bool		m_fShowHelp;
};
#endif // _SFXARCHIVE_H_INCLUDED_
