import matplotlib.pyplot as plt
import sys
import seaborn

if __name__ == "__main__":
    args = sys.argv

    xdata = []
    ydatas = []
    count = 0;
    ylen = 0;
    for line in open(args[1], 'r'):
        items = line.split()
        items = [float(i) for i in items]
        xdata.append(items[0])
        if count == 0:
            ylen = len(items)
            for i in range(1,ylen):
                ydatas.append([items[i]])
            count += 1
        else:
            for i in range(1,ylen):
                ydatas[i-1].append(items[i])

    plt.plot(xdata,ydatas[0],label = "sum of ∂logL/∂w")
    plt.plot(xdata,ydatas[1],label = "sum of ∂logL/∂a")
    plt.plot(xdata,ydatas[1],label = "sum of ∂logL/∂b")
    plt.legend()
    plt.savefig(args[1][0:args[1].find('.')] + '.png')
