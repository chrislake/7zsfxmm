@Echo off
rem SetLocal enabledelayedexpansion

set archive_name=c:\tmp\test.7z
set config_name=..\7zSfxMod\configs\test20160306.txt
set test_sfx=c:\tmp\test_sfx.exe

del %test_sfx% 2>NUL
xcopy /Y /E configs\tools\* ..\output\ && cd ..\output || exit 1
mkdir win32
mkdir win64
move *_x64.sfx win64
move *.sfx win32
7z a %archive_name% win64\* x86\* *.sfx.* && copy /b win32\7zsd_LZMA_Dialogs.sfx + %config_name% +%archive_name% %test_sfx%
