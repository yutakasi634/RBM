import csv
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import seaborn as sns
import sys
#import seaborn

if __name__ == "__main__":

    args = sys.argv
    
    with open(args[1],'r') as f:
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

    plt.figure(figsize=(10,10))
    sns.heatmap(data[0] , annot = True, cmap = 'RdBu_r')            
    plt.savefig(args[1][0:args[1].find('.')] + '.png')
    

