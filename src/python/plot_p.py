import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import csv
import numpy as np


data = np.genfromtxt("out_p.o", delimiter=",", names=["N", "Time", "Error"])
plt.errorbar(data["N"], data["Time"], yerr = data["Error"],  fmt='_')
plt.grid(which = 'both')
plt.savefig("doc/plot1_p.png")