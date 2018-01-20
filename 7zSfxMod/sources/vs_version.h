//
// Code for VS_VERSION resource
// Modified from MIT licensed code published at https://ddverpatch.codeplex.com/
//
#pragma once

// Data for a vs_version resource
class file_ver_data
{
public:
	enum { MAX_VER_CUSTOM_STRINGS = 16 };
	VS_FIXEDFILEINFO m_fxi;
	WORD m_langid;			// language
	// Strings
	UString m_CustomStrNames[MAX_VER_CUSTOM_STRINGS];
	UString m_CustomStrVals[MAX_VER_CUSTOM_STRINGS];
	file_ver_data()
		: m_langid(0)
	{
		memset(&m_fxi, 0, sizeof m_fxi);
		m_fxi.dwSignature = VS_FFI_SIGNATURE;
		m_fxi.dwStrucVersion = VS_FFI_STRUCVERSION;
		m_fxi.dwFileFlagsMask = VS_FFI_FILEFLAGSMASK;
		m_fxi.dwFileOS = VOS_NT_WINDOWS32;
	}
	bool addTwostr(UString const &name, UString const &val)
	{
		int index = -1;
		for (int i = 0; i < ARRAYSIZE(m_CustomStrNames); ++i) {
			if (m_CustomStrNames[i].IsEmpty()) {
				if (index == -1)
					index = i;
			} else if (DoesWildcardMatchName(name, m_CustomStrNames[i])) {
				index = -2;
				m_CustomStrVals[i] = val;
			}
		}
		if (index >= 0) {
			m_CustomStrNames[index] = name;
			m_CustomStrVals[index] = val;
		}
		return index != -1;
	}
	PCWSTR getValStr(PCWSTR name)
	{
		for (int i = 0; i < ARRAYSIZE(m_CustomStrNames); i++) {
			PCWSTR s = m_CustomStrNames[i];
			if ( s && (0 == _wcsicmp(name, s) ) )
				return m_CustomStrVals[i];
		}
		return NULL;
	}
	DWORD makeVersionResource(PBYTE palloc, DWORD cballoc) const;
};
