rem Creates junctions for all the subfolders that have CreateJunction.bat
rem These are required by the C4 Engine and are linked into:
rem   {C4Root}\Data\V4\Examples\
rem   {C4Root}\Import\V4\Examples\

@echo off
setlocal

echo Scanning for example folders...

for /D %%D in (*) do (
    if exist "%%D\CreateJunction.bat" (
        echo Running CreateJunction for %%D...
        pushd "%%D"
        call CreateJunction.bat
        popd
    ) else (
        echo Skipping %%D (no CreateJunction.bat)
    )
)

echo All done.
pause