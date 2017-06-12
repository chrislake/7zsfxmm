@Echo off
rem SetLocal enabledelayedexpansion

Set SfxVersion=170

For /F %%i In (SfxBuild.txt) Do Set %%i

set source_dir=%CD%
set sfx_name=%source_dir%\..\snapshots\7zsd_%SfxVersion%_%SfxBuild%

cd ..\output\win32
7z a -mx=9 %sfx_name%_x86.7z -xr!7zsd_Debug* *.sfx
cd ..\win64
7z a -mx=9 %sfx_name%_x64.7z -xr!7zsd_Debug* *.sfx
