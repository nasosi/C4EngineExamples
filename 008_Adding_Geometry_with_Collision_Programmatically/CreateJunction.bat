rem MIT License
rem 
rem Copyright (c) 2023-2026 Athanasios Iliopoulos
rem 
rem Permission is hereby granted, free of charge, to any person obtaining a copy
rem of this software and associated documentation files (the "Software"), to deal
rem in the Software without restriction, including without limitation the rights
rem to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
rem copies of the Software, and to permit persons to whom the Software is
rem furnished to do so, subject to the following conditions:
rem 
rem The above copyright notice and this permission notice shall be included in all
rem copies or substantial portions of the Software.
rem 
rem THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
rem IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
rem FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
rem AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
rem LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
rem OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
rem SOFTWARE.

rem This software may contain work that is licensed under different terms. In that 
rem event, appropriate license files will be included in close folder proximity 
rem to said work. You need to ensure that your use of this software is compatible
rem with the license of any other work that may be part of this software.

rem Creates junctions for this example's Data and Import folders.
rem These are required by the C4 Engine and are linked into:
rem   {C4Root}\Data\V4\Examples\
rem   {C4Root}\Import\V4\Examples\

@echo off
setlocal

for %%I in ("%cd%") do set DIRNAME=%%~nxI

echo Creating junctions for %DIRNAME%...

if not exist "..\..\..\Data\V4\Examples\" (
    mkdir "..\..\..\Data\V4\Examples\"
)

if not exist "..\..\..\Import\V4\Examples\" (
    mkdir "..\..\..\Import\V4\Examples\"
)

if exist "Data" (
    mklink /J "..\..\..\Data\V4\Examples\%DIRNAME%" "Data"
)

if exist "Import" (
    mklink /J "..\..\..\Import\V4\Examples\%DIRNAME%" "Import"
)

echo Done: %DIRNAME%