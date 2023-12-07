import json
import requests
import numpy as np
import matplotlib as mbl
import matplotlib.pyplot as plt
from datetime import datetime

class Fedalto:
    mean: float
    min: float
    max: float

    def __init__(self, mean, min, max):
        self.mean = mean
        self.min = min
        self.max = max

    def __str__(self) -> str:
        return f"{self.mean =} {self.min =}{self.max =}"
    


def get_request(url, proxies=None, auth=None):
    try:
        req = requests.get(url, proxies=proxies, auth=auth)
    
    except:
        raise Exception(f"erro na requisição: {req.status_code}")
    
    return req.content

firebase_url = "" #url do banco
proxy = {"",""} #http e https

def get_current_data(firebase_url, proxies=proxy):
    data = json.loads(get_request(firebase_url))

    data_len = len(data)

    indexs = np.array([int(x[-2:]) for x in data.keys()]) 
    humidity = np.full(data_len, np.nan, dtype=np.float64)
    temperature = np.full(data_len, np.nan, dtype=np.float64)

    for i, j in zip(range(data_len), indexs):
        humidity[i]= data[f"subsys_{j:02}"]["humidity"]
        temperature[i]= data[f"subsys_{j:02}"]["temperature"]

    humidity_data=Fedalto(np.mean(humidity), np.min(humidity), np.max(humidity) )
    temperature_data = Fedalto( np.mean(temperature), np.min(temperature), np.max(temperature) )


    return humidity_data, temperature_data


def normalize(x, avg, min, max):
    return (x - avg)/(max - min)


# MOSTRANDO GRAFICOS SEPARADOS
# plt.plot(indexs, humidity, label = "umidade")
# plt.grid()
# plt.legend()
# plt.show()

# plt.plot(indexs, temperature, label = "temperatura")
# plt.grid()
# plt.legend()
# plt.show()
cmap = mbl.colormaps["coolwarm"]

fig, axs = plt.subplots(2, sharex=True, gridspec_kw={"hspace":0.4}, figsize=(16,8), dpi=600)
ax_humidity, ax_temperature = axs

ax_humidity.legend()
ax_humidity.grid()
ax_temperature.legend()
ax_temperature.grid()
ax_temperature.set_ylabel("Temperatura")
ax_humidity.set_ylabel("Umidade")

fig.supxlabel("Tempo")
fig.suptitle("Revisão")

last_time = datetime.now()
humidity_last = Fedalto(None, None, None)
temperature_last = Fedalto(None, None, None)

while(True):
    current_time = datetime.now()
    humidity, temperature =  get_current_data(firebase_url, proxy)

    ax_humidity.plot([current_time, last_time], [humidity.mean, humidity_last.mean], marker="o", markersize =4, color = cmap(normalize(humidity.mean, 50, 0, 100)), label="mean")
    ax_humidity.plot([current_time, last_time], [humidity.min, humidity_last.min], marker="o", markersize =4, color = cmap(normalize(humidity.min, 50, 0, 100)), label="min")
    ax_humidity.plot([current_time, last_time], [humidity.max, humidity_last.max], marker="o", markersize =4, color = cmap(normalize(humidity.max, 50, 0, 100)), label="max")
    
    ax_temperature.plot([current_time, last_time], [temperature.mean, temperature_last.mean], marker="o", markersize =4, color = cmap(normalize(temperature.mean, 23, 20, 30)), label="mean")
    ax_temperature.plot([current_time, last_time], [temperature.min, temperature_last.mix], marker="o", markersize =4, color = cmap(normalize(temperature.min, 23, 20, 30)), label="min")
    ax_temperature.plot([current_time, last_time], [temperature.max, temperature_last.max], marker="o", markersize =4, color = cmap(normalize(temperature.max, 23, 20, 30)), label="max")
    
    plt.ion()
    fig.show() #interativa
    fig.canvas.draw() #desenha nosso desenho

    humidity_last = humidity
    temperature_last = temperature
    last_time = current_time
    plt.pause(1)
