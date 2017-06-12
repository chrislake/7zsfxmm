/*---------------------------------------------------------------------------*/
/* File:        SfxDialogs.h                                                 */
/* Created:     Sat, 13 Jan 2007 12:01:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Fri, 01 Apr 2016 20:19:25 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    3367                                                         */
/*---------------------------------------------------------------------------*/
/* Revision:    1240                                                         */
/* Updated:     Sun, 06 Jun 2010 08:48:12 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Add CSfxDialog_Warning                                       */
/*---------------------------------------------------------------------------*/
/* Revision:    1164                                                         */
/* Updated:     Mon, 22 Mar 2010 11:32:27 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#ifndef _SFXDIALOGS_H_INCLUDED_
#define _SFXDIALOGS_H_INCLUDED_

#define SD_YESNO				0
#define SD_OK					1
#define SD_CANCEL				2
#define SD_OKCANCEL				3
#define SD_BUTTONS_MASK			3

#define SD_ICONMODULE			0x04
#define SD_ICONQUESTION			0x08
#define SD_ICONSTOP				0x0C
#define SD_ICONINFORMATION		0x10
#define SD_ICONWARNING			0x14
#define SD_ICON_MASK			0x1C

#ifdef _SFX_USE_WIN7_PROGRESSBAR
	#include <shobjidl.h>
#endif // _SFX_USE_WIN7_PROGRESSBAR

class CSfxDialog
{
public:
	INT_PTR	Show( UINT uType, LPCWSTR lpwszCaption, LPCWSTR lpwszText, HWND hwndParent = NULL );
	HWND	GetHwnd() { return m_hWnd; };
	BOOL	IsWindow() { return (GetHwnd() != NULL && ::IsWindow(GetHwnd()) != FALSE) ? TRUE : FALSE; };
	static BOOL IsTimedOut() { return m_fTimedOut; }
	CSfxDialog();
	virtual ~CSfxDialog();

protected:
	void	SetButtonTimerText();
	void	SetButtonTimer( int nTimer );
	void	DisableTimer();
	virtual INT_PTR	ShowImpl( HWND hwndParent );
	void	ResizeAndPositionButton( int nButtonID, LPCWSTR lpwszText );
	void	SetDialogPos();
	void	DisableXPStyles();
	virtual BOOL	CalculateTextRect( LPCWSTR lpwszText, LPRECT lpRect, HFONT hFont, UINT uFormat ) sealed;
	BOOL	EndDialog( INT_PTR nResult ) { return ::EndDialog( GetHwnd(), nResult ); };
	HWND	GetDlgItem( int nItemID ) { return ::GetDlgItem( m_hWnd, nItemID );	};
	CSfxStringU	GetDlgItemText( int nItemID ) { return ::GetWindowStringU(GetDlgItem(nItemID)); }
	void	SetDlgItemText( int nItemID, LPCWSTR lpwszText )
						{ SetWindowText( GetDlgItem( nItemID ), lpwszText ); };
	void	SetWindowText( HWND hwnd, LPCWSTR lpwszText );
	void	SetButtonText( int nButtonID, LPCWSTR lpwszText );
	void	GetDlgItemRect( int nIDItem, LPRECT rc );
	void	SetCaption( LPCWSTR lpwszCaption );
	LRESULT	SendMessage( UINT Msg, WPARAM wParam, LPARAM lParam ) { return ::SendMessage( GetHwnd(), Msg, wParam, lParam ); };
	LRESULT	SendDlgItemMessage( int nItemID, UINT Msg, WPARAM wParam, LPARAM lParam ) { return ::SendMessage( GetDlgItem(nItemID), Msg, wParam, lParam ); };
	BOOL	ShowControl( int nControlID, BOOL fShow );
	void	SetDlgItemPos( int nItemID, int x, int y, int cx, int cy, UINT uFlags );
	void	GetClientRect( LPRECT lpRect ) { ::GetClientRect( GetHwnd(), lpRect ); };
	BOOL	CalculateTitleSize( LPCWSTR lpwszText, LPSIZE size );
	void	SetDefaultButton( int nButtonID );
	virtual BOOL	OnInitDialog();
	virtual void	OnDestroy();
	virtual void	OnCommand( int nControlID );
	virtual void	OnOK() { EndDialog(TRUE); };
	virtual void	OnCancel();
	virtual void	OnTimer();
	virtual void	ResizeAndPosition();
	virtual void	CalculateDialogSize();
	virtual INT_PTR DialogProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
	HWND	m_hWnd;
	UINT	m_uType;
	LPCWSTR	m_lpwszCaption;
	LPCWSTR	m_lpwszText;
	SIZE	m_dlgSize;
	int		m_nCaptionWidthExtra;
	RECT	m_rcText;
	BOOL	m_fUseIcon;
	HFONT	m_hFont;
	UINT	m_uDlgResourceId;
// Def button timer
	CSfxStringU	m_strDefButtonText;
	int		m_nTimer;
	int		m_nDefButtonID;
	static	BOOL m_fTimedOut;
	static CSfxDialog * m_pActiveDialog;
	static LRESULT CALLBACK hookMouseProc( int nCode, WPARAM wParam, LPARAM lParam );
	static LRESULT CALLBACK hookKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam );
	static HHOOK	m_hMouseHook;
	static HHOOK	m_hKeyboardHook;

	static POINT	m_ptCenter;
	static BYTE m_DialogsTemplate[];
	static INT_PTR CALLBACK SfxDialogProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};

class CSfxDialog_WithoutCancelPrompt: public CSfxDialog
{
protected:
	virtual void OnCancel() { EndDialog(FALSE); };
};

class CSfxDialog_BeginPromptClassic : public CSfxDialog
{
public:
	CSfxDialog_BeginPromptClassic() { m_uDlgResourceId = IDD_BEGINPROMT_CLASSIC; };
#ifdef _SFX_USE_BEGINPROMPTTIMEOUT
protected:
	virtual BOOL	OnInitDialog();
#endif // _SFX_USE_BEGINPROMPTTIMEOUT
};

class CSfxDialog_ExtractPath : public CSfxDialog
{
public:
	CSfxDialog_ExtractPath() { m_uDlgResourceId = IDD_EXTRACTPATH; };
protected:
	void			SetPathText();
	virtual BOOL	OnInitDialog();
	virtual void	ResizeAndPosition();
	virtual void	CalculateDialogSize();
	virtual void	OnCommand( int nControlID );
	void			BrowseExtractPath();
	static INT_PTR CALLBACK ButtonIconProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	HICON	m_hBrowseIcon;
	WNDPROC	m_pfnOldButtonProc;
	int		m_nBrowseButtonSize;
};

class CSfxDialog_BeginPromptWithExtractPath : public CSfxDialog_ExtractPath
{
public:
	CSfxDialog_BeginPromptWithExtractPath() { m_uDlgResourceId = IDD_BEGINPROMT_EXTRACTPATH; };
protected:
	virtual BOOL	OnInitDialog();
	virtual void	ResizeAndPosition();
	virtual void	CalculateDialogSize();
	RECT			m_rcExtractPathText;
};

class CSfxDialog_Extract : public CSfxDialog
{
public:
	CSfxDialog_Extract() { m_uDlgResourceId = IDD_EXTRACT; };
#ifdef _SFX_USE_WIN7_PROGRESSBAR
	void SetTaskbarState( TBPFLAG tbpFlags );
#endif // _SFX_USE_WIN7_PROGRESSBAR
protected:
	virtual BOOL	OnInitDialog();
	virtual void	ResizeAndPosition();
	virtual void	CalculateDialogSize();
	virtual void	OnCancel();
	virtual INT_PTR DialogProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

	void SetPercentTextFont();
	void SetPercents( LPARAM lPercents );
	BOOL IsCancel();
	RECT	m_rcPercentText;
	__int64	m_i64ProgressTotal;

#ifdef _SFX_USE_TEST
	__int64	m_i64TestCurrent;
#endif // _SFX_USE_TEST

#ifdef _SFX_USE_WIN7_PROGRESSBAR
	ITaskbarList3 * m_pTaskbarList;
#endif // _SFX_USE_WIN7_PROGRESSBAR

};

class CSfxDialog_FinishMessage : public CSfxDialog_WithoutCancelPrompt
{
public:
	CSfxDialog_FinishMessage() { m_uDlgResourceId = IDD_FINISHMESSAGE; };
protected:
	virtual BOOL OnInitDialog();
};

class CSfxDialog_CancelPrompt : public CSfxDialog_WithoutCancelPrompt
{
public:
	BOOL IsCancel( CSfxDialog * pParent );
protected:
	virtual BOOL OnInitDialog();
};

class CSfxDialog_Error : public CSfxDialog_WithoutCancelPrompt
{
protected:
	virtual BOOL OnInitDialog();
};

#ifdef _SFX_USE_WARNINGS
class CSfxDialog_Warning : public CSfxDialog_WithoutCancelPrompt
{
protected:
	virtual BOOL OnInitDialog();
};
#endif // _SFX_USE_WARNINGS

class CSfxDialog_HelpText : public CSfxDialog_WithoutCancelPrompt
{
public:
	CSfxDialog_HelpText() { m_uDlgResourceId = IDD_HELPTEXT; };
private:
	int dummy;
};

class CSfxDialog_Version: public CSfxDialog_WithoutCancelPrompt
{
private:
	int dummy;
};

#ifdef SFX_CRYPTO
	class CSfxDialog_Password: public CSfxDialog_ExtractPath
	{
	public:
		CSfxDialog_Password() { m_uDlgResourceId = IDD_PASSWORD; };
		LPCWSTR GetPassword() { return m_strPassword; }
	protected:
		virtual BOOL	OnInitDialog();
		virtual void	OnCommand( int nControlID );
	private:
		CSfxStringU		m_strPassword;
	};
#endif // SFX_CRYPTO

#ifdef _SFX_USE_LANG
	LPVOID LoadInterfaceResource( LPCSTR lpType, LPCSTR lpName, size_t * lpSize = NULL );
#endif // _SFX_USE_LANG

#if defined(_SFX_USE_SFXAPI) || defined(_SFX_USE_COMPRESSED_CONFIG)
	class CSfxDialog_SfxApiPrepare : public CSfxDialog
	{
	public:
		CSfxDialog_SfxApiPrepare() { m_uDlgResourceId = IDD_SFXAPI_PREPARE; };
		void Show();
		void Hide();
	protected:
		virtual BOOL	OnInitDialog();
		virtual void	OnCancel();
	private:
		static HRESULT WINAPI ShowThread( CSfxDialog_SfxApiPrepare * pThis );
	};
#endif // defined(_SFX_USE_SFXAPI) || defined(_SFX_USE_COMPRESSED_CONFIG)

#endif // _SFXDIALOGS_H_INCLUDED_
