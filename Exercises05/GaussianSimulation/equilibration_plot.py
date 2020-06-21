import matplotlib
import matplotlib.pyplot as plt
import numpy as np

plt.rc('text', usetex=True)

r100 = np.loadtxt("equilibration_test_100.out", usecols = (0), delimiter=' ', unpack ='true')
r210 = np.loadtxt("equilibration_test_210.out", usecols = (0), delimiter=' ', unpack ='true')
x1 = np.arange(len(r100)) + 1 
x2 = np.arange(len(r210)) + 1

plt.figure(figsize=(9,6))
plt.plot(x1, r100, color='black')
plt.xlabel('Step', fontsize=18)
plt.ylabel(r'r/$a_0$', fontsize=18)
plt.title('Distance from the origin - $(n,l,m) = (1,0,0)$', fontsize=22)
plt.grid(True)

plt.show()

plt.figure(figsize=(9,6))
plt.plot(x2, r210, color='blue')
plt.xlabel('Step', fontsize=18)
plt.ylabel(r'r/$a_0$', fontsize=18)
plt.title(r'Distance from the origin - $(n,l,m) = (2,1,0)$', fontsize=22)
plt.grid(True)

plt.show()