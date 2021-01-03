# DFT
import iss3
import cmath
import numpy as np
import matplotlib.pyplot as plt

N = 1024

"""""""""""""""""""""""""""""""""""""""""""""""""""
    My dft implementation
"""""""""""""""""""""""""""""""""""""""""""""""""""
# def dft(data,k):
#     result = 0
#     for n in range(N):
#         result += data[n]*cmath.exp(-1j*2*cmath.pi*k*n/N)
#     return result

# dft_result_on = np.zeros((len(iss3.on_frames),N),dtype=np.complex128)
# dft_result_off = np.zeros((len(iss3.off_frames),N),dtype=np.complex128) 

# def padding(data):
#     result = np.zeros((len(data),N))
#     for i in range(len(data)):
#         result[i] = np.pad(data[i], (0,N-len(data[i])), "constant", constant_values = (0,0))
#     return result

# padded_off = padding(iss3.off_frames)
# padded_on = padding(iss3.on_frames)

# for i in range(len(padded_off)):
#     for k in range(N):
#         dft_result_off[i][k] = dft(padded_off[i],k)
#         dft_result_on[i][k] = dft(padded_on[i],k)


# to_print_off  = 10* np.log10(np.abs(dft_result_off)**2)
# to_print_on  = 10* np.log10(np.abs(dft_result_on)**2)

# half_to_print_off =  np.array(to_print_off)[:, 0:512]
# half_to_print_on=  np.array(to_print_on)[:, 0:512]
"""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""""""""""""""""""""""""""""""""""""

# FFT
global dft_result_on
global dft_result_off
dft_result_on = np.fft.fft(iss3.on_frames,n=N)
dft_result_off = np.fft.fft(iss3.off_frames,n=N)

to_print_off = 10* np.log10(np.abs(dft_result_off)**2)
to_print_on  = 10* np.log10(np.abs(dft_result_on)**2)

half_to_print_off =  np.array(to_print_off)[:, 0:512]
half_to_print_on=  np.array(to_print_on)[:, 0:512]

# Draw
fig5, ax5 = plt.subplots()
fig5.gca().set_xlabel("time")
fig5.gca().set_ylabel("frequency")
fig5.gca().set_title("Spectogram mask_off")
fig5.tight_layout()
im = ax5.imshow(np.rot90(np.fliplr(half_to_print_off), -1),extent=[0, 1, 0, 8000],aspect="auto")
cbar = fig5.colorbar(im)
cbar.set_label("Spectral density of power [dB]", rotation=270, labelpad=15)
fig5.savefig("figure5.png")

fig6, ax6 = plt.subplots()
fig6.gca().set_xlabel("time")
fig6.gca().set_ylabel("frequency")
fig6.gca().set_title("Spectogram mask_on")
fig6.tight_layout()
im = ax6.imshow(np.rot90(np.fliplr(half_to_print_on), -1),extent=[0, 1, 0, 8000],aspect="auto")
cbar = fig6.colorbar(im)
cbar.set_label("Spectral density of power [dB]", rotation=270, labelpad=15)
fig6.savefig("figure6.png")