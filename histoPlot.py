import os
import numpy as np
import matplotlib.pyplot as plt
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
        plt.hist2d(x_collisions, y_collisions, bins=50, cmap='RdPu', range=[[1e21, np.max(x_collisions)], [1e21, np.max(x_collisions)]]) # you can change the number of bins.
        plt.colorbar(label='Number of Collisions')
        plt.xlabel('X Coordinate')
        plt.ylabel('Y Coordinate')
        plt.title('2D Histogram of Collisions')
        plt.show()

    except FileNotFoundError:
        print(f"Error: File not found at {filepath}")
    except ValueError:
        print("Error: Invalid data format in the file.")


    except FileNotFoundError:
        print(f"Error: File not found at {filepath}")
    except ValueError:
        print("Error: Invalid data format in the file.")

#os.chdir("C:\\Users\\veerk\\source\\repos\\General_Physics\\forceRKF45")

# Example usage (replace 'data.txt' with your file path)
generate_collision_histogram('deflection_results.txt')
