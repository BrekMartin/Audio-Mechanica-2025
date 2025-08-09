#ifndef MAIN_H_
#define MAIN_H_

// button press commands:

#define UNKNOWN_COMMAND			-2
#define NO_COMMAND				-1
#define TOGGLE_SOUND_LOOP 		0
#define EXIT_PROG				1
#define TEST_AUDIO_INPUT_INIT	2

#define RECORD_DURATION		60
#define RECORD_SAMPLE_RATE	44100


// Structure containing a sigle stereo sound sample:

typedef struct {
        short l, r;
} sample_t;

int exitCallback(int arg1, int arg2, void *common);
int callbackThread(SceSize args, void *argp);
int setupCallbacks(void);
void audioOutputLoopCallback(void* buf, unsigned int length, void *userdata);
void audioInputLoopCallback(void* buf, unsigned int length, void *userdata);

void audioLoopStart();
void audioLoopStop();

void printMenu();

#endif /*MAIN_H_*/
