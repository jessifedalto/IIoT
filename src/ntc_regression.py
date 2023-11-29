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

#Função simples: 
# def function(x, a, b, c):
#     return a * np.log(x - b) + c

#RESULT: popt =array([-19.49123972, 204.88328885, 208.78577114])    error_max = 5.601230861917969
    # return a / (x -  b) + c

#Função um pouco melhor: 
# def function(x, a, b, c, d):
#     return a / ((x-b)**d) + c
#RESULT: popt =array([ 5.32288605e+02,  3.74724144e+01, -1.69700640e+02,  1.09131102e-01])       error_max = 0.6839268926885751


#Função média:
# def function(x, a, b, c, d, e):
#     return a / ((x-b)**0.2) + c * np.log(x-d) + e
#popt =array([400.58960771,  -5.95789302,  -8.46980631,  42.3907488 , 39.53922748])         error_max = 0.18971598035081882

def function(x, a, b, c, d, e):
    return b*(np.log(x - a))**3 + c*(np.log(x-a))**2 + d * (np.log(x - a)) + e
#popt =array([ 1.29611833e+01, -7.61498101e-02,  3.40889111e+00, -6.45957953e+01, 3.90218793e+02])       error_max = 0.06365088052945111

popt, pcov = curve_fit(function, NTC10K_RES, NTC10K_TEMP)

x = np.linspace(0, 1250000, 1000)

plt.plot(NTC10K_RES, NTC10K_TEMP)

plt.plot(x, function(x, *popt))

plt.grid()
plt.show()

error_max = np.max(function(NTC10K_RES, *popt) - NTC10K_TEMP)

print(f"{popt =}error_max = {error_max}")






