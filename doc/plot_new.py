import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import csv
import numpy as np


data = np.genfromtxt("out_s.o", delimiter=",", names=["N", "Time", "Error"])
plt.errorbar(data["N"], data["Time"], yerr = data["Error"],  fmt='-+', elinewidth=3, capsize=0)
data = np.genfromtxt("out_p.o", delimiter=",", names=["N", "Time", "Error"])
plt.errorbar(data["N"], data["Time"], yerr = data["Error"],  fmt='-*', elinewidth=3, capsize=0)
data = np.genfromtxt("out_t.o", delimiter=",", names=["N", "Time", "Error"])
plt.errorbar(data["N"], data["Time"], yerr = data["Error"],  fmt='-o', elinewidth=3, capsize=0)
plt.legend(["Single line", "Process", "Thread"])
plt.grid(which = 'both')
plt.savefig("plot_total.png")
