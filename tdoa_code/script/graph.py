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
        y1.append(count)
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
    y = []
    x = []
    count = 0
    beginning = 10000

    for i in open(type+"peaks.txt",'r'):
        y.append(i) #includes the bias unlike the following part

    y.sort()
    j = 0
    # print(len(y))
    for i in open(type+"fall.txt",'r'):

        x1.append(i)
        y1.append(count+beginning)

        if ((j < len(y) and y1[count] == int(y[j]))):
            x.append(1.5)
            # print(str(j) + " " + (y[j]))
            j +=1
        else:
            x.append(0)

        count +=1

    # print(y1[0],int(y[0]))



    plt.figure(n)
    plt.subplot(311)
    plt.plot(y1,x1,y1,x)

    count = 0
    for i in open("h1tfilt.txt",'r'):
        x2.append(i)
        y2.append(count)
        count +=1

    plt.subplot(312)
    plt.plot(y2[10000:-10000],x2[10000:-10000])

    count = 0
    x2 = []
    y2 = []
    for i in open("h2tfilt.txt",'r'):
        x2.append(i)
        y2.append(count)
        count +=1

    plt.subplot(313)
    plt.plot(y2[10000:-10000],x2[10000:-10000])
    plt.show()


def plot_seg(filename,n,win):
    x =[]
    y =[]
    count = 0;

    for i in open("h1"+filename):
        x.append(i)
        y.append(count)
        count +=1


    print(count)

    plt.figure(n)
    plt.subplot(2,7,1)
    plt.plot(y[:win-1],x[:win-1])

    plt.subplot(2,7,2)
    plt.plot(y[win:2*win-1],x[win:2*win-1])

    plt.subplot(2,7,3)
    plt.plot(y[2*win:3*win-1],x[2*win:3*win-1])

    plt.subplot(2,7,4)
    plt.plot(y[3*win:4*win-1],x[3*win:4*win-1])

    plt.subplot(2,7,5)
    plt.plot(y[4*win:5*win-1],x[4*win:5*win-1])

    plt.subplot(2,7,6)
    plt.plot(y[5*win:6*win-1],x[5*win:6*win-1])

    plt.subplot(2,7,7)
    plt.plot(y[6*win:7*win-1],x[6*win:7*win-1])

    x =[]
    y =[]
    count = 0;

    for i in open("h2"+filename):
        x.append(i)
        y.append(count)
        count +=1


    print(count)

    plt.figure(n)
    plt.subplot(2,7,8)
    plt.plot(y[:win-1],x[:win-1])

    plt.subplot(2,7,9)
    plt.plot(y[win:2*win-1],x[win:2*win-1])

    plt.subplot(2,7,10)
    plt.plot(y[2*win:3*win-1],x[2*win:3*win-1])

    plt.subplot(2,7,11)
    plt.plot(y[3*win:4*win-1],x[3*win:4*win-1])

    plt.subplot(2,7,12)
    plt.plot(y[4*win:5*win-1],x[4*win:5*win-1])

    plt.subplot(2,7,13)
    plt.plot(y[5*win:6*win-1],x[5*win:6*win-1])

    plt.subplot(2,7,14)
    plt.plot(y[6*win:7*win-1],x[6*win:7*win-1])

    plt.show()

def plot_cc(filename,n):
    x1 = []
    y1 = []
    count = 0;
    for i in open(filename,'r'):
        x1.append(i)
        y1.append(count)
        count +=1

    plt.figure(n)
    plt.plot(y1,x1)
    plt.show()

# plot_tf("t.txt",1)
# plot_tf("fr.txt",2)
# plot_tf("filt.txt",3)
# plot_tf("tfilt.txt",4)
plot_pk("h1",5)
plot_seg("working.txt",6,20000)
plot_cc("correlation.txt",7)
