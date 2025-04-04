import numpy as np
import matplotlib.pyplot as plt

delta_time = 100
data = np.loadtxt("celestial_object_momentum.txt", dtype = float, delimiter=',')
time = np.linspace(0, data.size, data.size) * delta_time
plt.plot(time, data)
plt.ylabel("Log10(Momentum(t) / Initial Momentum)")
plt.xlabel("Time/s")
plt.savefig("figures/Momentum_leapfrog.png", dpi=1000)
plt.show()
