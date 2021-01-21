import matplotlib.pyplot as plt

threads_x = [1, 2, 3, 4]

# times of single lock tree
conc_y = [0.077794, 0.145959, 0.156857, 0.181931]
plt.plot(threads_x, conc_y, marker='.', label='single lock list')

# times of individual node lock tree
hoh_y = [0.125221, 0.169981, 0.222011, 0.267852]
plt.plot(threads_x, hoh_y, marker='.', label='hand-over-hand list')


# label the x and y axes
plt.xlabel("Threads")
plt.ylabel("Time (seconds)")

# give the graph a title
plt.title("Concurrent Linked Lists")

# make the legend
plt.legend()
plt.savefig("listplot.png", dpi=300)

plt.show()