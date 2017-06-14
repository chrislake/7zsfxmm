@ECHO OFF
REM SETLOCAL ENABLEDELAYEDEXPANSION

SET SfxVersion=170

FOR /F %%i IN (SfxBuild.txt) DO SET %%i

SET source_dir=%CD%
SET sfx_bin_name=%source_dir%\..\bin\7zsd_%SfxVersion%_%SfxBuild%
SET sfx_src_name=%source_dir%\..\bin\7zsd_src_%SfxVersion%_%SfxBuild%

CD ..\output
"C:\Program Files\7-Zip\7z.exe" a -mx=9 %sfx_bin_name%.7z -xr!7zsd_Debug* *.sfx 
"C:\Program Files\7-Zip\7z.exe" a -mx=9 %sfx_src_name%.7z ..\7-Zip ..\7zSfxMod

