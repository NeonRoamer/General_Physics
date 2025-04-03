import numpy as np
import matplotlib.pyplot as plt

delta_time = 100
data = np.loadtxt("celestial_object_energy.txt", dtype = float, delimiter=',')
time = np.linspace(0, data.size, data.size) * delta_time
plt.scatter(time, data)
plt.ylabel("Log10(Energy(t) / Initial Energy)")
plt.xlabel("Time/s")
plt.show()
