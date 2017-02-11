import csv
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import seaborn as sns

if __name__ == "__main__":

    with open('testd/connection.dat','r') as f:
        reader = csv.reader(f)
        data = []
        arr = []
        for row in reader:
            if row != []:
                arr.append([float(i) for i in row])
            else:
                data.append(arr)
                arr = []
        if arr != []:
            data.append(arr)
    plt.figure(figsize=(12,9))
    sns.heatmap(data[0] , annot = True, cmap = 'RdBu')
    sns.plt.show()
    
"""
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    ax.set_title('Connection Matrix')
    
    cax = ax.imshow(data[0], cmap = cm.seismic, interpolation = 'nearest')
    cbar = fig.colorbar(cax)
    
    fig.set_size_inches(9.0,9.0,True)
    plt.show()
"""
