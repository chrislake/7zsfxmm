@ECHO OFF
REM SETLOCAL ENABLEDELAYEDEXPANSION

SET SfxVersion=170

FOR /F %%i IN (SfxBuild.txt) DO SET %%i

SET tm=%time%
SET HH=%tm:~0,2%
SET HH=%HH: =0%
SET timestamp=%date:~10,4%%date:~7,2%%date:~4,2%_%HH%%tm:~3,2%
ECHO %timestamp%

SET archive_name=7zsd.%SfxVersion%.%SfxBuild%.%timestamp%

SET exclude_files=-x!*/7zSfxMod.*.user -x!*/7zSfxMod.ncb -x!*/7zSfxMod.suo -x!*/*/.svn -x!*/.svn -x!*/*/*/.svn
CD ..
"C:\Program Files\7-Zip\7z.exe"  a -mx=9 snapshots\%archive_name%.7z %exclude_files% 7zSfxMod/sources/* output/win32/*.sfx output/win32/*.exe output/win32/*.map 7zSfxMod/7zSfxMod.* output/win64/*.sfx output/win64/*.exe output/win64/*.map changes.txt 7zSfxMod/*.cmd ..\configs\*
