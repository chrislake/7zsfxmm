@Echo off
rem SetLocal enabledelayedexpansion

Set SfxVersion=170

For /F %%i In (SfxBuild.txt) Do Set %%i

set source_dir=%CD%
set sfx_bin_name=%source_dir%\..\bin\7zsd_%SfxVersion%_%SfxBuild%
set sfx_src_name=%source_dir%\..\bin\7zsd_src_%SfxVersion%_%SfxBuild%

cd ..\output
7z a -mx=9 %sfx_bin_name%.7z -xr!7zsd_Debug* *.sfx || exit 1
7z a -mx=9 %sfx_src_name%.7z ..\7-Zip ..\7zSfxMod
