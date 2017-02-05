import csv
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm

if __name__= "__main__":

    with open('testd/testData.dat','r') as f:
        reader = csv.reader(f)
        data = []
        arr = []
        for row in reader:
            if row != []:
                arr.append(map(int,row))
            else:
                data.append(arr)
                arr = []
        if arr != []:
            data.append(arr)

        print data[0]
        print data[1]
        print data[2]
        print data[3]

    """
    arr1 = [[1,0,0],[0,1,0],[0,0,1]]
    arr2 = [[0,0,1],[0,1,0],[1,0,0]]
    data = [arr1,arr2]
    """

    fig = plt.figure()

    ax1 = fig.add_subplot(141)
    ax2 = fig.add_subplot(142)
    ax3 = fig.add_subplot(143)
    ax4 = fig.add_subplot(144)

    ax1.imshow(data[0], cmap = cm.Greys, interpolation = 'nearest')
    ax1.set_axis_off()
    ax2.imshow(data[1], cmap = cm.Greys, interpolation = 'nearest')
    ax2.set_axis_off()
    ax3.imshow(data[2], cmap = cm.Greys, interpolation = 'nearest')
    ax3.set_axis_off()
    ax4.imshow(data[3], cmap = cm.Greys, interpolation = 'nearest')
    ax4.set_axis_off()

    fig.set_size_inches(9.0,4.0,True)
    plt.show()
    

