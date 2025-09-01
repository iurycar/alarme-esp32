## Introdução

Este repositório contém o código e a documentação para um sistema de alarme simples, baseado em um ESP32, que pode ser ativado via requisições HTTP. O projeto foi desenvolvido como um componente secundário para um sistema de visão computacional que detecta embalagens danificadas, mas pode ser facilmente adaptado para outros projetos.

## Como funciona

O ESP32 atua como um servidor web que controla um buzzer e um LED. Quando recebe uma requisição HTTP com um token de autenticação válido, ele ativa o alarme. O sistema também possui um botão físico para desativar o alarme manualmente.

O projeto de visão computacional inclui um script Python (`enviarDados.py`), responsável por enviar um comando HTTP para o ESP32, ativando o sistema de alarme. Este script utiliza a biblioteca `requests` para fazer a requisição de forma simples.

#### Script

O script realiza as seguintes etapas:
1. **Importação da biblioteca `requests`:** Utilizada para fazer requisições HTTP em Python. Pode ser instalada com pip `install requests`.
2. **Configuração da URL e token:** As constantes `ENDERECO` e `AUTH_TOKEN` são definidas para o endereço IP do ESP32 e o token de autenticação correspondente.
3. **Envio da requisição:** A função `enviarDados()` utiliza o método `requests.get()`.

## Pré-requesitos

* **1x** ESP32
* **1x** Buzzer
* **1x** LED
* **1x** Resistor de 330Ω
* **1x** Push-button
* **1x** Cabos de conexão (`jumper wires`)
* **1x** Protoboard (opcional)

## Diagrama de circuito
![Diagrama](/assets/diagrama.jpg)

* **LED:** Conecte o pino positivo (ânodo) do LED ao **GPIO 26** do ESP32, e o pino negativo (catodo) ao GND através de um resistor de 330Ω.
* **Buzzer:** Conecte o pino positivo do buzzer ao **GPIO 33** e o pino negativo ao GND.
* **Botão:** Conecte um lado do botão ao **GPIO 18** e o outro lado ao GND. O código utiliza o `INPUT_PULLUP` interno do ESP32, então não é necessário um resistor externo.

## Como usar

#### 1. Carregue o código no ESP32
1. Abra o arquivo `alarme_esp32.ino` na IDE do Arduino.
2. Instale a biblioteca `WebServer` se ainda não a tiver.
3. Modifique o `ssid` e `password` no código para as credenciais da sua rede Wi-Fi.
4. Defina um **token de autenticação** na constante `AUTH_TOKEN`.
5. Conecte o ESP32 ao seu computador e carregue o código.

#### 2. Ative e desative o alarme
Para ativar o alarme, envie uma requisição HTTP GET para o endereço IP do seu ESP32, incluindo o token de autenticação como um parâmetro de URL.

##### Exemplo de URL:
`http://[IP_DO_SEU_ESP32]/?token=[SEU_TOKEN]`

A requisição pode ser feita a partir do navegador, um script Python (como mostrado no arquivo `enviarDados.py`) ou qualquer outro cliente HTTP. O alarme será ativado apenas se o token for válido.

Para desativar o alarme pode ser feito manualmente pressionando o botão, ou implementando no código uma requisição para desativar. Por exemplo, adicionando outra rota ao servidor web, como `/desligar`.