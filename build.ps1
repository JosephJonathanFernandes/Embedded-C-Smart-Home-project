<#
Simple Windows build helper. Requires `gcc` (MinGW/MSYS2) in PATH.
#>
param()

$exe = "smart_home_sim.exe"
if (Get-Command gcc -ErrorAction SilentlyContinue) {
    gcc -std=c11 -Wall -Wextra -O2 -o $exe smart_home_sim.c
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Build succeeded: $exe"
    } else {
        Write-Host "Build failed with exit code $LASTEXITCODE" -ForegroundColor Red
        exit $LASTEXITCODE
    }
} else {
    Write-Host "gcc not found in PATH. Install MinGW or MSYS2 and ensure gcc is available." -ForegroundColor Yellow
}
