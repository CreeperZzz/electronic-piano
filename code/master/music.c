#include "avr.h"
#include "music.h"
#include "uart.h"
#include "keypad.h"

unsigned char tempo = 120;
unsigned int notetime; //time of a eighth note in ms

void wait10us(unsigned short m) //10^-5s or 10 us
{
	TCCR0 = 1;
	while (m--) {
		TCNT0 = (unsigned char)(256 - XTAL_FRQ * 0.00001);
		SET_BIT(TIFR, TOV0);
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}

unsigned int note_wait[] = {0, 382, 361, 341, 321, 303, 286, 270, 255, 241, 227, 215, 202,
	191, 180, 170, 161, 152, 143, 135, 128, 120, 114, 107, 101,
96, 90, 85, 80, 76, 72, 68, 64, 60, 57, 54, 51};
unsigned int note_hz[] = {0, 131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247,
	262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988};

PlayingNote music_scale[] = {{C,1,2},{D,1,2},{E,1,2},{F,1,2},{G,1,2},{A,1,2},{B,1,2},{C,2,2},{P,0,2}}; //9


PlayingNote twinkle_seq[] = {{C,2,2},{C,2,2},{G,2,2},{G,2,2},{A,2,2},{A,2,2},{G,2,4},{F,2,2},{F,2,2},{E,2,2},{E,2,2},{D,2,2},{D,2,2},{C,2,4},
{G,2,2},{G,2,2},{F,2,2},{F,2,2},{E,2,2},{E,2,2},{D,2,4},{G,2,2},{G,2,2},{F,2,2},{F,2,2},{E,2,2},{E,2,2},{D,2,4},
{C,2,2},{C,2,2},{G,2,2},{G,2,2},{A,2,2},{A,2,2},{G,2,4},{F,2,2},{F,2,2},{E,2,2},{E,2,2},{D,2,2},{D,2,2},{C,2,4}}; //42

PlayingNote hoshi_no_arika_seq[] = {{E,2,1},{Ds,2,1},{E,2,1},{Gs,2,1},{Ds,2,4},{Cs,2,1},{B,1,1},{Cs,2,1},{E,2,1},{B,1,4},
{A,1,1},{Gs,1,1},{A,1,1},{E,2,1},{Ds,2,2},{C,2,2},{Cs,2,1},{Ds,2,1},{E,2,1},{Gs,2,1},{Fs,2,4},
{E,2,1},{Ds,2,1},{E,2,1},{Gs,2,1},{Ds,2,3},{B,1,1},{Cs,2,1},{Ds,2,1},{E,2,1},{Fs,2,1},{Gs,2,3},{Gs,2,1},
{A,2,1},{Gs,2,1},{Fs,2,1},{E,2,1},{Ds,2,1},{Gs,2,1},{C,2,1},{Ds,2,1},{Cs,2,4},{P,0,4}}; //43

PlayingNote thank_you_seq[] = {{As,1,2},{C,2,2},{D,2,2},{F,2,2},{D,2,2},{D,2,4},{C,2,1},{C,2,1},{As,1,2},{C,2,4},
						   {As,1,2},{G,1,2},{As,1,2},{C,2,2},{D,2,8},{As,1,2},{G,1,2},{As,1,4},{F,1,1},{F,1,1},
						   {C,2,2},{As,1,4},{D,2,2},{C,2,2},{C,2,2},{As,1,2},{C,2,4},{As,1,2},{C,2,2},{F,2,2},
						   {D,2,2},{D,2,4},{C,2,1},{C,2,1},{As,1,2},{C,2,4},{As,1,2},{G,1,2},{As,1,2},{F,2,2},
						   {D,2,8},{As,1,2},{G,1,2},{As,1,4},{F,1,1},{F,1,1},{C,2,2},{As,1,4},{D,2,2},{C,2,2},
						   {As,1,2},{G,1,2},{As,1,8}};//53

music twinkle = {120,42,twinkle_seq};
music hoshi_no_arika = {80, 43, hoshi_no_arika_seq};
music thank_you = {240,53,thank_you_seq};

char song_names[3][15] = {"Litte Star", "Hoshi No Arika", "Thank You"};

music song_list[3];

music music_list[3];

unsigned char music_recorded = 0;

unsigned char is_start = 0;

void init_song_list(){
	song_list[0] = twinkle;
	song_list[1] = hoshi_no_arika;
	song_list[2] = thank_you;
}

void play_note(const PlayingNote* note) {

	// int i;
	unsigned char note_offset = note->note? note->note + note->octave*12 : 0; //determine index according to which octave the note is in

	uart_sendbyte(note_offset);
	avr_wait(notetime*note->duration-1);
	uart_sendbyte(0);
	avr_wait(1);
}

void play_song(const music m)
{
	tempo = m.default_tempo;
	notetime = (unsigned int)1000*60/tempo/2; //It seems the constant 1000 is signed int by default, which can cause overflow, need type casting
	const PlayingNote* note = m.note_seq;
	int i;

	for(i=0;i<m.length;i++)
	{
		play_note(note);
		note++;
	}
}

// char temp[20];

void record_song(){
	long timecount;
	long notecount = 0;
	unsigned char key = 0;
	unsigned char record_key = 0;

	PlayingNote notes[100];
	PlayingNote note;
	music music;
	unsigned char octave = 1;
	while(1){
		if(is_pressed(3,2)){
			if(is_start){
				music.note_seq = notes;
				music.length = notecount;
				music.default_tempo = 120;
				music_list[music_recorded] = music;
				music_recorded++;
				is_start = 0;
				break;
			}else{
				continue;
			}
		}
		if(is_pressed(3,3)){
			break;
		}
		if(is_pressed(3,0)){
			octave = octave>0 ? octave-1 : octave;
			avr_wait(100);
			continue;
		}
		if(is_pressed(3,1)){
			octave = octave<2 ? octave+1 : octave;
			avr_wait(100);
			continue;
		}	
		timecount = 0;
		while((key=get_key())){
			is_start = 1;
			record_key = key;
			uart_sendbyte(key? key+12*octave : key);
			avr_wait(50);
			timecount++;
			// lcd_clr();
			// ltoa(timecount,temp,10);
			// lcd_puts(temp);
		}
		// lcd_clr();
		// ltoa(timecount*50/250+1,temp,10);
		// lcd_puts(temp);
		if(record_key){
			note.note = record_key;
			note.octave = octave;
			note.duration = timecount*50/250 + 1;
			notes[notecount] = note;
			notecount++;
		}


		timecount = 0;
		while((key=get_key())==0){
			record_key = key;
			uart_sendbyte(key? key+12 : key);
			avr_wait(50);
			timecount++;
			// lcd_clr();
			// ltoa(timecount,temp,10);
			// lcd_puts(temp);
		}
		// lcd_clr();
		// ltoa(timecount*50/250+1,temp,10);
		// lcd_puts(temp);
		if(record_key == 0){
			note.note = record_key;
			note.octave = 1;
			note.duration = timecount*50/250 + 1;
			notes[notecount] = note;
			notecount++;
		}
		avr_wait(50);
	}
}


void play_recorded_music(const unsigned char index){
	if(music_recorded){
		play_song(music_list[index]);
	}
}

unsigned char music_list_full(){
	return music_recorded==3;
}

// void remove_recorded_music(const unsigned char index){
// 	if(music_recorded){
// 		music_list[index] = NULL;
// 	}
// }