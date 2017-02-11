import csv
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import sys
#import seaborn

if __name__ == "__main__":

    args = sys.argv
    
    with open(args[1],'r') as f:
        reader = csv.reader(f)
        data = []
        arr = []
        count = 0
        meanPotentioalflag = 0
        for row in reader:
            if count <= 100 and meanPotentioalflag == 0:
                if row != [] and row != ['meanPotential']:
                    if row[-1] != '':
                        arr.append([float(i) for i in row])
                    else:
                        arr.append([float(i) for i in row[:-1]])

                elif row == ['meanPotential']:
                    meanPotentioalflag = 1
                else:
                    data.append(arr)
                    arr = []
                    count += 1
            else:
                continue
        if arr != []:
            data.append(arr)

    if len(data) <= 10 and len(data) > 1:
        fig, axes = plt.subplots(1,len(data), subplot_kw={'xticks': [], 'yticks': []})
        
        for i in range(0, len(data)):
            axes[i].imshow(data[i], cmap = cm.RdBu_r, interpolation = 'nearest')
        fig.set_size_inches(3.0*len(data),3.0*len(data),True)
    elif len(data) == 1:
        fig, axes = plt.subplots(1,1, subplot_kw={'xticks': [], 'yticks': []})
        axes.imshow(data[0], cmap = cm.RdBu_r, interpolation = 'nearest')
        fig.set_size_inches(5.0, 5.0, True)
    else:
        rownum = int((len(data) / 10))
        fig, axes = plt.subplots(rownum,10, subplot_kw={'xticks': [], 'yticks': []})
        
        for i in range(0, rownum):
            for j in range(0, 10):
                axes[i][j].imshow(data[i * 10 + j], cmap = cm.RdBu_r, interpolation = 'nearest')
                fig.set_size_inches(15.0,1.5*rownum,True)                

    plt.savefig(args[1][0:args[1].find('.')] + '.png')
    
"""            
    fig = plt.figure()
    ax1 = fig.add_subplot(1,4,1,subplot_kw={'xticks': [], 'yticks': []})
    ax2 = fig.add_subplot(1,4,2)
    ax3 = fig.add_subplot(1,4,3)
    ax4 = fig.add_subplot(1,4,4)
    
    ax1.imshow(data[0], cmap = cm.RdBu_r, interpolation = 'nearest')
    #ax1.set_axis_off()
    ax2.imshow(data[1], cmap = cm.RdBu_r, interpolation = 'nearest')
    ax3.imshow(data[2], cmap = cm.RdBu_r, interpolation = 'nearest')
    ax4.imshow(data[3], cmap = cm.RdBu_r, interpolation = 'nearest')

    fig.set_size_inches(9.0,4.0,True)
plt.show()
"""
    
