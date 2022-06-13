@echo off
setlocal ENABLEDELAYEDEXPANSION

set /p NAME="Enter folder name: "

if exist "%NAME%" (
  set /p CONFIRM="Are you sure you want to delete "%NAME%" [y/n]? "
  
  if "!CONFIRM!" == "y" (
    echo "This may take some time..."
    del /F/Q/S "%NAME%" > NUL
    rmdir /Q/S "%NAME%"
  ) else echo Delete aborted.
) else echo Folder "%NAME%" not found.

