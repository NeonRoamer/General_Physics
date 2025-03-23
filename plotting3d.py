import numpy as np
import matplotlib.pyplot as plt
from matplotlib import colors, cm

paths = {}
with open("celestial_object_simulation.txt", "r") as file:
	for line in file:
		line = np.array(line.strip("\n").split(", "), dtype=float)
		for i in range(int(len(line)/4)):
			if i not in paths:
				paths[i] = np.zeros((0,3))
			paths[i] = np.vstack((paths[i], line[4*i:4*i+3]))

fig = plt.figure()
ax = fig.add_subplot(111, projection="3d")

cmap = cm.plasma
norm = colors.Normalize(vmin=0, vmax=len(paths)-1)

for i in paths:
	x_ax, y_ax, z_ax = paths[i][:,0], paths[i][:,1], paths[i][:,2]
	ax.plot(x_ax, y_ax, z_ax, label=f"Particle {i+1}", color=cmap(norm(i)))

ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")
ax.legend()
plt.show()
print("Finished Program")
