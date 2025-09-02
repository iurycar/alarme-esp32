import requests
import time

# Endereço do IP do ESP32 mostrado no Serial Monitor
ENDERECO = "http://192.168.0.8/"  

# Token de autenticação, deve ser o mesmo que no ESP32
AUTH_TOKEN = "A7OmEetdXwYtEJ6b1eVccngugO89WxPm"

# Adicionar o token como parâmetro para URL
parametros = {'token': AUTH_TOKEN}

# Endpoints do ESP32
STATUS_ENDPOINT = "status" # Rota para solicitar o status do alarme
DADOS_ENDPOINT = "" # Rota para modificar o estado do alarme

def enviarDados():
    try:
        response = requests.get(ENDERECO + DADOS_ENDPOINT, params=parametros, timeout=5)
        if response.status_code == 200:
            print("Acesso bem-sucedido. Estado alterado no ESP32.")
        else:
            print(f"Falha ao acessar ESP32. Código: {response.status_code}")
            print(f"Mensagem: {response.text}")
    except Exception as e:
        print("Erro ao conectar:", e)

def solicitarDados():
    try:
        response = requests.get(ENDERECO + STATUS_ENDPOINT, params=parametros, timeout=5)
        if response.status_code == 200:
            print("Acesso bem-sucedido.")
            data = response.json()
            status = data.get('danificado', False)
            print(f"Estado atual do alarme: {'Ativado' if status else 'Desativado'}")
            return status
        else:
            print(f"Falha ao solicitar o status. Código: {response.status_code}")
            return False
    except requests.exceptions.RequestException as e:
        print("Erro ao conectar:", e)
        return False
    except ValueError:
        print("Erro ao decodificar a resposta JSON do ESP32.")
        return False


# Exemplo de uso: só ativa quando uma condição é satisfeita
if __name__ == "__main__":
    while True:
        condicao = int(input("Alterar estado? (0-False)(1-True):::> "))

        if condicao == 1:
            print(f"Embalagem danificada detectada, enviando comando ao alarme...")
            enviarDados()
        elif condicao == 0:
            break
    
        print("Testando...")
        status_alarme = solicitarDados()
        if status_alarme:
            print("Alarme está ATIVO.")
        else:
            print("Alarme está INATIVO.")