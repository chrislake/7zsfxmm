@ECHO OFF
REM SETLOCAL ENABLEDELAYEDEXPANSION

SET SfxVersion=171

FOR /F %%i IN (SfxBuild.txt) DO SET %%i

SET archive_name=snapshots\7zsd_src_%SfxVersion%_%SfxBuild%

CD ..
SET sources_7zip=7-Zip\Asm\* 7-Zip\C\* 7-Zip\CPP\* 7-Zip\DOC\*
SET sources_7zSfxMod=7zSfxMod\sources\* 7zSfxMod\7zSfxMod.vcproj 7zSfxMod\7zSfxMod.sln

"C:\Program Files\7-Zip\7z.exe" a -mx=9 -m0=PPMd %archive_name% -xr!.svn %sources_7zip% %sources_7zSfxMod%