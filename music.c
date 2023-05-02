#include "open_interface.h"
#include "music.h"

/// Load three songs onto the iRobot
/**
 * Before calling this method, be sure to initialize the open interface by calling:
 *
 *   oi_t* sensor = oi_alloc();
 *   oi_init(sensor); 
 *
 */
void load_songs() {

	// Notes: oi_load_song takes four arguments
	// arg1 - an integer from 0 to 16 identifying this song
	// arg2 - an integer that indicates the number of notes in the song (if greater than 16, it will consume the next song index's storage space)
	// arg3 - an array of integers representing the midi note values (example: 60 = C, 61 = C sharp)
	// arg4 - an array of integers representing the duration of each note (in 1/64ths of a second)
	// 
	// For a note sheet, see page 12 of the iRobot Creat Open Interface datasheet

	unsigned char WellerManNumNotes = 16;
	unsigned char WellerManNotes[16]    = {60, 60, 55, 55, 58, 55, 55, 55, 56, 53, 53, 55, 56, 60, 55, 55};
	unsigned char WellerManDuration[16] = {64, 48, 16, 16, 16, 32, 48, 16, 32, 32, 16, 16, 32, 32, 32, 64};
	oi_loadSong(1, WellerManNumNote, WellerManNotes, WellerManDuration);

void play_songs(int num) {
	oi_play_song(num);
}
