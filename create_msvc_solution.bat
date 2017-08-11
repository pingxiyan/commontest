@REM Create VS2013 WIN64 project


@setlocal
@echo off

set "ROOT_DIR=%~dp0"

set "SOLUTION_DIR64=%ROOT_DIR%\build"

echo Creating Visual Studio 2013 (x64) files in %SOLUTION_DIR64%... && ^
cd "%ROOT_DIR%" && cmake -E make_directory "%SOLUTION_DIR64%" && cd "%SOLUTION_DIR64%" && cmake -G "Visual Studio 12 2013 Win64" "%ROOT_DIR%"

echo Done.
pause