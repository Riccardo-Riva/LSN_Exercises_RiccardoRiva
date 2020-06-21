import matplotlib
import matplotlib.pyplot as plt
import numpy as np

plt.rc('text', usetex=True)

u, erru = np.loadtxt("output.ene.0", usecols = (2,3), delimiter=' ', unpack ='true')
c, errc = np.loadtxt("output.heat.0", usecols = (2,3), delimiter=' ', unpack ='true')
m, errm = np.loadtxt("output.mag.0", usecols = (2,3), delimiter=' ', unpack ='true')
x, errx = np.loadtxt("output.chi.0", usecols = (2,3), delimiter=' ', unpack ='true')

blocks = np.arange(len(u)) +1 

plt.figure(figsize=(9,6))
plt.errorbar(blocks, u, yerr=erru, color='green')
plt.xlabel('Number of Blocks', fontsize=18)
plt.ylabel('U/N', fontsize=18)
plt.title('Energy', fontsize=22)
plt.grid(True)

plt.figure(figsize=(9,6))
plt.errorbar(blocks, c, yerr=errc, color='green')
plt.xlabel('Number of Blocks', fontsize=18)
plt.ylabel('C', fontsize=18)
plt.title('Heat Capacity', fontsize=22)
plt.grid(True)

plt.figure(figsize=(9,6))
plt.errorbar(blocks, m, yerr=errm, color='green')
plt.xlabel('Number of Blocks', fontsize=18)
plt.ylabel('M', fontsize=18)
plt.title('Magnetization', fontsize=22)
plt.grid(True)

plt.figure(figsize=(9,6))
plt.errorbar(blocks, x, yerr=errx, color='green')
plt.xlabel('Number of Blocks', fontsize=18)
plt.ylabel(r'$\chi$', fontsize=18)
plt.title('Susceptibility', fontsize=22)
plt.grid(True)

plt.show()