import matplotlib.pyplot as plt
import numpy as np


def plot_tf(filename,n):
    x1 = []
    y1 = []
    x2 = []
    y2 = []
    count = 0;
    for i in open("h1"+filename,'r'):
        x1.append(i)
        y1.append(count+10000)
        count +=1

    plt.figure(n)
    plt.subplot(211)
    plt.plot(y1[10000:-10000],x1[10000:-10000])

    count = 0
    for i in open("h2"+filename,'r'):
        x2.append(i)
        y2.append(count)
        count +=1

    plt.subplot(212)
    plt.plot(y2[10000:-10000],x2[10000:-10000])

    plt.show()



def plot_pk(type,n):
    x1 = []
    y1 = []
    x2 = []
    y2 = []
    x = []
    y = []
    count = 0;
    for i in open(type+"seg.txt",'r'):
        x1.append(i)
        y1.append(count+10000)
        count +=1
    for i in open(type+"peaks.txt",'r'):
        x.append(10000)
        y.append(i)



    plt.figure(n)
    plt.subplot(211)
    plt.plot(y1,x1)
    plt.plot(y,x)

    count = 0
    for i in open("h1tfilt.txt",'r'):
        x2.append(i)
        y2.append(count)
        count +=1

    plt.subplot(212)
    plt.plot(y2[10000:-10000],x2[10000:-10000])

    plt.show()



# plot("filt.txt",1)
# plot("tfilt.txt",1)
plot_pk("h1",2)
