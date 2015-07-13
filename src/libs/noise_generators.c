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

// Define some constant
#define RPI 1.7724538509055159
#define R2 1.4142135623730951
#define PI 3.1415926535897932
#define COV 0.01
#define SIG 1.00

// Seed Random Number Generator
void init_rnd(){
  struct timeval tv;
  gettimeofday(&tv,NULL);
  srand(tv.tv_usec);
}
float rnd(){return (float)rand()/(float)RAND_MAX;}

// Define Gaussian PDF and CDF
float gaussianPDF(float x){return ((1.0)/(R2*RPI*SIG))*exp(-(x*x)/(2*SIG*SIG));}
float gaussianCDF(float x){return 0.5*(1+erf(x/ (R2*SIG)));}

// Newtons method to reconstruct noise value from
// random number thrown on [0,1).
float NEWTON(float(*PDF)(float), float(*CDF)(float), float V)
{
  float DELTA;
  float G = 0.0;
  while(1){
    DELTA = (CDF(G) - V);
    if (fabs(DELTA) < COV){return G;}
    else{G -= (DELTA)/PDF(G);}	
  }
}

////////////////////////////////////////////////
//            NOISE GENERATORS                //
////////////////////////////////////////////////


////////////////////////
// White Noise Generator
//
// The key to it all ! All fucntion utilize genWhiteNoise a
// a basis for colored noise generation.
float genWhiteNoise(){return NEWTON(gaussianPDF, gaussianCDF, rnd());}

////////////////////////
// Brown Noise Generator
//
// For Brown noise audio, it is necessary to prevent the noise
// generator random walking too far from zero. So we need two
// bounding functions for nice clean brown noise.
//
float fixBrownNoiseU(float x){return NEWTON(gaussianPDF, gaussianCDF, x*rnd());}
float fixBrownNoiseL(float x){return NEWTON(gaussianPDF, gaussianCDF, (1- x*rnd()));}

float BN = 0;
float genBrownNoiseCorr(){
  if (BN> 600) {BN += fixBrownNoiseU(0.7);}
  else if (BN<-600) {BN += fixBrownNoiseL(0.3);}
  else {BN+=genWhiteNoise();}
  return BN;
}
float genBrownNoisePure(){
  BN+=genWhiteNoise();
  return BN;
}

/////////////////////////
// Violet Noise Generator
//
// Generate white noise and take discrete derivative
float VN = 0;
float genVioletNoise(){
  float TMP = VN;
  VN = genWhiteNoise();
  return TMP-VN;
}


////////////////////////
// Pink noise generator
//
// Complicated. Instead of generating a value, a seed vector of
// white noise is generated. The autocorrelation for 1/f noise
// is then invoked for all subsequent noise values.
float* autoCorr(int depth, float alpha){
  int n;
  float* A  = malloc(depth *  sizeof(float));
  for(n=0;n<depth;n++){
    if (n==0){A[n] = 1.0;}
    else{A[n] =((float)n - 1.0 - alpha/2)*A[n-1]/(float)n;}
  }
  return A;
}  

float* initPink(int depth, float alpha){
  int n;
  float* PN = malloc((depth-1)*sizeof(float));
  for(n=0;n<(depth-1);n++){PN[n] = genWhiteNoise();}
  return PN;
}

float genPinkNoise(float* P, float* A, int depth){
  int n; 
  // Create the V matrix
  float* V = malloc((depth)*sizeof(float));
  for (n=0;n<depth;n++){
    if (n==depth-1){V[n]=genWhiteNoise();}
    else{V[n] = -1*P[n];}
  }
  for (n=0;n<depth-1;n++){
    if (n==depth-2){P[n]=0.0;}
    else {P[n]=P[n+1];}
  }
  float dot=0.0;
  for (n=0;n<depth;n++){dot+=V[n]*A[depth-1-n];}
  free(V);
  
  // Assign the dot product to the last value of PN
  P[depth-2] = dot;
  return P[depth-2];
}

///////////////////////////////////////
// Azure Noise Generator
//
// Same thing as pink noise, only a violet noise vector is used as a
// seed vector. Note that the 'pinking' of violet noise produces blue
// noise (i.e. noise fractioning) 
float* initBlue(int depth, float alpha){
  int n;
  float* BN = malloc((depth-1)*sizeof(float));
  for(n=0;n<(depth-1);n++){BN[n] = genVioletNoise();}
  return BN;
}

float genBlueNoise(float* B, float* A, int depth){
  int n; 
  // Create the V matrix
  float* V = malloc((depth)*sizeof(float));
  for (n=0;n<depth;n++){
    if (n==depth-1){V[n]=genVioletNoise();}
    else{V[n] = -1*B[n];}
  }
  for (n=0;n<depth-1;n++){
    if (n==depth-2){B[n]=0.0;}
    else {B[n]=B[n+1];}
  }
  float dot=0.0;
  for (n=0;n<depth;n++){dot+=V[n]*A[depth-1-n];}
  free(V);

  // Assign the dot product to the last value of PN
  B[depth-2] = dot;
  return B[depth-2];
}
