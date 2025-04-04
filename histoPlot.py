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
        plt.hist2d(x_collisions, y_collisions, bins=10, cmap='RdPu') # you can change the number of bins.
        plt.colorbar(label='Number of Collisions')
        plt.xlabel('X Coordinate')
        plt.ylabel('Y Coordinate')
        plt.title('2D Histogram of Collisions')
        plt.show()

    except FileNotFoundError:
        print(f"Error: File not found at {filepath}")
    except ValueError:
        print("Error: Invalid data format in the file.")

def generate_collision_probability_histogram(filepath):
    """
    Generates a 2D histogram of collision probability data with contour lines.

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
                collision, magnitude, angle = line.strip().split(", ")
                collisions.append(int(collision))
                magnitudes.append(float(magnitude))
                angles.append(float(angle))

        # Convert to numpy arrays
        collisions = np.array(collisions)
        magnitudes = np.array(magnitudes)
        angles = np.array(angles)

        # Calculate x and y coordinates
        x = magnitudes * np.cos(angles)
        y = magnitudes * np.sin(angles)

        # Generate histograms for collisions and all data points
        hist_collisions, xedges, yedges = np.histogram2d(
            x[collisions == 1], y[collisions == 1], bins=10
        )
        hist_total, _, _ = np.histogram2d(x, y, bins=10)

        # Calculate collision probabilities
        probability_hist = hist_collisions / hist_total
        probability_hist[np.isnan(probability_hist)] = 0  # Handle bins with no data

        # Generate the figure and axes
        fig, ax = plt.subplots()

        # Plot the 2D probability histogram
        im = ax.imshow(
            probability_hist.T,
            origin='lower',
            extent=[xedges[0], xedges[-1], yedges[0], yedges[-1]],
            cmap='RdPu',
        )
        cbar = fig.colorbar(im, ax=ax, label='Collision Probability')

        """
        # Generate the contour lines
        levels = [0.25, 0.50, 0.75]  # Probability levels for contour lines
        contour = ax.contour(
            (xedges[:-1] + xedges[1:]) / 2,
            (yedges[:-1] + yedges[1:]) / 2,
            probability_hist.T,
            levels=levels,
            colors='white',
            linewidths=1,
        )

        # Label the contour lines
        ax.clabel(contour, inline=True, fontsize=8, fmt='%1.2f')
        """
        # Set labels and title
        ax.set_xlabel('X Coordinate')
        ax.set_ylabel('Y Coordinate')
        ax.set_title('2D Histogram of Collision Probability')
        plt.show()

    except FileNotFoundError:
        print(f"Error: File not found at {filepath}")
    except ValueError:
        print("Error: Invalid data format in the file.")

def fit_ellipses_to_probability_histogram(filepath):
    """
    Generates a 2D histogram of collision probability data with fitted ellipses.

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
                collision, magnitude, angle = line.strip().split(", ")
                collisions.append(int(collision))
                magnitudes.append(float(magnitude))
                angles.append(float(angle))

        # Convert to numpy arrays
        collisions = np.array(collisions)
        magnitudes = np.array(magnitudes)
        angles = np.array(angles)

        # Calculate x and y coordinates
        x = magnitudes * np.cos(angles)
        y = magnitudes * np.sin(angles)

        # Generate histograms for collisions and all data points
        hist_collisions, xedges, yedges = np.histogram2d(
            x[collisions == 1], y[collisions == 1], bins=10
        )
        hist_total, _, _ = np.histogram2d(x, y, bins=10)

        # Calculate collision probabilities
        probability_hist = hist_collisions / hist_total
        probability_hist[np.isnan(probability_hist)] = 0  # Handle bins with no data

        # Calculate bin centers
        xcenters = (xedges[:-1] + xedges[1:]) / 2
        ycenters = (yedges[:-1] + yedges[1:]) / 2

        # Probability levels for ellipses
        probability_levels = [0.25, 0.50, 0.75]

        # Generate the figure and axes
        fig, ax = plt.subplots()

        # Plot the 2D probability histogram
        im = ax.imshow(
            probability_hist.T,
            origin='lower',
            extent=[xedges[0], xedges[-1], yedges[0], yedges[-1]],
            cmap='RdPu',
        )
        cbar = fig.colorbar(im, ax=ax, label='Collision Probability')

        # Fit ellipses for each probability level
        for level in probability_levels:
            # Get points within the current probability level
            points_within_level = np.where(probability_hist >= level)
            x_points = xcenters[points_within_level[1]]
            y_points = ycenters[points_within_level[0]]

            if len(x_points) > 5: # Ensure we have enough points to fit an ellipse.
                # Fit an ellipse to the points
                cov = np.cov(x_points, y_points)
                center = np.mean([x_points, y_points], axis=1)
                eigenvalues, eigenvectors = np.linalg.eig(cov)
                angle = np.degrees(np.arctan2(*eigenvectors[:, 0][::-1]))
                width, height = 2 * np.sqrt(eigenvalues)

                # Create the ellipse patch
                ellipse = Ellipse(
                    xy=center, width=width, height=height, angle=angle, edgecolor='red', facecolor='none'
                )
                ax.add_patch(ellipse)

        # Set labels and title
        ax.set_xlabel('X Coordinate')
        ax.set_ylabel('Y Coordinate')
        ax.set_title('2D Histogram of Collision Probability with Ellipses')
        plt.show()

    except FileNotFoundError:
        print(f"Error: File not found at {filepath}")
    except ValueError:
        print("Error: Invalid data format in the file.")


def fit_ellipses_to_probability_histogram_centered(filepath):
    """
    Generates a 2D histogram of collision probability data with fitted ellipses
    centered at the origin.

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
                collision, magnitude, angle = line.strip().split(", ")
                collisions.append(int(collision))
                magnitudes.append(float(magnitude))
                angles.append(float(angle))

        # Convert to numpy arrays
        collisions = np.array(collisions)
        magnitudes = np.array(magnitudes)
        angles = np.array(angles)

        # Calculate x and y coordinates
        x = magnitudes * np.cos(angles)
        y = magnitudes * np.sin(angles)
        """
        # Generate histograms for collisions and all data points
        hist_collisions, xedges, yedges = np.histogram2d(
            x[collisions == 1], y[collisions == 1], bins=10
        )
        hist_total, _, _ = np.histogram2d(x, y, bins=10)

        # Calculate collision probabilities
        probability_hist = hist_collisions / hist_total
        probability_hist[np.isnan(probability_hist)] = 0  # Handle bins with no data
        """
        probability_hist, xedges, yedges = np.histogram2d(
            x[collisions == 1], y[collisions == 1], bins=10
        )
        # Calculate bin centers
        xcenters = (xedges[:-1] + xedges[1:]) / 2
        ycenters = (yedges[:-1] + yedges[1:]) / 2

        # Probability levels for ellipses
        probability_levels = [0.25, 0.50, 0.75]

        # Generate the figure and axes
        fig, ax = plt.subplots()

        # Plot the 2D probability histogram
        im = ax.imshow(
            probability_hist.T,
            origin='lower',
            extent=[xedges[0], xedges[-1], yedges[0], yedges[-1]],
            cmap='RdPu',
        )
        cbar = fig.colorbar(im, ax=ax, label='Collision Probability')

        # Fit ellipses for each probability level, centered at the origin
        for level in probability_levels:
            # Get points within the current probability level
            points_within_level = np.where(probability_hist >= level)
            x_points = xcenters[points_within_level[1]]
            y_points = ycenters[points_within_level[0]]

            if len(x_points) > 5: # Ensure we have enough points to fit an ellipse.
                # Fit an ellipse to the points, centered at the origin
                cov = np.cov(x_points, y_points)
                eigenvalues, eigenvectors = np.linalg.eig(cov)
                angle = np.degrees(np.arctan2(*eigenvectors[:, 0][::-1]))
                width, height = 2 * np.sqrt(eigenvalues)

                # Create the ellipse patch, centered at (0, 0)
                ellipse = Ellipse(
                    xy=(0, 0), width=width, height=height, angle=angle, edgecolor='red', facecolor='none'
                )
                ax.add_patch(ellipse)

        # Set labels and title
        ax.set_xlabel('X Coordinate')
        ax.set_ylabel('Y Coordinate')
        ax.set_title('2D Histogram of Collision Probability with Ellipses (Centered)')
        plt.show()

    except FileNotFoundError:
        print(f"Error: File not found at {filepath}")
    except ValueError:
        print("Error: Invalid data format in the file.")

#os.chdir("C:\\Users\\veerk\\source\\repos\\General_Physics\\forceRKF45")

# Example usage (replace 'data.txt' with your file path)
fit_ellipses_to_probability_histogram_centered('deflection_results.txt')

generate_collision_probability_histogram('deflection_results.txt')

generate_collision_histogram('deflection_results.txt')