/**
 * 		PSP Audio Input Library v0.35
 * 
 * 		Copyright (c) 2006 Luis Teixeira <teixeluis@gmail.com>
 * 
 * 		Last Modified 28/08/2006
 * 	
 *		Defines functions for simplifying audio input usage within applications.
 * 
 */

#ifndef PSP_AUDIO_EXT_H_
#define PSP_AUDIO_EXT_H_


/*
 * Additional functionality to what is present in pspaudiolib
 * from pspsdk. Audio input handling routines added.
 * 
 */

#include <psptypes.h>
#include <pspaudio.h>
#include <pspaudiolib.h>

#define NUM_FRAMES	4
#define FRAME_SIZE  PSP_NUM_AUDIO_SAMPLES
#define BUF_SIZE 	NUM_FRAMES * FRAME_SIZE


extern s32 sceAudioInputInit(s32 arg0, s32 vol, s32 arg2); 					// NID = 0x7DE61688
extern s32 sceAudioInput(s32 length, s32 sample_rate, void *buf);				// NID = 0x6D4BEC68
extern s32 sceAudioInputBlocking(s32 length, s32 sample_rate, void *buf);		// NID = 0x086E5895
extern s32 sceAudioGetInputLength();											// NID = 0xA708C6A6
extern s32 sceAudioWaitInputEnd();												// NID = 0x87B2E651

typedef struct {
  int cb_th_handle; 		// callback thread handle.
  int audio_in_th_handle;	// audio input thread handle.
  int handle;				// audio input handle.
  int volume;
  int sample_rate;
  pspAudioCallback_t callback;
  void *pdata;
} psp_audio_input_info;

typedef struct  {
	short buffer[BUF_SIZE]; 		// the buffer pointer.
	int read_index; 				// the index incremented on each read.
	int write_index;				// the index incremented on each write.
	int used_frames;   				// the ammount of buffer space used.
} audio_input_buffer_t;

// audio input prototypes:

int pspAudioInputInit(int volume, int sample_rate);
int pspAudioInputSoftInit(int volume);
void pspAudioInputSoftEnd();
void pspAudioInputEnd();


void pspAudioSetInputVolume(int vol);
void pspAudioInputThreadCallback(void *buf, unsigned int reqn);
void pspAudioSetInputCallback(pspAudioCallback_t callback, void *pdata);
int pspAudioInputBlocking(s32 length, s32 sample_rate, void *buf);
int pspAudioInput(s32 length, s32 sample_rate, void *buf);


#endif /*PSP_AUDIO_EXT_H_*/
