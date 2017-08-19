#u-boot - DO NOT FLASH THIS TO THE eMMC YET

This repository describes the changes to TI SDK U-boot to properly boot the SatFi2 unit.


Work in progress.  To test use serial loading.

1. erase the beginning of eMMC so that you get 'CCCCCC' on console

2. Follow Boot over uart instructions from this page
http://processors.wiki.ti.com/index.php/AM335x_U-Boot_User%27s_Guide#Boot_Over_UART

Here are the instructions they give for teraterm.  Minicom seems to work better, teraterm sometimes hangs after the ymodem send completes.  

When “CCCC” characters appear on TeraTerm window, from the File Menu select Transfer --> XMODEM --> Send (1K mode)
Select “u-boot-spl.bin” for the transfer
After image is successfully downloaded, the ROM code will boot it.
When “C” characters appear on TeraTerm window, from the File Menu select Transfer --> YMODEM --> Send (1K mode)
Select “u-boot.img” for the transfer
After image is successfully downloaded, U-Boot will boot it.
Hit enter and get to u-boot prompt “=> ”
