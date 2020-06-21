import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from scipy import constants as const
from scipy.constants import physical_constants
from math import sqrt

######## ARGON ########

sigma = 0.34e-9
T0 = 120
m = 39.948 * physical_constants['atomic mass constant'][0]
epsilon = T0 * const.k
time0 = sigma * sqrt(m / epsilon)

#######################


plt.rc('text', usetex=True)

#k = np.loadtxt("output_ekin.dat", usecols = (0), unpack ='true') #* epsilon
#v = np.loadtxt("output_epot.dat", usecols = (0), unpack ='true') #* epsilon
#e = np.loadtxt("output_etot.dat", usecols = (0), unpack ='true') #* epsilon
t = np.loadtxt("output_temp.dat", usecols = (0), unpack ='true') #* T0

time = np.arange(len(t))* 10 * 0.0005 #* time0 

#plt.plot(time, v, label="$V [J]$")
#plt.plot(time, k, label="$K [J]$")
#plt.plot(time, e, label="$E_m [J]$")
plt.plot(time, t)

plt.xlabel('Time')
plt.ylabel('Temperature')
plt.title('Liquid Phase')
plt.grid(True)
#plt.legend()

plt.show()