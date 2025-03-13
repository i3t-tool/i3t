:: Requires dos2unix to be installed in wsl
wsl dos2unix FormatCode.sh
wsl cd ../; ./Scripts/FormatCode.sh;
wsl unix2dos FormatCode.sh
echo "Format done"
pause