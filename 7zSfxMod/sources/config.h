/*---------------------------------------------------------------------------*/
/* File:        config.h                                                     */
/* Created:     Sat, 23 Jan 2010 11:11:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Sat, 18 Nov 2017 by https://github.com/datadiode             */
/*---------------------------------------------------------------------------*/
/* Revision:    2261                                                         */
/* Updated:     Fri, 01 Apr 2016 20:59:44 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/*---------------------------------------------------------------------------*/
/* Revision:    154                                                          */
/* Updated:     Sat, 26 Jun 2010 04:15:20 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New prefix 'waitall',										 */
/*				make old prefixes as selectable for build:					 */
/*					'hidcon','nowait','forcenowait'							 */
/*---------------------------------------------------------------------------*/
/* Revision:    134                                                          */
/* Updated:     Sun, 06 Jun 2010 07:47:47 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Add warnings stuff											 */
/*				(_SFX_USE_CHECK_RAM && _SFX_USE_CHECK_FREE_SPACE)            */
/*---------------------------------------------------------------------------*/
/* Revision:    55                                                           */
/* Updated:     Thu, 18 Mar 2010 14:04:32 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#ifndef _CONFIG_H_INCLUDED_
#define _CONFIG_H_INCLUDED_

/* custom pre-defines */
#ifndef NO_REGISTRY
	#define NO_REGISTRY
#endif // NO_REGISTRY

#ifndef EXTRACT_ONLY
	#define EXTRACT_ONLY
#endif // EXTRACT_ONLY

#ifndef NO_READ_FROM_CODER
	#define NO_READ_FROM_CODER
#endif // NO_READ_FROM_CODER

#ifndef _SFX
	#define _SFX
#endif // _SFX

#if defined(_DEBUG) || defined(RELEASE_DEBUG)
	#define SFXBUILD_OPTIONS1	0xffff
	#define SFXBUILD_OPTIONS2	0xffff
	#define SFXBUILD_OPTIONS3	0xffff
	#define SFXBUILD_OPTIONS4	0xffff
	#define SFXBUILD_OPTIONS5	0xffff
#endif // defined(_DEBUG) || defined(RELEASE_DEBUG)

#define SFXBUILD_OPTIONS1_FORMAT_7Z				0x001

#define SFXBUILD_OPTIONS2_COMPRESS_COPY			0x001
#define SFXBUILD_OPTIONS2_COMPRESS_BCJ			0x002
#define SFXBUILD_OPTIONS2_COMPRESS_BCJ2			0x004
#define SFXBUILD_OPTIONS2_COMPRESS_DEFLATE		0x008
#define SFXBUILD_OPTIONS2_COMPRESS_LZMA			0x010
#define SFXBUILD_OPTIONS2_COMPRESS_LZMA2		0x020
#define SFXBUILD_OPTIONS2_COMPRESS_PPMD			0x040
#define SFXBUILD_OPTIONS2_CRYPTO				0x080
#define SFXBUILD_OPTIONS2_VOLUMES				0x100
#define SFXBUILD_OPTIONS2_PROTECT				0x200
#define SFXBUILD_OPTIONS2_COMPRESS_DELTA		0x400

#define SFXBUILD_OPTIONS3_PREFIX_PLATFORM		0x001
#define SFXBUILD_OPTIONS3_PRIFIX_WAITALL		0x002
#define SFXBUILD_OPTIONS3_PREFIX_HIDCON			0x004
#define SFXBUILD_OPTIONS3_PREFIX_NOWAIT			0x008
#define SFXBUILD_OPTIONS3_PREFIX_FORCENOWAIT	0x010

#define SFXBUILD_OPTIONS4_TEST					0x0001
#define SFXBUILD_OPTIONS4_LANG					0x0002
#define SFXBUILD_OPTIONS4_ELEVATION				0x0004
#define SFXBUILD_OPTIONS4_CHECK_RAM				0x0008
#define SFXBUILD_OPTIONS4_CHECK_FREE_SPACE		0x0010
#define SFXBUILD_OPTIONS4_BEGINPROMPTTIMEOUT	0x0020
#define SFXBUILD_OPTIONS4_CONFIG_PLATFORM		0x0040
#define SFXBUILD_OPTIONS4_EARLY_PASSWORD		0x0080
#define SFXBUILD_OPTIONS4_VOLUME_NAME_STYLE		0x0100
#define SFXBUILD_OPTIONS4_ENVIRONMENT_VARS		0x0200
#define SFXBUILD_OPTIONS4_CONFIG_EARLY_EXECUTE	0x0400
#define SFXBUILD_OPTIONS4_EXTRACT_MASK			0x1000

#define SFXBUILD_OPTIONS5_WIN7_PROGRESSBAR		0x001
#define SFXBUILD_OPTIONS5_RTF_CONTROL			0x002
#define SFXBUILD_OPTIONS5_IMAGES				0x004

/* SFXBUILD_OPTIONS3 (SFX prefixes), defaults: all prefixes */
#ifndef SFXBUILD_OPTIONS3
	#define _SFX_USE_PREFIX_PLATFORM		/* use x86 & x64 prefixes for execution parameters */
	#define _SFX_USE_PREFIX_WAITALL			/* external programs: 'wait all child processes' trick */
	#define _SFX_USE_PREFIX_HIDCON			/* external programs: 'hide console' trick */
	#define _SFX_USE_PREFIX_NOWAIT			/* external programs: 'don't wait end of the execution' trick */
	#define _SFX_USE_PREFIX_FORCENOWAIT		/* external programs: 'don't wait end of the execution' trick */
											/* even unpacking to temporary folder */
#endif // SFXBUILD_OPTIONS3

/* SFXBUILD_OPTIONS4 (SFX switches, config parameters), defaults: all available */
#ifndef SFXBUILD_OPTIONS4
	#define _SFX_USE_LANG					/* use -sfxlang:XXXX command line switch, localized resources and configs */
	#define _SFX_USE_ELEVATION				/* Elevate user rights on start */
	#define _SFX_USE_CHECK_RAM				/* check available physical memory to prevent swap */
	#define _SFX_USE_CHECK_FREE_SPACE		/* check available free space on destination drive */
	#define _SFX_USE_BEGINPROMPTTIMEOUT		/* BeginPrompt default button timeout */
	#define _SFX_USE_CONFIG_PLATFORM		/* Separate configs for different platforms */
	#define _SFX_USE_EARLY_PASSWORD			/* Prompt for password before any dialogs */
	#define _SFX_USE_ENVIRONMENT_VARS		/* Environment variables 7zSfxVarXXX */
	#define _SFX_USE_CONFIG_EARLY_EXECUTE	/* 'ExecuteOnLoad' & 'PreExtract' config param */
	#define _SFX_USE_EXTRACT_MASK			/* 'ExtractMask' config parameter */
#endif // SFXBUILD_OPTIONS4

/* SFXBUILD_OPTIONS5 (GUI options), defaults: all available */
#ifndef SFXBUILD_OPTIONS5
	#define _SFX_USE_WIN7_PROGRESSBAR		/* Windows 7 progress bar on taskbar button */
											/* require latest SDK */
	#define _SFX_USE_RTF_CONTROL			/* Enable RTF in texts */
	#define _SFX_USE_IMAGES					/* Enable images in custom dialogs */
#endif // SFXBUILD_OPTIONS5

// 7-Zip options
#ifndef SFXBUILD_OPTIONS1
	#define SFXBUILD_OPTIONS1		0
#endif
#ifndef SFXBUILD_OPTIONS2
	#define SFXBUILD_OPTIONS2		0
#endif
#ifndef SFXBUILD_OPTIONS3
	#define SFXBUILD_OPTIONS3		0xffff
#endif
#ifndef SFXBUILD_OPTIONS4
	#define SFXBUILD_OPTIONS4		0xffff
#endif
#ifndef SFXBUILD_OPTIONS5
	#define SFXBUILD_OPTIONS5		0xffff
#endif

#if (SFXBUILD_OPTIONS1&SFXBUILD_OPTIONS1_FORMAT_7Z) != 0 && !defined(SFX_USE_FORMAT_7Z)
	#define SFX_USE_FORMAT_7Z
#endif

#ifndef SFX_USE_FORMAT_7Z
	#undef SFXBUILD_OPTIONS1_FORMAT_7Z
	#define SFXBUILD_OPTIONS1_FORMAT_7Z	0
#endif // SFX_USE_FORMAT_7Z

#undef SFXBUILD_OPTIONS1
#define SFXBUILD_OPTIONS1			\
	SFXBUILD_OPTIONS1_FORMAT_7Z	|	\
	0
#if SFXBUILD_OPTIONS1 == 0
	#define SFX_USE_FORMAT_7Z
	#undef SFXBUILD_OPTIONS1
	#define SFXBUILD_OPTIONS1	0x001
#endif // SFXBUILD_OPTIONS1

#if (SFXBUILD_OPTIONS2&SFXBUILD_OPTIONS2_COMPRESS_COPY) != 0 && !defined(COMPRESS_COPY)
	#define COMPRESS_COPY
#endif
#if (SFXBUILD_OPTIONS2&SFXBUILD_OPTIONS2_COMPRESS_BCJ) != 0 && !defined(COMPRESS_BCJ)
	#define COMPRESS_BCJ
#endif
#if (SFXBUILD_OPTIONS2&SFXBUILD_OPTIONS2_COMPRESS_BCJ2) != 0 && !defined(COMPRESS_BCJ2)
	#define COMPRESS_BCJ2
#endif
#if (SFXBUILD_OPTIONS2&SFXBUILD_OPTIONS2_COMPRESS_DEFLATE) != 0 && !defined(COMPRESS_DEFLATE)
	#define COMPRESS_DEFLATE
#endif
#if (SFXBUILD_OPTIONS2&SFXBUILD_OPTIONS2_COMPRESS_LZMA) != 0 && !defined(COMPRESS_LZMA)
	#define COMPRESS_LZMA
#endif
#if (SFXBUILD_OPTIONS2&SFXBUILD_OPTIONS2_COMPRESS_LZMA2) != 0 && !defined(COMPRESS_LZMA2)
	#define COMPRESS_LZMA2
#endif
#if (SFXBUILD_OPTIONS2&SFXBUILD_OPTIONS2_COMPRESS_PPMD) != 0 && !defined(COMPRESS_PPMD)
	#define COMPRESS_PPMD
#endif
#if (SFXBUILD_OPTIONS2&SFXBUILD_OPTIONS2_CRYPTO) == 0 && !defined(SFX_CRYPTO)
	#define _NO_CRYPTO
#else
	#ifndef SFX_CRYPTO
		#define SFX_CRYPTO
	#endif // SFX_CRYPTO
	#ifdef _NO_CRYPTO
		#undef _NO_CRYPTO
	#endif // _NO_CRYPTO
#endif
#if (SFXBUILD_OPTIONS2&SFXBUILD_OPTIONS2_VOLUMES) != 0 && !defined(SFX_VOLUMES)
	#define SFX_VOLUMES
#endif
#if (SFXBUILD_OPTIONS2&SFXBUILD_OPTIONS2_PROTECT) != 0 && !defined(SFX_PROTECT)
	#define SFX_PROTECT
#endif
#if (SFXBUILD_OPTIONS2&SFXBUILD_OPTIONS2_COMPRESS_DELTA) != 0 && !defined(COMPRESS_DELTA)
	#define COMPRESS_DELTA
#endif

#ifndef COMPRESS_COPY
	#undef SFXBUILD_OPTIONS2_COMPRESS_COPY
	#define SFXBUILD_OPTIONS2_COMPRESS_COPY		0
#endif
#ifndef COMPRESS_BCJ
	#undef SFXBUILD_OPTIONS2_COMPRESS_BCJ
	#define SFXBUILD_OPTIONS2_COMPRESS_BCJ		0
#endif
#ifndef COMPRESS_BCJ2
	#undef SFXBUILD_OPTIONS2_COMPRESS_BCJ2
	#define SFXBUILD_OPTIONS2_COMPRESS_BCJ2		0
#endif
#ifndef COMPRESS_DEFLATE
	#undef SFXBUILD_OPTIONS2_COMPRESS_DEFLATE
	#define SFXBUILD_OPTIONS2_COMPRESS_DEFLATE	0
#endif
#ifndef COMPRESS_LZMA
	#undef SFXBUILD_OPTIONS2_COMPRESS_LZMA
	#define SFXBUILD_OPTIONS2_COMPRESS_LZMA		0
#endif
#ifndef COMPRESS_LZMA2
	#undef SFXBUILD_OPTIONS2_COMPRESS_LZMA2
	#define SFXBUILD_OPTIONS2_COMPRESS_LZMA2	0
#endif
#ifndef COMPRESS_PPMD
	#undef SFXBUILD_OPTIONS2_COMPRESS_PPMD
	#define SFXBUILD_OPTIONS2_COMPRESS_PPMD		0
#endif
#ifdef _NO_CRYPTO
	#undef SFXBUILD_OPTIONS2_CRYPTO
	#define SFXBUILD_OPTIONS2_CRYPTO			0
#endif
#ifndef SFX_VOLUMES
	#undef SFXBUILD_OPTIONS2_VOLUMES
	#define SFXBUILD_OPTIONS2_VOLUMES			0
#endif
#ifndef SFX_PROTECT
	#undef SFXBUILD_OPTIONS2_PROTECT
	#define SFXBUILD_OPTIONS2_PROTECT			0
#endif
#ifndef COMPRESS_DELTA
	#undef SFXBUILD_OPTIONS2_COMPRESS_DELTA
	#define SFXBUILD_OPTIONS2_COMPRESS_DELTA	0
#endif

#undef SFXBUILD_OPTIONS2
#define SFXBUILD_OPTIONS2					\
	SFXBUILD_OPTIONS2_COMPRESS_COPY		|	\
	SFXBUILD_OPTIONS2_COMPRESS_BCJ		|	\
	SFXBUILD_OPTIONS2_COMPRESS_BCJ2		|	\
	SFXBUILD_OPTIONS2_COMPRESS_DEFLATE	|	\
	SFXBUILD_OPTIONS2_COMPRESS_LZMA		|	\
	SFXBUILD_OPTIONS2_COMPRESS_LZMA2	|	\
	SFXBUILD_OPTIONS2_COMPRESS_PPMD		|	\
	SFXBUILD_OPTIONS2_CRYPTO			|	\
	SFXBUILD_OPTIONS2_VOLUMES			|	\
	SFXBUILD_OPTIONS2_PROTECT			|	\
	SFXBUILD_OPTIONS2_COMPRESS_DELTA	|	\
	0



#if (SFXBUILD_OPTIONS3&SFXBUILD_OPTIONS3_PREFIX_PLATFORM) != 0 && !defined(_SFX_USE_PREFIX_PLATFORM)
	#define _SFX_USE_PREFIX_PLATFORM
#endif

#if (SFXBUILD_OPTIONS3&SFXBUILD_OPTIONS3_PREFIX_WAITALL) != 0 && !defined(_SFX_USE_PREFIX_WAITALL)
	#define _SFX_USE_PREFIX_WAITALL
#endif

#if (SFXBUILD_OPTIONS3&SFXBUILD_OPTIONS3_PREFIX_HIDCON) != 0 && !defined(_SFX_USE_PREFIX_HIDCON)
	#define _SFX_USE_PREFIX_HIDCON
#endif

#if (SFXBUILD_OPTIONS3&SFXBUILD_OPTIONS3_PREFIX_NOWAIT) != 0 && !defined(_SFX_USE_PREFIX_NOWAIT)
	#define _SFX_USE_PREFIX_NOWAIT
#endif

#if (SFXBUILD_OPTIONS3&SFXBUILD_OPTIONS3_PREFIX_FORCENOWAIT) != 0 && !defined(_SFX_USE_PREFIX_FORCENOWAIT)
	#define _SFX_USE_PREFIX_FORCENOWAIT
#endif

#ifndef _SFX_USE_PREFIX_PLATFORM
	#undef SFXBUILD_OPTIONS3_PREFIX_PLATFORM
	#define SFXBUILD_OPTIONS3_PREFIX_PLATFORM		0
#endif

#ifndef _SFX_USE_PREFIX_WAITALL
	#undef SFXBUILD_OPTIONS3_PREFIX_WAITALL
	#define SFXBUILD_OPTIONS3_PREFIX_WAITALL		0
#endif

#ifndef _SFX_USE_PREFIX_HIDCON
	#undef SFXBUILD_OPTIONS3_PREFIX_HIDCON
	#define SFXBUILD_OPTIONS3_PREFIX_HIDCON			0
#endif

#ifndef _SFX_USE_PREFIX_NOWAIT
	#undef SFXBUILD_OPTIONS3_PREFIX_NOWAIT
	#define SFXBUILD_OPTIONS3_PREFIX_NOWAIT			0
#endif

#ifndef _SFX_USE_PREFIX_FORCENOWAIT
	#undef SFXBUILD_OPTIONS3_PREFIX_FORCENOWAIT
	#define SFXBUILD_OPTIONS3_PREFIX_FORCENOWAIT	0
#endif

#undef SFXBUILD_OPTIONS3
#define SFXBUILD_OPTIONS3						\
	SFXBUILD_OPTIONS3_PREFIX_PLATFORM		|	\
	SFXBUILD_OPTIONS3_PRIFIX_WAITALL		|	\
	SFXBUILD_OPTIONS3_PREFIX_HIDCON			|	\
	SFXBUILD_OPTIONS3_PREFIX_NOWAIT			|	\
	SFXBUILD_OPTIONS3_PREFIX_FORCENOWAIT	|	\
	0

#if (SFXBUILD_OPTIONS4&SFXBUILD_OPTIONS4_TEST) != 0 && !defined(_SFX_USE_TEST)
	#define _SFX_USE_TEST
#endif
#if (SFXBUILD_OPTIONS4&SFXBUILD_OPTIONS4_LANG) != 0 && !defined(_SFX_USE_LANG)
	#define _SFX_USE_LANG
#endif
#if (SFXBUILD_OPTIONS4&SFXBUILD_OPTIONS4_ELEVATION) != 0 && !defined(_SFX_USE_ELEVATION)
	#define _SFX_USE_ELEVATION
#endif
#if (SFXBUILD_OPTIONS4&SFXBUILD_OPTIONS4_CHECK_RAM) != 0 && !defined(_SFX_USE_CHECK_RAM)
	#define _SFX_USE_CHECK_RAM
#endif
#if (SFXBUILD_OPTIONS4&SFXBUILD_OPTIONS4_CHECK_FREE_SPACE) != 0 && !defined(_SFX_USE_CHECK_FREE_SPACE)
	#define _SFX_USE_CHECK_FREE_SPACE
#endif
#if (SFXBUILD_OPTIONS4&SFXBUILD_OPTIONS4_BEGINPROMPTTIMEOUT) != 0 && !defined(_SFX_USE_BEGINPROMPTTIMEOUT)
	#define _SFX_USE_BEGINPROMPTTIMEOUT
#endif
#if (SFXBUILD_OPTIONS4&SFXBUILD_OPTIONS4_CONFIG_PLATFORM) != 0 && !defined(_SFX_USE_CONFIG_PLATFORM)
	#define _SFX_USE_CONFIG_PLATFORM
#endif
#if defined(SFX_CRYPTO) && (SFXBUILD_OPTIONS4&SFXBUILD_OPTIONS4_EARLY_PASSWORD) != 0
	#ifndef _SFX_USE_EARLY_PASSWORD
		#define _SFX_USE_EARLY_PASSWORD
	#endif // _SFX_USE_EARLY_PASSWORD
#else
	#ifdef _SFX_USE_EARLY_PASSWORD
		#undef _SFX_USE_EARLY_PASSWORD
	#endif // _SFX_USE_EARLY_PASSWORD
#endif
#if defined(SFX_VOLUMES) && (SFXBUILD_OPTIONS4&SFXBUILD_OPTIONS4_VOLUME_NAME_STYLE) != 0
	#ifndef _SFX_USE_VOLUME_NAME_STYLE
		#define _SFX_USE_VOLUME_NAME_STYLE
	#endif // _SFX_USE_VOLUME_NAME_STYLE
#else
	#ifdef _SFX_USE_VOLUME_NAME_STYLE
		#undef _SFX_USE_VOLUME_NAME_STYLE
	#endif // _SFX_USE_VOLUME_NAME_STYLE
#endif
#if (SFXBUILD_OPTIONS4&SFXBUILD_OPTIONS4_ENVIRONMENT_VARS) != 0 && !defined(_SFX_USE_ENVIRONMENT_VARS)
	#define _SFX_USE_ENVIRONMENT_VARS
#endif
#if (SFXBUILD_OPTIONS4&SFXBUILD_OPTIONS4_CONFIG_EARLY_EXECUTE) != 0 && !defined(_SFX_USE_CONFIG_EARLY_EXECUTE)
	#define _SFX_USE_CONFIG_EARLY_EXECUTE
#endif
#if (SFXBUILD_OPTIONS4&SFXBUILD_OPTIONS4_EXTRACT_MASK) != 0 && !defined(_SFX_USE_EXTRACT_MASK)
	#define _SFX_USE_EXTRACT_MASK
#endif

#ifndef _SFX_USE_TEST
	#undef SFXBUILD_OPTIONS4_TEST
	#define SFXBUILD_OPTIONS4_TEST					0
#endif
#ifndef _SFX_USE_LANG
	#undef SFXBUILD_OPTIONS4_LANG
	#define SFXBUILD_OPTIONS4_LANG					0
#endif
#ifndef _SFX_USE_ELEVATION
	#undef SFXBUILD_OPTIONS4_ELEVATION
	#define SFXBUILD_OPTIONS4_ELEVATION				0
#endif
#ifndef _SFX_USE_CHECK_RAM
	#undef SFXBUILD_OPTIONS4_CHECK_RAM
	#define SFXBUILD_OPTIONS4_CHECK_RAM				0
#endif
#ifndef _SFX_USE_CHECK_FREE_SPACE
	#undef SFXBUILD_OPTIONS4_CHECK_FREE_SPACE
	#define SFXBUILD_OPTIONS4_CHECK_FREE_SPACE		0
#endif
#ifndef _SFX_USE_BEGINPROMPTTIMEOUT
	#undef SFXBUILD_OPTIONS4_BEGINPROMPTTIMEOUT
	#define SFXBUILD_OPTIONS4_BEGINPROMPTTIMEOUT	0
#endif
#ifndef _SFX_USE_CONFIG_PLATFORM
	#undef SFXBUILD_OPTIONS4_CONFIG_PLATFORM
	#define SFXBUILD_OPTIONS4_CONFIG_PLATFORM		0
#endif
#ifndef _SFX_USE_EARLY_PASSWORD
	#undef SFXBUILD_OPTIONS4_EARLY_PASSWORD
	#define SFXBUILD_OPTIONS4_EARLY_PASSWORD		0
#endif
#ifndef _SFX_USE_VOLUME_NAME_STYLE
	#undef SFXBUILD_OPTIONS4_VOLUME_NAME_STYLE
	#define SFXBUILD_OPTIONS4_VOLUME_NAME_STYLE		0
#endif // _SFX_USE_VOLUME_EXTENSION
#ifndef _SFX_USE_ENVIRONMENT_VARS
	#undef SFXBUILD_OPTIONS4_ENVIRONMENT_VARS
	#define SFXBUILD_OPTIONS4_ENVIRONMENT_VARS		0
#endif // _SFX_USE_VOLUME_EXTENSION
#ifndef _SFX_USE_CONFIG_EARLY_EXECUTE
	#undef SFXBUILD_OPTIONS4_CONFIG_EARLY_EXECUTE
	#define SFXBUILD_OPTIONS4_CONFIG_EARLY_EXECUTE	0
#endif // _SFX_USE_CONFIG_EARLY_EXECUTE
#ifndef _SFX_USE_EXTRACT_MASK
	#undef SFXBUILD_OPTIONS4_EXTRACT_MASK
	#define SFXBUILD_OPTIONS4_EXTRACT_MASK			0
#endif // _SFX_USE_EXTRACT_MASK

#undef SFXBUILD_OPTIONS4
#define SFXBUILD_OPTIONS4						\
	SFXBUILD_OPTIONS4_TEST					|	\
	SFXBUILD_OPTIONS4_LANG					|	\
	SFXBUILD_OPTIONS4_ELEVATION				|	\
	SFXBUILD_OPTIONS4_CHECK_RAM				|	\
	SFXBUILD_OPTIONS4_CHECK_FREE_SPACE		|	\
	SFXBUILD_OPTIONS4_BEGINPROMPTTIMEOUT	|	\
	SFXBUILD_OPTIONS4_CONFIG_PLATFORM		|	\
	SFXBUILD_OPTIONS4_EARLY_PASSWORD		|	\
	SFXBUILD_OPTIONS4_VOLUME_NAME_STYLE		|	\
	SFXBUILD_OPTIONS4_ENVIRONMENT_VARS		|	\
	SFXBUILD_OPTIONS4_CONFIG_EARLY_EXECUTE	|	\
	SFXBUILD_OPTIONS4_EXTRACT_MASK			|	\
	0


#if (SFXBUILD_OPTIONS5&SFXBUILD_OPTIONS5_WIN7_PROGRESSBAR) != 0 && !defined(_SFX_USE_WIN7_PROGRESSBAR)
	#define _SFX_USE_WIN7_PROGRESSBAR
#endif
#if (SFXBUILD_OPTIONS5&SFXBUILD_OPTIONS5_RTF_CONTROL) != 0 && !defined(_SFX_USE_RTF_CONTROL)
#define _SFX_USE_RTF_CONTROL
#endif
#if (SFXBUILD_OPTIONS5&SFXBUILD_OPTIONS5_IMAGES) != 0 && !defined(_SFX_USE_IMAGES)
#define _SFX_USE_IMAGES
#endif

#ifndef _SFX_USE_WIN7_PROGRESSBAR
	#undef SFXBUILD_OPTIONS5_WIN7_PROGRESSBAR
	#define SFXBUILD_OPTIONS5_WIN7_PROGRESSBAR		0
#endif
#ifndef _SFX_USE_RTF_CONTROL
	#undef SFXBUILD_OPTIONS5_RTF_CONTROL
	#define SFXBUILD_OPTIONS5_RTF_CONTROL			0
#endif
#ifndef _SFX_USE_IMAGES
	#undef SFXBUILD_OPTIONS5_IMAGES
	#define SFXBUILD_OPTIONS5_IMAGES				0
#endif

#undef SFXBUILD_OPTIONS5
#define SFXBUILD_OPTIONS5						\
	SFXBUILD_OPTIONS5_WIN7_PROGRESSBAR		|	\
	SFXBUILD_OPTIONS5_RTF_CONTROL			|	\
	SFXBUILD_OPTIONS5_IMAGES				|	\
	0

#ifndef _SFX_USE_WARNINGS
	#if defined(_SFX_USE_CHECK_FREE_SPACE) || defined(_SFX_USE_CHECK_RAM)
		#define _SFX_USE_WARNINGS
	#endif // defined(_SFX_USE_CHECK_FREE_SPACE) || defined(_SFX_USE_CHECK_RAM)
#endif // _SFX_USE_WARNINGS

/* backward capability within 7zSfx... and Sfx... environment */
#define _SFX_USE_ENVIRONMENT_OLD_VARS	1

#endif // _CONFIG_H_INCLUDED_