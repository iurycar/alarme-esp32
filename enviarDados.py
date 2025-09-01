import requests
import time

# Endereço do IP do ESP32 mostrado no Serial Monitor
ENDERECO = "http://192.168.0.8/"  

# Token de autenticação, deve ser o mesmo que no ESP32
AUTH_TOKEN = "A7OmEetdXwYtEJ6b1eVccngugO89WxPm"

def enviarDado():
    # Adicionar o token como parâmetro para URL
    parametros = {'token': AUTH_TOKEN}
    try:
        response = requests.get(ENDERECO, params=parametros, timeout=5)
        if response.status_code == 200:
            print("Acesso bem-sucedido. Estado alterado no ESP32.")
        else:
            print(f"Falha ao acessar ESP32. Código: {response.status_code}")
            print(f"Mensagem: {response.text}")
    except Exception as e:
        print("Erro ao conectar:", e)


# Exemplo de uso: só ativa quando uma condição é satisfeita
if __name__ == "__main__":
    while True:
        condicao = int(input("(0-False)(1-True):::> "))

        if condicao == 1:
            print(f"Embalagem danificada detectada, enviando comando ao alarme...")
            enviarDado()