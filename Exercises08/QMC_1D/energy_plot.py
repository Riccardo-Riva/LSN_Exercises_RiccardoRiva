
import matplotlib
import matplotlib.pyplot as plt
import numpy as np

plt.rc('text', usetex=True)

V, errV = np.loadtxt("potential.dat", usecols = (1,2), delimiter=' ', unpack ='true')
K, errK = np.loadtxt("kinetic.dat", usecols = (1,2), delimiter=' ', unpack ='true')
x = np.arange(len(V)) +1 

pos, N, errN = np.loadtxt("probability.dat", usecols = (0,1,2), delimiter=' ', unpack ='true')

plt.figure(figsize=(9,6))
plt.errorbar(x, V, yerr=errV, color='blue', label='V')
plt.xlabel('Polymer Particle', fontsize=18)
plt.ylabel('$V$', fontsize=18)
plt.title('Potential Energy', fontsize=22)
plt.legend()
plt.grid(True)

plt.figure(figsize=(9,6))
plt.errorbar(x, K, yerr=errK, color='red', label='K')
plt.xlabel('Polymer Particle', fontsize=18)
plt.ylabel('$K$', fontsize=18)
plt.title('Kinetic Energy', fontsize=22)
plt.legend()
plt.grid(True)

plt.figure(figsize=(9,6))
plt.errorbar(pos, N, yerr=errN, color='blue')
plt.xlabel('r', fontsize=18)
plt.ylabel(r'$p(r)$',fontsize=18)
plt.title('Probability Density', fontsize=22)
plt.grid(True)

plt.show()

print(V[0]+K[0])