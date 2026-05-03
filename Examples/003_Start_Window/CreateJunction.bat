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