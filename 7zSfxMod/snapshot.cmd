@Echo off
rem SetLocal enabledelayedexpansion

Set SfxVersion=160

For /F %%i In (SfxBuild.txt) Do Set %%i

Set tm=%time%
Set HH=%tm:~0,2%
Set HH=%HH: =0%
set timestamp=%date:~6,4%%date:~3,2%%date:~0,2%_%HH%%tm:~3,2%
echo %timestamp%

set archive_name=7zsd.%SfxVersion%.%SfxBuild%.%timestamp%

set exclude_files=-x!*/7ZSfxMod.*.user -x!*/7zSfxMod.ncb -x!*/7zSfxMod.suo -x!*/*/.svn -x!*/.svn -x!*/*/*/.svn
cd ..
7z a -mx=9 snapshots\%archive_name%.7z %exclude_files% 7zSfxMod/sources/* output/win32/*.sfx output/win32/*.exe output/win32/*.map 7zSfxMod/7zSfxMod.* output/win64/*.sfx output/win64/*.exe output/win64/*.map changes.txt 7zSfxMod/*.cmd ..\configs\*