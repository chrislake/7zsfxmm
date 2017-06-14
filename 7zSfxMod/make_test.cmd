@ECHO OFF
REM SETLOCAL ENABLEDELAYEDEXPANSION

SET archive_name=..\tmp\test.7z
SET config_name=..\7zSfxMod\configs\test20160306.txt
SET test_sfx=..\tmp\test_sfx.exe

DEL %test_sfx% 2>NUL
XCOPY /Y /E configs\tools\* ..\output\ && CD ..\output || EXIT 1
MKDIR win32
MKDIR win64
MOVE *_x64.sfx win64
MOVE *.sfx win32
"C:\Program Files\7-Zip\7z.exe" a %archive_name% win64\* win32\* *.sfx.* && COPY /b win32\7zsd_LZMA_Dialogs.sfx + %config_name% + %archive_name% %test_sfx%
