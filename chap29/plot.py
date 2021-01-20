import matplotlib.pyplot as plt
import numpy as np

# each threshold used
# thold_x = [2, 4, 8, 16, 32, 64, 128, 256, 512, 1024]

# this is required to get each number in thold_x to show on the graph
# and make it similar to the graph in the book
thold_x = np.arange(0, 11)
x = 2 ** thold_x # exponents


# timing of each threshold to finish
time_y = [0.601, 0.468, 0.401, 0.361, 0.351, 0.329, 0.319, 0.319, 0.315, 0.315, 0.315]

plt.plot(thold_x, time_y, marker='o')
plt.xticks(thold_x, x)

# label the x and y axes
plt.xlabel("Threshold (Approximation)")
plt.ylabel("Time (seconds)")

# give the graph a title
plt.title("Approximation Counter")
plt.savefig("plot.png", dpi=300)

# get it all together
plt.show()