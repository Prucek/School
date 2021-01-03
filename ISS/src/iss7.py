# IDFT
import iss6
import cmath
import numpy as np
import matplotlib.pyplot as plt

N = 1024

"""""""""""""""""""""""""""""""""""""""""""""""""""
    My idft implementation
"""""""""""""""""""""""""""""""""""""""""""""""""""

def idft(data,n):
    result = 0
    for k in range(N):
        result += data[k]*cmath.exp(1j*2*cmath.pi*k*n/N)
    return result/N

my_idft = np.zeros((len(iss6.result)), dtype=np.complex128)

for i in range(N):
    my_idft[i] = idft(iss6.result,i)

"""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""""""""""""""""""""""""""""""""""""""""""

# IFFT
_idft = np.fft.ifft(iss6.result,n=N)

# Draw
global to_print
to_print =  np.array(my_idft)[0:512]

fig8, ax8 = plt.subplots()
fig8.gca().set_ylabel("h[n]")
fig8.gca().set_xlabel("frequency")
fig8.gca().set_title("Impulse response")
ax8 = plt.plot(to_print.real)
fig8.savefig("figure8.png")