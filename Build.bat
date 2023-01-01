@echo off
if not "%1" == "max" start /MAX cmd /c %0 max & exit/b
@title CE C Toolchain - Compiling ...
cd src\gfx
..\..\..\bin\convimg.exe
cd ..\..
..\bin\make.exe
@title CE C Toolchain - Finished !
pause