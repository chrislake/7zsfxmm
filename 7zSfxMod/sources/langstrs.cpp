/*---------------------------------------------------------------------------*/
/* File:        langstrs.cpp                                                 */
/* Created:     Wed, 10 Jan 2007 23:44:00 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Last update: Fri, 01 Apr 2016 20:44:42 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Revision:    3369                                                         */
/*---------------------------------------------------------------------------*/
/* Revision:    1404                                                         */
/* Updated:     Sun, 14 Nov 2010 01:04:46 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Add 'password' strings                                       */
/*---------------------------------------------------------------------------*/
/* Revision:    1263                                                         */
/* Updated:     Sat, 26 Jun 2010 04:44:01 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: Add 'copyright' strings                                      */
/*---------------------------------------------------------------------------*/
/* Revision:    1167                                                         */
/* Updated:     Mon, 22 Mar 2010 11:17:07 GMT                                */
/*              by Oleg N. Scherbakov, mailto:oleg@7zsfx.info                */
/* Description: New file stamp.                                              */
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "7zSfxModInt.h"
#include "langstrs.h"
#include "version.h"

/* RU-russian */
const UINT SfxSecondaryLangId = 0x419;

LANGSTRING SfxLangStrings[] = {
// Версия модуля
	{ STR_SFXVERSION,		"SFX module - Copyright (c) 2005-2016 Oleg Scherbakov\n"
							"\t" VERSION_SFX_TEXT_EN "\n"
							"\n7-Zip archiver - Copyright (c) 1999-2016 Igor Pavlov\n"
							"\t" VERSION_7ZIP_TEXT_EN "\n"
							"\nSupported methods and filters, build options:\n\t",
						"SFX модуль - Copyright (c) 2005-2016 Олег Щербаков\n"
						"\t" VERSION_SFX_TEXT_RU "\n"
						"\nАрхиватор 7-Zip - Copyright (c) 1999-2016 Игорь Павлов\n"
						"\t" VERSION_7ZIP_TEXT_RU "\n"
						"\nПоддерживаемые методы и фильтры, опции сборки:\n\t", NULL },
// Заголовок окон сообщений по умолчанию, отображаемое, если
// модуль не смог определить имя exe-файла.
// Если SFX модуль смог распознать свое имя (а это практически 100%), вместо него
// будет выводится имя exe-файла без расширения. Или, если файл конфигурации
// успешно прочитан и в нем указан "Title" - содержимое "Title"
	{ STR_TITLE,			"7z SFX",
							"7z SFX", NULL },
	{ STR_ERROR_TITLE,		"7z SFX: error",
							"7z SFX: ошибка", NULL },
// Данная подстрока будет присоединена к основному заголовку для окон ошибок
// Т.е. заголовок окон будет что-то вроде "7-Zip SFX: error"
	{ STR_ERROR_SUFFIX,		": error",
							": ошибка", NULL },
// Заголовок по умолчанию для окна распаковки
	{ STR_EXTRACT_TITLE,	"Extracting",
							"Распаковка", NULL },
// Невозможно получить имя файла SFX установки. Теоритически, никогда не появится,
// я даже не знаю, как сэмулировать эту ситуацию.
	{ ERR_MODULEPATHNAME,	"Could not get SFX filename.",
							"Невозможно получить имя SFX архива.", NULL },
// Невозможно открыть файл архива
// Может возникнуть, если файл блокирован другим приложением, недостаточно прав и т.п.
// Вторая строка будет содержать ТЕКСТ системной ошибки НА ЯЗЫКЕ, выставленном в системе по умолчанию
	{ ERR_OPEN_ARCHIVE,		"Could not open archive file \"%s\".",
							"Невозможно открыть файл архива \"%s\".", NULL },
// Архив поврежден/не найдена сигнатура, т.е внутренние структуры не соответствуют формату 7-Zip SFX
// Может возникнуть на "корявых" или поврежденных 7-Zip SFX установках
	{ ERR_NON7Z_ARCHIVE,	"Non 7z archive.",
							"Файл не является 7z архивом.", NULL },
// Невозможно прочитать файл конфигурации (или он отсутствует)
	{ ERR_READ_CONFIG,		"Could not read SFX configuration or configuration not found.",
							"Невозможно прочитать или отсутствует файл конфигурации SFX архива.", NULL },
	{ ERR_WRITE_CONFIG,		"Could not write SFX configuration.",
							"Невозможно сохранить файл конфигурации.", NULL },
// Ошибка в файле конфигурации, отсутствие обязательных кавычек, несоответствие
// формату Параметр="Значение", значение параметра не в UTF8 и т.п.
// "Ошибка в строке Х конфигурации"
	{ ERR_CONFIG_DATA,		"Error in line %d of configuration data:\n\n%s",
							"Ошибка в строке %d файла конфигурации:\n\n%s", NULL },
// Невозможно создать папку "такую-то"
// Вторая строка будет содержать ТЕКСТ системной ошибки НА ЯЗЫКЕ, выставленном в системе по умолчанию
	{ ERR_CREATE_FOLDER,	"Could not create folder \"%s\".",
							"Невозможно создать папку \"%s\".", NULL },
// Невозможно удалить файл или директорию "такой-то-такая-то"
// Вторая строка будет содержать ТЕКСТ системной ошибки НА ЯЗЫКЕ, выставленном в системе по умолчанию
	{ ERR_DELETE_FILE,		"Could not delete file or folder \"%s\".",
							"Невозможно удалить файл или папку \"%s\".", NULL },
// Невозможно найти команду для "AutoInstallX"
// Может вылезти, когда пользователь указал в командной строке -aiX или -ai,
// а AutoInstall с индексом X (или без индекса для -ai) в конфиге не найден
	{ ERR_AUTOINSTALL_NOTFOUND,	"Could not find command for \"%s\".",
								"Параметр \"%s\" не найден в файле конфигурации.", NULL },
// Не указаны ни "RunProgram", ни "AutoInstall", а распаковка должна происходит
// во временную папку. А чего хотели-то? Распаковать и удалить?
	{ ERR_NO_SETUP_EXE,		"Could not find \"setup.exe\".",
							"Файл \"setup.exe\" не найден.", NULL },
// Произошла ошибка при выполнении "такой-то программы"
// Вторая строка будет содержать ТЕКСТ системной ошибки НА ЯЗЫКЕ, выставленном в системе по умолчанию
	{ ERR_EXECUTE,			"Error during execution \"%s\".",
							"Произошла ошибка при выполнении \"%s\".", NULL },
// Ошибки ядра распаковки
	{ ERR_7Z_UNSUPPORTED_METHOD,	"7-Zip: Unsupported method.",
									"7-Zip: Метод не поддерживается.", NULL },
	{ ERR_7Z_CRC_ERROR,				"7-Zip: CRC error.",
									"7-Zip: Ошибка контрольной суммы (CRC).", NULL },
	{ ERR_7Z_DATA_ERROR,			"7-Zip: Data error.\nThe archive is corrupted"
#ifdef SFX_CRYPTO
									", or invalid password was entered"
#endif // SFX_CRYPTO
									".",
									"7-Zip: Ошибка данных.\nАрхив поврежден"
#ifdef SFX_CRYPTO
									" или введен неверный пароль"
#endif // SFX_CRYPTO
									".",
									NULL },
	{ ERR_7Z_INTERNAL_ERROR,		"7-Zip: Internal error, code %u.",
									"7-Zip: Внутренняя ошибка. Код ошибки %u.", NULL },
	{ ERR_7Z_EXTRACT_ERROR1,		"7-Zip: Internal error, code 0x%08X.",
									"7-Zip: Внутренняя ошибка. Код ошибки 0x%08X.", NULL },
	{ ERR_7Z_EXTRACT_ERROR2,		"7-Zip: Extraction error.",
									"7-Zip: Ошибка распаковки.", NULL },

	{ STR_EXTRACT_PATH_TITLE,		"Extraction path",
									"Путь распаковки", NULL },
	{ STR_EXTRACT_PATH_TEXT,		"Extraction path:",
									"Путь распаковки:", NULL },
	{ STR_CANCEL_PROMPT,			"Really cancel the installation?",
									"Вы действительно хотите отменить установку?", NULL },
// Нет встроенной справки (параметр "HelpText")
	{ STR_DEFAULT_HELP_TEXT,	"No \"HelpText\" in the configuration file.",
								"Параметр \"HelpText\" не указан в файле конфигурации.", NULL },
// Кнопки диалогов
	{ STR_BUTTON_OK,			"OK",		"OK", NULL },
	{ STR_BUTTON_CANCEL,		"Cancel",	"Отмена", NULL },
	{ STR_BUTTON_YES,			"Yes",		"Да", NULL },
	{ STR_BUTTON_NO,			"No",		"Нет", NULL },
	{ STR_SECONDS,				" s",		" с", NULL },
// Added April 9, 2008
// Невозможно создать файл "такой-то"
// Вторая строка будет содержать ТЕКСТ системной ошибки НА ЯЗЫКЕ, выставленном в системе по умолчанию
	{ ERR_CREATE_FILE,		"Could not create file \"%s\".",
							"Невозможно создать файл \"%s\".", NULL },

	{ ERR_OVERWRITE,		"Could not overwrite file \"%s\".",
							"Невозможно перезаписать файл \"%s\".", NULL },
	{ ERR_CONFIG_CMDLINE,	"Error in command line:\n\n%s",
							"Ошибка в командной строке:\n\n%s", NULL },
	{ STR_BUTTON_BACK,		"Back",		"Назад", NULL },
	{ STR_BUTTON_NEXT,		"Next",		"Далее", NULL },
	{ STR_BUTTON_FINISH,	"Finish",	"Готово", NULL },
	{ STR_BUTTON_CANCEL2,	"Cancel",	"Отменить", NULL },
#ifdef _SFX_USE_CUSTOM_EXCEPTIONS
	{ ERR_SFX_EXCEPTION,		"Application error:\n\nException code:\n\t0x%08x\nAddress:\n\t0x%08x\nException data:\n",
								"Ошибка приложения:\n\nКод исключения:\n\t0x%08x\nАдрес:\n\t0x%08x\nДанные исключения:\n" },
#endif // _SFX_USE_CUSTOM_EXCEPTIONS

// Added June 6, 2010: warnings dialogs
#ifdef _SFX_USE_WARNINGS
	{ STR_WARNING_TITLE,		"7z SFX: warning",
								"7z SFX: предупреждение", NULL },
	{ STR_WARNING_SUFFIX,		": warning",
								": предупреждение", NULL },
#ifdef _SFX_USE_CHECK_FREE_SPACE
	{ STR_DISK_FREE_SPACE,		"Not enough free space for extracting.\n\nDo you want to continue?",
								"На диске недостаточно места для распаковки.\n\nВы хотите продолжить?", NULL },
#endif // _SFX_USE_CHECK_FREE_SPACE
#ifdef _SFX_USE_CHECK_RAM
	{ STR_PHYSICAL_MEMORY,		"Insufficient physical memory.\nExtracting may take a long time.\n\nDo you want to continue?",
								"Недостаточно физической памяти.\nРаспаковка может занять продолжительное время.\n\nВы хотите продолжить?", NULL },
#endif // _SFX_USE_CHECK_FREE_SPACE
#endif // _SFX_USE_WARNINGS
#ifdef SFX_CRYPTO
	{ STR_PASSWORD_TEXT,		"Enter password:", "Введите пароль:", NULL },
#endif // SFX_CRYPTO
#if defined(_SFX_USE_SFXAPI) || defined(_SFX_USE_COMPRESSED_CONFIG)
	{ STR_SFXAPI_PREPARE,		"Preparing...",	"Подготовка...", NULL },
#endif // defined(_SFX_USE_SFXAPI) || defined(_SFX_USE_COMPRESSED_CONFIG)
	{ 0, "", "", NULL }
};
