#ifndef MUSIC_H_
#define MUSIC_H_

typedef enum {
	P, C, Cs, D, Ds, E, F, Fs, G, Gs, A, As, B
} Note;

typedef struct {
	Note note;
	unsigned char octave;
	unsigned char duration;
} PlayingNote;

typedef struct{
	unsigned char default_tempo;
	unsigned int length;
	PlayingNote* note_seq;
} music;

void play_note(const PlayingNote* note);
void play_song(const music m);
void record_song();
void play_recorded_music(const unsigned char index);
unsigned char music_list_full();
void init_song_list();

music twinkle;
music hoshi_no_arika;
music thank_you;

char song_names[3][15];
music song_list[3];

unsigned char music_recorded;

#endif /* MUSIC_H_ */