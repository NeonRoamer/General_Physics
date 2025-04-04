import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
from matplotlib.patches import Ellipse
from scipy.optimize import minimize

def generate_collision_histogram(filepath):
    """
    Generates a 2D histogram of collision data from a text file.

    Args:
        filepath (str): The path to the text file containing the collision data.
    """

    collisions = []
    magnitudes = []
    angles = []

    try:
        with open(filepath, 'r') as file:
            file.readline()
            for line in file:
                line = np.array(line.strip("\n").split(", "), dtype=float)
                collisions.append(int(line[0]))
                magnitudes.append(float(line[1]))
                angles.append(float(line[2]))

        # Convert to numpy arrays for efficient calculations
        collisions = np.array(collisions)
        magnitudes = np.array(magnitudes)
        angles = np.array(angles)

        # Calculate x and y coordinates
        x = magnitudes * np.cos(angles)
        y = magnitudes * np.sin(angles)

        # Filter x, y and collision arrays to only include collisions.
        x_collisions = x[collisions == 1]
        y_collisions = y[collisions == 1]

        # Generate the 2D histogram
        plt.ylim(np.max(x_collisions))
        plt.hist2d(x_collisions, y_collisions, bins=50, cmap='RdPu', norm=LogNorm(vmax=40), range=[[1e15, 1e17], [1e15, 1e17]]) # you can change the number of bins.
        #plt.hist2d(x_collisions, y_collisions, bins=50, cmap='RdPu', norm=LogNorm(vmax=30), range=[[1e21, np.max(x_collisions)], [1e21, np.max(x_collisions)]]) # you can change the number of bins.
        cbar = plt.colorbar(label='Number of Collisions')
        #cbar.set_ticklabels(list(range(np.max(x_collisions))))
        plt.xlabel(r'Force in X axis / N')
        plt.ylabel(r'Force in Y axis / N')
        plt.title('2D Histogram of Collisions')

        plt.savefig("force_histogram_rkf45.png", dpi=1000)
        
        plt.show()

    except FileNotFoundError:
        print(f"Error: File not found at {filepath}")
    except ValueError as e:
        print(f"{e}\nError: Invalid data format in the file.")


os.chdir("C:\\Users\\veerk\\source\\repos\\General_Physics")

# Example usage (replace 'data.txt' with your file path)
generate_collision_histogram('force_results.txt')
