@ECHO OFF
REM SETLOCAL ENABLEDELAYEDEXPANSION

SET SfxVersion=170

FOR /F %%i IN (SfxBuild.txt) DO SET %%i

SET source_dir=%CD%
SET sfx_name=%source_dir%\..\snapshots\7zsd_%SfxVersion%_%SfxBuild%

CD ..\output\win32
"C:\Program Files\7-Zip\7z.exe" a -mx=9 %sfx_name%_x86.7z -xr!7zsd_Debug* *.sfx
CD ..\win64
"C:\Program Files\7-Zip\7z.exe" a -mx=9 %sfx_name%_x64.7z -xr!7zsd_Debug* *.sfx
