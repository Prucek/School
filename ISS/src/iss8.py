# Filter
import iss7
import scipy.signal
import scipy.io.wavfile
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile 
import os
import platform

if platform.system() == 'Linux':
    os.chdir("../audio")
if platform.system() == 'Windows':
    os.chdir("..\\audio")

fs, data = wavfile.read("maskoff_tone.wav")
filtered = scipy.signal.lfilter(iss7.to_print.real, [1], data)
scipy.io.wavfile.write("sim_maskon_tone.wav",16000,filtered.real)

fs, off = wavfile.read("maskoff_sentence.wav")
filtered = scipy.signal.lfilter(iss7.to_print.real, [1], off)
scipy.io.wavfile.write("sim_maskon_sentence.wav",16000,filtered.real)

fs, normal = wavfile.read("maskon_sentence.wav")
normal = normal[2500:]

if platform.system() == 'Linux':
    os.chdir("../src")
if platform.system() == 'Windows':
    os.chdir("..\\src")


# Draw
fig9, ax9 = plt.subplots()
fig9.gca().set_xlabel("samples")
fig9.gca().set_title("Filter simulation")
ax9 = plt.plot(normal,label="Real Mask on")
ax9 = plt.plot(filtered.real,label="Filtered")
fig9.tight_layout()
fig9.legend()
fig9.savefig("figure9.png")

fig10, ax10 = plt.subplots()
fig10.gca().set_xlabel("samples")
fig10.gca().set_title("Filter simulation")
ax10 = plt.plot(off,label="Real Mask off")
ax10 = plt.plot(filtered.real,label="Filtered")
fig10.tight_layout()
fig10.legend()
fig10.savefig("figure10.png")