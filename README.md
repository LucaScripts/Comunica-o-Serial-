Claro! Aqui está o README reformulado de forma mais atrativa, seguindo o estilo que você mencionou:

---

# 🌟 **Projeto de Comunicação Serial com RP2040 e BitDogLab**

📌 **Autor**: Lucas Dias  
📆 **Data**: 07/02/2025

Este projeto integra a comunicação serial com o **RP2040** e a placa de desenvolvimento **BitDogLab**, explorando o controle de LEDs e a interação com botões. O código e a estrutura do projeto foram baseados no repositório do professor Wilton para um melhor aprendizado e desenvolvimento prático.

🔗 **Repositório do Professor Wilton**: [EmbarcaTechU4C6](https://github.com/wiltonlacerda/EmbarcaTechU4C6.git)

---

## 📌 **Objetivos**
✅ **Compreensão da comunicação serial** em microcontroladores.  
✅ **Aplicação prática de UART e I2C** para controle de dispositivos.  
✅ **Manipulação de LEDs comuns e WS2812**.  
✅ **Implementação de interrupções e debouncing** para botões.  
✅ **Desenvolvimento de um sistema embarcado** funcional, combinando hardware e software.

---

## 🛠 **Materiais Necessários**
🔹 1x **Placa BitDogLab**  
🔹 1x **Matriz 5x5 de LEDs WS2812** (GPIO 7)  
🔹 1x **LED RGB** (GPIOs 11, 12, 13)  
🔹 2x **Botões** (GPIO 5 e 6)  
🔹 1x **Display SSD1306** (I2C, GPIOs 14 e 15)

---

## 🏗 **Esquema de Ligação**
| Componente        | Pino do BitDogLab   |
|-------------------|---------------------|
| Matriz de LEDs    | GPIO 7              |
| LED RGB Verde     | GPIO 11             |
| LED RGB Vermelho  | GPIO 12             |
| LED RGB Azul      | GPIO 13             |
| Botão A           | GPIO 5              |
| Botão B           | GPIO 6              |
| Display SSD1306   | GPIO 14 e GPIO 15   |

**⚠️ OBS**: Conecte o lado positivo dos LEDs e botões aos pinos correspondentes e o lado negativo ao GND.

---

## 📜 **Funcionalidades do Projeto**
### 1️⃣ **Modificação da Biblioteca `font.h`**
- Criação de novos caracteres minúsculos para exibição no display SSD1306.

### 2️⃣ **Entrada de Caracteres via PC**
- Caracteres digitados no **Serial Monitor** são exibidos no display SSD1306.
- Números entre 0 e 9 acionam a exibição do símbolo correspondente na matriz 5x5 de LEDs WS2812.

### 3️⃣ **Interação com o Botão A**
- Alterna o estado do **LED RGB Verde** ao pressionar o botão A.
- Mensagem sobre o estado do LED é exibida no display SSD1306 e enviada ao Serial Monitor.

### 4️⃣ **Interação com o Botão B**
- Alterna o estado do **LED RGB Azul** ao pressionar o botão B.
- Mensagem sobre o estado do LED é exibida no display SSD1306 e enviada ao Serial Monitor.

---

## 🧰 **Requisitos do Projeto**
- **Uso de Interrupções** para botões com **debouncing** implementado via software.
- **Controle de LEDs** comuns e WS2812.
- **Exibição de texto no display SSD1306** utilizando fontes maiúsculas e minúsculas.
- **Envio de informações via UART** para depuração.
- **Código organizado e bem documentado**.

---

## 📥 **Instalação e Execução**

### 1️⃣ **Configuração do Ambiente**
Certifique-se de ter o **Pico SDK** instalado e configurado no seu sistema.

Se necessário, consulte o guia oficial:  
🔗 [Guia Oficial do Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk)

### 2️⃣ **Clonando o Repositório e Enviando para o RP2040**
Clone o repositório e compile o código para a placa utilizando o **VS Code** e o **Pico SDK**.

---

## 🎯 **Funcionamento**
- **Botão A** alterna o **LED Verde**.
- **Botão B** alterna o **LED Azul**.
- **Display SSD1306** exibe o estado dos LEDs em tempo real.
- **Matriz de LEDs WS2812** exibe números de 0 a 9 quando digitados no Serial Monitor.

---

## 🚦 **Demonstração do Funcionamento**

Veja abaixo uma prévia de como o projeto se comporta na placa **BitDogLab**:

![Demonstração do Projeto](https://github.com/LucaScripts/Clock-e-Temporizadores/blob/main/docs/Clock-e-Temporizadores-2%20-%20Visual%20Studio%20Code%202025-01-31%2019-22-00.gif?raw=true)

---

## 📌 **Melhorias Futuras**
🚀 **Integração com outras interfaces de comunicação**, como Bluetooth ou Wi-Fi.  
🛠 **Adição de novos modos de controle** para LEDs, como piscante ou sequencial.  
📡 **Controle remoto** via redes sem fio.

---

Agora, o README está mais estruturado, com emojis e títulos claros para facilitar a leitura e tornar a apresentação mais atrativa.
