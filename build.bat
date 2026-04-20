@echo off
if not exist build mkdir build
g++ -std=c++17 -Wall -g src/main.cpp src/repl.cpp -o build/database.exe
if %errorlevel% neq 0 (
    echo Build failed.
    exit /b %errorlevel%
)
echo Build successful! -> build/database.exe