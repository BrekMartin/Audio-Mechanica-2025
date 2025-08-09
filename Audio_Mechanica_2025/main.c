/*
                               Audio Mechanica
                              Effects Processor
                               Art 2006 - 2025
*/

#include <pspkernel.h>
#include <pspiofilemgr.h>
#include <pspaudiolib.h>
#include <pspaudio.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <stdlib.h>
#include <string.h>
#include <psphprm.h>
#include "main.h"
#include "psp_audio_ext.h"
#include <png.h>
#include <pspgu.h>
#include "graphics.h"
#include "math.h"
#include "Art.h"

PSP_MODULE_INFO("Audio Mechanica 2025", 0, 1, 1);

short bigbuffer[7340032];			// 14Mb buffer
short delay_buffer[1024];			//
int delay_index = 0;				//
int lfo_phase = 0;				//
int recordindex = 0;				// record index
int nux;					// delay variable
int operating = 0;				// operating status
int rate = 44100;				// sample rate
int indexbuf = 0;				//
int echodelay = 0;				// echo delay variable
int playindex = 0;				// play index for echo mode
int cxbuf;					//
int effect = 0;					// echo monitor status
int oldeffect = 0;				//
int det;					// delete RAM reel index
int seconds;					// number of seconds for display
int divisor = 44108;				// seconds divisor for sample rates
int minutes;					// number of minutes for display
int milisecs;					// number of miliseconds for echo display
int ixn = 0;					// delay counter
int bitcrusher = 1;				//
int shifter = 8;				// resolution reduction
int redcnt = 0;					// index for random table
int redadd = 0;					// add to resolution reduction value
int xover = 2000;				// crossover distortion value
int flaflop = 0;				//
int dflavalue = 13;				//
int flavalue = 140;				//
unsigned char lastscaled = 0;			//
int buttontime = 0;				//
int xx = 0;
int yy = 0;
int x = 0;
int y = 0;
int bitcnt = 0;
int tindex = 0;
unsigned int tx = 0;
unsigned int ty = 0;
int txp = 0;
int typ = 0;
int sinvalue = 0;
unsigned int tcnt = 0;
unsigned char tchar = 0;
int sinindex = 0;
int sinstart = 0;
unsigned char sindir = 0;
unsigned char sinstartdir = 0;
unsigned char sinamp = 10;
int scroller = 0;
char texts[88];
char textn[88];
int pxx = 0;
int pyy = 0;
u32 pixel;					//
u32 sample;					//
int imgbuf = 0;
//
sample_t* loop_buffer;
short mic_level;
Image *background;
Image *abackground;
Image *bbackground;
Image *tbackground;
Image * Art;
//
// half wave sine table precalculated
const unsigned short sin_table[181] = {
0,1,4,11,19,31,44,61,79,100,124,150,179,209,243,279,317,357,400,446,494,544,
596,651,708,767,829,892,958,1027,1097,1170,1244,1321,1400,1481,1564,1649,1736,1825,
1916,2009,2104,2200,2299,2399,2501,2605,2710,2817,2926,3036,3148,3261,3376,3493,3611,
3730,3850,3972,4096,4220,4346,4472,4600,4729,4860,4991,5123,5256,5390,5524,5660,5796,
5933,6071,6210,6349,6488,6628,6769,6910,7051,7193,7335,7478,7620,7763,7906,8049,8192,
8334,8477,8620,8763,8905,9048,9190,9332,9473,9614,9755,9895,10034,10173,10312,10450,
10587,10723,10859,10993,11127,11260,11392,11523,11654,11783,11911,12037,12163,12288,
12411,12533,12653,12772,12890,13007,13122,13235,13347,13457,13566,13673,13778,13882,
13984,14084,14183,14279,14374,14467,14558,14647,14734,14819,14902,14983,15062,15139,
15213,15286,15356,15425,15491,15554,15616,15675,15732,15787,15839,15889,15937,15983,
16026,16066,16104,16140,16174,16204,16233,16259,16283,16304,16322,16339,16352,16364,
16372,16379,16382,16384
};
//
// drip flanger lfo sine table
const int sine_table[226] = {
128, 131, 135, 138, 142, 145, 149, 152,
156, 159, 162, 166, 169, 173, 176, 179,
182, 186, 189, 192, 195, 198, 201, 204,
206, 209, 212, 214, 217, 220, 222, 224,
227, 229, 231, 233, 235, 237, 239, 240,
242, 243, 245, 246, 247, 249, 250, 251,
251, 252, 253, 254, 254, 254, 255, 255,
255, 255, 255, 255, 254, 254, 254, 253,
252, 251, 251, 250, 249, 247, 246, 245,
243, 242, 240, 239, 237, 235, 233, 231,
229, 227, 224, 222, 220, 217, 214, 212,
209, 206, 204, 201, 198, 195, 192, 189,
186, 182, 179, 176, 173, 169, 166, 162,
159, 156, 152, 149, 145, 142, 138, 135,
131, 128, 124, 120, 117, 113, 110, 106,
103, 99, 96, 93, 89, 86, 82, 79,
76, 73, 69, 66, 63, 60, 57, 54,
51, 49, 46, 43, 41, 38, 35, 33,
31, 28, 26, 24, 22, 20, 18, 16,
15, 13, 12, 10, 9, 8, 6, 5,
4, 4, 3, 2, 1, 1, 1, 0,
0, 0, 0, 0, 0, 1, 1, 1,
2, 3, 4, 4, 5, 6, 8, 9,
10, 12, 13, 15, 16, 18, 20, 22,
24, 26, 28, 31, 33, 35, 38, 41,
43, 46, 49, 51, 54, 57, 60, 63,
66, 69, 73, 76, 79, 82, 86, 89,
93, 96, 99, 103, 106, 110, 113, 117,
120, 124};
//
// sin table for flanger lfo
const int la_sine_table[226] = {
32, 33, 34, 35, 36, 36, 37, 38,
39, 40, 41, 42, 42, 43, 44, 45,
46, 47, 47, 48, 49, 50, 50, 51,
52, 52, 53, 54, 54, 55, 56, 56,
57, 57, 58, 58, 59, 59, 60, 60,
61, 61, 61, 62, 62, 62, 63, 63,
63, 63, 63, 64, 64, 64, 64, 64,
64, 64, 64, 64, 64, 64, 64, 63,
63, 63, 63, 63, 62, 62, 62, 61,
61, 61, 60, 60, 59, 59, 58, 58,
57, 57, 56, 56, 55, 54, 54, 53,
52, 52, 51, 50, 50, 49, 48, 47,
47, 46, 45, 44, 43, 42, 42, 41,
40, 39, 38, 37, 36, 36, 35, 34,
33, 32, 31, 30, 29, 28, 28, 27,
26, 25, 24, 23, 22, 22, 21, 20,
19, 18, 17, 17, 16, 15, 14, 14,
13, 12, 12, 11, 10, 10, 9, 8,
8, 7, 7, 6, 6, 5, 5, 4,
4, 3, 3, 3, 2, 2, 2, 1,
1, 1, 1, 1, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 1, 1, 1, 2, 2, 2,
3, 3, 3, 4, 4, 5, 5, 6,
6, 7, 7, 8, 8, 9, 10, 10,
11, 12, 12, 13, 14, 14, 15, 16,
17, 17, 18, 19, 20, 21, 22, 22,
23, 24, 25, 26, 27, 28, 28, 29,
30, 31};
//
// wave table for spring physics scrolling
const unsigned char spmessage[27] = {
1,1,2,4,6,9,12,16,22,30,39,48,57,66,74,82,
90,98,106,112,116,119,122,124,126,127,127
};
//
// bold variable width character map
const unsigned char charmapb[420] = {
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // space
0b00000000, 0b00000000, 0b00000110, 0b01011111, 0b01011111, 0b00000110, 0b00000000, // !
0b00000000, 0b00000011, 0b00000011, 0b00000000, 0b00000011, 0b00000011, 0b00000000, // "
0b00000000, 0b00110010, 0b01001001, 0b01111001, 0b01000001, 0b00111110, 0b00000000, // @
0b00100100, 0b00101110, 0b01101011, 0b01101011, 0b00111010, 0b00010010, 0b00000000, // $
0b01000110, 0b01100110, 0b00110000, 0b00011000, 0b00001100, 0b01100110, 0b01100010, // %
0b00110000, 0b01111010, 0b01001111, 0b01011101, 0b00110111, 0b01111010, 0b01001000, // &
0b00000000, 0b00000100, 0b00000111, 0b00000011, 0b00000000, 0b00000000, 0b00000000, // '
0b00000000, 0b00011100, 0b00111110, 0b01100011, 0b01000001, 0b00000000, 0b00000000, // (
0b00000000, 0b01000001, 0b01100011, 0b00111110, 0b00011100, 0b00000000, 0b00000000, // )
0b00010000, 0b01010100, 0b01111100, 0b00111000, 0b01111100, 0b01010100, 0b00010000, // *
0b00000000, 0b00010000, 0b00010000, 0b01111100, 0b00010000, 0b00010000, 0b00000000, // +
0b00000000, 0b01011000, 0b00111000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // ,
0b00000000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00000000, 0b00000000, // -
0b00000000, 0b01100000, 0b01100000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // .
0b00000000, 0b00000000, 0b00110000, 0b00011000, 0b00001100, 0b00000000, 0b00000000, // /
0b00111110, 0b01111111, 0b01000001, 0b01000001, 0b01111111, 0b00111110, 0b00000000, // 0
0b01000000, 0b01000010, 0b01111111, 0b01111111, 0b01000000, 0b01000000, 0b00000000, // 1
0b01100010, 0b01110011, 0b01011001, 0b01001001, 0b01101111, 0b01100110, 0b00000000, // 2
0b00100010, 0b01100011, 0b01001001, 0b01001001, 0b01111111, 0b00110110, 0b00000000, // 3
0b00011000, 0b00011100, 0b00010110, 0b01010011, 0b01111111, 0b01010000, 0b00000000, // 4
0b00100111, 0b01100101, 0b01000101, 0b01000101, 0b01111101, 0b00111001, 0b00000000, // 5
0b00111100, 0b01111110, 0b01001011, 0b01001001, 0b01111001, 0b00110000, 0b00000000, // 6
0b00000011, 0b00000011, 0b01110001, 0b01111001, 0b00001111, 0b00000111, 0b00000000, // 7
0b00110110, 0b01111111, 0b01001001, 0b01001001, 0b01111111, 0b00110110, 0b00000000, // 8
0b00000110, 0b01001111, 0b01001001, 0b01101001, 0b00111111, 0b00011110, 0b00000000, // 9
0b00000000, 0b00000000, 0b01100110, 0b01100110, 0b00000000, 0b00000000, 0b00000000, // :
0b00000000, 0b01000000, 0b01110011, 0b00110011, 0b00000000, 0b00000000, 0b00000000, // ;
0b00000000, 0b00001000, 0b00011100, 0b00110110, 0b01100011, 0b01000001, 0b00000000, // <
0b01000110, 0b01100110, 0b00110000, 0b00011000, 0b00001100, 0b01100110, 0b01100010, // %
0b00000000, 0b01000001, 0b01100011, 0b00110110, 0b00011100, 0b00001000, 0b00000000, // >
0b00000010, 0b00000011, 0b01010001, 0b01011001, 0b00001111, 0b00000110, 0b00000000, // ?
0b00000000, 0b00010100, 0b00010100, 0b00010100, 0b00010100, 0b00000000, 0b00000000, // =
0b01111100, 0b01111110, 0b00010011, 0b00010011, 0b01111110, 0b01111100, 0b00000000, // A
0b01000001, 0b01111111, 0b01111111, 0b01001001, 0b01001001, 0b01111111, 0b00110110, // B
0b00011100, 0b00111110, 0b01100011, 0b01000001, 0b01000001, 0b01100011, 0b00100010, // C
0b01000001, 0b01111111, 0b01111111, 0b01000001, 0b01100011, 0b00111110, 0b00011100, // D
0b01000001, 0b01111111, 0b01111111, 0b01001001, 0b01011101, 0b01000001, 0b01100011, // E
0b01000001, 0b01111111, 0b01111111, 0b01001001, 0b00011101, 0b00000001, 0b00000011, // F
0b00011100, 0b00111110, 0b01100011, 0b01000001, 0b01010001, 0b01110011, 0b01110010, // G
0b01111111, 0b01111111, 0b00001000, 0b00001000, 0b01111111, 0b01111111, 0b00000000, // H
0b00000000, 0b01000001, 0b01111111, 0b01111111, 0b01000001, 0b00000000, 0b00000000, // I
0b00110000, 0b01110000, 0b01000000, 0b01000001, 0b01111111, 0b00111111, 0b00000001, // J
0b01000001, 0b01111111, 0b01111111, 0b00001000, 0b00011100, 0b01110111, 0b01100011, // K
0b01000001, 0b01111111, 0b01111111, 0b01000001, 0b01000000, 0b01100000, 0b01110000, // L
0b01111111, 0b01111111, 0b00001110, 0b00011100, 0b00001110, 0b01111111, 0b01111111, // M
0b01111111, 0b01111111, 0b00000110, 0b00001100, 0b00011000, 0b01111111, 0b01111111, // N
0b00011100, 0b00111110, 0b01100011, 0b01000001, 0b01100011, 0b00111110, 0b00011100, // O
0b01000001, 0b01111111, 0b01111111, 0b01001001, 0b00001001, 0b00001111, 0b00000110, // P
0b00011110, 0b00111111, 0b00100001, 0b01110001, 0b01111111, 0b01011110, 0b00000000, // Q
0b01000001, 0b01111111, 0b01111111, 0b00001001, 0b00011001, 0b01111111, 0b01100110, // R
0b00100010, 0b01100111, 0b01001101, 0b01011001, 0b01110011, 0b00100010, 0b00000000, // S
0b00000011, 0b01000001, 0b01111111, 0b01111111, 0b01000001, 0b00000011, 0b00000000, // T
0b00111111, 0b01111111, 0b01000000, 0b01000000, 0b01111111, 0b00111111, 0b00000000, // U
0b00011111, 0b00111111, 0b01100000, 0b01100000, 0b00111111, 0b00011111, 0b00000000, // V
0b01111111, 0b01111111, 0b00110000, 0b00011000, 0b00110000, 0b01111111, 0b01111111, // W
0b01100011, 0b00110110, 0b00011100, 0b00011100, 0b00110110, 0b01100011, 0b00000000, // X
0b00000111, 0b01001111, 0b01111000, 0b01111000, 0b01001111, 0b00000111, 0b00000000, // Y
0b01000111, 0b01100011, 0b01110001, 0b01011001, 0b01001101, 0b01100111, 0b01110011, // Z
0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 // custom
};
//
// random variable sample rate for bit crusher
const unsigned char reduction[60] = {
10,	10,	7,	9,	10,
8,	7,	9,	9,	10,
10,	8,	11,	11,	9,
11,	9,	8,	7,	10,
11,	8,	8,	9,	9,
9,	7,	7,	11,	9,
11,	7,	8,	11,	8,
10,	11,	7,	11,	11,
8,	10,	10,	9,	10,
10,	7,	11,	11,	11,
7,	9,	8,	11,	9,
8,	10,	9,	9,	7
};
//
void jumptoa(sample_t* ubuf) { // delay monitor
	int i;
	for (i = 0; i < 1024; i++) {
	ubuf[i].l = bigbuffer[playindex];
	ubuf[i].r = bigbuffer[playindex];
	playindex++;
	}
}
//
void jumptob(sample_t* ubuf) { // echo monitor
	int i;
	for (i = 0; i < 1024; i++) {
	ubuf[i].l = bigbuffer[playindex];
	ubuf[i].r = loop_buffer[i].r;
	playindex++;
	}
}
//
void jumptoc(sample_t* ubuf) { // bit crusher
	int i;
	int framecount = 9;
	unsigned short heldsamplea = 0;
	unsigned short heldsampleb = 0;
	for (i = 0; i < 1024; i++) {
	framecount++;
	if (framecount > reduction[redcnt]+redadd) { // variable samplerate reduction
	framecount = 0; redcnt++;
	if (redcnt > 59) {redcnt = 0;}

	heldsamplea = (bigbuffer[playindex] >> shifter) << shifter; // resulution reduction
	heldsampleb = (loop_buffer[i].r >> shifter) << shifter;
	} // framecount

	ubuf[i].l = heldsamplea;
	ubuf[i].r = heldsampleb;
	playindex++;
	} // bitcrusher
}
//
void jumptod(sample_t* ubuf) { // crossover distortion
	int i;
	for (i = 0; i < 1024; i++) {
	if ((loop_buffer[i].r < (0+xover)) && (loop_buffer[i].r > (0-xover))) {
	ubuf[i].l = 0;
	ubuf[i].r = 0;
	} else {
	ubuf[i].l = bigbuffer[playindex];
	ubuf[i].r = loop_buffer[i].r;
	}
	playindex++;
	}
}
//
void jumptoe(sample_t* ubuf) { // drip flanger
	int i;
    	for (i = 0; i < 1024; i++) {
        unsigned int lfo_val = sine_table[lfo_phase];
	if (flaflop < dflavalue) {
	flaflop++;
	} else {
	flaflop = 0;
	lfo_phase++;
	if (lfo_phase > 225) {lfo_phase = 0;}
	} // flaflop
       	int read_index = delay_index - lfo_val;
	read_index = read_index - 10;
        if (read_index < 0) {read_index += 1000;}
	long bb = (delay_buffer[read_index] + loop_buffer[i].r) / 2;
	ubuf[i].l = (short)bb;
	ubuf[i].r = loop_buffer[i].l;
        delay_buffer[delay_index] = loop_buffer[i].l;
        delay_index++;
	if (delay_index > 999) {delay_index = 0;}
	playindex++; // is this really needed here?
	}
}
//
void jumptof(sample_t* ubuf) { // flanger
	int i;
    	for (i = 0; i < 1024; i++) {
        unsigned int lfo_val = la_sine_table[lfo_phase];
	if (flaflop < flavalue) {
	flaflop++;
	} else {
	flaflop = 0;
	lfo_phase++;
	if (lfo_phase > 225) {lfo_phase = 0;}
	} // flaflop
       	int read_index = delay_index - lfo_val;
	read_index = read_index - 10;
        if (read_index < 0) {read_index += 1000;}
	long bb = (delay_buffer[read_index] + loop_buffer[i].r) / 2;
	ubuf[i].l = (short)bb;
	ubuf[i].r = loop_buffer[i].l;
        delay_buffer[delay_index] = loop_buffer[i].l;
        delay_index++;
	if (delay_index > 999) {delay_index = 0;}
	playindex++; // is this really needed here?
	}
}
//
void jumptog(sample_t* ubuf) { // effect bypass mode
	int i;
	for (i = 0; i < 1024; i++) {
	ubuf[i].l = loop_buffer[i].l;
	ubuf[i].r = loop_buffer[i].r;
	}
}
//
// array of function pointers initialization
void (*jumpto[7]) (sample_t* ubuf) = {jumptoa,jumptob,jumptoc,jumptod,jumptoe,jumptof,jumptog};
//
void audioLoopStop() {
	operating = 0;
	pspAudioSetChannelCallback(0, NULL, NULL);
	pspAudioInputEnd();	
	if(loop_buffer != NULL) {
	free(loop_buffer);
	loop_buffer = NULL;
	}
}
//
int exit_callback(int arg1, int arg2, void *common) {
          audioLoopStop();
          sceKernelExitGame();
          return 0;
}
//
int CallbackThread(SceSize args, void *argp) {
          int cbid;
          cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
          sceKernelRegisterExitCallback(cbid);
          sceKernelSleepThreadCB();
          return 0;
}
//
int SetupCallbacks(void) {
          int thid = 0;
          thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
          if(thid >= 0) {sceKernelStartThread(thid, 0, 0);}
          return thid;
}
//
// draw variable width text
void drawtextvarwidth(void) {
	tcnt = 0;
	tindex = 0;
	tchar = 0;
	xx = 0;
	yy = 0;
	bitcnt = 0;
	while (texts[tchar] != 0) {
	tindex = (texts[tchar] - 0x20) * 7;
	tcnt = 0;
	if (charmapb[tindex] == 0) {
	tcnt++;
	if (charmapb[tindex + 1] == 0) {
	tcnt++;
	if (charmapb[tindex + 2] == 0 && texts[tchar] != 0x20) {tcnt++;}
	}
	} // charmapb
	while (tcnt < 7) {
	xx = tx;
	bitcnt = 7;
	while (bitcnt > 0) {
	bitcnt--;
	if ((charmapb[tindex + tcnt] & (0b00000001 << bitcnt)) != 0) {
	yy = ty + bitcnt;

	if (xx > -1 && xx < 480) {
	if (yy > -1 && yy < 272) {
	putPixelImage(0x00EFDDCA, xx, yy, tbackground);
	}}

	}
	} // bitcnt
	tx++; tcnt++;
	} // tcnt
	if (charmapb[tindex + 6] == 0) {
	tx--;
	if (charmapb[tindex + 5] == 0) {
	tx--;
	if (charmapb[tindex + 4] == 0) {
	tx--;
	}
	}
	} // charmapb
	tchar++; tx++;
	}
}
//
// draw sin variable width text
void drawtextvarwidthsin(void) {
	unsigned short sinvalue;
	tcnt = 0;
	tindex = 0;
	tchar = 0;
	xx = 0;
	yy = 0;
	bitcnt = 0;
	sinindex = sinstart; sindir = sinstartdir;
	if (sindir == 0) {
	sinindex = sinindex + 2;
	if (sinindex > 358) {sindir = 1;}
	} else {
	sinindex = sinindex - 2;
	if (sinindex < 1) {sindir = 0;}
	} // sindir
	sinstart = sinindex; sinstartdir = sindir;
	while (textn[tchar] != 0) {
	tindex = (textn[tchar] - 0x20) * 7;
	tcnt = 0;
	if (charmapb[tindex] == 0) {
	tcnt++;
	if (charmapb[tindex + 1] == 0) {
	tcnt++;
	if (charmapb[tindex + 2] == 0 && textn[tchar] != 0x20) {
	tcnt++;
	}
	}
	} // charmapb
	while (tcnt < 7) {
	if (sinindex > 179) {
	sinvalue = sin_table[359 - sinindex];
	} else {
	sinvalue = sin_table[sinindex];
	}
	if (sindir == 0) {
	sinindex = sinindex + 2;
	if (sinindex > 358) { sindir = 1; }
	} else {
	sinindex = sinindex - 2;
	if (sinindex < 1) { sindir = 0; }
	} // sindir
	xx = tx;
	sinvalue = sinvalue >> sinamp;
	if (sinindex > 350) {sinvalue = 0;}
	bitcnt = 7;
	while (bitcnt > 0) {
	bitcnt--;
	if ((charmapb[tindex + tcnt] & (0b00000001 << bitcnt)) != 0) {
	yy = ty + bitcnt;
	if (xx > -1 && xx < 480) {
	if (yy+sinvalue > -1 && yy+sinvalue < 272) {
	putPixelImage(0x00EFDDCA, xx, yy+sinvalue, background);
	}}
	}
	} // bitcnt
	tx++; tcnt++;
	} // tcnt
	if (charmapb[tindex + 6] == 0) {
	tx--;
	if (charmapb[tindex + 5] == 0) {
	tx--;
	if (charmapb[tindex + 4] == 0) {
	tx--;
	}
	}
	} // charmapb
	tchar++; tx++;
	}
}
//
// draw large variable width sine text to framebuffer
	void drawtextvarwidthsinlarge(void) {
	int tcnt = 0;
	int tindex = 0;
	int tchar = 0;
	int xx = 0;
	int yy = 0;
	unsigned char looptwice;
	bitcnt = 0;
	sinindex = sinstart; sindir = sinstartdir;
	if (sindir == 0) {
	sinindex = sinindex + 2;
	if (sinindex > 358) {sindir = 1;}
	} else {
	sinindex = sinindex - 2;
	if (sinindex < 1) {sindir = 0;}
	} // sindir
	sinstart = sinindex; sinstartdir = sindir;
	while (textn[tchar] != 0) {
	txp++;
	tindex = (textn[tchar] - 0x20) * 7;
	tcnt = 0;
	if (charmapb[tindex] == 0) {
	tcnt++;
	if (charmapb[tindex+1] == 0) {
	tcnt++;
	if (charmapb[tindex+2] == 0) {
	tcnt++;
	if (charmapb[tindex+3] == 0 && textn[tchar] != 0x20) {
	tcnt++;
	}
	}
	}
	} // charmapb 
	while (tcnt < 7) {
	if (sinindex > 179) {
	sinvalue = sin_table[359-sinindex];
	} else {
	sinvalue = sin_table[sinindex];
	}
	if (sindir == 0) {
	sinindex = sinindex + 2;
	if (sinindex > 358) {sindir = 1;}
	} else {
	sinindex = sinindex - 2;
	if (sinindex < 1) {sindir = 0;}
	} // sindir
	looptwice = 0;
	sinvalue = sinvalue >> sinamp;
	if (sinindex > 350) {sinvalue = 0;}
	while (looptwice < 2) {
	xx = txp;
	if (txp > 0) { // skip left of screen for performance
	int bitcnt = 16; // support scaled 8 pixel high characters
	while (bitcnt > 0) {
	bitcnt = bitcnt - 2;
	if ((charmapb[tindex+tcnt] & (0b00000001 << (bitcnt / 2))) != 0) {
	yy = typ + bitcnt;
	if (xx > -1 && xx < 480) {
	if (yy+sinvalue > -1 && yy+sinvalue < 271) {
	putPixelImage(0x00EFDDCA, xx, yy+sinvalue, background);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy+sinvalue, background);
	}}
	} // charmap
	} // bitcnt
	} // tx
	txp++; looptwice++;
	} // looptwice
	tcnt++;
	} // tcnt
	if (charmapb[tindex+6] == 0) {
	txp--;
	if (charmapb[tindex+5] == 0) {
	txp--;
	if (charmapb[tindex+4] == 0) {
	txp--;
	}
	}
	} // charmapb
	tchar++; txp++;
	}
}
//
// draw large variable width text to framebuffer
	void drawtextvarwidthlarge(void) {
	int tcnt = 0;
	int tindex = 0;
	int tchar = 0;
	int xx = 0;
	int yy = 0;
	unsigned char looptwice;
	txp++; // shift string one pixel right
	while (texts[tchar] != 0) {
	txp++;
	tindex = (texts[tchar] - 0x20) * 7;
	tcnt = 0;
	if (charmapb[tindex] == 0) {
	tcnt++;
	if (charmapb[tindex+1] == 0) {
	tcnt++;
	if (charmapb[tindex+2] == 0) {
	tcnt++;
	if (charmapb[tindex+3] == 0 && texts[tchar] != 0x20) {
	tcnt++;
	}
	}
	}
	} // charmapb 
	while (tcnt < 7) {
	looptwice = 0;
	while (looptwice < 2) {
	xx = txp;
	if (txp > 0) { // skip left of screen for performance
	int bitcnt = 16; // support scaled 8 pixel high characters
	while (bitcnt > 0) {
	bitcnt = bitcnt - 2;
	if ((charmapb[tindex+tcnt] & (0b00000001 << (bitcnt / 2))) != 0) {
	yy = typ + bitcnt;
	if (xx > -1 && xx < 480) {
	if (yy > -1 && yy < 271) {
	putPixelImage(0x00EFDDCA, xx, yy, background);
	yy++;
	putPixelImage(0x00EFDDCA, xx, yy, background);
	}}
	} // charmap
	} // bitcnt
	} // tx
	txp++; looptwice++;
	} // looptwice
	tcnt++;
	} // tcnt
	if (charmapb[tindex+6] == 0) {
	txp--;
	if (charmapb[tindex+5] == 0) {
	txp--;
	if (charmapb[tindex+4] == 0) {
	txp--;
	}
	}
	} // charmapb
	tchar++; txp++;
	}
}
//
// Bresenham's line drawing algorithm
void adrawline (int bx1,int by1,int bx2,int by2) {
	int bx,by,bdx,bdy,bdx1,bdy1,bpx,bpy,bxe,bye,bi;
	bdx=bx2-bx1; bdy=by2-by1;
	bdx1=fabs(bdx); bdy1=fabs(bdy);
	bpx=2*bdy1-bdx1; bpy=2*bdx1-bdy1;
	if(bdy1<=bdx1) {
	if(bdx>=0) {bx=bx1; by=by1; bxe=bx2;} else {bx=bx2; by=by2; bxe=bx1;}
	putPixelImage(0xFFF22C9A, bx, by, abackground);
	putPixelImage(0xFFF22C9A, bx, by+1, abackground);
	for(bi=0;bx<bxe;bi++) {
	bx=bx+1;
	if(bpx<0) {
	bpx=bpx+2*bdy1;
	} else {
	if((bdx<0 && bdy<0) || (bdx>0 && bdy>0)) {by++;} else {by--;}
	bpx=bpx+2*(bdy1-bdx1);
	}
	putPixelImage(0xFFF22C9A, bx, by, abackground);
	putPixelImage(0xFFF22C9A, bx, by+1, abackground);
	}
	} else {
	if(bdy>=0) {bx=bx1; by=by1; bye=by2;} else {bx=bx2; by=by2; bye=by1;}
	putPixelImage(0xFFF22C9A, bx, by, abackground);
	putPixelImage(0xFFF22C9A, bx, by+1, abackground);
	for(bi=0;by<bye;bi++) {
	by=by+1;
	if(bpy<=0) {
	bpy=bpy+2*bdx1;
	} else {
	if((bdx<0 && bdy<0) || (bdx>0 && bdy>0)) {bx++;} else {bx--;}
	bpy=bpy+2*(bdx1-bdy1);
	}
	putPixelImage(0xFFF22C9A, bx, by, abackground);
	putPixelImage(0xFFF22C9A, bx, by+1, abackground);
	}}
}
//
// Bresenham's line drawing algorithm
void bdrawline (int bx1,int by1,int bx2,int by2) {
	int bx,by,bdx,bdy,bdx1,bdy1,bpx,bpy,bxe,bye,bi;
	bdx=bx2-bx1; bdy=by2-by1;
	bdx1=fabs(bdx); bdy1=fabs(bdy);
	bpx=2*bdy1-bdx1; bpy=2*bdx1-bdy1;
	if(bdy1<=bdx1) {
	if(bdx>=0) {bx=bx1; by=by1; bxe=bx2;} else {bx=bx2; by=by2; bxe=bx1;}
	putPixelImage(0xFFF22C9A, bx, by, bbackground);
	putPixelImage(0xFFF22C9A, bx, by+1, bbackground);
	for(bi=0;bx<bxe;bi++) {
	bx=bx+1;
	if(bpx<0) {
	bpx=bpx+2*bdy1;
	} else {
	if((bdx<0 && bdy<0) || (bdx>0 && bdy>0)) {by++;} else {by--;}
	bpx=bpx+2*(bdy1-bdx1);
	}
	putPixelImage(0xFFF22C9A, bx, by, bbackground);
	putPixelImage(0xFFF22C9A, bx, by+1, bbackground);
	}
	} else {
	if(bdy>=0) {bx=bx1; by=by1; bye=by2;} else {bx=bx2; by=by2; bye=by1;}
	putPixelImage(0xFFF22C9A, bx, by, bbackground);
	putPixelImage(0xFFF22C9A, bx, by+1, bbackground);
	for(bi=0;by<bye;bi++) {
	by=by+1;
	if(bpy<=0) {
	bpy=bpy+2*bdx1;
	} else {
	if((bdx<0 && bdy<0) || (bdx>0 && bdy>0)) {bx++;} else {bx--;}
	bpy=bpy+2*(bdx1-bdy1);
	}
	putPixelImage(0xFFF22C9A, bx, by, bbackground);
	putPixelImage(0xFFF22C9A, bx, by+1, bbackground);
	}}
}
//
void print_times() {
	milisecs = echodelay / 441;
	seconds = milisecs / 100;
	milisecs = milisecs % 100;
	if (effect == 0) {
	sprintf(texts,"DELAY TIME: %02d.%02d",seconds,milisecs);
	}
	if (effect == 1) {
	sprintf(texts,"ECHO TIME: %02d.%02d",seconds,milisecs);
	}
	if (effect == 2) {
	sprintf(texts,"REDUCTION: %d RESOLUTION: %d ECHO TIME: %02d.%02d",redadd,shifter,seconds,milisecs);
	}
	if (effect == 3) {
	sprintf(texts,"XOVER DISTORTION: %d ECHO TIME: %02d.%02d",xover,seconds,milisecs);
	}
	if (effect == 4) {
	sprintf(texts,"DRIP FLANGER VALUE: %d",dflavalue);
	}
	if (effect == 5) {
	sprintf(texts,"FLANGER VALUE: %d",flavalue);
	}
	if (effect == 6) {
	sprintf(texts,"BYPASS MODE");
	}
}
//
// called by pspaudiolib when the audio output buffer needs to be filled
void audioOutputLoopCallback(void* buf, unsigned int length, void *userdata) {
	sample_t* ubuf = (sample_t*) buf;
	int i;

	if (recordindex > echodelay - 1) {
	playindex = recordindex - echodelay;
	} else {
	cxbuf = echodelay - recordindex;
	playindex = 7340032 - cxbuf;
	}

	jumpto[effect]((sample_t*) ubuf); // jump to effect function


	lastscaled = 128;
	if (imgbuf != 0) { // new wave visualiser for 2025
	for (i = 8; i < 470; i = i + 2) {
    	long sample = (ubuf[i].r + 32768) >> 9; // 8
    	//unsigned char scaled = sample;
    	unsigned char scaled = sample + 64; // quick fix for crashing impacts visualiser
	adrawline (i-2,lastscaled,i,scaled);
	lastscaled = scaled;
	} // i
	i = i + 2;
	adrawline (i-2,lastscaled,i,128);
	} else { // imgbuf
	for (i = 8; i < 470; i = i + 2) {
    	long sample = (ubuf[i].r + 32768) >> 9; // 8
	//unsigned char scaled = sample;
    	unsigned char scaled = sample + 64;
	bdrawline (i-2,lastscaled,i,scaled);
	lastscaled = scaled;
	} // i
	i = i + 2;
	bdrawline (i-2,lastscaled,i,128);
	} // imgbuf


}
//
// This function gets called by pspaudiolib every time the audio input buffer needs to be drained.
void audioInputLoopCallback(void* buf, unsigned int length, void *userdata) {
	short* ubuf = (short*) buf;
	int i;
	for (i = 0; i < 1024; i++) {
	loop_buffer[i].l = ubuf[i];
	loop_buffer[i].r = ubuf[i];
	bigbuffer[recordindex] = ubuf[i];
	recordindex++;
	}
	if (recordindex > 7340031) {recordindex = 0;} // if 14Mb or greater
}
//
void audioLoopStart() {
	operating = 1;
	loop_buffer = (sample_t*) malloc(PSP_NUM_AUDIO_SAMPLES * sizeof(sample_t));
	pspAudioInputInit(mic_level, 44100);
	sceKernelDelayThread(200000);	
	pspAudioSetInputCallback(audioInputLoopCallback, NULL);
	pspAudioSetChannelCallback(0, audioOutputLoopCallback, NULL);
}
//
//
//
int main(int argc, char* argv[]) {
	SetupCallbacks();
	initGraphics();
	background = createImage(480,272);
	tbackground = createImage(480,16);
	abackground = createImage(480,256);
	bbackground = createImage(480,256);

	clearImage(0,background);
	clearImage(0,bbackground);
	clearImage(0,abackground);
	clearImage(0,tbackground); // black

	sprintf (texts, "BREK MARTIN 2006 - 2025");
	sprintf (textn, "AUDIO MECHANICA");

	Artpic[5] = 0x64;
	Artpic[25] = 0x6D;
	Artpic[37] = 0x1D;
	Artpic[12] = 0x27;
	Artpic[7] = 0x64;
	Artpic[26] = 0x6E;
	Artpic[0] = 0xE7;
	Artpic[6] = 0x74;
	Artpic[1] = 0x3E;
	int xor;
	for(xor=0; xor<316; xor++) {
	Artpic[xor] = Artpic[xor] ^ 0x6E;	// simple xor decrypt Art logo graphic
	} // xor

	Art = loadImageMemory(Artpic, 316);

	pyy = 1;
	pxx = 1;
	pixel = getPixelImage(pxx, pyy, Art);
	sample = pixel;
	for (pyy=0; pyy<57; pyy++) { // image height
	for (pxx=0; pxx<100; pxx++) { // image width
	pixel = getPixelImage(pxx, pyy, Art);
	if (pixel == sample) {
	putPixelImage(0x00000000, pxx, pyy, Art);
	} else {
	putPixelImage(0xFFEFDDCA, pxx, pyy, Art);
	}
	} // width
	} // height

	scroller = 26; // reset scroll position
	for(nux=0; nux<88; nux++) { // scroll in and wait
	tx = 126-(spmessage[scroller]+spmessage[scroller]+spmessage[scroller]); ty = 70;
	txp = tx; typ = ty;
	drawtextvarwidthsinlarge();
	tx = 81+(spmessage[scroller]+spmessage[scroller]+spmessage[scroller]); ty = 160;
	txp = tx; typ = ty;
	drawtextvarwidthlarge();
	if (scroller > 0) {
	scroller--;
	} else {

	pyy = 1; pxx = 1;
	pixel = getPixelImage(pxx, pyy, Art);
	sample = pixel;
	for (pyy=0; pyy<57; pyy++) { // image height
	for (pxx=0; pxx<100; pxx++) { // image width
	pixel = getPixelImage(pxx, pyy, Art);
	if (pixel != sample) {
	putPixelImage(0xFF090907, pxx+378, pyy+220, background);
	}
	} // width
	} // height
	} // scroller

	if(sceHprmIsHeadphoneExist() == 0 && nux > 86) { // check headphones plugged in
	sprintf(texts,"WAITING FOR HEADPHONES");
	tx = 166; ty = 5; drawtextvarwidth();
	sprintf (texts, "BREK MARTIN 2006 - 2025");
	nux--;
	} else {
	if(sceHprmIsMicrophoneExist() == 0 && nux > 86) { // check microphone plugged in
	sprintf(texts,"WAITING FOR MICROPHONE");
	tx = 166; ty = 5; drawtextvarwidth();
	sprintf (texts, "BREK MARTIN 2006 - 2025");
	nux--;
	} // microphone
	} // headphone

	blitImageToScreen(0, 0, 480, 272, background, 0, 0);
	blitImageToScreen(0, 0, 480, 16, tbackground, 0, 206); // could contain headphone message
	sceDisplayWaitVblankStart();
	flipScreen();
	clearImage(0,background);
	} // nux

	for(nux=0; nux<26; nux++) { // scroll out and wait
	tx = 126+(spmessage[scroller]+spmessage[scroller]+spmessage[scroller]); ty = 70;
	txp = tx; typ = ty;
	drawtextvarwidthsinlarge();
	tx = 81-(spmessage[scroller]+spmessage[scroller]+spmessage[scroller]); ty = 160;
	txp = tx; typ = ty;
	drawtextvarwidthlarge();
	if (scroller < 26) {scroller++;}
	blitImageToScreen(0, 0, 480, 272, background, 0, 0);
	sceDisplayWaitVblankStart();
	flipScreen();
	clearImage(0,background);
	} // nux
	blitImageToScreen(0, 0, 480, 272, background, 0, 0);
	sceDisplayWaitVblankStart();
	flipScreen();
	clearImage(0,background);
	clearImage(0x114410,tbackground);

	SceCtrlData pad;
	mic_level = 20480;
	loop_buffer = NULL;
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	recordindex = 0; // reset reel position
	rate = 44100;
	divisor = 44108;
	echodelay = 9216;

	pspAudioInit();
	audioLoopStart();
	print_times();


for(;;) {

if (buttontime > 0) {buttontime--;} // decrement button timer

sceCtrlReadBufferPositive(&pad, 1);
if (operating == 1 && buttontime == 0) {

if(pad.Buttons & PSP_CTRL_RIGHT) {
echodelay = echodelay + 10240;
if (echodelay > 7340032) {echodelay = 7340032;}
print_times();
}

if(pad.Buttons & PSP_CTRL_LEFT) {
echodelay = echodelay - 10240;
if (echodelay < 1) {echodelay = 1024;}
print_times();
}

if(pad.Buttons & PSP_CTRL_UP) {
echodelay = echodelay + 512;
if (echodelay > 7340032) {echodelay = 7340032;}
if (echodelay < 1024) {echodelay = 1024;}
print_times();
}

if(pad.Buttons & PSP_CTRL_DOWN) {
echodelay = echodelay - 512;
if (echodelay < 1024) {echodelay = 1024;}
if (echodelay > 7340032) {echodelay = 7340032;}
print_times();
}

if(pad.Buttons & PSP_CTRL_RTRIGGER) {
effect++; echodelay = 9216;
if (effect > 5) {effect = 0;}
print_times();
buttontime = 14;
}

if(pad.Buttons & PSP_CTRL_LTRIGGER) {
effect--; echodelay = 9216;
if (effect < 0) {effect = 5;}
print_times();
buttontime = 14;
}

if(pad.Buttons & PSP_CTRL_SQUARE) {
if (effect == 2) { // bit crusher
if (shifter > 0) {shifter--;}
print_times();
buttontime = 6;
} // effect
if (effect == 3) { // crossover distortion
if (xover > 0) {xover = xover - 100;}
print_times();
buttontime = 6;
} // effect
if (effect == 4) { // drip flanger
if (dflavalue > 1) {dflavalue = dflavalue - 1;}
print_times();
buttontime = 6;
} // effect
if (effect == 5) { // flanger
if (flavalue > 1) {flavalue = flavalue - 1;}
print_times();
buttontime = 6;
} // effect
}

if(pad.Buttons & PSP_CTRL_CIRCLE) {
if (effect == 2) { // bit crusher
if (shifter < 12) {shifter++;}
print_times();
buttontime = 6;
} // effect
if (effect == 3) { // crossover distortion
if (xover < 5000) {xover = xover + 100;}
print_times();
buttontime = 6;
} // effect
if (effect == 4) { // drip flanger
if (dflavalue < 250) {dflavalue = dflavalue + 1;}
print_times();
buttontime = 6;
} // effect
if (effect == 5) { // flanger
if (flavalue < 250) {flavalue = flavalue + 1;}
print_times();
buttontime = 6;
} // effect
}

if(pad.Buttons & PSP_CTRL_TRIANGLE) {
if (effect == 2) { // bit crusher
if (redadd < 4) {redadd++;}
print_times();
buttontime = 6;
} // effect
}

if(pad.Buttons & PSP_CTRL_CROSS) {
if (effect == 2) { // bit crusher
if (redadd > 0) {redadd--;}
print_times();
buttontime = 6;
} // effect
}

if(pad.Buttons & PSP_CTRL_SELECT) {
if (effect == 6) {effect = oldeffect;} else {oldeffect = effect; effect = 6;}
print_times();
buttontime = 14;
}

u32 keya;
if(sceHprmIsRemoteExist() == 1) {
sceHprmPeekCurrentKey(&keya);
if(keya == PSP_HPRM_PLAYPAUSE){
if (effect == 6) {effect = oldeffect;} else {oldeffect = effect; effect = 6;}
print_times();
buttontime = 14;
} // playpause
} // remoteexist

} // operating


tx = 10; ty = 5;
drawtextvarwidth();
if (imgbuf == 0) {
blitImageToScreen(0, 0, 480, 256, abackground, 0, 16);
blitImageToScreen(0, 0, 480, 16, tbackground, 0, 0);
sceDisplayWaitVblankStart();
flipScreen();
clearImage(0x000000,abackground);	
imgbuf = 1;
} else {
blitImageToScreen(0, 0, 480, 256, bbackground, 0, 16);
blitImageToScreen(0, 0, 480, 16, tbackground, 0, 0);
sceDisplayWaitVblankStart();
flipScreen();
clearImage(0x000000,bbackground);	
imgbuf = 0;
} // imgbuf
clearImage(0x114410,tbackground);

}

}




