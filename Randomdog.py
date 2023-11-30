import json
import requests

url = "https://random.dog/woof.json"

proxy = {
    "http": "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080",
    "https": "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080"
}

auth = requests.auth.HTTPProxyAuth("disrct", "ets@bosch207")

req = requests.get(url, proxies = proxy, auth = auth)

if req.status_code != 200:
    raise Exception("Erro na requisicao")

dados = json.loads(req.content) 

url_imagem = dados["url"] #apenas o endereço da imagem, não a imagem em si

with open("cachorro.jpg", "wb") as image:
    req_image = requests.get(url_imagem, proxies = proxy, auth = auth)
    if req.status_code != 200:
        raise Exception("Erro na requisicao")
        
    image.write(req_image.content)
