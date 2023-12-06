import datetime
import json
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import requests

def get_request(url, proxy):
    req = requests.get(url, proxies = proxy)

    if req.status_code != 200:
        raise Exception("Erro na requisicao")
    return req.content

url = "https://iiot-dta-default-rtdb.firebaseio.com/challenge02.json"

proxy = {
    "http": "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080",
    "https": "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080"
}
def update_data():
    dados = json.loads(get_request(url, proxy)) 

    dados_len = len(dados)

    indices = np.array([int(x[-2:])for x in dados.keys()])

    humidity = np.full(dados_len, np.nan, dtype = np.float64) #np.full preenche o array
    temperature = np.full(dados_len, np.nan, dtype =np.float64)

    for i, j in zip(range(dados_len), indices):
        try:
            humidity[i] = dados[f"subsys_{j:02}"]["humidity"]
            temperature[i] = dados[f"subsys_{j:02}"]["temperature"]
        except KeyError:
            pass

    humidity_mean = np.mean(humidity[~np.isnan(humidity)])
    humidity_min = np.min(humidity[~np.isnan(humidity)])
    humidity_max = np.max(humidity[~np.isnan(humidity)])
    temperature_mean = np.mean(temperature[~np.isnan(temperature)])
    temperature_min = np.min(temperature[~np.isnan(temperature)])
    temperature_max = np.max(temperature[~np.isnan(temperature)])
    

    return humidity_mean, humidity_min, humidity_max, temperature_mean, temperature_min, temperature_max

fig, axs = plt.subplots(2, sharex = True, figsize = (16, 8), gridspec_kw ={"hspace":0.4}) #o ultimo e o espaçamento entre os graficos ***axs são os eixos
fig.supxlabel("Tempo")
ax_humidity, ax_temperature = axs

cmap = mpl.colormaps["coolwarm"]

ax_humidity.grid()
ax_humidity.set_ylabel("Humidity")

ax_temperature.grid()
ax_temperature.set_ylabel("Temperature")

plt.ion()    #faz com que o gráfico fique dinâmico
fig.show()
fig.canvas.draw() #so inicializa o grafico

while True:
    tempo_atual = datetime.datetime.now()

    humidity_mean, humidity_min, humidity_max, temperature_mean, temperature_min, temperature_max = update_data()

    ax_humidity.plot(tempo_atual, humidity_mean, linestyle = '-', marker = 'o', markersize = 5, color = 'c')
    ax_humidity.plot(tempo_atual, humidity_min, linestyle = '-', marker = 'o', markersize = 5, color = 'b')
    ax_humidity.plot(tempo_atual, humidity_max, linestyle = '-', marker = 'o', markersize = 5, color = 'r')

    ax_temperature.plot(tempo_atual, temperature_mean, linestyle = '-', marker = 'o', markersize = 5, color = 'c')
    ax_temperature.plot(tempo_atual, temperature_min, linestyle = '-', marker = 'o', markersize = 5, color = 'b')
    ax_temperature.plot(tempo_atual, temperature_max, linestyle = '-', marker = 'o', markersize = 5, color = 'r')
    
    
    fig.canvas.draw()
    plt.pause(1)
