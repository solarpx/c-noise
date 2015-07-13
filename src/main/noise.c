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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include <signal.h>
#include <alsa/asoundlib.h>
#include "noise_generators.h"
#include "alsa_functions.h"
#include "nonblocking.h"

typedef struct{
  int cx;
  char* cxs;
  int depth;
  double alpha;
  int ts;

  int alsa;
  int npoints;
  char* filename;
}args;

args* parseArgs (int argc, char **argv){


  args* a = malloc(sizeof(args)); 
  a->cx = -1;
  a->cxs = "\n";
  a->depth = -1;
  a->alpha = -1;
  a->ts = -1;

  a->alsa = -1;
  a->npoints = 1000;
  a->filename = "./noise.dat"; 
  
  int idx = 0;

  if (argc == 1){
    printf("\n");
    printf("\n");

    printf("Noise Colors: Choose brown, pink, white, azure or violet\n");
    printf("Dynamic Volume Control: ---(9)   (0)+++\n");
    printf("---------------------------------------------------------\n");
    printf("noise -c <color>\n");
    printf("---------------------------------------------------------\n");
    printf("Optional Arguments:\n");
    printf("\t -t <time>  (default = 60s) \n");
    printf("\t -a <alpha> (option for pink/azure noise) \n");
    printf("\t -d <depth> (option for pink/azure noise) \n");
    printf("\n");
    printf("Generate Noise Data:\n");
    printf("\t -w <npoints> (write noise data to file) \n");
    printf("\t -f <filename> (default = ./noise.dat) \n");
    printf("\n");
    exit(1);
  }
    
  for (idx = 1; idx < argc;  idx++) {
    if (strcmp(argv[idx], "-c") == 0) {
      char* color = argv[idx+1];
      if (strcmp(color, "brown")  == 0) {a->cx = 0; a->cxs = "brown";}
      else if (strcmp(color, "pink")   == 0) {a->cx = 1; a->cxs = "pink";}
      else if (strcmp(color, "white")  == 0) {a->cx = 2; a->cxs = "white";}
      else if (strcmp(color, "azure")  == 0) {a->cx = 3; a->cxs = "azure";}
      else if (strcmp(color, "violet") == 0) {a->cx = 4; a->cxs = "violet";}
      else {
	printf("Invalid Noise Color: Choose brown, pink, white, azure or violet\n");
	exit(1);
      }
    }
    if (strcmp(argv[idx], "-t") == 0){a->ts = atoi(argv[idx+1]);}
    if (strcmp(argv[idx], "-a") == 0){a->alpha = atof(argv[idx+1]);}
    if (strcmp(argv[idx], "-d") == 0){a->depth = atoi(argv[idx+1]);}

    // For noise geneartion 
    if (strcmp(argv[idx], "-f") == 0){
      a->filename = argv[idx+1];
    }
    if (strcmp(argv[idx], "-w") == 0){
      a->npoints = atoi(argv[idx+1]);
      a->alsa = 0;
    }
  }  

  if (a->alpha == -1){a->alpha = 1.0;}
  if ( ((a->alpha < 0) || (a->alpha > 2)) && ((a->cx == 1) || (a->cx==3)) ){
    printf("---------------------------------------------------------\n");
    printf("alpha=%f is out of range, defaulting to alpha = 1 \n", a->alpha);
    a->alpha = 1.0;
  }
  return a;
}

void* noise_alsa(void* a_ptr){

  args* a = (args*)a_ptr;
  
  // Initialize pink and azure noise
  float* A = autoCorr(a->depth,a->alpha);
  float* P = initPink(a->depth,a->alpha);  
  float* B = initBlue(a->depth,a->alpha);  
  
  // Initialize ALSA and play noise
 
  ALSA_params* ALSA_P = initALSA();
  printf("---------------------------------------------------------\n");
  printf("Playing %is of %s noise",a->ts, a->cxs);
  if ( (a->cx == 1) || (a->cx == 3)){
    printf(": depth=%i and alpha=%f\n",a->depth, a->alpha);
  }
  else{
    printf("\n");
  }
  printf("---------------------------------------------------------\n");
    
  if (a->cx == 0){playBrownNoise(ALSA_P,a->ts);}
  if (a->cx == 1){playPinkNoise(ALSA_P, P, A, a->depth, a->ts);}
  if (a->cx == 2){playWhiteNoise(ALSA_P, a->ts);}
  if (a->cx == 3){playBlueNoise(ALSA_P, B, A, a->depth, a->ts);}
  if (a->cx == 4){playVioletNoise(ALSA_P, a->ts);} 

  pthread_exit(NULL);
}

void* volume_alsa(void* a){
  char c;
  nonblock(1);
  while(1){
    usleep(1);
    if (!kbhit()){
      c=fgetc(stdin);
      if (c=='9'){ volumeALSA(0);} //decrement volume 5%
      if (c=='0'){ volumeALSA(1);} //increment volume 5%
    }
  }
  nonblock(0);
}

void noise_data(args* a){

  // Initialize pink and azure noise
  float* A = autoCorr(a->depth,a->alpha);
  float* P = initPink(a->depth,a->alpha);  
  float* B = initBlue(a->depth,a->alpha);  

  printf("---------------------------------------------------------\n");
  printf("Generating %i points of %s noise \n",a->npoints, a->cxs);
  printf("Saving noise to %s \n",a->filename);
  printf("---------------------------------------------------------\n");
  
  FILE* fp = fopen(a->filename, "w");
  if (fp == NULL){
    fprintf(stderr, "Can't open output file %s!\n","noise");
  }
  int i = 0;
  if (a->cx == 0){
    while(i < a->npoints){fprintf(fp, "%f\n", genBrownNoisePure()); i++;}
  }
  if (a->cx == 1){
    while(i < a->npoints){fprintf(fp, "%f\n", genPinkNoise(P, A, a->depth)); i++;}
  }
  if (a->cx == 2){
    while(i < a->npoints){fprintf(fp, "%f\n", genWhiteNoise()); i++;}
  }
  if (a->cx == 3){
    while(i < a->npoints){fprintf(fp, "%f\n", genBlueNoise(B, A, a->depth)); i++;}
  }
  if (a->cx == 4){
    while(i < a->npoints){fprintf(fp, "%f\n", genVioletNoise()); i++;}
  }
}


int main(int argc, char **argv)
{
  // Cool Ascii art intro
  printf(" ____                   _          \n");
  printf("/ ___|      _ __   ___ (_)___  ___ \n");
  printf("| |   _____| '_ \\ / _ \\| / __|/ _ \\\n");
  printf("| |__|_____| | | | (_) | \\__ \\  __/\n");
  printf("\\____|     |_| |_|\\___/|_|___/\\___|\n");
                                       
  // Look for command line arguments
  args* a = parseArgs(argc, argv);
 
  // Initialization
  init_rnd();
  if (a->cx == -1){return 0;}  
  if (a->depth == -1){a->depth = 20;}
  if (a->ts == -1){a->ts= 60;}
  
  // Play noise 
  if (a->alsa == -1){
    pthread_t noise_thread[2];
    if(pthread_create(&noise_thread[0], NULL, noise_alsa, (void*)a)) {
      fprintf(stderr, "Error creating thread\n");
    }
    if(pthread_create(&noise_thread[1], NULL, volume_alsa, (void*)a)) {
      fprintf(stderr, "Error creating thread\n");
    }

    pthread_join(noise_thread[0], NULL);
    pthread_kill(noise_thread[1],17);
  }
  
  if (a->alsa == 0){noise_data(a);}
  return 0;
}
