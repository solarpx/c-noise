/* C-noise: The open source noise generator  */ 
/* Copyright (C) 2015  Michael Winters  */
/*
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or  */
/* (at your option) any later version.  */
/*
/* This program is distributed in the hope that it will be useful,  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of  */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  */
/* GNU General Public License for more details.  */
/*
/* You should have received a copy of the GNU General Public License  */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */


// ALSA data structure
typedef struct{
  int rate;
  int frames;
  int periods;
  
  char* pcm_name;
  snd_pcm_t *pcm_handle;
  snd_pcm_stream_t stream;
  snd_pcm_uframes_t periodsize;
  
}ALSA_params;

// ALSA Init function
ALSA_params* initALSA(void);
void volumeALSA(int inc_volume);

// Major/Minor Chords for Testing 
int majorChord(ALSA_params* P, int FREQ, int SEC);
int minorChord(ALSA_params* P, int FREQ, int SEC);

// Noise playing functions ... invokes noise spigot algorithms
int playWhiteNoise(ALSA_params* P, int SEC);
int playBrownNoise(ALSA_params* P, int SEC);
int playVioletNoise(ALSA_params* P, int SEC);
int playPinkNoise(ALSA_params* P, float* PN, float* A, int depth, int SEC);
int playBlueNoise(ALSA_params* P, float* BN, float* A, int depth, int SEC);
