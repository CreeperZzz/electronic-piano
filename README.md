# electronic-piano
Electronic piano using ATmega32
an electronic piano with recording function, with a maximum of 3 recorded songs and maximum 100 notes per song. The length of each note played by the user is recorded. In our design, the master MCU is in charge of handling keypad input, LCD display, and recording. The slave MCU is in charge of playing sounds. When playing a note, the master MCU sends the note data to the slave through UART, triggering the ISR function of the slave and it starts to play that note.

