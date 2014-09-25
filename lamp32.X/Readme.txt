Description:
This code sample is intended to be use on PIC explorer 16.
It demonstrate how to read file from SD Card using Pictail board for SD  & MMC cards.
This firmware utilize the 3 switches on explorer 16. S3 and S6 is used to navigate 
to the file. and S5 is used to go to the next file.
The filename and the content of the file is displayed to the LCD of explorer 16.

Set-up:
To set up, just connect the SD Card daughter board to SPI1 Pictail slot (first small slot from top) then burn the firmware.

Limitation:
This firmware only supports file up to 496 bytes of file.

Requirements:
- Explorer 16
- Pictail board for SD and MMC cards


Jumper Configuration
	J7  PIC24           ; enables communications to target
    	JP2 IN              ; enables LEDs
    	S2  PIM             ; enables device on PIM   