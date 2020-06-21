import matplotlib
import matplotlib.pyplot as plt
import numpy as np

plt.rc('text', usetex=True)

u = np.loadtxt("equilibration_v.out", usecols = (0), delimiter=' ', unpack ='true')
w = np.loadtxt("equilibration_w.out", usecols = (0), delimiter=' ', unpack ='true') 
x = np.arange(len(u)) + 1

plt.figure(figsize=(9,6))
plt.plot(x, u, color='black')
plt.xlabel('Step', fontsize=18)
plt.ylabel(r'$V$', fontsize=18)
plt.title('Metropolis Algorithm equilibration - Potential Energy', fontsize=22)
plt.grid(True)

plt.figure(figsize=(9,6))
plt.plot(x, w, color='black')
plt.xlabel('Step', fontsize=18)
plt.ylabel(r'$W$', fontsize=18)
plt.title('Metropolis Algorithm equilibration - Virial', fontsize=22)
plt.grid(True)

plt.show()