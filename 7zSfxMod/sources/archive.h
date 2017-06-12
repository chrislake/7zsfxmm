/*---------------------------------------------------------------------------*/
/* File:        archive.h                                                    */
/* Created:     Sat, 05 Mar 2016 22:37:28 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Fri, 01 Apr 2016 20:32:01 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    27                                                           */
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
		UInt32 GetSize() { return m_outStreamSpec->GetSize(); };
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
	void SetCommandLine( LPCWSTR lpwszCommandLine ) { m_strCommandLine = L" "; m_strCommandLine += lpwszCommandLine; };
	LPCWSTR GetCommandLine() { return m_strCommandLine; };
#ifdef _SFX_USE_ENVIRONMENT_VARS
	LPCWSTR	GetCommandLine1() { return (LPCWSTR)m_strCommandLine1; };
	LPCWSTR	GetCommandLine2() { return (LPCWSTR)m_strCommandLine2; };
#endif // _SFX_USE_ENVIRONMENT_VARS
#ifdef _SFX_USE_COMPRESSED_CONFIG
	bool LoadAndParseCompressedConfig();
#else
	#define LoadAndParseCompressedConfig	(void)
#endif // _SFX_USE_COMPRESSED_CONFIG
#if defined(_SFX_USE_SFXAPI) || defined(_SFX_USE_COMPRESSED_CONFIG)
	void ShowPrepareDialog() { m_dlgPrepare.Show(); };
	void HidePrepareDialog() { m_dlgPrepare.Hide(); };
	bool IsPrepareDialog() { return m_dlgPrepare.IsWindow() ? true : false; };
	void SetOperationBegin() { m_dwTics = ::GetTickCount(); };
	DWORD GetOperationDuration() { return ::GetTickCount()-m_dwTics; };
#endif // defined(_SFX_USE_SFXAPI) || defined(_SFX_USE_COMPRESSED_CONFIG)
#ifdef SFX_CRYPTO
	void	SetPassword( LPCWSTR lpwszPassword ) { CSfxArchive::CPassword::Set(lpwszPassword); };
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
	CSfxStringU	m_strCommandLine;
#ifdef _SFX_USE_ENVIRONMENT_VARS
	CSfxStringU	m_strCommandLine1;
	CSfxStringU	m_strCommandLine2;
#endif // _SFX_USE_ENVIRONMENT_VARS
#if defined(_SFX_USE_SFXAPI) || defined(_SFX_USE_COMPRESSED_CONFIG)
	CSfxDialog_SfxApiPrepare	m_dlgPrepare;
	DWORD	m_dwTics;
#endif // defined(_SFX_USE_SFXAPI) || defined(_SFX_USE_COMPRESSED_CONFIG)
};
#endif // _SFXARCHIVE_H_INCLUDED_