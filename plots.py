import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button
from matplotlib import colors, cm
from mpl_toolkits.mplot3d import Axes3D  # Required for 3D plotting

# Define labels for the 3 objects.
labels = ["Earth", "Moon", "Asteroid"]

# -------------------------------
# Read trajectories and radii from file
# -------------------------------
# Each line is expected to have 12 comma-separated numbers:
# For each object: x, y, z, and radius.
from typing import List

trajectories: List[np.ndarray] = []  # List of NumPy arrays (floats)
radii: List[float] = []             # List of floats (one radius per object)

with open("celestial_object_simulation.txt", "r") as file:
    for line_idx, line in enumerate(file):
        # Convert the line into a numpy array of floats.
        values = np.array(line.strip().split(", "), dtype=float)
        
        # Check if any value exceeds 5e9 (in absolute value).
        if np.any(np.abs(values) > 5e9):
            print(f"Value exceeding 5e9 encountered on line {line_idx}. Stopping reading new lines.")
            break

        n_obj = int(len(values) / 4)  # Should be 3 for Earth, Moon, Asteroid.
        if line_idx == 0:
            trajectories = [[] for _ in range(n_obj)]
            radii = [0] * n_obj
        for i in range(n_obj):
            pos = values[4 * i: 4 * i + 3]  # x, y, z coordinates.
            r = values[4 * i + 3]            # radius.
            # Divide by 1e9 to convert to units of 10^9 m.
            trajectories[i].append(pos / 1e9)
            if line_idx == 0:
                radii[i] = r / 1e9

# Convert each object's trajectory list into a NumPy array.
trajectories = [np.array(traj) for traj in trajectories]

# -------------------------------
# Set up the interactive 3D plot
# -------------------------------
fig = plt.figure(figsize=(12, 10))
ax = fig.add_subplot(111, projection='3d')
plt.subplots_adjust(bottom=0.25)

# Set plot background colors.
ax.set_facecolor('white')
fig.patch.set_facecolor('white')

# Set perspective projection for a more realistic 3D view.
ax.set_proj_type('persp')

# Set pane colors to light gray and enable grid lines to enhance depth cues.
ax.xaxis.set_pane_color((0.9, 0.9, 0.9, 1.0))
ax.yaxis.set_pane_color((0.9, 0.9, 0.9, 1.0))
ax.zaxis.set_pane_color((0.9, 0.9, 0.9, 1.0))
ax.grid(True)

# Define colors for the objects.
colors_list = ['blue', 'gray', 'red']
if len(trajectories) > len(colors_list):
    cmap = cm.plasma
    norm = colors.Normalize(vmin=0, vmax=len(trajectories) - 1)
    colors_list = [cmap(norm(i)) for i in range(len(trajectories))]

# Helper function: Plot a sphere at a given center with a given radius.
def plot_sphere(ax, center, radius, color='cyan', resolution=20):
    u, v = np.mgrid[0:2*np.pi:complex(0, resolution), 0:np.pi:complex(0, resolution)]
    x = radius * np.cos(u) * np.sin(v) + center[0]
    y = radius * np.sin(u) * np.sin(v) + center[1]
    z = radius * np.cos(v) + center[2]
    # Adding edge color and slight transparency to help with depth perception.
    return ax.plot_surface(x, y, z, color=color, shade=True, rstride=1, cstride=1,
                           edgecolor='k', alpha=0.9)

# Create line objects (for trails) and sphere objects (to mark current positions).
lines = []
scale_factor = 10  # Adjust this factor to scale the visible size of the spheres.

for i, traj in enumerate(trajectories):
    # Initialize the trail with the first point.
    line, = ax.plot(traj[:1, 0], traj[:1, 1], traj[:1, 2],
                    color=colors_list[i], label=labels[i], linewidth=2)
    # Plot a sphere at the initial position.
    sphere_artist = plot_sphere(ax, traj[0], radii[i] * scale_factor, color=colors_list[i])
    lines.append((line, sphere_artist))

# Set labels, title, and legend.
ax.set_xlabel("X (10⁹ m)")
ax.set_ylabel("Y (10⁹ m)")
ax.set_zlabel("Z (10⁹ m)")
ax.set_title("Celestial Object Trajectories")
leg = ax.legend()
for text in leg.get_texts():
    text.set_color("black")

# -------------------------------
# Fix axis limits and set custom ticks.
# -------------------------------
# Set axis limits to -3e9 to 3e9.
ax.set_xlim(-2, 2)
ax.set_ylim(-2, 2)
ax.set_zlim(-2, 2)

# Define tick positions at 1e9 intervals.
ticks = np.arange(-2, 2, 2)
ax.set_xticks(ticks)
ax.set_yticks(ticks)
ax.set_zticks(ticks)
# Format tick labels to show values in units of 10^9 m (displaying just the number).
ax.set_xticklabels([f"{tick:.0f}" for tick in ticks])
ax.set_yticklabels([f"{tick:.0f}" for tick in ticks])
ax.set_zticklabels([f"{tick:.0f}" for tick in ticks])

# Set the view angle (elevated and rotated) for better depth perception.
ax.view_init(elev=30, azim=45)

# -------------------------------
# Set up Interactive Widgets
# -------------------------------
steps = trajectories[0].shape[0]  # Total number of time steps.
ax_slider = plt.axes((0.25, 0.15, 0.5, 0.03))
time_slider = Slider(ax_slider, 'Time Step', 0, steps - 1, valinit=0, valstep=1)

ax_speed = plt.axes((0.25, 0.1, 0.5, 0.03))
speed_slider = Slider(ax_speed, 'Speed', 100, 5000, valinit=100, valstep=100)

ax_button = plt.axes((0.8, 0.025, 0.1, 0.04))
play_button = Button(ax_button, 'Play', color='lightgray', hovercolor='gray')
playing = [False]  # Mutable container to track play/pause state.

# -------------------------------
# Update Function for the Animation
# -------------------------------
def update(val):
    t_idx = int(time_slider.val)
    for i, traj in enumerate(trajectories):
        line, artist = lines[i]
        # Update the trajectory trail.
        line.set_data(traj[:t_idx + 1, 0], traj[:t_idx + 1, 1])
        line.set_3d_properties(traj[:t_idx + 1, 2])
        # Remove the old sphere and plot a new one at the current position.
        try:
            artist.remove()
        except Exception:
            pass
        new_sphere = plot_sphere(ax, traj[t_idx], radii[i] * scale_factor, color=colors_list[i])
        lines[i] = (line, new_sphere)
    fig.canvas.draw_idle()

time_slider.on_changed(update)

# -------------------------------
# Timer Callback for Animation
# -------------------------------
def timer_callback():
    if playing[0]:
        current_val = time_slider.val
        jump_size = int(speed_slider.val / 100)
        if current_val < steps - jump_size:
            time_slider.set_val(current_val + jump_size)
        else:
            time_slider.set_val(0)

timer = fig.canvas.new_timer(interval=int(speed_slider.val))
timer.add_callback(timer_callback)

def update_speed(val):
    timer.interval = int(val)
    if playing[0]:
        timer.stop()
        timer.start()

speed_slider.on_changed(update_speed)

def toggle_play(event):
    if playing[0]:
        playing[0] = False
        play_button.label.set_text("Play")
        timer.stop()
    else:
        playing[0] = True
        play_button.label.set_text("Pause")
        timer.start()

play_button.on_clicked(toggle_play)

plt.show()
