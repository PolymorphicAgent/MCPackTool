@echo off
setlocal enabledelayedexpansion

REM Check if arguments are provided
if "%~1" == "" (
    echo Usage: %~nx0 "source file" "destination file"
    goto :eof
)

REM Check if source file exists
if not exist "%~1" (
    echo Source file "%~1" not found.
    goto :eof
)

REM Set variables for source and destination files
set "sourceFile=%~1"
set "destinationFile=%~2"

REM Prompt user for elevation using VBScript
echo Set objShell = CreateObject^("Shell.Application"^) > "%temp%\elevate.vbs"
echo objShell.ShellExecute "%~s0", "%~1 %~2", "", "runas", 1 >> "%temp%\elevate.vbs"
echo WScript.Quit >> "%temp%\elevate.vbs"
cscript //nologo "%temp%\elevate.vbs"
del "%temp%\elevate.vbs"

REM Check the return code of the UAC prompt
if %errorlevel% neq 0 (
    echo User declined elevation. Script terminated.
    goto :eof
)

REM Copy the file using PowerShell's Copy-Item cmdlet
powershell -Command "Copy-Item -LiteralPath '%sourceFile%' -Destination '%destinationFile%' -Force"

echo File copied successfully: "%sourceFile%" to "%destinationFile%"
