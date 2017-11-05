/*---------------------------------------------------------------------------*/
/* File:        7zSfxMod.h                                                   */
/* Created:     Thu, 28 Jul 2005 02:44:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Sat, 02 Apr 2016 05:00:21 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    3902                                                         */
/*---------------------------------------------------------------------------*/
/* Revision:    2518                                                         */
/* Updated:     Mon, 18 Jun 2012 06:00:51 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Rename names of environment variables						 */
/*				from '7zSfxVarXXX' to 'SfxVarXXX'							 */
/*---------------------------------------------------------------------------*/
/* Revision:    1775                                                         */
/* Updated:     Sun, 06 Jun 2010 07:46:34 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Add warnings stuff                                           */
/*---------------------------------------------------------------------------*/
/* Revision:    1699                                                         */
/* Updated:     Mon, 22 Mar 2010 11:00:15 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
	#ifndef _7ZSFXMOD_H_INCLUDED_
#define _7ZSFXMOD_H_INCLUDED_

#ifndef _CFG_PARAM_TYPE
	#define _CFG_PARAM_TYPE	L
#endif // _CFG_PARAM_TYPE

#define CFG_TITLE						_CFG_PARAM_TYPE"Title"
#define CFG_ERRORTITLE					_CFG_PARAM_TYPE"ErrorTitle"
#ifdef _SFX_USE_WARNINGS
	#define CFG_WARNINGTITLE			_CFG_PARAM_TYPE"WarningTitle"
#endif // _SFX_USE_WARNINGS
#define CFG_GUIMODE						_CFG_PARAM_TYPE"GUIMode"
#define CFG_GUIFLAGS					_CFG_PARAM_TYPE"GUIFlags"
#define CFG_MISCFLAGS					_CFG_PARAM_TYPE"MiscFlags"
#define CFG_BEGINPROMPT					_CFG_PARAM_TYPE"BeginPrompt"
#define CFG_INSTALLPATH					_CFG_PARAM_TYPE"InstallPath"
#define CFG_EXTRACT_TITLE				_CFG_PARAM_TYPE"ExtractTitle"
#define CFG_EXTRACT_CANCELTEXT			_CFG_PARAM_TYPE"ExtractCancelText"
#define CFG_EXTRACT_DIALOGTEXT			_CFG_PARAM_TYPE"ExtractDialogText"
#define CFG_EXTRACT_DIALOGWIDTH			_CFG_PARAM_TYPE"ExtractDialogWidth"
#define CFG_RUNPROGRAM					_CFG_PARAM_TYPE"RunProgram"
#define CFG_AUTOINSTALL					_CFG_PARAM_TYPE"AutoInstall"
#define CFG_SHORTCUT					_CFG_PARAM_TYPE"Shortcut"
#define CFG_DELETE						_CFG_PARAM_TYPE"Delete"
#define CFG_SELFDELETE					_CFG_PARAM_TYPE"SelfDelete"
#define CFG_EXTRACT_PATH_TITLE			_CFG_PARAM_TYPE"ExtractPathTitle"
#define CFG_EXTRACT_PATH_TEXT			_CFG_PARAM_TYPE"ExtractPathText"
#define CFG_HELP_TEXT					_CFG_PARAM_TYPE"HelpText"
#define CFG_OVERWRITE_MODE				_CFG_PARAM_TYPE"OverwriteMode"
#define CFG_CANCEL_PROMPT				_CFG_PARAM_TYPE"CancelPrompt"
#define CFG_EXTRACTPATH_WIDTH			_CFG_PARAM_TYPE"ExtractPathWidth"
#define CFG_FINISHMESSAGE				_CFG_PARAM_TYPE"FinishMessage"
#define CFG_EXECUTEFILE					_CFG_PARAM_TYPE"ExecuteFile"
#define CFG_EXECUTEPARAMETERS			_CFG_PARAM_TYPE"ExecuteParameters"
#define CFG_DIRECTORY					_CFG_PARAM_TYPE"Directory"
#define CFG_PROGRESS					_CFG_PARAM_TYPE"Progress"
#define CFG_SETENVIRONMENT				_CFG_PARAM_TYPE"SetEnvironment"
#define CFG_BEGINPROMPTTIMEOUT			_CFG_PARAM_TYPE"BeginPromptTimeout"
#ifdef SFX_CRYPTO
	#define CFG_PASSWORD_TITLE			_CFG_PARAM_TYPE"PasswordTitle"
	#define CFG_PASSWORD_TEXT			_CFG_PARAM_TYPE"PasswordText"
#endif // SFX_CRYPTO
#ifdef _SFX_USE_VOLUME_NAME_STYLE
	#define CFG_VOLUME_NAME_STYLE		_CFG_PARAM_TYPE"VolumeNameStyle"
#endif // _SFX_USE_VOLUME_NAME_STYLE
#ifdef _SFX_USE_CONFIG_EARLY_EXECUTE
	#define	CFG_EXECUTE_ON_LOAD			_CFG_PARAM_TYPE"ExecuteOnLoad"
	#define	CFG_PREEXTRACT				_CFG_PARAM_TYPE"PreExtract"
#endif // _SFX_USE_CONFIG_EARLY_EXECUTE
#ifdef _SFX_USE_EXTRACT_MASK
	#define	CFG_EXTRACTMASK_INCLUDE		_CFG_PARAM_TYPE"ExtractMaskInclude"
	#define	CFG_EXTRACTMASK_EXCLUDE		_CFG_PARAM_TYPE"ExtractMaskExclude"
#endif // _SFX_USE_EXTRACT_MASK


#define GUIMODE_STANDARD				0
#define GUIMODE_NOCANCEL				1
#define GUIMODE_HIDDEN					2
#define GUIMODE_MAX						2

#define OVERWRITE_MODE_ALL				0
#define OVERWRITE_MODE_NONE				1
#define OVERWRITE_MODE_OLDER			2
//#define OVERWRITE_MODE_CONFIRM			3
//#define OVERWRITE_MODE_CONFIRM_NEWER	4
#define OVERWRITE_MODE_MAX				2 /* current up to OVERWRITE_MODE_OLDER */
#define OVERWRITE_MODE_MASK				0x07
#define OVERWRITE_FLAG_SKIP_LOCKED		0x08


#define GUIFLAGS_TITLE_PERCENT_RIGHT	0x0001
#define GUIFLAGS_TITLE_PERCENT_NONE		0x0002
#define GUIFLAGS_PERCENT_TEXT			0x0004
//#define GUIFLAGS_XPSTYLE				0x0008
#define GUIFLAGS_PERCENT_BOLD			0x0010
#define GUIFLAGS_USEICON				0x0020
#define GUIFLAGS_EXTRACT_PATH1			0x0040
#define GUIFLAGS_EXTRACT_PATH2			0x0080
#define GUIFLAGS_CONFIRM_CANCEL			0x0100
#define GUIFLAGS_NO_TITLE_ICON			0x0200
#define GUIFLAGS_EPD_USEICON			0x0400
#define GUIFLAGS_BP_MODULEICON			0x0800
#define	GUIFLAGS_BP_OKCANCEL			0x1000
#define	GUIFLAGS_NO_WIN7_PROGRESSBAR	0x2000
#define GUIFLAGS_ALLOW_AMPERSAND		0x4000
#define GUIFLAGS_MASK					0x7FFF

#define MISCFLAGS_NO_CHECK_DISK_FREE	0x0001
#define MISCFLAGS_NO_CHECK_RAM			0x0002
#define MISCFLAGS_ELEVATE				0x0004
#ifdef _SFX_USE_EARLY_PASSWORD
	#define MISCFLAGS_LATE_PASSWORD		0x0008
#endif // _SFX_USE_EARLY_PASSWORD
#define MISCFLAGS_NO_CANCELLED			0x0010
#ifdef _SFX_USE_EXTRACT_MASK
	#define MISCFLAGS_EXTRACTMASK_INVERT	0x0020
#endif // _SFX_USE_EXTRACT_MASK
#ifdef _SFX_USE_COMPRESSED_CONFIG
	#define MISCFLAGS_NO_COMPRESSED_CONFIG	0x0040
#endif // _SFX_USE_COMPRESSED_CONFIG
#ifdef _SFX_USE_SFXAPI
	#define MISCFLAGS_NO_SFXAPI			0x0080
#endif // _SFX_USE_SFXAPI

#define ERRC_NONE						0
#define ERRC_GET_PATHNAME				1
#define ERRC_OPEN_ARCHIVE				2
#define ERRC_READ_CONFIG				3
#define ERRC_CONFIG_DATA				4
#define ERRC_CANCELED					5
#define ERRC_BAD_AUTOINSTALL			6
#define ERRC_NOTHING_TO_EXECUTE			7
#define ERRC_EXTRACT					8
#define ERRC_EXECUTE					9
#define ERRC_EXCEPTION					10
#define ERRC_ELEVATE					11
#define ERRC_PLATFORM					20
#define ERRC_SFXTEST					100

#define CMDLINE_SFXCONFIG			_CFG_PARAM_TYPE"sfxconfig"
#define CMDLINE_SFXCREATE			_CFG_PARAM_TYPE"sfxcreate"
#define CMDLINE_SFXMANIFEST			_CFG_PARAM_TYPE"sfxmanifest"
#define CMDLINE_SFXVERSION			_CFG_PARAM_TYPE"sfxversion"
#define CMDLINE_SFXLANG				_CFG_PARAM_TYPE"sfxlang"
#define	CMDLINE_SFXTEST				_CFG_PARAM_TYPE"sfxtest"

#ifdef _SFX_USE_TEST

	#define TSD_BEGINPROMPT			L'b'
	#define TSD_BEGINPROMPT_FORCE	L'B'
	#define TSD_CANCELPROMPT		L'c'
	#define TSD_CANCELPROMPT_FORCE	L'C'
	#define TSD_EXTRACT				L'e'
	#define TSD_EXTRACT_FORCE		L'E'
	#define TSD_FINISHMESSAGE		L'f'
	#define TSD_FINISHMESSAGE_FORCE	L'F'
	#define TSD_HELPTEXT			L'h'
	#define TSD_HELPTEXT_FORCE		L'H'
	#define TSD_EXTRACTPATH			L'p'
	#define TSD_EXTRACTPATH_FORCE	L'P'
#if defined(_SFX_USE_SFXAPI) || defined(_SFX_USE_COMPRESSED_CONFIG)
	#define TSD_SFXAPI				L's'
	#define TSD_SFXAPI_FORCE		L'S'
#endif // defined(_SFX_USE_SFXAPI) || defined(_SFX_USE_COMPRESSED_CONFIG)
	#define TSD_WARNING				L'w'
#ifdef _SFX_USE_WARNINGS
	#define TSD_WARNING_FORCE		L'W'
#endif // _SFX_USE_WARNINGS
#ifdef SFX_CRYPTO
	#define TSD_PASSWORD			L'x'
	#define TSD_PASSWORD_FORCE		L'X'
#endif // SFX_CRYPTO
	#define TSD_ERROR				L'z'
	#define TSD_ERROR_FORCE			L'Z'

	#define TMT_DIALOGS				1
	#define TMT_ARCHIVE				2
	#define TMT_DIALOGS_TO_STDOUT	3
	#define TMT_CHECK_CONFIG		4

#endif // _SFX_USE_TEST

#define SPECIAL_FOLDER_FORMAT			L"SfxFolder%02d"
#define SFX_STRING_FORMAT				L"SfxString%d"

#ifdef _SFX_USE_ENVIRONMENT_VARS
	#define SFX_ENV_VAR_PREFIX				L"SfxVar"
	#define SFX_ENV_VAR_SYSTEM_PLATFORM	SFX_ENV_VAR_PREFIX L"SystemPlatform"
	#define SFX_ENV_VAR_SYSTEM_LANGUAGE	SFX_ENV_VAR_PREFIX L"SystemLanguage"
	#define SFX_ENV_VAR_MODULE_PLATFORM	SFX_ENV_VAR_PREFIX L"ModulePlatform"
	#define SFX_ENV_VAR_CMDLINE_0		SFX_ENV_VAR_PREFIX L"CmdLine0"
	#define SFX_ENV_VAR_CMDLINE_1		SFX_ENV_VAR_PREFIX L"CmdLine1"
	#define SFX_ENV_VAR_CMDLINE_2		SFX_ENV_VAR_PREFIX L"CmdLine2"
	#define SFX_ENV_VAR_APIPATH			SFX_ENV_VAR_PREFIX L"ApiPath"
#endif // _SFX_USE_ENVIRONMENT_VARS



/* Custom resources IDs */
#define IDC_BANNER						3001

#define IDD_BEGINPROMT_CLASSIC          2001
#define IDD_EXTRACTPATH                 2002
#define IDD_BEGINPROMT_EXTRACTPATH      2003
#define IDD_EXTRACT                     2004
#define IDD_FINISHMESSAGE               2005
#define IDD_HELPTEXT                    2006
#define IDD_PASSWORD                    2007
#if defined(_SFX_USE_SFXAPI) || defined(_SFX_USE_COMPRESSED_CONFIG)
	#define IDD_SFXAPI_PREPARE			2008
#endif // defined(_SFX_USE_SFXAPI) || defined(_SFX_USE_COMPRESSED_CONFIG)

#define SDC_ICON						1201
#define SDC_TEXT						1202
#define SDC_BUTTON1						1203
#define SDC_BUTTON2						1204
#define SDC_TEXT2						1205
#define SDC_EXTRACTPATHEDIT				1206
#define SDC_EXTRACTPATHBROWSE			1207
#define SDC_PROGRESS					1208
#define SDC_BACK						1209
#define SDC_PASSWORDEDIT				SDC_EXTRACTPATHEDIT

#define IDC_NEXT                        SDC_BUTTON1
#define IDC_CANCEL						SDC_BUTTON2
#define IDC_BACK						SDC_BACK
#define IDC_MAIN_TEXT					SDC_TEXT
#define IDC_EXTRA_TEXT					SDC_TEXT2
#define IDC_EXTRACT_PATH_EDIT			SDC_EXTRACTPATHEDIT
#define IDC_EXTRACT_PATH_BROWSE			SDC_EXTRACTPATHBROWSE


#endif // _7ZSFXMOD_H_INCLUDED_
