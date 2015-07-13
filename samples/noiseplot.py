#
#    C-noise: The open source noise generator
#    Copyright (C) 2015  Michael Winters
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

#!/usr/bin/env python 
import re
import sys
import numpy as np
import matplotlib.pyplot as plt

############# PLOTTING ##############
import matplotlib.pyplot as plt
from matplotlib import rc
rc('xtick', labelsize=20) 
rc('ytick', labelsize=20) 
rc('font',**{'family':'sans-serif','sans-serif':['Helvetica'],'size':20})
rc('text', usetex=True)

# Global Variables
PI    = 3.1415927
R2    = np.sqrt(2)
RPI   = np.sqrt(3.1415927)
CONV  = 0.01 
SIGMA = 1.0

def genPSD(NOISE):
    FREQ = np.fft.fftfreq(len(NOISE),d = 1./512000)
    FFT  = np.fft.fft(NOISE)
    PSD  = (1.0/len(NOISE))*abs(FFT)*abs(FFT) 
    return 10*np.log10(PSD[0:len(NOISE)/2]), FREQ[0:len(NOISE)/2], FFT

## TESTING 
#################################################################
def normalize(NOISE):
    amplitude = max(NOISE)- min(NOISE)
    NOISE     = [n/amplitude for n in NOISE] 
    return NOISE

def normalize_PSD(NOISE):
    amplitude = sum(NOISE[500:1500])/1000
    NOISE     = [n-amplitude for n in NOISE] 
    return NOISE

if __name__ == "__main__":
    NOISE_DICT, colors = {},sys.argv[1:]
    print colors
    for c in colors:
        NOISE_DICT[c] =[]
        for line in open(c,"r"):
            NOISE_DICT[c].append(float(line.split()[0]))

    # Plot the PSD as a function of frequency to verify behaviour
    if True:
        fig = plt.figure(2)
        ax0 = fig.add_subplot(111)

        colors = ["violet","azure","white","pink","brown"]
        mpl_c  = ["violet", "b", "grey", "pink","brown"]
        for i,c in enumerate(colors):
            for key in NOISE_DICT.keys():
                NOISE,F,FFT = genPSD(NOISE_DICT[key])
                if c in key:
                    ax0.semilogx(F, normalize_PSD(NOISE), mpl_c[i])

        ax0.set_xlim(1000,128000)
        ax0.set_ylim(-70,55)
        ax0.set_xlabel("Frequency $(Hz)$")
        ax0.set_ylabel("Power Spectral Density $(dB)$")
        
        ax0.text(1200, 42, r'{\it The Colors of C-noise}')
        plt.tight_layout()
        plt.show()


