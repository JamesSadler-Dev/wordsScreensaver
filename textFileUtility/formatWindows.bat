python.exe %~dp0screensavertextformatter.py %~dp0input.txt ..\words.txt
timeout /t 2 /nobreak > NUL
copy /d /Y "..\words.txt" "C:\Windows\System32\"
