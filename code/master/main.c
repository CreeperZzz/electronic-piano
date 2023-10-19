#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avr.h"
#include "lcd.h"
#include "uart.h"
#include "keypad.h"
#include "music.h"


void main_display(){
	lcd_clr();
	lcd_puts("Music Player");
	lcd_pos(1,0);
	lcd_puts("#:RecordD:Select");
}

int main(void)
{
	SET_BIT(DDRB, 4);
	uart_init();
	lcd_init();
	init_song_list();

	// play_song(twinkle);
	unsigned char key = 0;
	unsigned char octave = 1;
	unsigned char song_index;
	char temp[20];
    while(1)
    {
		main_display();

		key = get_key();

		if(key == 15){
			if(!music_list_full()){
				lcd_clr();
				lcd_puts("Recording");
				lcd_pos(1,0);
				lcd_puts("#:Save D:Quit");
				avr_wait(100);
				record_song();
			}else{
				lcd_clr();
				lcd_puts("Music List Full");
				avr_wait(3000);
				continue;
			}
		}
		if(key == 16){
			while(1){
				lcd_clr();
				lcd_puts("*:save 0:record");
				lcd_pos(1,0);
				lcd_puts("D:quit");
				avr_wait(100);
				key = get_key();
				if(key==13){
					song_index = 0;
					while(1){
						lcd_clr();
						lcd_pos(0,0);
						lcd_puts(song_names[song_index]);
						lcd_pos(1,0);
						lcd_puts("A:prev|B:next");
						if(is_pressed(3,3)){
							break;
						}
						if(is_pressed(0,3)){
							if(song_index == 0){
								song_index = 2;
							}else{
								song_index--;
							}
						}
						if(is_pressed(1,3)){
							if(song_index == 2){
								song_index = 0;
							}else{
								song_index++;
							}	
						}
						if(is_pressed(2,3)){

							play_song(song_list[song_index]);
							
						}
						avr_wait(200);
					}
				}
				if(key==14){
					if(!music_recorded){
						lcd_clr();
						lcd_puts("No song recorded");
						avr_wait(3000);
					}else{
						song_index = 0;
						while (1)
						{	
							lcd_clr();
							itoa(song_index+1,temp,10);
							lcd_puts(temp);	
							lcd_pos(1,0);
							lcd_puts("A:prev|B:next");
							if(is_pressed(3,3)){
								avr_wait(100);
								break;
							}
							if(is_pressed(0,3)){
								if(song_index == 0){
									song_index = music_recorded-1;
								}else{
									song_index--;
								}
							}
							if(is_pressed(1,3)){
								if(song_index == music_recorded-1){
									song_index = 0;
								}else{
									song_index++;
								}	
							}
							if(is_pressed(2,3)){
								lcd_pos(0,2);
								lcd_puts("playing");
								play_recorded_music(song_index);
								lcd_pos(0,2);
								lcd_puts("finished");
							}
							avr_wait(200);
						}
					}
				}
				if(key==16){
					avr_wait(100);
					break;
				}
			}
		}
		if(key == 13){
			octave = octave>0 ? octave-1 : octave;
			avr_wait(100);
			continue;
		}
		if(key == 14){
			octave = octave<2 ? octave+1 : octave;
			avr_wait(100);
			continue;
		}	
		// lcd_clr();
		// lcd_puts("playing");

		if(key>=0 && key < 13){
			uart_sendbyte(key? key+12*octave : key);
		}
		avr_wait(100);
		
	}
}