:: A3 SCRIPT
echo off
echo '............................................'
echo '..     ALGONQUIN COLLEGE - COM - 25S      ..'
echo '............................................'
echo '                                            '
echo ' ...=====.................................. '
echo ' ..==...................................... '
echo ' .==.......===...=.=..==..=.=..=...==..==.. '
echo ' ..==.....==.==.=.=.=.=.=.=.=..==..=..==... '
echo ' ...====...===..=.=.=.=...=.==.===.=...==.. '
echo '                                            '
echo '               [A3 - Scanner]               '
echo ' [Team: Mihretab Meaza / Mohamad Al Dakkak           
echo '............................................'
set "arg=%1"
if "%arg%"=="" set "arg=T1.mpp"
:: del T1.mpp RESTORED.txt
gcc compilers.c main1coder.c step1coder.c main2reader.c step2reader.c main3scanner.c step3scanner.c -o compilers
timeout /t 1 /nobreak >nul 
:: compilers 1 1 %arg% CODED.txt > out.txt 2> err.txt
:: timeout /t 1 /nobreak >nul
compilers 3 %arg% > out.txt 2> err.txt
timeout /t 1 /nobreak >nul
dir out.txt err.txt
type out.txt
