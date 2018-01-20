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
	DWORD const m_inisize;

public:
	yybuf(PBYTE start, DWORD size)
		: m_startptr(start), m_inisize(size), m_curptr(start)
	{
		assert((reinterpret_cast<INT_PTR>(m_curptr) & 3) == 0); // must be aligned on 4
	}

	void align4() {
		while (reinterpret_cast<INT_PTR>(m_curptr) & 3)
			*m_curptr++ = 0;
	}

	DWORD cbwritten() { return static_cast<DWORD>(m_curptr - m_startptr); }

	void checkspace(DWORD n = 8) {
		if (cbwritten() + n > m_inisize) {
			throw ":checkspace";
		}
	}

	void pushw(WORD v) {
		*reinterpret_cast<PWORD>(m_curptr) = v;
		m_curptr += sizeof(WORD);
	}

	void pushstr(LPCWSTR ws) {
		WORD n = static_cast<WORD>(wcslen(ws));
		n = (n + 1) * sizeof(WCHAR);
		checkspace(n + sizeof(DWORD));
		memcpy(m_curptr, ws, n);
		m_curptr += n;
	}

	PBYTE getptr() { return m_curptr;}

	void incptr(DWORD n) { checkspace(n); m_curptr += n; }

	PWORD marksize() { return reinterpret_cast<PWORD>(m_curptr); }

	void patchsize(PWORD mp) {
		*mp = static_cast<WORD>(getptr() - reinterpret_cast<PBYTE>(mp));
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
		WORD wValueLength = static_cast<WORD>(wcslen(val));
		WORD wValueSize = static_cast<WORD>(wValueLength ? (wValueLength + 1) * sizeof(WCHAR) : 0);
		WORD wNameSize = static_cast<WORD>((wcslen(name) + 1) * sizeof(WCHAR));
		assert(wNameSize > sizeof(WCHAR));

		checkspace(wValueSize + wNameSize + 5 * sizeof(WORD));

		PWORD porig = marksize();
		pushw(0xFFFF); //length, patch
		pushw(wValueLength + 1);
		pushw(1); //type
		pushstr(name); // with align
		align4();
		if (wValueLength)
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

	try {
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
		PWORD pTotalLen = vbuf.marksize();
		vbuf.pushw(0xFFFF); // wLength <-FIXUP LATER
		vbuf.pushw(sizeof(VS_FIXEDFILEINFO)); //wValueLength=0x34
		vbuf.pushw(0); //wType
		vbuf.pushstr(L"VS_VERSION_INFO"); // szKey, Padding1
		vbuf.align4();
		// Fixed info
		VS_FIXEDFILEINFO *const fxi = reinterpret_cast<VS_FIXEDFILEINFO *>(vbuf.getptr());
		*fxi = m_fxi;
		vbuf.incptr(sizeof(VS_FIXEDFILEINFO));
		vbuf.align4(); // Padding2

		// String File Info

		PWORD stringStart = vbuf.marksize();
		vbuf.pushw(0xFFFF); //wLength FIXUP LATER
		vbuf.pushw(0); //wValueLength
		vbuf.pushw(1); //wType
		vbuf.pushstr(L"StringFileInfo");
		vbuf.align4();

		PWORD stringTableStart = vbuf.marksize();
		vbuf.pushw(0xFFFF); //wLength FIXUP LATER
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
		PWORD varStart = vbuf.marksize();
		vbuf.pushw(0xFFFF); // size, patch
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
		vbuf.checkspace(); 

		cballoc = vbuf.cbwritten();
	} catch (char *) {
		cballoc = 0;
	}
	return cballoc;
}
