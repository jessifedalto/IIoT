import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

NTC10K_RES = np.array([1214600, 844390, 592430, 419380, 299480,
                       215670, 156410, 114660, 84510, 62927,
                       47077, 35563, 27119, 20860, 16204,
                       12683, 10000, 7942, 6327, 5074,
                       4103, 3336, 2724, 2237, 1846,
                       1530, 1275, 1068, 899.3, 760.7,
                       645.2, 549.4, 470.0, 403.6, 347.4,
                       300.1, 260.1],
                      dtype=np.float64)

NTC10K_TEMP = np.array([-55, -50, -45, -40, -35,
                        -30, -25, -20, -15, -10,
                        -5, 0, 5, 10, 15,
                        20, 25, 30, 35, 40,
                        45, 50, 55, 60, 65,
                        70, 75, 80, 85, 90,
                        95, 100, 105, 110, 115,
                        120, 125],
                       dtype=np.float64)

def function(x, a, b, c):
    return a * np.log(x - b) + c
    # return a / (x -  b) + c

popt, pcov = curve_fit(function, NTC10K_RES, NTC10K_TEMP)

error = np.max(function(NTC10K_RES, *popt) - NTC10K_TEMP)

print(f"{popt =}error_max = { error}")

x = np.linspace(0, 1250000, 1000)

plt.plot(NTC10K_RES, NTC10K_TEMP)

plt.plot(x, function(x, *popt))
plt.grid()
plt.show()
