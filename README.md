# electronic-piano
Electronic piano using ATmega32
an electronic piano with recording function, with a maximum of 3 recorded songs and maximum 100 notes per song. The length of each note played by the user is recorded. In our design, the master MCU is in charge of handling keypad input, LCD display, and recording. The slave MCU is in charge of playing sounds. When playing a note, the master MCU sends the note data to the slave through UART, triggering the ISR function of the slave and it starts to play that note.

![image](https://github.com/CreeperZzz/electronic-piano/assets/51710762/00d7943d-19a2-4e7a-9f0f-53fa26f5e3b5)

![image](https://github.com/CreeperZzz/electronic-piano/assets/51710762/d6e4c4c6-f3b6-4448-8b6b-b0a988cd96c8) 

This diagram shows the keypad structure. We will connect keypad using Port C. Connect PC0 to pin 8 of keypad, PC1 to pin 7 of keypad … and PC7 to pin1 of keypad.

