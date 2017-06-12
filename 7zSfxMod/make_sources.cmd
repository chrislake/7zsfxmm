@Echo off
rem SetLocal enabledelayedexpansion

Set SfxVersion=170

For /F %%i In (SfxBuild.txt) Do Set %%i

set archive_name=snapshots\7zsd_src_%SfxVersion%_%SfxBuild%

cd ..
set sources_7zip=7-Zip\Asm\* 7-Zip\C\* 7-Zip\CPP\* 7-Zip\DOC\*
set sources_7zSfxMod=7zSfxMod\sources\* 7zSfxMod\7zSfxMod.vcproj 7zSfxMod\7zSfxMod.sln

7z a -mx=9 -m0=PPMd %archive_name% -xr!.svn %sources_7zip% %sources_7zSfxMod%