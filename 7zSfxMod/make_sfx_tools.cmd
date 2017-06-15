@ECHO OFF
REM SETLOCAL ENABLEDELAYEDEXPANSION

SET SfxVersion=171

FOR /F %%i IN (SfxBuild.txt) DO SET %%i

SET tm=%time%
SET HH=%tm:~0,2%
SET HH=%HH: =0%
SET timestamp=%date:~10,4%%date:~7,2%%date:~4,2%_%HH%%tm:~3,2%
ECHO %timestamp%

SET sfx_name=..\snapshots\7zsd_tools_%SfxVersion%_%SfxBuild%
SET archive_name=%sfx_name%_%timestamp%

CD ..\output
"C:\Program Files\7-Zip\7z.exe" a -mx=9 %archive_name%.7z -xr!7zsd_Debug* *.sfx
COPY /b 7zsd_LZMA_Dialogs.sfx + ..\7zSfxMod\configs\test_config.txt + %archive_name%.7z %sfx_name%_x86_test.exe
COPY /b 7zsd_LZMA_Dialogs.sfx + ..\7zSfxMod\configs\7zsd_tools_RTF_uk_ru.txt + %archive_name%.7z %sfx_name%_x86.exe
COPY /b 7zsd_LZMA_Dialogs_x64.sfx + ..\7zSfxMod\configs\7zsd_tools_RTF_uk_ru.txt + %archive_name%.7z %sfx_name%_x64.exe
COPY /b 7zsd_LZMA_Dialogs.sfx + ..\7zSfxMod\configs\ + %archive_name%.7z %sfx_name%_x86_2.exe
DEL %archive_name%.7z
