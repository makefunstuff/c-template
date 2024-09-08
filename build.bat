@echo off
setlocal

mkdir build
pushd build

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

set CFLAGS=/std:c11 /FC /W4 /Zl /wd4996 /nologo /Zi
rem 'vendor code dir %USERPROFILE%\Code\vendor'
set VENDOR_DIR=

rem 'Thrid party headers'
rem 'Example how to add include /I %VENDOR_DIR%\glfw\include /I %VENDOR_DIR%\glew\include /I %VENDOR_DIR%\cglm\include /I ..\include'
set INCLUDE_L=

rem 'Including vendor code provide pats to your 3rd pary libs'
rem 'Example how to include libs %VENDOR_DIR%\glfw\lib-vc2022\glfw3.lib %VENDOR_DIR%\glew\lib\Release\x64\glew32s.lib User32.lib Shell32.lib Gdi32.lib opengl32.lib %VENDOR_DIR%\cglm\win\x64\Release\cglm.lib'

set LIBS=

rem 'list all files here'

set FILES=..\src\main.c

cl %CFLAGS% %INCLUDE_L% %FILES% /link %LIBS% /OUT:ogl.exe /DEBUG

popd
