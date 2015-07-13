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


// For random number generation
void init_rnd(void);
float rnd(void);

// PDF and CDF for Gaussian Noise
float gaussianPDF(float x);
float gaussianCDF(float x);

// Newtons Method for Calculating Gaussian noise value
float NEWTON(float(*PDF)(float), float(*CDF)(float), float V);

// Noise Generators. All colors of noise
float genWhiteNoise(void);
float genBrownNoiseCorr(void);
float genBrownNoisePure(void);
float genVioletNoise(void);
float genPinkNoise(float* P, float* A, int depth);
float genBlueNoise(float* B, float* A, int depth);

// Autocorrrelation functions  and initialization
// for pink and blue noise generation 
float* autoCorr(int depth, float alpha);
float* initPink(int depth, float alpha);
float* initBlue(int depth, float alpha);
