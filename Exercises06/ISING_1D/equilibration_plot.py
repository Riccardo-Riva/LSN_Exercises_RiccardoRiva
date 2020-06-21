import matplotlib
import matplotlib.pyplot as plt
import numpy as np

plt.rc('text', usetex=True)

u, m = np.loadtxt("equilibration_u.out", usecols = (0, 1), delimiter=' ', unpack ='true') 
x = np.arange(len(u)) + 1

plt.figure(figsize=(9,6))
plt.plot(x, u, color='black')
plt.xlabel('Step', fontsize=18)
plt.ylabel(r'$u$', fontsize=18)
plt.title('Metropolis Algorithm equilibration - $T = 2,0$', fontsize=22) # Metropolis Algorithm Gibbs Sampling
plt.grid(True)

plt.show()