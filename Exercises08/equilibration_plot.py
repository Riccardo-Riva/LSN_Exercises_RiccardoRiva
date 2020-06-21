import matplotlib
import matplotlib.pyplot as plt
import numpy as np

plt.rc('text', usetex=True)

r = np.loadtxt("equilibration.out", usecols = (0), delimiter=' ', unpack ='true')
x = np.arange(len(r)) + 1 

plt.figure(figsize=(9,6))
plt.plot(x, r, color='black')
plt.xlabel('Step', fontsize=18)
plt.ylabel(r'$|x|$', fontsize=18)
plt.title('$\mu = 1.0 \:\: \sigma = 2.0$', fontsize=22)
plt.grid(True)

plt.show()