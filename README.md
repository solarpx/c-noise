## Welcome to C-noise ! 

C-noise is an open source noise generation program which allows one to explore the colors of noise. 

* Listen to noise in real time
* Generate noise data for scientific purposes

## Installation
C-noise makes use of ALSA, so be sure to have alsa-utils, asla-lib, and alsa-headers installed. To install C-noise simply do

`make`
`sudo make install`

## Volume Control

Increase volume: hold 0
Decrease volume: hold 9

## Examples: Listen to noise 

Generate 300s of white noise:

	`cnoise -c white -t 300`

Generate 300s of pink noise with depth and alpha

	`cnoise -c pink -d 15 -a 1.66 -t 300`

	*depth (-d) controls the length of the autocorrelation vector, and
	alpha (-a) controls the degree of autocorrelation. Alpha is always
	between 0 and 2 and defaults to 1 if not specified* 

## Examples: Generate noise data

To generate data points, use the `-w` flag.

Generate 1024 points of azure noise

	`cnoise -c azure -w 1024`

Generate 512 points of pink noise and save to pink.dat

	`cnoise -c pink -w 512 -f pink.dat`
