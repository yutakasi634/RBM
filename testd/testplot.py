import csv
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.cm as cm
#import seaborn

n = 300
x = np.random.randn(n)
y1 = np.exp(x) + np.random.randn(n)
y2 = np.exp(x) * 3 + np.random.randn(n)

fig = plt.figure()
ax1 = fig.add_subplot(121)
ax2 = fig.add_subplot(122)

ax1.plot(x, y1, ".")
ax2.plot(x, y2, ".")

