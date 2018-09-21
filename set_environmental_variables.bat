
copy %~dp0x64\Release\liba.lib %~dp0liba\liba.lib
copy %~dp0x64\Debug\liba.lib %~dp0liba\dliba.lib
copy %~dp0x64\Debug\liba.pdb %~dp0liba\dliba.pdb

setx AKECHILIB %~dp0liba