#include <windows.h>
#pragma comment( lib, "C:/WinDDK/7600.16385.1/lib/Crt/amd64/msvcrt.lib" )

void ReportException( PEXCEPTION_RECORD rec )
{
	MessageBoxA( NULL, "ReportException Called", "ReportException", MB_OK|MB_ICONINFORMATION );  // Never shown!
	ExitProcess( ERRC_EXCEPTION );  // When this line is commented out, the two message boxes are shown correctly
}

extern "C" WINBASEAPI __out PVOID WINAPI AddVectoredExceptionHandler(__in ULONG First,__in PVECTORED_EXCEPTION_HANDLER Handler);
extern "C" WINBASEAPI ULONG WINAPI RemoveVectoredExceptionHandler(__in PVOID Handle);

class CCustomExceptions
{
public:
	CCustomExceptions();
	~CCustomExceptions();
	static LONG WINAPI ExceptionHandler( LPEXCEPTION_POINTERS pExection );
};

static CCustomExceptions __cef;

CCustomExceptions::CCustomExceptions()
{
	AddVectoredExceptionHandler( 0, ExceptionHandler );
	MessageBoxA( NULL, "AddVectoredExceptionHandler Successful", "CCustomExceptions::CCustomExceptions", MB_OK|MB_ICONINFORMATION );
}
CCustomExceptions::~CCustomExceptions()
{
	RemoveVectoredExceptionHandler( ExceptionHandler );
	MessageBoxA( NULL, "RemoveVectoredExceptionHandler Successful", "CCustomExceptions::~CCustomExceptions", MB_OK|MB_ICONINFORMATION );
}

LONG WINAPI CCustomExceptions::ExceptionHandler( LPEXCEPTION_POINTERS pException )
{
	MessageBoxA( NULL, "ExceptionHandler Called", "CCustomExceptions::ExceptionHandler", MB_OK|MB_ICONINFORMATION );  // Never shown!
	ReportException( pException->ExceptionRecord );
	return NULL;
}

int APIENTRY WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow )
{
	MessageBoxA( NULL, "WinMain Called", "WinMain", MB_OK|MB_ICONINFORMATION );
}

