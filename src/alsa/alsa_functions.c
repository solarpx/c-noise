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

#include <alsa/asoundlib.h>
#include <math.h>
#include "noise_generators.h"
#include "alsa_functions.h"

#define BUFFER_LEN 44100
ALSA_params* initALSA(void){

  ALSA_params* P = malloc(sizeof(ALSA_params));
  P->rate = BUFFER_LEN;
  P->stream = SND_PCM_STREAM_PLAYBACK;
  P->pcm_name = strdup("plughw:0,0");
  P->periodsize = BUFFER_LEN;
  P->periods = 1;
  P->frames = (P->periodsize) >> 2; 

  snd_pcm_hw_params_t *hwparams;

  // Try to initialize soundcard
  snd_pcm_hw_params_alloca(&hwparams);
  if (snd_pcm_open(&P->pcm_handle, P->pcm_name, P->stream, 0) < 0) {
    fprintf(stderr, "Error opening PCM device %s\n", P->pcm_name);
  }

  int exact_rate = P->rate;
  // All of the following functions "fill up" hwparams
  snd_pcm_hw_params_any(P->pcm_handle, hwparams);
  snd_pcm_hw_params_set_access(P->pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) ;
  snd_pcm_hw_params_set_format(P->pcm_handle, hwparams, SND_PCM_FORMAT_FLOAT);
  snd_pcm_hw_params_set_rate_near(P->pcm_handle, hwparams, &exact_rate, 0);
  snd_pcm_hw_params_set_channels(P->pcm_handle, hwparams, 1);
  snd_pcm_hw_params_set_periods(P->pcm_handle, hwparams, P->periods, 0);
  snd_pcm_hw_params_set_buffer_size(P->pcm_handle, hwparams, (P->periodsize*P->periods)>>2); 

  // Finally send the final hwparams to the sound card
  snd_pcm_hw_params(P->pcm_handle, hwparams);
  return P;
}

void volumeALSA(int inc_volume)
{
  long min, max, value;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "Master";

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    snd_mixer_selem_get_playback_volume (elem, 0, &value);

    if(inc_volume == 1){
      snd_mixer_selem_set_playback_volume_all(elem, value + 1.0);
    }
    if(inc_volume == 0){
      snd_mixer_selem_set_playback_volume_all(elem, value - 1.0);
    }
    snd_mixer_close(handle);
}

int playBrownNoise(ALSA_params* P, int SEC){  
  int i,j;
  float buffer [BUFFER_LEN];
  snd_pcm_sframes_t frames;

  for (i = 0; i < SEC * 44100 / BUFFER_LEN; i++) {
    for (j = 0; j < BUFFER_LEN; j++)
      buffer[j] = 0.0015*genBrownNoiseCorr();
    frames = snd_pcm_writei(P->pcm_handle, buffer, BUFFER_LEN);
  }
}

int playPinkNoise(ALSA_params* P, float* PN, float* A, int depth, int SEC){  
  int i,j;
  float buffer [BUFFER_LEN];
  snd_pcm_sframes_t frames;
  
  for (i = 0; i < SEC * 44100 / BUFFER_LEN; i++) {
    for (j = 0; j < BUFFER_LEN; j++)
      buffer[j] = 0.005*genPinkNoise(PN, A, depth);
    frames = snd_pcm_writei(P->pcm_handle, buffer, BUFFER_LEN);
  }
}

int playWhiteNoise(ALSA_params* P, int SEC){  
  int i,j;
  float buffer [BUFFER_LEN];
  snd_pcm_sframes_t frames;

  for (i = 0; i < SEC * 44100 / BUFFER_LEN; i++) {
    for (j = 0; j < BUFFER_LEN; j++)
       buffer[j] = 0.01*genWhiteNoise();
    frames = snd_pcm_writei(P->pcm_handle, buffer, BUFFER_LEN);
  }
}

int playBlueNoise(ALSA_params* P, float* BN, float* A, int depth, int SEC){  
  int i,j;
  float buffer [BUFFER_LEN];
  snd_pcm_sframes_t frames;

  for (i = 0; i < SEC * 44100 / BUFFER_LEN; i++) {
    for (j = 0; j < BUFFER_LEN; j++)
	buffer[j] = 0.02*genBlueNoise(BN, A, depth);
    frames = snd_pcm_writei(P->pcm_handle, buffer, BUFFER_LEN);
  }
}

int playVioletNoise(ALSA_params* P, int SEC){  
  int i,j;
  float buffer [BUFFER_LEN];
  snd_pcm_sframes_t frames;

  for (i = 0; i < SEC * 44100 / BUFFER_LEN; i++) {
      for (j = 0; j < BUFFER_LEN; j++)
	buffer[j] = 0.02*genVioletNoise();
      frames = snd_pcm_writei(P->pcm_handle, buffer, BUFFER_LEN);
  }
}
