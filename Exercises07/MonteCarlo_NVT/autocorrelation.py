import matplotlib
import matplotlib.pyplot as plt
import numpy as np

plt.rc('text', usetex=True)

u = np.loadtxt("v_instant_values.out", usecols = (0), delimiter=' ', unpack ='true')
p = np.loadtxt("p_instant_values.out", usecols = (0), delimiter=' ', unpack ='true')

x = np.arange(len(u)) + 1


############ Autocorrelation Function #############

steps = np.arange(2, 5001, 20)
ac_u = np.array([]) #this contains the values of the autocorrelation function for potential energy
ac_p = np.array([]) #this contains the values of the autocorrelation function for pressure
 
sumu = 0.0
sum2u = 0.0
sump = 0.0
sum2p = 0.0

for i in range(len(u)):
	sumu += u[i]
	sum2u += u[i]**2
	sump += p[i]
	sum2p += p[i]**2

sumu /= len(u)
sum2u /= len(u)
sump /= len(p)
sum2p /= len(p)

var_u = sum2u - sumu**2
var_p = sum2p - sump**2

for i in steps:
	print(i)
	jmax = len(x) - i
	sumu_12 = 0.0
	sumu_1 = 0.0
	sumu_2 = 0.0
	sump_12 = 0.0
	sump_1 = 0.0
	sump_2 = 0.0
	
	for j in range(jmax):
		sumu_12 += u[j]*u[j+i]
		sumu_1 += u[j]
		sumu_2 += u[j+i]
		sump_12 += p[j]*p[j+i]
		sump_1 += p[j]
		sump_2 += p[j+i]
	sumu_12 /= jmax
	sumu_1 /= jmax
	sumu_2 /= jmax
	sump_12 /= jmax
	sump_1 /= jmax
	sump_2 /= jmax

	ac_u = np.append(ac_u, (sumu_12 - sumu_1*sumu_2)/var_u)
	ac_p = np.append(ac_p, (sump_12 - sump_1*sump_2)/var_p)

######## Uncertainties in Data Blocking ##########

L = np.arange(20, 5001, 20) # Block Length
err_u = np.array([])
err_p = np.array([])

for l in L:
	print(l)
	N = int(len(u)/l) # Number of blocks
	prog_u = 0.0
	prog2_u = 0.0
	prog_p= 0.0
	prog2_p = 0.0

	for i in range(N):
		sum_u = 0.0
		sum_p = 0.0
		for j in range(l):
			sum_u += u[i*l + j]
			sum_p += p[i*l + j]
		
		stima_u = sum_u/l
		stima_p = sum_p/l
		
		prog_u += stima_u
		prog_p += stima_p

		prog2_u += stima_u*stima_u
		prog2_p += stima_p*stima_p

	err_u = np.append(err_u, (prog2_u/N - (prog_u/N)**2)/(N-1))
	err_p = np.append(err_p, (prog2_p/N - (prog_p/N)**2)/(N-1))

############# PLOTS ######################

plt.figure(figsize=(9,6))
plt.plot(steps, ac_u, color='black')
plt.xlabel('step $n$', fontsize=18)
plt.ylabel(r'$Ac_{[u]}(n)$', fontsize=18)
plt.title('Autocorrelation Function - Potential Energy', fontsize=22)
plt.grid(True)

plt.figure(figsize=(9,6))
plt.plot(steps, ac_p, color='black')
plt.xlabel('step $n$', fontsize=18)
plt.ylabel(r'$Ac_{[P]}(n)$', fontsize=18)
plt.title('Autocorrelation Function - Pressure', fontsize=22)
plt.grid(True)

plt.figure(figsize=(9,6))
plt.plot(L, err_u, color='black')
plt.xlabel('Block Length', fontsize=18)
plt.ylabel(r'$Error$', fontsize=18)
plt.title('Uncertainties - Potential Energy', fontsize=22)
plt.grid(True)

plt.figure(figsize=(9,6))
plt.plot(L, err_p, color='black')
plt.xlabel('Block Length', fontsize=18)
plt.ylabel(r'$Error$', fontsize=18)
plt.title('Uncertainties - Pressure', fontsize=22)
plt.grid(True)

plt.show()

