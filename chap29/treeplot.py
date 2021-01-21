import matplotlib.pyplot as plt

threads_x = [1, 2, 3, 4]

# times of single lock tree
conc_y = [1.265759, 2.880477, 4.676656, 6.576530]
plt.plot(threads_x, conc_y, marker='.', label='single lock b-tree')

# times of individual node lock tree
hoh_y = [1.610825, 5.876285, 16.454166, 24.092482]
plt.plot(threads_x, hoh_y, marker='.', label='individual node lock b-tree')


# label the x and y axes
plt.xlabel("Threads")
plt.ylabel("Time (seconds)")

# give the graph a title
plt.title("Concurrent Binary Trees")

# make the legend
plt.legend()
plt.savefig("treeplot.png", dpi=300)

plt.show()
