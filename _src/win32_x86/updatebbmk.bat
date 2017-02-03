copy /Y ..\..\src\bbmk\*.bmk ..\..\bin
copy ..\..\bin\bbmk.exe ..\..\bin\.bbmk.exe
..\..\bin\bmk makeapp -r -t console -o ..\..\bin\bbmk ..\..\src\bbmk\bbmk
..\..\bin\bbmk.exe -v
