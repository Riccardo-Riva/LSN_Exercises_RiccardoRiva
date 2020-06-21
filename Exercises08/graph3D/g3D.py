import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D

plt.rc('text', usetex=True)
plt.rc('font', family='arial')

E = np.loadtxt('energy_estimates.dat')

mu = np.linspace(0.2, 1.2, 101)
sigma = np.linspace(0.2, 1.2, 101)
MU, SIGMA = np.meshgrid(mu, sigma)

fig = plt.figure(figsize=(10,10))
ax = fig.add_subplot(111, projection='3d')

ax.plot_wireframe(MU, SIGMA, E, rstride=2, cstride=2)
ax.set_xlabel('$\mu$')
ax.set_ylabel('$\sigma$')
ax.set_zlabel('$E$')
ax.text2D(0.05, 0.95, 'Energy', transform=ax.transAxes, fontsize=20)
ax.view_init(10, 30)

plt.show()