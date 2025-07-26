# Audio-Mechanica-2025
Live audio effects (Flanger, Xover Distortion, Bit Crusher)


             AUDIO MECHANICA 2025 (BIT CRUSHER EDITION)


I'm back! Fancy an update to a software project that I abandoned in 2008!

To be honest, this isn't so much an update, but a different slant on the
project, since this version doesn't even record audio to memory stick,
but rather, explores the possibility of producing live effects for guitar!
It's little more than an interesting demo however, due to the lag between
filling the audio input buffer, and populating the output buffer, there's
enough delay to put anyone off using it for anything rhythmic in real time
Maybe it could serve as a live wet delay channel in a multi amp rig.

If any PSP homebrew people out there have any idea how to lower the sample
chunk sizes from 1024 down to 512, or 256, please let me know. This might
be enough to use a PSP live without too much noticeable lag. It looks like
the key to this would be the ability to change PSP_NUM_AUDIO_SAMPLES, which
is defined in pspaudiolib.h I don't know if it's is possible, or how to do
it, but there is at least another effect I'd like to write (phaser)  but I
consider the above a bit of a brick wall to produce anything practical.

This version has been redone beginning from a 2006 version of the program.
It has been made as efficient as possible to leave as much processing time
as possible for potential effects.

The first, easiest, and most obvious effect, after the delay and echo that
were already present in the original software versions, is "bit crushing".
The bit crusher reduces the resolution of the 16 bit samples as they are
passing through the buffer, resulting in a more stepped waveform, and more
harsh sound. The software also performs non-constant downsampling to avoid
a lot of the humming associated with this effect.

Settings for sample rate reduction and resolution reduction are available,
but when the bit crusher mode is active, there is always a little of both,
and the sample rate reduction is always non-constant.

The next new effect is crossover distortion. Unlike clipping distortion,
which clips the tops and bottoms of the audio wave, crossover distortion
flattens the area around the zero point of the wave, simulating a pair of
back to back diodes in series with the signal path. This software version
is more flexible then hardware diodes however, because the exact level of
crossover distortion is adjustable from none at all, to a very high value.

Next is a flanger, written with help from ChatGPT, and results that could
be expected. Let's just say it's "something", but not a typical flanger.
I called it a "Drip Flanger" because a low setting sounds like dripping.
Like a flanger, it makes a copy the input waveform to a circular buffer.
An LFO created with a sin table varies an index that addresses the delay
buffer, and mixes samples from the delay buffer with the current audio.
This is how a flanger works, but again, the result here isn't the same.
It's more like a constant vibrato.

Looks like I've figured out a typical flanger, so I retained the previous
effort, and named the next effect "flanger".



20/07/25:
First acceptable bit crusher demo with user setting via button controls,
and a user adjustable crossover distortion implementation.

21/07/25:
A strange kind of flanger implementation with a user adjustable setting.

22/07/25:
Double buffered wave display results in less flicker of audio waveform.

23/07/25:
Figured out a real flanger that actually sounds like a typical flanger.
Did proper button debounce with counters instead of blocking delays.

25/07/25:
Used a jump table to seperate effects as distinguishable functions.

