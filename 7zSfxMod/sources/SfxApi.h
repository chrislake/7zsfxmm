/*---------------------------------------------------------------------------*/
/* File:        SfxApi.h                                                     */
/* Created:     Sat, 06 Feb 2016 23:17:51 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Fri, 01 Apr 2016 04:52:23 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    55                                                           */
/*---------------------------------------------------------------------------*/
#ifndef _SFXAPI_H_UNCLUDED_
#define _SFXAPI_H_UNCLUDED_

#ifdef _SFX_USE_SFXAPI

#define SFXAPI_HELPERS_NAME	".sfx.api"

class CSfxAPI
{
public:
	CSfxAPI() {};
	virtual ~CSfxAPI() { Free(); };
	bool Init();
	bool Free();
	LPCWSTR GetDirectory() { Init(); return m_strTmpDir; }

private:
	bool Extract();

	CSfxStringU	m_strTmpDir;
	CSfxStringU m_strDefaultDirectory;
};

#endif // _SFX_USE_SFXAPI

#endif // _SFXAPI_H_UNCLUDED_
