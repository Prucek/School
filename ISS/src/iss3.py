# Frame creating
from pydub import AudioSegment
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile   #This library is used for reading the .wav file
import os
import platform

"""
    Uloha 1,2,3
"""

def prepare_file(filename):
    t1 = 780
    t2 = 1780
    tone = AudioSegment.from_wav(filename + ".wav")
    tone = tone[t1:t2] # 1 sec
    tone.export(filename  + "_s.wav", format="wav")

    fs, data = wavfile.read(filename  + "_s.wav")
    data = np.subtract(data, np.mean(data))    # centralizing
    data = np.divide(data, np.abs(data).max()) # normalisation
    return fs, data

# https://deerishi.wordpress.com/2013/09/23/signal-processing-using-python-part-1/
def create_frames(signal, fs):
    window_hop_length = 0.01 # 10ms
    window_size = 0.02       # 20 ms
    length = len(signal)
    overlap = int(fs * window_hop_length)
    framesize = int(window_size * fs)
    number_of_frames = int(length/overlap)
    frames = np.ndarray((number_of_frames,framesize)) 
    for k in range(0,number_of_frames):
        for i in range(0,framesize):
            if((k*overlap+i)<length):
                frames[k][i]=signal[k*overlap+i]
            else:
                frames[k][i]=0
    frames = frames[:len(frames)-1] #delete last one
    return frames

if platform.system() == 'Linux':
    os.chdir("../audio")
if platform.system() == 'Windows':
    os.chdir("..\\audio")

# Preparing
fs_on, data_on = prepare_file("maskon_tone")
fs_off, data_off = prepare_file("maskoff_tone")

if platform.system() == 'Linux':
    os.chdir("../src")
if platform.system() == 'Windows':
    os.chdir("..\\src")

global fs  
fs = fs_on # or fs_off

# Framing
global on_frames
on_frames = create_frames(data_on,fs)
global off_frames 
off_frames = create_frames(data_off,fs)

# Draw
t1 = np.arange(on_frames[len(on_frames)-1].size) / fs
t2 = np.arange(off_frames[len(off_frames)-1].size) / fs

fig1, ax1 = plt.subplots()

ax1.plot(t1,on_frames[40], label = "Mask on")
ax1.plot(t2,off_frames[40], label = "Mask off")

fig1.gca().set_xlabel("$t[s]$")
fig1.gca().set_ylabel("y")
fig1.gca().set_title("Frame 40")
ax1.legend(loc="best")

fig1.tight_layout()
fig1.savefig("figure1.png")

