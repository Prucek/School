import iss3
import numpy as np
import matplotlib.pyplot as plt


def clipping(data):
    max = np.abs(data).max()

    for i in range(len(data)):
        if data[i] > 0.7*max:
            data[i] = 1
        elif data[i] < -0.7*max:
            data[i] = -1
        else:
            data[i] = 0

def autocorrelation(frames):
    correlation = np.zeros((len(frames),2*len(frames[0])))
    half_cor = np.zeros((len(frames),len(frames[0])))
    for i in range(len(frames)):
        clipping(frames[i])
        counter = len(frames[i]) - 1
        for j in range(2*len(frames[i])):   

            if j < len(frames[i]):
                tmp = counter
                it = 0
                while counter <= (len(frames[i]) -1):
                    correlation[i][j] += frames[i][counter] * frames[i][it]
                    counter += 1
                    if(it < (len(frames[i]) -1)):
                        it += 1

                counter = tmp
                if(counter > 0):
                    counter -= 1

            elif j >= len(frames[i]):

                if(counter <= len(frames[i]) - 1):
                    counter += 1

                tmp = counter
                it = 0
                while counter <= (len(frames[i]) -1):
                    correlation[i][j] += frames[i][counter] * frames[i][it]
                    counter += 1
                    if(it < (len(frames[i]) -1)):
                        it += 1

                counter = tmp
            
        half_cor[i] = correlation[i][len(correlation[i])//2:]
    return half_cor
            
cor_on = autocorrelation(iss3.on_frames)
cor_off = autocorrelation(iss3.off_frames)

prah = 500
to_ignore = iss3.fs/prah

max_on = np.zeros(len(cor_on))
max_off  = np.zeros(len(cor_off))

for i in range(len(cor_on)):
    freq_on = cor_on[i][int(to_ignore+1):]
    freq_off = cor_off[i][int(to_ignore+1):]

    max_on[i] = iss3.fs / np.argmax(freq_on)
    max_off[i] = iss3.fs / np.argmax(freq_off)


fig2, ax2 = plt.subplots()
ax2.plot(iss3.on_frames[40])
fig2.gca().set_title("Clipping")
fig2.gca().set_ylabel("y")
fig2.gca().set_xlabel("data")
fig2.tight_layout()
fig2.savefig("figure2.png")



fig3, ax3 = plt.subplots()
ax3.plot(cor_on[40])
fig3.gca().set_title("Autocorrelation")
ax3.axvline(x=to_ignore,color="red",label="Prah")
fig3.gca().set_xlabel("data")
fig3.gca().set_ylabel("y")
ax3.axvline(x=(iss3.fs/max_on[40])+to_ignore,color="yellow",label="Lag")
ax3.legend(loc="best")
fig3.tight_layout()
fig3.savefig("figure3.png")


fig4, ax4 = plt.subplots()
ax4.plot(max_off)
ax4.plot(max_on)
fig4.gca().set_xlabel("frames")
fig4.gca().set_ylabel("f0")
fig4.gca().set_title("Frequency")
fig4.tight_layout()
fig4.savefig("figure4.png")

median_on = np.median(max_on)
median_off = np.median(max_off)

print("On",median_on,"\n","Off",median_off)

variance_on = np.var(max_on)
variance_off = np.var(max_off)
print("On",variance_on,"\n","Off",variance_off)