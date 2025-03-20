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

REM Copy the file using PowerShell's Copy-Item cmdlet
powershell -Command "Copy-Item -LiteralPath '%sourceFile%' -Destination '%destinationFile%' -Force"

echo File copied successfully: "%sourceFile%" to "%destinationFile%"
