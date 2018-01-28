//
// Code for VS_VERSION resource
// Modified from MIT licensed code published at https://ddverpatch.codeplex.com/
//
#include "stdafx.h"
#include "vs_version.h"

// Stupid helper classes for the version struct
class yybuf
{
	PBYTE const m_startptr;
	PBYTE m_curptr;
	PBYTE const m_endptr;

public:
	yybuf(PBYTE start, DWORD size)
		: m_startptr(start), m_curptr(start), m_endptr(start + size)
	{
		// ensure 32-bit alignment
		assert((reinterpret_cast<INT_PTR>(m_curptr) & 3) == 0);
		assert((reinterpret_cast<INT_PTR>(m_endptr) & 3) == 0);
	}

	void align4()
	{
		while (reinterpret_cast<INT_PTR>(m_curptr) & 3)
			*m_curptr++ = 0;
	}

	DWORD cbwritten() const
	{
		return m_curptr ? static_cast<DWORD>(m_curptr - m_startptr) : 0;
	}

	PVOID incptr(DWORD n)
	{
		PVOID p = m_curptr;
		m_curptr += n;
		if (m_curptr > m_endptr) // overflow?
			p = m_curptr = NULL;
		return p;
	}

	void pushw(WORD v)
	{
		if (PWORD p = static_cast<PWORD>(incptr(sizeof(WORD))))
			*p = v;
	}

	void pushstr(LPCWSTR ws)
	{
		DWORD n = static_cast<DWORD>(wcslen(ws));
		n = (n + 1) * sizeof(WCHAR);
		if (PVOID p = incptr(n))
			memcpy(p, ws, n);
	}

	PWORD marksize()
	{
		return static_cast<PWORD>(incptr(sizeof(WORD)));
	}

	void patchsize(PWORD mp)
	{
		if (mp)
			*mp = static_cast<WORD>(m_curptr - reinterpret_cast<PBYTE>(mp));
	};

	void pushTwostr(LPCWSTR name, LPCWSTR val)
	{
		//struct String { 
		//  WORD   wLength; 
		//  WORD   wValueLength; 
		//  WORD   wType; 
		//  WCHAR  szKey[]; 
		//  WORD   Padding[]; 
		//  WORD   Value[]; 
		//};
		PWORD const porig = marksize();
		pushw(static_cast<WORD>(wcslen(val)) + 1);
		pushw(1); //type
		pushstr(name); // with align
		align4();
		pushstr(val); // don't align yet
		patchsize(porig);
		align4();
	}

private:
	yybuf(yybuf const &); // not copyable
	yybuf &operator=(yybuf const &); // not assignable
};

/// Make a version resource from file_ver_data_s
DWORD file_ver_data::makeVersionResource(PBYTE palloc, DWORD cballoc) const
{
	yybuf vbuf(palloc, cballoc);
	WCHAR temps[256];

	// Fill the res header
	// struct VS_VERSIONINFO { 
	//  WORD  wLength; 
	//  WORD  wValueLength; 
	//  WORD  wType; 
	//  WCHAR szKey[]; 
	//  WORD  Padding1[]; 
	//  VS_FIXEDFILEINFO Value; 
	//  WORD  Padding2[]; 
	//  WORD  Children[]; 
	// };
	PWORD const pTotalLen = vbuf.marksize();
	vbuf.pushw(sizeof(VS_FIXEDFILEINFO)); //wValueLength=0x34
	vbuf.pushw(0); //wType
	vbuf.pushstr(L"VS_VERSION_INFO"); // szKey, Padding1
	vbuf.align4();
	// Fixed info
	VS_FIXEDFILEINFO *const fxi = static_cast<VS_FIXEDFILEINFO *>(vbuf.incptr(sizeof(VS_FIXEDFILEINFO)));
	*fxi = m_fxi;
	vbuf.align4(); // Padding2

	// String File Info

	PWORD const stringStart = vbuf.marksize();
	vbuf.pushw(0); //wValueLength
	vbuf.pushw(1); //wType
	vbuf.pushstr(L"StringFileInfo");
	vbuf.align4();

	PWORD const stringTableStart = vbuf.marksize();
	vbuf.pushw(0); // ?
	vbuf.pushw(1); //wType
	wsprintfW(temps, L"%4.4X04B0", m_langid);  /* "040904B0" = LANG_ENGLISH/SUBLANG_ENGLISH_US, Unicode CP */
	vbuf.pushstr(temps);
	vbuf.align4();

	// Strings
	for (int k = 0; k < ARRAYSIZE(m_CustomStrNames); k++) {
		if (!m_CustomStrNames[k].IsEmpty()) {
			vbuf.pushTwostr(m_CustomStrNames[k], m_CustomStrVals[k]);
			if (0 == _wcsicmp(L"SpecialBuild", m_CustomStrNames[k]))
				fxi->dwFileFlags |= VS_FF_SPECIALBUILD;
			if (0 == _wcsicmp(L"PrivateBuild", m_CustomStrNames[k]))
				fxi->dwFileFlags |= VS_FF_PRIVATEBUILD;
		}
	}

	vbuf.patchsize(stringTableStart);
	vbuf.patchsize(stringStart);
	vbuf.align4();

	// Var info
	// struct VarFileInfo { 
	//  WORD  wLength; 
	//  WORD  wValueLength; 
	//  WORD  wType; 
	//  WCHAR szKey[]; 
	//  WORD  Padding[]; 
	//  Var   Children[]; 
	// };
	PWORD const varStart = vbuf.marksize();
	vbuf.pushw(0);
	vbuf.pushw(1);
	vbuf.pushstr(L"VarFileInfo");
	vbuf.align4();

	vbuf.pushw(0x24);
	vbuf.pushw(0x04);
	vbuf.pushw(0x00);
	vbuf.pushstr(L"Translation");
	vbuf.align4();
	vbuf.pushw(m_langid);
	vbuf.pushw(0x04B0);   // 0x04B0 = 1200 = Unicode CP

	vbuf.patchsize(varStart);
	/////////////////////////////
	vbuf.patchsize(pTotalLen);

	return vbuf.cbwritten();
}
