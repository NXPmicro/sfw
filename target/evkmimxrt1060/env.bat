@echo off

if /i "%processor_architecture%"=="x86" (
        start  ..\..\tools\env\ConEmu\ConEmu.exe 
) else if /i "%processor_architecture%"=="amd64" (
    if defined processor_architew6432 (
        start  ..\..\tools\env\ConEmu\ConEmu.exe 
    ) else (
        start  ..\..\tools\env\ConEmu\ConEmu64.exe 
    )
)

@echo success
