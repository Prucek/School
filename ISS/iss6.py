# Frequnce response of mask
import scipy.signal
import iss5
import numpy as np
import matplotlib.pyplot as plt

H = np.zeros((len(iss5.dft_result_on),len(iss5.dft_result_on[0])),dtype=np.complex128)
for i in range(len(iss5.dft_result_on)):
    w, H[i] = scipy.signal.freqz(a=iss5.dft_result_off[i],b=iss5.dft_result_on[i], worN=len(iss5.dft_result_on[0]))

global result
result = np.zeros(len(H[0]))
absolut = np.abs(H)
sum = 0
for j in range(len(H[0])): 
    for i in range(len(H)):  
        sum += absolut[i][j]
    result[j] = sum/len(H[0]) 
    sum = 0


# Draw
to_print = 10* np.log10(result**2)
x = np.linspace(0,8000,len(to_print))

fig7, ax7 = plt.subplots()
fig7.gca().set_xlabel("frequency")
fig7.gca().set_ylabel("spectral density of power")
fig7.gca().set_title("Frequnce response of mask")
ax7 = plt.plot(x,to_print)
fig7.savefig("figure7.png")