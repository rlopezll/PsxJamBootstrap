cp .\MAIN.ps-exe cdrom\MAIN.EXE
cd cdrom
rm myimage.*
..\..\..\tools\mkpsxiso\mkpsxiso-2.02-win64\bin\mkpsxiso.exe -y makeiso.xml
cd..