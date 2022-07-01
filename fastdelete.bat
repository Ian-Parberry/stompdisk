@echo off

REM Batch file that prompts the user for the name of a folder, then deletes
REM the folder and its contents. Using the command line like this will be
REM faster than the drag-and-drop Windows deletion mechanism because this
REM script doesn't waste time enumerating the files first.
REM
REM Warning: This operation cannot be undone.

REM This file is part of the stompdisk project, see  
REM https://github.com/Ian-Parberry/stompdisk for more details.

REM MIT License
REM
REM Copyright (c) 2022 Ian Parberry
REM
REM Permission is hereby granted, free of charge, to any person obtaining a copy
REM of this software and associated documentation files (the "Software"), to
REM deal in the Software without restriction, including without limitation the
REM rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
REM sell copies of the Software, and to permit persons to whom the Software is
REM furnished to do so, subject to the following conditions:
REM
REM The above copyright notice and this permission notice shall be included in
REM all copies or substantial portions of the Software.
REM
REM THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
REM IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
REM FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
REM AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
REM LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
REM FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
REM IN THE SOFTWARE.

setlocal ENABLEDELAYEDEXPANSION

set /p NAME="Enter folder name: "

if exist "%NAME%" (
  set /p CONFIRM="Are you sure you want to delete "%NAME%" [y/n]? "
  
  if "!CONFIRM!" == "y" (
    echo This may take some time...
    del /F/Q/S "%NAME%" > NUL
    rmdir /Q/S "%NAME%"
  ) else echo Delete aborted.
) else echo Folder "%NAME%" not found.

