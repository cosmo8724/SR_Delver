rmdir /s /q "ipch"
rmdir /s /q "Debug"
rmdir /s /q "Release"

rmdir /s /q .\Client\Default\Debug
rmdir /s /q .\Client\Default\ipch
rmdir /s /q .\Client\Default\Release
rmdir /s /q .\Client\Default\x64

rmdir /s /q .\Engine\System\Default\Debug
rmdir /s /q .\Engine\Utility\Default\Debug

del /f /q .\Client\Bin\Client.exe
del /f /q .\Client\Bin\Client.ilk
del /f /q .\Client\Bin\Client.pdb
del /f /q Delver.VC.db
