/*---------------------------------------------------------------------------*/
/* File:        SfxDialogs.cpp                                               */
/* Created:     Sat, 13 Jan 2007 02:03:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Sat, 10 Feb 2018 by https://github.com/datadiode             */
/*---------------------------------------------------------------------------*/
/* Revision:    3367                                                         */
/* Updated:     Fri, 01 Apr 2016 20:42:56 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/*---------------------------------------------------------------------------*/
/* Revision:    1262                                                         */
/* Updated:     Sun, 27 Jun 2010 06:55:56 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Change 'fErrorShown' variable location						 */
/*				(incorrect work on files overwrites)						 */
/*---------------------------------------------------------------------------*/
/* Revision:    1241                                                         */
/* Updated:     Sun, 06 Jun 2010 09:04:54 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Add CSfxDialog_Warning                                       */
/*---------------------------------------------------------------------------*/
/* Revision:    1165                                                         */
/* Updated:     Mon, 22 Mar 2010 11:30:01 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "7ZSfxModInt.h"
#include "LangStrs.h"
#include "ExtractEngine.h"

#define SDM_BORDER_TOP			10
#define SDM_BORDER_BOTTOM		10
#define SDM_BORDER_LEFT			12
#define SDM_BORDER_RIGHT		14
#define SDM_BUTTONS_CX_SPACING	10
#define SDM_BUTTONS_CY_SPACING	18
#define SDM_PARENT_OFFSET_X		10
#define SDM_PARENT_OFFSET_Y		10
#define SDM_EXTRACTPATH_SPACING	14
//#define SDM_BROWSE_BUTTON_SIZE	22

#define SDM_ICON_OFFSET_CX				0
#define SDM_ICON_OFFSET_CY				2
#define SDM_ICON_TO_TEXT_SPACING		(SDM_BORDER_LEFT-1)
#define SDM_EXTRACT_PATH_SPACE_TO_TEXT	18
#define SDM_EXTRACT_PATH_SPACE_TO_EDIT	5

#define SDM_PROGRESS_TO_TEXT			(10+4)
#define SDM_PROGRESS_TO_PERCENT			5
#define SDM_PROGRESS_BUTTON_SPACING		(10)

#define MAX_PROGRESS_VALUE	30000

BYTE const CSfxDialog::m_DialogsTemplate[] = {
	0x01, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC8, 0x00, 0xC8, 0x80,
	0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0x00, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x00, 0x00, 0x00, 0x00, 0x01, 0x4D, 0x00, 0x53, 0x00, 0x20, 0x00, 0x53, 0x00, 0x68, 0x00,
	0x65, 0x00, 0x6C, 0x00, 0x6C, 0x00, 0x20, 0x00, 0x44, 0x00, 0x6C, 0x00, 0x67, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x50, 0x07, 0x00, 0x07, 0x00,
	0x14, 0x00, 0x14, 0x00, 0xB1, 0x04, 0x00, 0x00, 0xFF, 0xFF, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x50, 0x24, 0x00, 0x07, 0x00,
	0x49, 0x00, 0x1A, 0x00, 0xB2, 0x04, 0x00, 0x00, 0xFF, 0xFF, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x81, 0x40, 0x07, 0x00, 0x39, 0x00,
	0x51, 0x00, 0x0C, 0x00, 0xB6, 0x04, 0x00, 0x00, 0xFF, 0xFF, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x01, 0x40, 0x5F, 0x00, 0x39, 0x00,
	0x13, 0x00, 0x0D, 0x00, 0xB7, 0x04, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x50, 0x07, 0x00, 0x64, 0x00,
	0x32, 0x00, 0x0E, 0x00, 0xB3, 0x04, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x50, 0x40, 0x00, 0x64, 0x00,
	0x32, 0x00, 0x0E, 0x00, 0xB4, 0x04, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x40, 0x07, 0x00, 0x29, 0x00,
	0x6B, 0x00, 0x08, 0x00, 0xB5, 0x04, 0x00, 0x00, 0xFF, 0xFF, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x07, 0x00, 0x4E, 0x00,
	0x6B, 0x00, 0x0A, 0x00, 0xB8, 0x04, 0x00, 0x00, 0x6D, 0x00, 0x73, 0x00, 0x63, 0x00, 0x74, 0x00,
	0x6C, 0x00, 0x73, 0x00, 0x5F, 0x00, 0x70, 0x00, 0x72, 0x00, 0x6F, 0x00, 0x67, 0x00, 0x72, 0x00,
	0x65, 0x00, 0x73, 0x00, 0x73, 0x00, 0x33, 0x00, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


POINT CSfxDialog::m_ptCenter =
{
	::GetSystemMetrics( SM_CXFULLSCREEN ) / 2,
	::GetSystemMetrics( SM_CYFULLSCREEN ) / 2
};

CSfxDialog *CSfxDialog::m_pActiveDialog = NULL;
HHOOK CSfxDialog::m_hMouseHook = NULL;
HHOOK CSfxDialog::m_hKeyboardHook = NULL;
BOOL CSfxDialog::m_fTimedOut = FALSE;

CSfxDialog::CSfxDialog()
{
	m_hWnd = NULL;
	m_uType = 0;
	m_nCaptionWidthExtra = 24;
	m_uDlgResourceId = 0;
}

CSfxDialog::~CSfxDialog()
{
}

void CSfxDialog::SetButtonTimerText()
{
	CSfxStringU strButtonText = m_strDefButtonText;
	WCHAR wszTimer[256];
	if( m_nTimer > 0 )
	{
		wsprintf( wszTimer, L" (%d%s)", m_nTimer, GetLanguageString(STR_SECONDS) );
		strButtonText += wszTimer;
	}
	SetDlgItemText( m_nDefButtonID, strButtonText );
}

LRESULT CALLBACK CSfxDialog::hookMouseProc( int code, WPARAM wParam, LPARAM lParam )
{
	static UINT const uHookCodes[] = {
		WM_LBUTTONUP, WM_LBUTTONDOWN, WM_LBUTTONDBLCLK,
		WM_RBUTTONUP, WM_RBUTTONDOWN, WM_RBUTTONDBLCLK,
		WM_MBUTTONUP, WM_MBUTTONDOWN, WM_MBUTTONDBLCLK,
		0
	};
	if( m_pActiveDialog != NULL )
	{
		if( m_pActiveDialog->m_nTimer > 0 )
		{
			UINT const* msg = uHookCodes;
			while( *msg != 0 )
			{
				if( *msg == wParam )
				{
					RECT rc;
					LPMOUSEHOOKSTRUCT lpmhs = (LPMOUSEHOOKSTRUCT)lParam;
					ScreenToClient( m_pActiveDialog->m_hWnd, &lpmhs->pt );
					m_pActiveDialog->GetClientRect( &rc );
					if( PtInRect(&rc, lpmhs->pt) )
						m_pActiveDialog->DisableTimer();
					break;
				}
				msg++;
			}
		}
		return CallNextHookEx( m_pActiveDialog->m_hMouseHook, code, wParam, lParam );
	}
	return FALSE;
}

LRESULT CALLBACK CSfxDialog::hookKeyboardProc( int code, WPARAM wParam, LPARAM lParam )
{
	if( m_pActiveDialog != NULL )
	{
		m_pActiveDialog->DisableTimer();
		return CallNextHookEx( m_pActiveDialog->m_hKeyboardHook, code, wParam, lParam );
	}
	return FALSE;
}

void CSfxDialog::SetButtonTimer( int nTimer )
{
	if( nTimer == 0 || m_hMouseHook != NULL || m_hKeyboardHook != NULL || m_pActiveDialog != NULL )
		return;
	UINT uButtonID = SDC_BUTTON1;
	if( nTimer < 0 )
	{
		uButtonID = SDC_BUTTON2;
		nTimer = -nTimer;
	}
	m_pActiveDialog = this;
	m_nTimer = nTimer;
	m_nDefButtonID = uButtonID;
	GetDlgItemText( uButtonID, m_strDefButtonText );
	SetButtonTimerText();
	SetDefaultButton( uButtonID );
	if( m_uDlgResourceId == 0 )
	{
		CSfxStringU str;
		GetDlgItemText( uButtonID, str );
		ResizeAndPositionButton( uButtonID, str );
	}
	SetTimer( m_hWnd, 1, 1000, NULL );
}

void CSfxDialog::OnTimer()
{
	m_nTimer--;
	if( m_hMouseHook == NULL )
		m_hMouseHook = ::SetWindowsHookEx( WH_MOUSE, hookMouseProc, NULL, ::GetCurrentThreadId() );
	if( m_hKeyboardHook == NULL )
		m_hKeyboardHook = ::SetWindowsHookEx( WH_KEYBOARD, hookKeyboardProc, NULL, ::GetCurrentThreadId() );
	if( m_nTimer == 0 )
	{
		m_fTimedOut = TRUE;
		EndDialog( m_nDefButtonID == SDC_BUTTON2 ? FALSE : TRUE );
	}
	else
		SetButtonTimerText();
}

INT_PTR CALLBACK CSfxDialog::SfxDialogProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CSfxDialog * pThis = (CSfxDialog *)::GetWindowLongPtr( hwnd, GWLP_USERDATA );

	if( pThis == NULL && uMsg == WM_INITDIALOG )
	{
		pThis = (CSfxDialog *)lParam;
		::SetWindowLongPtr( hwnd, GWLP_USERDATA, lParam );
		pThis->m_hWnd = hwnd;
		pThis->m_hFont = (HFONT)::SendMessage( hwnd, WM_GETFONT, 0, 0 );
	}
	
	if( pThis != NULL )
		return pThis->DialogProc( uMsg, wParam, lParam );
	return 0;
}

void CSfxDialog::DisableTimer()
{
	if( m_nTimer == 0 )
		return;
	::KillTimer( m_hWnd, 1 );
	m_nTimer = 0;
	SetButtonTimerText();
}

INT_PTR CSfxDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/)
{
	switch( uMsg )
	{
	case WM_INITDIALOG:
		return OnInitDialog();
	case WM_COMMAND:
		OnCommand( LOWORD(wParam) );
		break;
	case WM_DESTROY:
		if( m_hMouseHook != NULL )
		{
			::UnhookWindowsHookEx( m_hMouseHook );
			m_hMouseHook = NULL;
		}
		if( m_hKeyboardHook != NULL )
		{
			::UnhookWindowsHookEx( m_hKeyboardHook );
			m_hKeyboardHook = NULL;
		}
		m_pActiveDialog = NULL;
		OnDestroy();
		break;
	case WM_TIMER:
		OnTimer();
		break;
	}
	return 0;
}

void CSfxDialog::OnDestroy()
{
	// save center
	RECT rc;
	GetWindowRect( m_hWnd, &rc );
	m_ptCenter.x = (rc.right-rc.left)/2+rc.left;
	m_ptCenter.y = (rc.bottom-rc.top)/2+rc.top;
	m_hWnd = NULL;
}
void CSfxDialog::OnCommand( int nControlID )
{
	switch( nControlID )
	{
	case SDC_BUTTON1:
		OnOK();
		break;
	case IDCANCEL:
	case SDC_BUTTON2:
		OnCancel();
		break;
	case SDC_BACK:
		fUseBackward = true;
		EndDialog(FALSE);
		break;
	}
}

void CSfxDialog::OnCancel()
{
	CSfxDialog_CancelPrompt	CancelPromptDlg;
	if( CancelPromptDlg.IsCancel( m_hWnd ) )
		EndDialog(FALSE);
}

void CSfxDialog::SetDefaultButton( int nButtonID )
{
	SendDlgItemMessage( SDC_BUTTON1, BM_SETSTYLE, BS_PUSHBUTTON, TRUE );
	SendDlgItemMessage( SDC_BUTTON2, BM_SETSTYLE, BS_PUSHBUTTON, TRUE );
	SendMessage( DM_SETDEFID, nButtonID, 0 );
	SendDlgItemMessage( nButtonID, BM_SETSTYLE, BS_DEFPUSHBUTTON, TRUE );
	SetFocus( GetDlgItem(nButtonID) );
}

BOOL CSfxDialog::CalculateTitleSize( LPCWSTR lpwszText, LPSIZE size )
{
	BOOL Ret = FALSE;

	RECT rc;
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(ncm);
	if( SystemParametersInfo( SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0 ) )
	{
		int nTitleWidth = ncm.iCaptionWidth + m_nCaptionWidthExtra - (SDM_BORDER_LEFT+SDM_BORDER_RIGHT);
		if( (GUIFlags & GUIFLAGS_NO_TITLE_ICON) == 0 )
			nTitleWidth += ::GetSystemMetrics( SM_CXSMICON );

		HFONT hTitleFont = ::CreateFontIndirect( &ncm.lfCaptionFont );
		if( hTitleFont != NULL )
		{
			if( CalculateTextRect( lpwszText, &rc, hTitleFont, DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_EXPANDTABS ) )
			{
				nTitleWidth += rc.right;
				Ret = TRUE;
			}
			::DeleteObject( hTitleFont );
		}
		size->cx = nTitleWidth;
		size->cy = ncm.iCaptionHeight;
	}

	return Ret;
}

void CSfxDialog::CalculateDialogSize()
{
	m_dlgSize.cx = m_dlgSize.cy = 0;
	m_rcText.left = m_rcText.top = m_rcText.right = m_rcText.bottom = 0;

	if( m_fUseIcon )
	{
		m_dlgSize.cx += ::GetSystemMetrics(SM_CXICON) + SDM_ICON_TO_TEXT_SPACING + SDM_ICON_OFFSET_CX;
		m_dlgSize.cy += ::GetSystemMetrics(SM_CYICON) + SDM_ICON_OFFSET_CY;
	}

	// dialog text
	if( CalculateTextRect( m_strText, &m_rcText, m_hFont, DT_LEFT|DT_NOPREFIX|DT_WORDBREAK|DT_EXPANDTABS ) )
	{
		m_dlgSize.cx += m_rcText.right;
		if( m_dlgSize.cy < m_rcText.bottom )
			m_dlgSize.cy = m_rcText.bottom;
	}

	// title
	SIZE sizeTitle;
	if( CalculateTitleSize( m_strCaption, &sizeTitle ) )
	{
		if( m_dlgSize.cx < sizeTitle.cx )
			m_dlgSize.cx = sizeTitle.cx;
		m_dlgSize.cy += sizeTitle.cy;
	}

	RECT rc;
	m_dlgSize.cx += SDM_BORDER_LEFT + SDM_BORDER_RIGHT;
	GetDlgItemRect( SDC_BUTTON1, &rc );
	m_dlgSize.cy += SDM_BORDER_TOP + SDM_BORDER_BOTTOM + SDM_BUTTONS_CY_SPACING + (rc.bottom - rc.top);
}

void CSfxDialog::SetDialogPos()
{
	if( GetParent(m_hWnd) == NULL )
	{
		RECT rc;
		GetWindowRect( m_hWnd, &rc );
		SetWindowPos( m_hWnd, NULL,
						m_ptCenter.x-(rc.right-rc.left)/2, m_ptCenter.y-(rc.bottom-rc.top)/2,
						0,0, SWP_NOZORDER|SWP_NOSIZE );
	}
}
BOOL CSfxDialog::OnInitDialog()
{
	fUseBackward = false;
	if( (GUIFlags & GUIFLAGS_NO_TITLE_ICON) == 0 )
	{
		HICON hBigIcon = ::LoadIcon( hRsrcModule, MAKEINTRESOURCE(IDI_7ZSFX) );
		HICON hSmallIcon = (HICON)::LoadImage( hRsrcModule, MAKEINTRESOURCE(IDI_7ZSFX), IMAGE_ICON,
												GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0 );
		if( hSmallIcon == NULL ) hSmallIcon = hBigIcon;
		SendMessage( WM_SETICON, ICON_BIG, (LPARAM)hBigIcon );
		SendMessage( WM_SETICON, ICON_SMALL, (LPARAM)hSmallIcon );
	}

	if( GUIFlags & GUIFLAGS_ALLOW_AMPERSAND )
	{
		::SetWindowLongPtr( GetDlgItem(SDC_TEXT), GWL_STYLE, ::GetWindowLongPtr(GetDlgItem(SDC_TEXT),GWL_STYLE)|SS_NOPREFIX );
		::SetWindowLongPtr( GetDlgItem(SDC_TEXT2), GWL_STYLE, ::GetWindowLongPtr(GetDlgItem(SDC_TEXT2),GWL_STYLE)|SS_NOPREFIX );
	}

	SetDlgItemText( SDC_TEXT, m_strText );
	SetCaption( m_strCaption );

	switch( m_uType&SD_BUTTONS_MASK )
	{
	case SD_YESNO:
		SetButtonText( SDC_BUTTON1, GetLanguageString(STR_BUTTON_YES) );
		SetButtonText( SDC_BUTTON2, GetLanguageString(STR_BUTTON_NO) );
		break;
	case SD_OK:
		SetButtonText( SDC_BUTTON1, GetLanguageString(STR_BUTTON_OK) );
		ShowControl( SDC_BUTTON2, FALSE );
		break;
	case SD_CANCEL:
		SetButtonText( SDC_BUTTON2, GetLanguageString(STR_BUTTON_CANCEL) );
		ShowControl( SDC_BUTTON1, FALSE );
		SetDefaultButton( SDC_BUTTON2 );
		break;
	case SD_OKCANCEL:
		SetButtonText( SDC_BUTTON1, GetLanguageString(STR_BUTTON_OK) );
		SetButtonText( SDC_BUTTON2, GetLanguageString(STR_BUTTON_CANCEL) );
		break;
	}

	if( m_uDlgResourceId != 0 )
	{
		HWND hwndChild = ::GetWindow( m_hWnd, GW_CHILD );
		while( hwndChild != NULL )
		{
			HWND hwndNext = ::GetWindow( hwndChild, GW_HWNDNEXT );
			ReplaceVariablesInWindow( hwndChild );
#			if defined(_SFX_USE_IMAGES) || defined(_SFX_USE_RTF_CONTROL)
			{
				LRESULT dlgcode = ::SendMessage( hwndChild, WM_GETDLGCODE, 0, 0 );
				if( dlgcode & DLGC_STATIC )
				{
					LONG style = ::GetWindowLong( hwndChild, GWL_STYLE );
					if( (style & SS_TYPEMASK) == SS_BITMAP )
					{
#						if defined(_SFX_USE_IMAGES)
						SetDlgControlImage( hwndChild );
#						endif // _SFX_USE_IMAGES
					}
					else
					{
#						if defined(_SFX_USE_RTF_CONTROL)
						RecreateAsRichEdit( hwndChild );
#						endif // _SFX_USE_RTF_CONTROL
					}
				}
			}
#			endif
			hwndChild = hwndNext;
		}
		SetDialogPos();
		return FALSE;
	}
	
	// icon
	HICON	hIcon = NULL;
	switch( m_uType&SD_ICON_MASK )
	{
	case SD_ICONMODULE:
		hIcon = ::LoadIcon( hRsrcModule, MAKEINTRESOURCE(IDI_7ZSFX) );
		break;
	case SD_ICONQUESTION:
		hIcon = ::LoadIcon( NULL, IDI_QUESTION );
		break;
	case SD_ICONSTOP:
		hIcon = ::LoadIcon( NULL, IDI_ERROR );
		break;
	case SD_ICONINFORMATION:
		hIcon = ::LoadIcon( NULL, IDI_INFORMATION );
		break;
#ifdef _SFX_USE_WARNINGS
	case SD_ICONWARNING:
		hIcon = ::LoadIcon( NULL, IDI_WARNING );
		break;
#endif // _SFX_USE_WARNINGS
	}
	if( hIcon != NULL )
	{
		m_fUseIcon = TRUE;
		::SendMessage( GetDlgItem(SDC_ICON), STM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon );
	}
	else
	{
		m_fUseIcon = FALSE;
		ShowControl( SDC_ICON, FALSE );
	}

	CalculateDialogSize();
	ResizeAndPosition();
	SetDialogPos();
	
	return FALSE;
}

void CSfxDialog::SetWindowText( HWND hwnd, LPCWSTR lpwszText )
{
	::SetWindowText( hwnd, lpwszText );
	ReplaceVariablesInWindow( hwnd );
}

void CSfxDialog::GetDlgItemRect( int nIDItem, LPRECT rc )
{
	if ( HWND hwndControl = GetDlgItem(nIDItem) )
	{
		::GetWindowRect( hwndControl, rc );
		::MapWindowPoints( NULL, m_hWnd, reinterpret_cast<LPPOINT>(rc), 2 );
	}
}

INT_PTR CSfxDialog::Show( UINT uType, LPCWSTR lpwszCaption, LPCWSTR lpwszText, HWND hwndParent /* = NULL  */ )
{
	if( lpwszCaption == NULL || lpwszText == NULL )
		return FALSE;
	m_strText = lpwszText;
	m_strCaption = lpwszCaption;
	ReplaceVariablesEx( m_strText );
	ReplaceVariablesEx( m_strCaption );
	m_uType = uType;
	return ShowImpl( hwndParent );
}

void CSfxDialog::ShowControl( int nControlID, BOOL fShow )
{
	if( m_uDlgResourceId != 0 )
		return;
	if ( HWND hwndControl = GetDlgItem( nControlID ) )
		::ShowWindow( hwndControl, fShow ? SW_SHOW : SW_HIDE );
}

BOOL CSfxDialog::CalculateTextRect(LPCWSTR lpwszText, LPRECT lpRect, HFONT hFont, UINT uFormat)
{
	BOOL bRet = FALSE;
	if( HDC hDC = ::GetDC( m_hWnd ) )
	{
		int nMaxWindowWidth = ::GetSystemMetrics(SM_CXMAXIMIZED) - ::GetSystemMetrics(SM_CXICON) - 60;
		int nMaxWindowHeight = ::GetSystemMetrics(SM_CYMAXIMIZED) - 120;
		lpRect->left = lpRect->top = lpRect->bottom = 0;
		lpRect->right = nMaxWindowWidth;
		HFONT hOldFont = (HFONT)::SelectObject( hDC, hFont );
		bRet = ::DrawText( hDC, lpwszText, -1, lpRect, uFormat | DT_CALCRECT ) > 0;
		if( nMaxWindowWidth < lpRect->right )
			lpRect->right = nMaxWindowWidth;
		if( nMaxWindowHeight < lpRect->bottom )
			lpRect->bottom = nMaxWindowHeight;
		::SelectObject( hDC, hOldFont );
		::ReleaseDC( m_hWnd, hDC );
	}
	return bRet;
}

void CSfxDialog::SetDlgItemPos( int nItemID, int x, int y, int cx, int cy, UINT uFlags )
{
	SetWindowPos( GetDlgItem(nItemID), NULL, x, y, cx, cy, uFlags|SWP_NOZORDER );
}

void CSfxDialog::SetButtonText( int nButtonID, LPCWSTR lpwszText )
{
	if( GetWindowTextLength( GetDlgItem(nButtonID) ) == 0 )
		SetDlgItemText( nButtonID, lpwszText );
}

void CSfxDialog::ResizeAndPosition()
{
	RECT rc;
	int nOneButtonID = 0;
	int nButton1width = 0;
	int nButton2width = 0;
	int nButtonsWidth = 0;
	int nButtonsHeight = 0;
	if( (::GetWindowLongPtr( GetDlgItem(SDC_BUTTON1), GWL_STYLE ) & WS_VISIBLE) != 0 )
	{
		GetDlgItemRect( SDC_BUTTON1, &rc );
		nButton1width = rc.right - rc.left;
		nButtonsHeight = rc.bottom - rc.top;
		nOneButtonID = SDC_BUTTON1;
	}
	if( (::GetWindowLongPtr( GetDlgItem(SDC_BUTTON2), GWL_STYLE ) & WS_VISIBLE) != 0 )
	{
		GetDlgItemRect( SDC_BUTTON2, &rc );
		nButton2width = rc.right - rc.left;
		nButtonsHeight = rc.bottom - rc.top;
		nOneButtonID = SDC_BUTTON2;
	}
	if( nButton1width != 0 && nButton2width != 0 )
	{
		if( nButton1width < nButton2width )
			nButton1width = nButton2width;
		else
			nButton2width = nButton1width;
		nButtonsWidth += SDM_BUTTONS_CX_SPACING;
	}
	nButtonsWidth += nButton1width + nButton2width;
	int cxMin = nButtonsWidth + SDM_BORDER_LEFT + SDM_BORDER_RIGHT;
	if( m_dlgSize.cx < cxMin )
		m_dlgSize.cx = cxMin;
	int const nDesktopWidth = ::GetSystemMetrics( SM_CXFULLSCREEN );
	int const nDesktopHeight = ::GetSystemMetrics( SM_CYFULLSCREEN );
	int const nDlgWidth = m_dlgSize.cx + ::GetSystemMetrics( SM_CXDLGFRAME );
	int const nDlgHeight = m_dlgSize.cy + ::GetSystemMetrics( SM_CYDLGFRAME );
	int x = (nDesktopWidth - nDlgWidth) / 2;
	int y = (nDesktopHeight - nDlgHeight) / 2;
	if( HWND hwndParent = GetParent(m_hWnd) )
	{
		::GetClientRect( hwndParent, &rc );
		::MapWindowPoints( hwndParent, NULL, reinterpret_cast<LPPOINT>(&rc), 2 );
		x = rc.left;
		y = rc.top;
		if( (rc.right - rc.left) > nDlgWidth )
			x += (rc.right - rc.left - nDlgWidth) / 2;
		if( (rc.bottom - rc.top) > nDlgHeight )
			y += (rc.bottom - rc.top - nDlgHeight) / 2;

		x += SDM_PARENT_OFFSET_X;
		y += SDM_PARENT_OFFSET_Y;
	}
	::SetWindowPos( m_hWnd, NULL, x, y, nDlgWidth, nDlgHeight, SWP_NOZORDER );
	
	if( m_fUseIcon )
	{
		SetDlgItemPos( SDC_ICON, SDM_BORDER_LEFT+SDM_ICON_OFFSET_CX, SDM_BORDER_TOP+SDM_ICON_OFFSET_CY, 0,0, SWP_NOSIZE );
		GetDlgItemRect( SDC_ICON, &rc );
		rc.bottom -= (rc.top-SDM_ICON_OFFSET_CY);
		int nTextTop = (m_rcText.bottom < rc.bottom) ? (rc.bottom - m_rcText.bottom) / 2 : 0;
//		SetDlgItemPos( SDC_TEXT, rc.right + SDM_ICON_TO_TEXT_SPACING, nTextTop + SDM_BORDER_TOP, m_rcText.right + SDM_BORDER_RIGHT - 1, m_rcText.bottom + 1, 0 );
		SetDlgItemPos( SDC_TEXT, rc.right + SDM_ICON_TO_TEXT_SPACING, nTextTop + SDM_BORDER_TOP, m_rcText.right + 1, m_rcText.bottom + 1, 0 );
	}
	else
	{
//		SetDlgItemPos( SDC_TEXT, SDM_BORDER_LEFT, SDM_BORDER_TOP, m_rcText.right + SDM_BORDER_RIGHT - 1, m_rcText.bottom + 1, 0 );
		SetDlgItemPos( SDC_TEXT, SDM_BORDER_LEFT, SDM_BORDER_TOP, m_rcText.right + 1, m_rcText.bottom + 1, 0 );
	}

	// Reposition button(s)
	if( nOneButtonID != 0 )
	{
		GetClientRect( &rc );
		if( nButton1width != 0 && nButton2width != 0 )
		{
			// 2 buttons
			SetDlgItemPos( SDC_BUTTON1,
				(rc.right - nButtonsWidth)/2, rc.bottom - SDM_BORDER_BOTTOM - nButtonsHeight,
				nButton1width, nButtonsHeight, 0 );
			GetDlgItemRect( SDC_BUTTON1, &rc );
			SetDlgItemPos( SDC_BUTTON2,
				rc.right + SDM_BUTTONS_CX_SPACING, rc.top,
				nButton2width, nButtonsHeight, 0 );
		}
		else
		{
			// 1 button
			SetDlgItemPos( nOneButtonID, (rc.right-nButtonsWidth)/2, rc.bottom-SDM_BORDER_BOTTOM-nButtonsHeight, 0,0, SWP_NOSIZE );
		}
	}
}

void CSfxDialog::ResizeAndPositionButton( int nButtonID, LPCWSTR lpwszText )
{
	RECT rc1, rc2;
	if( CalculateTextRect( lpwszText, &rc1, m_hFont, DT_SINGLELINE ) )
	{
		rc1.right += 32;
		GetDlgItemRect( nButtonID, &rc2 );
		if( (rc2.right-rc2.left) < rc1.right )
		{
			int nExtra = (rc1.right-rc2.right+rc2.left+1)/2;
			SetDlgItemPos( nButtonID, rc2.left-nExtra, rc2.top, (rc2.right-rc2.left+nExtra*2), rc2.bottom-rc2.top, 0 );
		}
	}
}

INT_PTR CSfxDialog::ShowImpl( HWND hwndParent )
{
	BYTE LocalDlgTemplate[sizeof(m_DialogsTemplate)];
	memcpy( LocalDlgTemplate, m_DialogsTemplate, sizeof(m_DialogsTemplate) );
	NONCLIENTMETRICS	ncm;
	ncm.cbSize = sizeof(ncm);
	if( SystemParametersInfo( SPI_GETNONCLIENTMETRICS, 0, &ncm, 0 ) )
	{
		HDC hdc = ::GetDC( NULL );
		int nHeight = -MulDiv( ncm.lfMessageFont.lfHeight, 72, GetDeviceCaps(hdc, LOGPIXELSY) );
		::ReleaseDC( NULL, hdc );
		*((LPWORD)(LocalDlgTemplate+32)) = (WORD)nHeight;
		*((LPWORD)(LocalDlgTemplate+34)) = (WORD)ncm.lfMessageFont.lfWeight;
		*((LPBYTE)(LocalDlgTemplate+36)) = ncm.lfMessageFont.lfItalic;
	}
	// new
	LPCDLGTEMPLATE lpDlgTemplate = NULL;
	if( m_uDlgResourceId != 0 )
	{
#ifdef _SFX_USE_LANG
		lpDlgTemplate = (LPCDLGTEMPLATE)LoadInterfaceResource( (LPCSTR)RT_DIALOG, MAKEINTRESOURCEA(m_uDlgResourceId) );
#else
		if( HRSRC hRsrc = FindResource( hRsrcModule, MAKEINTRESOURCE(m_uDlgResourceId), RT_DIALOG ) )
		{
			if( HGLOBAL hGb = LoadResource( hRsrcModule, hRsrc ) )
			{
				lpDlgTemplate = static_cast<LPCDLGTEMPLATE>( LockResource( hGb ) );
			}
		}
#endif // _SFX_USE_LANG
	}
	if( lpDlgTemplate == NULL )
	{
		lpDlgTemplate = (LPCDLGTEMPLATE)LocalDlgTemplate;
		m_uDlgResourceId = 0;
	}
	return DialogBoxIndirectParam( hRsrcModule, lpDlgTemplate, hwndParent, SfxDialogProc, (LPARAM)this );
}

void CSfxDialog::SetCaption( LPCWSTR lpwszCaption )
{
	CSfxStringU ustrCaption;
	while( *lpwszCaption != L'\0' )
	{
		if( *lpwszCaption < ' ' )
			ustrCaption += L' ';
		else
			ustrCaption += *lpwszCaption;
		lpwszCaption++;
	}
	SetWindowText( m_hWnd, ustrCaption );
}
/*--------------------------------------------------------------------------*/
// CSfxDialog_BeginPromptClassic
/*--------------------------------------------------------------------------*/
#ifdef _SFX_USE_BEGINPROMPTTIMEOUT
BOOL CSfxDialog_BeginPromptClassic::OnInitDialog()
{
	CSfxDialog::OnInitDialog();
	SetButtonTimer( BeginPromptTimeout );
	if( m_uDlgResourceId == 0 )
		ResizeAndPosition();
	return FALSE;
}
#endif // _SFX_USE_BEGINPROMPTTIMEOUT
/*--------------------------------------------------------------------------*/
// CSfxDialog_Error
/*--------------------------------------------------------------------------*/
BOOL CSfxDialog_Error::OnInitDialog()
{
	CSfxDialog::OnInitDialog();
	MessageBeep( MB_ICONHAND );
	return FALSE;
}

#ifdef _SFX_USE_WARNINGS
/*--------------------------------------------------------------------------*/
// CSfxDialog_Warning
/*--------------------------------------------------------------------------*/
BOOL CSfxDialog_Warning::OnInitDialog()
{
	CSfxDialog::OnInitDialog();
	SetDefaultButton( SDC_BUTTON2 );
	MessageBeep( MB_ICONEXCLAMATION );
	return FALSE;
}
#endif // _SFX_USE_WARNINGS

/*--------------------------------------------------------------------------*/
// CSfxDialog_CancelPrompt
/*--------------------------------------------------------------------------*/
BOOL CSfxDialog_CancelPrompt::OnInitDialog()
{
	CSfxDialog::OnInitDialog();
	SetDefaultButton( SDC_BUTTON2 );
	MessageBeep( MB_ICONEXCLAMATION );
	return FALSE;
}

INT_PTR CSfxDialog_CancelPrompt::IsCancel( HWND hParentWnd )
{
	if( (GUIFlags & GUIFLAGS_CONFIRM_CANCEL) == 0 )
		return TRUE;
#ifdef _SFX_USE_TEST
	if( hParentWnd != NULL && !TSD_Flags.IsEmpty() )
		return TRUE;
#endif // _SFX_USE_TEST
	return CSfxDialog::Show( SD_YESNO|SD_ICONQUESTION, lpwszTitle, lpwszCancelPrompt, hParentWnd );
}


/*--------------------------------------------------------------------------*/
// CSfxDialog_ExtractPath
/*--------------------------------------------------------------------------*/
BOOL CSfxDialog_ExtractPath::OnInitDialog()
{
	ShowControl( SDC_EXTRACTPATHEDIT, TRUE );
	ShowControl( SDC_EXTRACTPATHBROWSE, TRUE );
	
	SHFILEINFO shfi = { 0 };
	SHGetFileInfo( L"~", FILE_ATTRIBUTE_DIRECTORY, &shfi, sizeof shfi, SHGFI_ICON | SHGFI_SMALLICON | SHGFI_OPENICON | SHGFI_USEFILEATTRIBUTES );
	m_hBrowseIcon = shfi.hIcon;

	m_pfnOldButtonProc = (WNDPROC)::SetWindowLongPtr( GetDlgItem(SDC_EXTRACTPATHBROWSE), GWLP_WNDPROC, (LONG_PTR)ButtonIconProc );
	BOOL retValue = CSfxDialog::OnInitDialog();
	SetPathText();
	return retValue;
}

void CSfxDialog_ExtractPath::CalculateDialogSize()
{
	CSfxDialog::CalculateDialogSize();
	int cxMin = ExtractPathWidth - ::GetSystemMetrics(SM_CXDLGFRAME);
	if( m_dlgSize.cx < cxMin )
		m_dlgSize.cx = cxMin;
	RECT rc;
	GetDlgItemRect( SDC_EXTRACTPATHEDIT, &rc );
	m_dlgSize.cy += (rc.bottom - rc.top) + SDM_EXTRACTPATH_SPACING;
}

void CSfxDialog_ExtractPath::ResizeAndPosition()
{
	CSfxDialog::ResizeAndPosition();
	RECT rc1, rc2;
	GetDlgItemRect( SDC_BUTTON1, &rc1 );
	GetDlgItemRect( SDC_EXTRACTPATHEDIT, &rc2 );
	int nBrowseButtonSize = rc2.bottom - rc2.top + 2;
	rc1.top -= (rc2.bottom - rc2.top + SDM_BUTTONS_CY_SPACING);
	rc1.bottom = rc1.top + (rc2.bottom - rc2.top);
	rc1.left = SDM_BORDER_LEFT;
	GetClientRect( &rc2 );
	rc1.right = rc2.right - nBrowseButtonSize - SDM_BORDER_RIGHT + 1;
	SetDlgItemPos( SDC_EXTRACTPATHEDIT, rc1.left, rc1.top, rc1.right - rc1.left, rc1.bottom - rc1.top, 0 );
	SetDlgItemPos( SDC_EXTRACTPATHBROWSE, rc2.right - nBrowseButtonSize - SDM_BORDER_RIGHT + 3, rc1.top - 1,
					nBrowseButtonSize, nBrowseButtonSize, 0 );
}

LRESULT CALLBACK CSfxDialog_ExtractPath::ButtonIconProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	CSfxDialog_ExtractPath *pThis = (CSfxDialog_ExtractPath *)GetWindowLongPtr( GetParent(hwnd), GWLP_USERDATA );
	LRESULT Result = ::CallWindowProc( pThis->m_pfnOldButtonProc, hwnd, message, wParam, lParam );
	switch( message )
	{
	case WM_PAINT:
		if ( HDC hDC = ::GetDC( hwnd ) )
		{
			RECT rc;
			::GetClientRect( hwnd, &rc );
			int const w = ::GetSystemMetrics( SM_CXSMICON );
			int const h = ::GetSystemMetrics( SM_CYSMICON );
			int const x = (rc.right - rc.left - w) / 2;
			int const y = (rc.bottom - rc.top - h) / 2;
			::DrawIconEx( hDC, x, y, pThis->m_hBrowseIcon, w, h, 0, NULL, DI_MASK | DI_IMAGE );
			::ReleaseDC( hwnd, hDC );
		}
		break;
	}
	return Result;
}

void CSfxDialog_ExtractPath::SetPathText()
{
	::SetFocus( GetDlgItem(SDC_EXTRACTPATHEDIT) );
	SetDlgItemText( SDC_EXTRACTPATHEDIT, extractPath );
	SendDlgItemMessage( SDC_EXTRACTPATHEDIT, EM_SETSEL, extractPath.Len(), extractPath.Len() );
}

void CSfxDialog_ExtractPath::OnCommand( int nControlID )
{
	if( nControlID == SDC_EXTRACTPATHBROWSE )
	{
		BrowseExtractPath();
	}
	else if( nControlID == SDC_BUTTON1 )
	{
		GetDlgItemText( SDC_EXTRACTPATHEDIT, extractPath );
	}
	CSfxDialog::OnCommand( nControlID );
}

void CSfxDialog_ExtractPath::BrowseExtractPath()
{
	BROWSEINFO	bi;
	TCHAR		szPath[MAX_PATH+1];

	memset( &bi, 0, sizeof(bi) );
	bi.hwndOwner = m_hWnd;
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;
	
	LPITEMIDLIST pidl = SHBrowseForFolder( &bi );

	if( pidl == NULL )
		return;

	szPath[0] = '\0';

	if( SHGetPathFromIDList( pidl, szPath ) )
	{
		extractPath = szPath;
		SetPathText();
	}

	IMalloc *pMalloc = NULL; 
	if( SHGetMalloc( &pMalloc ) == NOERROR && pMalloc != NULL ) 
	{  
		pMalloc->Free( pidl );  
		pMalloc->Release(); 
	}
}

/*--------------------------------------------------------------------------*/
// CSfxDialog_BeginPromptWithExtractPath
/*--------------------------------------------------------------------------*/
BOOL CSfxDialog_BeginPromptWithExtractPath::OnInitDialog()
{
	ShowControl( SDC_TEXT2, TRUE );
	SetDlgItemText( SDC_TEXT2, lpwszExtractPathText );
	SetPathText();

	CSfxDialog_ExtractPath::OnInitDialog();

#ifdef _SFX_USE_BEGINPROMPTTIMEOUT
	SetButtonTimer( BeginPromptTimeout );
#endif // _SFX_USE_BEGINPROMPTTIMEOUT
	if( m_uDlgResourceId == 0 )
		ResizeAndPosition();

	return FALSE;
}

void CSfxDialog_BeginPromptWithExtractPath::CalculateDialogSize()
{
	CSfxDialog_ExtractPath::CalculateDialogSize();

	m_rcExtractPathText.left = m_rcExtractPathText.top =
		m_rcExtractPathText.right = m_rcExtractPathText.bottom = 0;
	CSfxStringU ustrExtractPathText;
	GetDlgItemText( SDC_TEXT2, ustrExtractPathText );
	if( CalculateTextRect( ustrExtractPathText, &m_rcExtractPathText, m_hFont, DT_LEFT|DT_NOPREFIX|DT_WORDBREAK|DT_EXPANDTABS ) )
	{
		int cxMin = m_rcExtractPathText.right + SDM_BORDER_LEFT + SDM_BORDER_RIGHT;
		if( m_dlgSize.cx < cxMin )
			m_dlgSize.cx = cxMin;
		m_dlgSize.cy += m_rcExtractPathText.bottom + SDM_EXTRACT_PATH_SPACE_TO_TEXT +
							SDM_EXTRACT_PATH_SPACE_TO_EDIT - SDM_EXTRACTPATH_SPACING;
	}
}

void CSfxDialog_BeginPromptWithExtractPath::ResizeAndPosition()
{
	CSfxDialog_ExtractPath::ResizeAndPosition();

	RECT rc;
	GetDlgItemRect( SDC_EXTRACTPATHEDIT, &rc );
	SetDlgItemPos( SDC_TEXT2, rc.left, rc.top - m_rcExtractPathText.bottom - SDM_EXTRACT_PATH_SPACE_TO_EDIT,
//										m_rcExtractPathText.right + SDM_BORDER_RIGHT - 1, m_rcExtractPathText.bottom + 1, 0 );
										m_rcExtractPathText.right + 1, m_rcExtractPathText.bottom + 1, 0 );
	SetPathText();
}


/*--------------------------------------------------------------------------*/
// CSfxDialog_FinishMessage
/*--------------------------------------------------------------------------*/
BOOL CSfxDialog_FinishMessage::OnInitDialog()
{
	CSfxDialog::OnInitDialog();
	if( FinishMessage > 1 )
		SetButtonTimer( FinishMessage );
	MessageBeep( MB_ICONASTERISK );
	return FALSE;
}


/*--------------------------------------------------------------------------*/
// CSfxDialog_Extract
/*--------------------------------------------------------------------------*/
HMODULE hRsrcModule = NULL;
HWND hwndExtractDlg = NULL;
CSfxDialog_Extract *pwndExtractDialog = NULL;
BOOL fCancelExtract = FALSE;

#ifdef _SFX_USE_WIN7_PROGRESSBAR
void CSfxDialog_Extract::SetTaskbarState( TBPFLAG tbpFlags )
{
	if( m_pTaskbarList )
		m_pTaskbarList->SetProgressState( m_hWnd, tbpFlags );
}
#endif // _SFX_USE_WIN7_PROGRESSBAR

void CSfxDialog_Extract::SetMarquee( BOOL bMarquee )
{
	if( HWND hwndProgress = ::GetDlgItem( m_hWnd, SDC_PROGRESS ) )
	{
		LONG const lStyle = ::GetWindowLong( hwndProgress, GWL_STYLE );
		if ( bMarquee )
		{
			::SetWindowLong( hwndProgress, GWL_STYLE, lStyle | PBS_MARQUEE );
			::SendMessage( hwndProgress, PBM_SETMARQUEE, TRUE, 0 );
		}
		else
		{
			::SendMessage( hwndProgress, PBM_SETMARQUEE, FALSE, 0 );
			::SetWindowLong( hwndProgress, GWL_STYLE, lStyle & ~PBS_MARQUEE );
		}
	}
}

void CSfxDialog_Extract::SetPercentTextFont()
{
	if( (GUIFlags & GUIFLAGS_PERCENT_BOLD) != 0 )
	{
		LOGFONT logFont;
		if( ::GetObject( m_hFont,sizeof(LOGFONT),&logFont ) != 0 )
		{
			logFont.lfWeight = FW_BOLD;
			HFONT hFont = ::CreateFontIndirect( &logFont );
			if( hFont != NULL )
				SendDlgItemMessage( SDC_TEXT2, WM_SETFONT, (WPARAM)hFont, FALSE );
		}
	}
}

BOOL CSfxDialog_Extract::OnInitDialog()
{
	hwndExtractDlg = m_hWnd;
	pwndExtractDialog = this;
	ShowControl(SDC_PROGRESS, TRUE);
	SendDlgItemMessage(SDC_PROGRESS, PBM_SETRANGE, 0, MAKELPARAM(0, MAX_PROGRESS_VALUE));
	if (lpwszCancelText != NULL)
	{
		ResizeAndPositionButton(SDC_BUTTON2, lpwszCancelText);
		SetDlgItemText(SDC_BUTTON2, lpwszCancelText);
	}
	if ((GUIFlags & GUIFLAGS_PERCENT_TEXT) != 0)
	{
		ShowControl(SDC_TEXT2, TRUE);
		HWND hwndControl = GetDlgItem(SDC_TEXT2);
		LONG lStyle = ::GetWindowLong(hwndControl, GWL_STYLE);
		::SetWindowLong(hwndControl, GWL_STYLE, lStyle | SS_CENTER | SS_CENTERIMAGE);
		SetPercentTextFont();
	}
	if (GUIMode == GUIMODE_NOCANCEL)
	{
		ShowControl(SDC_BUTTON2, FALSE);
		HMENU hMenu = GetSystemMenu(m_hWnd, FALSE);
		if (hMenu != NULL)
			EnableMenuItem(hMenu, SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);
	}

	SetFocus(GetDlgItem(SDC_BUTTON2));

#ifdef _SFX_USE_TEST
	if (TSD_ExtractTimeout != 0)
	{
		m_i64TestCurrent = 0;
		m_i64ProgressTotal = 100;
		SetTimer(m_hWnd, 1, TSD_ExtractTimeout * 10, NULL);
	}
#endif // _SFX_USE_TEST

#ifdef _SFX_USE_WIN7_PROGRESSBAR
	if ((GUIFlags & GUIFLAGS_NO_WIN7_PROGRESSBAR) == 0)
		::CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pTaskbarList));
	SetTaskbarState( TBPF_INDETERMINATE );
#endif // _SFX_USE_WIN7_PROGRESSBAR

	if( GUIMode == GUIMODE_NOCANCEL )
	{
		if( HWND hwndControl = GetDlgItem(IDCANCEL) )
			EnableWindow( hwndControl, FALSE );
	}
	if( (GUIFlags & GUIFLAGS_PERCENT_TEXT) == 0 )
		ShowWindow( GetDlgItem(SDC_TEXT2), SW_HIDE );
	__int64 i64Init = 0;
	SetPercents( (LPARAM)(&i64Init) );

	if (m_pSfxExtractEngine)
	{
		if (HANDLE hExtractThread = m_pSfxExtractEngine->GetExtractThread())
			::ResumeThread(hExtractThread);
	}

	return CSfxDialog::OnInitDialog();
}

void CSfxDialog_Extract::CalculateDialogSize()
{
#define PERCENTS_100	L" 100%% "
	CSfxDialog::CalculateDialogSize();
	int cxMin = ExtractDialogWidth - ::GetSystemMetrics(SM_CXDLGFRAME);
	if( m_dlgSize.cx < cxMin )
		m_dlgSize.cx = cxMin;
	// title
	SIZE sizeTitle;
	CSfxStringU strFullTitle = m_strCaption;
	if( (GUIFlags & GUIFLAGS_TITLE_PERCENT_NONE) == 0 )
		strFullTitle += PERCENTS_100;
	if( CalculateTitleSize( strFullTitle, &sizeTitle ) && sizeTitle.cx > m_dlgSize.cx )
		m_dlgSize.cx = sizeTitle.cx;

	RECT rc;
	GetDlgItemRect( SDC_PROGRESS, &rc );
	m_dlgSize.cy += (rc.bottom - rc.top) + SDM_PROGRESS_TO_TEXT;
	if( (GUIFlags & GUIFLAGS_PERCENT_TEXT) != 0 )
	{
		if( CalculateTextRect( PERCENTS_100, &m_rcPercentText, m_hFont, DT_LEFT|DT_NOPREFIX|DT_SINGLELINE ) )
		{
			m_dlgSize.cy += m_rcPercentText.bottom;
		}
		m_dlgSize.cy += SDM_PROGRESS_TO_PERCENT;
	}
	m_dlgSize.cy -= SDM_BUTTONS_CY_SPACING;
	if( GUIMode == GUIMODE_NOCANCEL )
	{
		GetDlgItemRect( SDC_BUTTON2, &rc );
		m_dlgSize.cy -= rc.bottom - rc.top;
	}
	else
	{
		m_dlgSize.cy += SDM_PROGRESS_BUTTON_SPACING;
	}
}

void CSfxDialog_Extract::ResizeAndPosition()
{
	CSfxDialog::ResizeAndPosition();

	RECT	rc;
	int nBottom;
	if( GUIMode == GUIMODE_NOCANCEL )
	{
		GetClientRect( &rc );
		nBottom = rc.bottom-SDM_BORDER_BOTTOM;
	}
	else
	{
		GetDlgItemRect( SDC_BUTTON2, &rc );
		nBottom = rc.top - SDM_PROGRESS_BUTTON_SPACING;
	}

	GetClientRect( &rc );
	int nClientWidth = rc.right - SDM_BORDER_LEFT - SDM_BORDER_RIGHT + 2;
	if( (GUIFlags & GUIFLAGS_PERCENT_TEXT) != 0 )
	{
		nBottom -= m_rcPercentText.bottom;
		SetDlgItemPos( SDC_TEXT2, SDM_BORDER_LEFT, nBottom, nClientWidth, m_rcPercentText.bottom+1, 0 );
		nBottom -= SDM_PROGRESS_TO_PERCENT;
	}

	GetDlgItemRect( SDC_PROGRESS, &rc );
	nBottom -= (rc.bottom-rc.top);
	SetDlgItemPos( SDC_PROGRESS, SDM_BORDER_LEFT, nBottom, nClientWidth, rc.bottom-rc.top, 0 );
}

void CSfxDialog_Extract::SetPercents( LPARAM lPercents )
{
	int			ipos;
	WCHAR		wszPercentText[12];
	CSfxStringU	Title;

	if( m_i64ProgressTotal != 0 )
		ipos = (int)((*((__int64 *)lPercents) * MAX_PROGRESS_VALUE) / m_i64ProgressTotal);
	else
		ipos = 0;
	if( ipos > MAX_PROGRESS_VALUE )
		ipos = MAX_PROGRESS_VALUE;
	SendDlgItemMessage( SDC_PROGRESS, PBM_SETPOS, ipos, 0 );
	wsprintf( wszPercentText, L"%d%%", (ipos+((MAX_PROGRESS_VALUE/100)-1))/(MAX_PROGRESS_VALUE/100) );
	if( (GUIFlags & GUIFLAGS_PERCENT_TEXT) != 0 )
	{
		SetDlgItemText( SDC_TEXT2, wszPercentText );
	}
	if( (GUIFlags & GUIFLAGS_TITLE_PERCENT_NONE) == 0 )
	{
		// percents in title present
		if( (GUIFlags & GUIFLAGS_TITLE_PERCENT_RIGHT) != 0 )
		{
			// percents on right
			Title = lpwszExtractTitle;
			Title += L' ';
			Title += wszPercentText;
		}
		else
		{
			// percents on left
			Title = wszPercentText;
			Title += L' ';
			Title += lpwszExtractTitle;
		}
		SetCaption( Title );
	}
#ifdef _SFX_USE_WIN7_PROGRESSBAR
	if( m_pTaskbarList )
	{
		m_pTaskbarList->SetProgressState( m_hWnd, TBPF_NORMAL );
		m_pTaskbarList->SetProgressValue( m_hWnd, *((__int64 *)lPercents), m_i64ProgressTotal );
	}
#endif // _SFX_USE_WIN7_PROGRESSBAR
}

INT_PTR CSfxDialog_Extract::DialogProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
#ifdef _SFX_USE_TEST
	case WM_TIMER:
		m_i64TestCurrent++;
		if( m_i64TestCurrent > 100 )
			EndDialog( TRUE );
		SendMessage( WM_7ZSFX_SETCOMPLETED, 0, (LPARAM)&m_i64TestCurrent );
		break;
#endif // _SFX_USE_TEST
	case WM_7ZSFX_SETTOTAL:
		m_i64ProgressTotal = *((__int64 *)lParam);
		#ifdef _SFX_USE_WIN7_PROGRESSBAR
			if( m_pTaskbarList )
			{
				m_pTaskbarList->SetProgressState( m_hWnd, TBPF_NORMAL );
				m_pTaskbarList->SetProgressValue( m_hWnd, 0, m_i64ProgressTotal );
			}
		#endif // _SFX_USE_WIN7_PROGRESSBAR
		break;
	case WM_7ZSFX_SETCOMPLETED:
		SetPercents( lParam );
		break;
	case WM_DESTROY:
		hwndExtractDlg = NULL;
		pwndExtractDialog = NULL;
#ifdef _SFX_USE_WIN7_PROGRESSBAR
		if( m_pTaskbarList )
		{
			m_pTaskbarList->Release();
			m_pTaskbarList = NULL;
		}
#endif
		break;
	}
	return CSfxDialog::DialogProc( uMsg, wParam, lParam );
}

INT_PTR CSfxDialog_Extract::IsCancel()
{
	CSfxDialog_CancelPrompt	cancelPrompt;
#ifdef _SFX_USE_WIN7_PROGRESSBAR
	SetTaskbarState( TBPF_PAUSED );
#endif // _SFX_USE_WIN7_PROGRESSBAR
	INT_PTR bRet = cancelPrompt.IsCancel( m_hWnd );
#ifdef _SFX_USE_WIN7_PROGRESSBAR
	SetTaskbarState( TBPF_NORMAL );
#endif // _SFX_USE_WIN7_PROGRESSBAR
	return bRet;
}

#if defined(_MSC_VER) && _MSC_VER == 1900
	#pragma warning(disable:6258)
#endif // _MSC_VER && _MSC_VER == 1200

void CSfxDialog_Extract::OnCancel()
{
#ifdef _SFX_USE_TEST
	if( !TSD_Flags.IsEmpty() )
		EndDialog( FALSE );
	if( TSD_ExtractTimeout != 0 )
	{
		KillTimer( m_hWnd, 1 );
		if( (GUIFlags & GUIFLAGS_CONFIRM_CANCEL) == 0 || IsCancel() )
		{
			EndDialog( FALSE );
			return;
		}
		SetTimer( m_hWnd, 1, TSD_ExtractTimeout * 10, NULL );
	}
#endif // _SFX_USE_TEST

	if( m_pSfxExtractEngine && (GUIMode != GUIMODE_NOCANCEL) )
	{
		HANDLE hExtractThread = m_pSfxExtractEngine->GetExtractThread();
		if( hExtractThread == NULL )
			return;
		::SuspendThread( hExtractThread );
		if( (GUIFlags & GUIFLAGS_CONFIRM_CANCEL) == 0 || IsCancel() )
		{
			fCancelExtract = TRUE;
			::TerminateThread( hExtractThread, 22 );
			EndDialog(FALSE);
		}
		else
		{
			::ResumeThread( hExtractThread );
		}
	}
}

#if defined(_MSC_VER) && _MSC_VER == 1900
	#pragma warning(default:6258)
#endif // _MSC_VER && _MSC_VER == 1200

#ifdef SFX_CRYPTO
/*--------------------------------------------------------------------------*/
// CSfxDialog_Password
/*--------------------------------------------------------------------------*/
BOOL CSfxDialog_Password::OnInitDialog()
{
	CSfxDialog_ExtractPath::OnInitDialog();
	RECT rc1, rc2;
	ShowControl( SDC_EXTRACTPATHBROWSE, FALSE );
	GetDlgItemRect( SDC_EXTRACTPATHBROWSE, &rc2 );
	GetDlgItemRect( SDC_PASSWORDEDIT, &rc1 );
	::DestroyWindow( GetDlgItem(SDC_PASSWORDEDIT) );
	::CreateWindowExA( WS_EX_CLIENTEDGE,
						WC_EDITA, "",
						WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_PASSWORD|ES_LEFT|ES_AUTOHSCROLL,
						rc1.left,rc1.top, rc2.right-rc1.left, rc1.bottom-rc1.top,
						m_hWnd, (HMENU)SDC_PASSWORDEDIT, NULL, NULL );
	SendDlgItemMessage( SDC_PASSWORDEDIT, WM_SETFONT, SendMessage(WM_GETFONT,0,0), 1 );
	SetFocus( GetDlgItem(SDC_PASSWORDEDIT) );
	return 0;
}

void CSfxDialog_Password::OnCommand( int nControlID )
{
	if( nControlID == SDC_BUTTON1 )
		GetDlgItemText( SDC_PASSWORDEDIT, m_strPassword );
	CSfxDialog::OnCommand( nControlID );
}
#endif // SFX_CRYPTO

///////////////////////////////////////////////////////////////////////////////
BOOL SfxBeginPrompt( LPCWSTR lpwszCaption, LPCWSTR lpwszText )
{
	UINT uType = (GUIFlags & GUIFLAGS_BP_OKCANCEL) != 0 ? SD_OKCANCEL : SD_YESNO;
	if( (GUIFlags & GUIFLAGS_BP_MODULEICON) != 0 )
	{
		uType += SD_ICONMODULE;
	}
	else
	{
		if( (GUIFlags & GUIFLAGS_BP_OKCANCEL) != 0 )
			uType += SD_ICONINFORMATION;
		else
			uType += SD_ICONQUESTION;
	}
	if( (GUIFlags & GUIFLAGS_EXTRACT_PATH1) == 0 )
	{
		CSfxDialog_BeginPromptClassic bp;
		return (BOOL)bp.Show( uType, lpwszCaption, lpwszText );
	}
	else
	{
		CSfxDialog_BeginPromptWithExtractPath bp;
		return (BOOL)bp.Show( uType, lpwszCaption, lpwszText );
	}
}

BOOL SfxExtractPathDialog( LPCWSTR lpwszTitle, LPCWSTR lpwszText )
{
	UINT uType = SD_OKCANCEL;
	if( (GUIFlags & GUIFLAGS_EPD_USEICON) != 0 )
		uType += SD_ICONMODULE;
	CSfxDialog_ExtractPath	dlg;
	return (BOOL)dlg.Show( uType, lpwszTitle, lpwszText );
}

void ShowSfxErrorDialog( LPCWSTR lpwszMessage )
{
	static bool fErrorShown = false;
	if( fErrorShown )
		return;
	fErrorShown = true;

	CSfxDialog_Error dlg;
#ifdef _SFX_USE_WIN7_PROGRESSBAR
	if( pwndExtractDialog )
		pwndExtractDialog->SetTaskbarState( TBPF_ERROR );
#endif // _SFX_USE_WIN7_PROGRESSBAR
	dlg.Show( SD_OK|SD_ICONSTOP, lpwszErrorTitle, lpwszMessage, hwndExtractDlg );
}

#ifdef _SFX_USE_WARNINGS
INT_PTR ShowSfxWarningDialog( LPCWSTR lpwszMessage )
{
	CSfxDialog_Warning dlg;
#ifdef _SFX_USE_WIN7_PROGRESSBAR
	if( pwndExtractDialog )
		pwndExtractDialog->SetTaskbarState( TBPF_PAUSED );
#endif // _SFX_USE_WIN7_PROGRESSBAR
	return dlg.Show( SD_YESNO|SD_ICONWARNING, lpwszWarningTitle, lpwszMessage, hwndExtractDlg );
}
#endif // _SFX_USE_WARNINGS

void SfxErrorDialog( BOOL fUseLastError, UINT idFormat, ... )
{
	WCHAR buf[1024];
	va_list va;

	LPCWSTR lpwszFormat = GetLanguageString( idFormat );
	va_start( va, idFormat );
	int nMessageLength = wvsprintf( buf, lpwszFormat, va );

	if( fUseLastError )
	{
		LPWSTR lpMsgBuf;
		DWORD dwLastError = ::GetLastError();
		LANGID idMsgLang = 0;
		DWORD dwMsgLen;
		do
		{
			idMsgLang ^= idSfxLang;
			dwMsgLen = ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
										NULL, dwLastError, idMsgLang, (LPWSTR)&lpMsgBuf, 0, &va );
		} while (dwMsgLen == 0 && idMsgLang != 0);
		if (dwMsgLen != 0)
		{
			LPWSTR lpszFullMessage = new WCHAR[nMessageLength + dwMsgLen + 2];
			wcscpy( lpszFullMessage, buf );
			lpszFullMessage[nMessageLength] = L'\n';
			wcscpy( lpszFullMessage + nMessageLength + 1, lpMsgBuf );
			ShowSfxErrorDialog( lpszFullMessage );
			delete[] lpszFullMessage;
			::LocalFree( lpMsgBuf );
			return;
		}
	}
	ShowSfxErrorDialog( buf );
}

INT_PTR ExtractDialog(CSfxExtractEngine *pSfxExtractEngine)
{
	UINT uType = SD_CANCEL;
	if( lpwszExtractDialogText == NULL )
		lpwszExtractDialogText = L"";

	if( (GUIFlags & GUIFLAGS_USEICON) != 0 )
		uType |= SD_ICONMODULE;
	CSfxDialog_Extract dlg(pSfxExtractEngine);
	return dlg.Show( uType, lpwszExtractTitle, lpwszExtractDialogText );
}
