
import datetime
import json
import matplotlib.pyplot as plt
import numpy as np
import requests
import time

def get_request(url, proxy):
    req = requests.get(url, proxies = proxy)

    if req.status_code != 200:
        raise Exception("Erro na requisicao")
    return req.content

url = "https://iiot-dta-default-rtdb.firebaseio.com/iiot-dta.json"

proxy = {
    "http": "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080",
    "https": "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080"
}

dados = json.loads(get_request(url, proxy)) 

dados_len = len(dados)

indices = np.array([int(x[-2:])for x in dados.keys()])

luminosity = np.full(dados_len, np.nan, dtype =np.float64) #np.full preenche o array
temp_sensor_00 = np.full(dados_len, np.nan, dtype =np.float64)
temp_sensor_01 = np.full(dados_len, np.nan, dtype =np.float64)

for i, j in zip(range(dados_len), indices):
    try:
        luminosity[i] = dados[f"subsys_{j:02}"]["luminosity"]
        temp_sensor_00[i] = dados[f"subsys_{j:02}"]["temperature"]["temp_sensor_00"]
        temp_sensor_01[i] = dados[f"subsys_{j:02}"]["temperature"]["temp_sensor_01"]
    except KeyError:
        pass

luminosity_mean = np.mean(luminosity)
temp_sensor_00_mean = np.mean(temp_sensor_00)
temp_sensor_01_mean = np.mean(temp_sensor_01)


fig, axs = plt.subplots(3, figsize = (16, 8), gridspec_kw ={"hspace":0.4}) #o ultimo e o espaçamento entre os graficos
fig.supxlabel("Tempo")
ax_luminosity, ax_temp_sensor_00, ax_temp_sensor_01 = axs

ax_luminosity.grid()
ax_luminosity.set_ylabel("Luminosidade")

ax_temp_sensor_00.grid()
ax_temp_sensor_00.set_ylabel("Temperatura Sensor 0")

ax_temp_sensor_01.grid()
ax_temp_sensor_01.set_ylabel("Temperatura Sensor 1")

plt.ion()
fig.show()
fig.canvas.draw()

while True:
    tempo_atual = datetime.datetime.now()
    ax_luminosity.plot(tempo_atual, luminosity_mean, linestyle = '-', marker = 'o', markersize = 5, color = 'c')
    ax_temp_sensor_00.plot(tempo_atual, temp_sensor_00_mean, linestyle = '-', marker = 'o', markersize = 5, color = 'b')
    ax_temp_sensor_01.plot(tempo_atual, temp_sensor_01_mean, linestyle = '-', marker = 'o', markersize = 5, color = 'm')
    
    fig.canvas.draw()
    time.sleep(15)
