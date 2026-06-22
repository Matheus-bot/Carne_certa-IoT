<<<<<<< HEAD
# CarneCerta IoT - Sistema de Monitoramento Inteligente de Câmaras Frias

## 📋 Visão Geral

**CarneCerta IoT** é um sistema completo de monitoramento em tempo real para câmaras frias e balcões refrigerados de estabelecimentos que comercializam produtos congelados. O sistema integra um dispositivo ESP32 com sensores, atuadores e uma interface web bidireccional para monitoramento e controle remoto.

---

## 🎯 Objetivos do Projeto

1. **Monitoramento em Tempo Real**: Capturar e exibir dados de temperatura de múltiplas câmaras/balcões
2. **Detecção de Anomalias**: Identificar automaticamente quando a temperatura ultrapassa limites seguros
3. **Controle Remoto**: Permitir acionamento manual de alarmes e recebimento de alertas
4. **Interface Web Responsiva**: Dashboards intuitivos e modernos para operadores
5. **Comunicação Bidirecional**: Sincronização em tempo real entre dispositivo e servidor via WebSocket
6. **Simulação Testável**: Modo Wokwi para prototipagem e teste em ambiente seguro

---

## 🏗️ Arquitetura do Sistema

```
┌─────────────────────────────────────────────────────────────┐
│                    CAMADA DE APRESENTAÇÃO                  │
│  ┌──────────────────────┐         ┌───────────────────────┐ │
│  │  Dashboard Principal │         │ Dashboard Balcões     │ │
│  │  (index.html)        │         │ (balcao.html)         │ │
│  │  - Câmara Fria       │         │ - 4 Balcões           │ │
│  │  - Status de Porta   │         │ - Simulação           │ │
│  └──────────────────────┘         └───────────────────────┘ │
└────────────────────────────────────────────────────────────┘
                         ↕ (WebSocket)
┌─────────────────────────────────────────────────────────────┐
│                  CAMADA DE COMUNICAÇÃO                      │
│          Servidor Node.js (server.js)                       │
│  - WebSocket Server (ws://localhost:8080)                   │
│  - Roteador de Mensagens (Browser ↔ ESP32)                 │
│  - Broadcast de Dados de Sensores                           │
└─────────────────────────────────────────────────────────────┘
                         ↕ (WebSocket)
┌─────────────────────────────────────────────────────────────┐
│                  CAMADA DE DISPOSITIVO                      │
│            ESP32 com Sensores (sketch.ino)                  │
│  ┌──────────────┐  ┌──────────┐  ┌──────────┐              │
│  │  DHT22       │  │ Botão    │  │  LEDs    │              │
│  │  Temperatura │  │ Porta    │  │ Verde/   │              │
│  │  (DHTPIN=4)  │  │ (GPIO18) │  │ Vermelho │              │
│  └──────────────┘  └──────────┘  │(GPIO26/27)              │
│                                   │ Buzzer   │              │
│                                   │(GPIO25)  │              │
│  WiFi: Wokwi-GUEST (Simulado)    └──────────┘              │
│  WebSocket: ws://10.0.2.2:8080                             │
└─────────────────────────────────────────────────────────────┘
```

---

## 🔧 Funcionalidades Principais

### 1. **Monitoramento de Temperatura**
- Leitura periódica do sensor DHT22 (a cada 2 segundos)
- Envio automático de dados ao servidor via WebSocket
- Exibição em tempo real nos dashboards (com 1-2 segundos de latência)
- Histórico gráfico de leituras (últimas 20 amostras)

### 2. **Detecção de Status da Porta**
- Sensor de contato magnético (botão GPIO18)
- Indica se porta está aberta (ABERTA) ou fechada (FECHADA)
- Alteração do status visual do dashboard
- Geração automática de alerta se porta estiver aberta

### 3. **Sistema de Alertas**
- **Automáticos**: Quando temperatura > 4°C ou porta aberta
- **Manuais**: Botão no dashboard para acionar alarme remoto
- **Visuais**: Mudança de cores nos cards (verde → âmbar → vermelho)
- **Sonoros**: Buzzer ativo durante condição de alerta
- **Histórico**: Lista de alertas com timestamp no painel de alertas

### 4. **Controle Remoto**
- Acionamento remoto do alarme manual via botão "Acionar Alarme Manual"
- Desativação remota via botão "Desativar Alarme Manual"
- Simulação de falhas de refrigeração
- Simulação de oscilação de temperatura
- Restauração para operação normal

### 5. **Dashboard Principal (Câmara Fria)**
- **Card de Temperatura**: Exibe temperatura atual com indicador de status
- **Card de Porta**: Status da porta com alertas visual
- **Gráfico em Tempo Real**: Histórico das últimas leituras
- **Painel Rápido**: Resumo com últimos valores
- **Simulação**: Temperatura manual, abertura/fechamento de porta, alarmes manuais

### 6. **Dashboard Balcões Refrigerados**
- **4 Balcões**: Monitoramento independente (Bovinos, Frangos, Suínos, Especiais)
- **Resumo de Operação**: Contador de alertas, temperatura média, balcão mais quente
- **Simulação Avançada**: 
  - Aplicar temperatura manual a balcão específico
  - Simular falha de refrigeração generalizada
  - Simular oscilação de temperatura
  - Restaurar operação normal
- **Gráfico Multi-linha**: Histórico de 4 balcões simultaneamente

---

## 💻 Tecnologias Utilizadas

### **Backend**
| Tecnologia | Versão | Função |
|-----------|--------|--------|
| **Node.js** | v18+ | Runtime JavaScript no servidor |
| **Express.js** | v4+ | Framework Web server |
| **ws** (WebSockets) | v8+ | Comunicação em tempo real |
| **JavaScript (ES6+)** | - | Lógica do servidor |

### **Frontend**
| Tecnologia | Versão | Função |
|-----------|--------|--------|
| **HTML5** | - | Estrutura da interface |
| **Tailwind CSS** | via CDN | Estilos e design responsivo |
| **Chart.js** | v3+ | Gráficos em tempo real |
| **JavaScript (ES6+)** | - | Interatividade e lógica |
| **WebSocket API** | - | Comunicação com servidor |

### **Dispositivo (IoT)**
| Componente | Especificação | Função |
|-----------|---------------|--------|
| **Microcontrolador** | ESP32 | Processamento e conectividade Wi-Fi |
| **Sensor de Temperatura** | DHT22 | Leitura de temperatura/umidade |
| **Sensor de Porta** | Botão Push | Detecção de abertura de porta |
| **LEDs** | Verde/Vermelho | Indicação visual de status |
| **Buzzer** | Ativo 5V | Alerta sonoro |
| **Biblioteca WebSocket** | WebSocketsClient | Comunicação bidirecional |
| **Linguagem** | Arduino C++ | Programação do ESP32 |

### **Ambiente de Simulação**
| Ferramenta | Função |
|-----------|--------|
| **Wokwi** | Simulador online de Arduino/ESP32 |
| **WiFi Simulado** | Rede Wokwi-GUEST |
| **Componentes Virtuais** | DHT22, Botões, LEDs, Buzzer |

---

## 📡 Fluxo de Dados e Comunicação

### **Protocolo WebSocket**

#### 1️⃣ **Registro de Clientes**
```json
// ESP32 registra como cliente
{
  "type": "register",
  "source": "esp32",
  "client": "esp32"
}

// Browser registra como cliente
{
  "type": "register",
  "client": "browser"
}
```

#### 2️⃣ **Envio de Dados do ESP32 para Servidor**
```json
{
  "source": "esp32",
  "type": "sensor",
  "temperatura": 2.5,
  "porta": "fechada",
  "manualAlert": false,
  "timestamp": "2026-06-16T04:00:00Z"
}
```

#### 3️⃣ **Broadcast do Servidor para Browsers**
```json
{
  "source": "esp32",
  "type": "sensor",
  "temperatura": 2.5,
  "porta": "fechada",
  "manualAlert": false,
  "timestamp": "2026-06-16T04:00:00Z"
}
```

#### 4️⃣ **Envio de Comando do Browser para ESP32**
```json
{
  "source": "browser",
  "type": "command",
  "command": "manualAlert",
  "value": true
}
```

### **Fluxo de Execução**
```
[ESP32] Lê Sensor → [Server] Recebe e Valida → [Server] Broadcast → [Browser] Atualiza UI
  ↑                                                                          ↓
  └─────────────── [Browser] Envia Comando ← [Server] Roteia ←─────────┘
```

---

## 🚀 Como Usar o Sistema

### **1. Iniciar o Servidor**
```bash
cd c:\Users\HOME\OneDrive\Desktop\CarneCertaIoT
npm install  # (primeira vez apenas)
node server.js
```
✅ Servidor rodará em `http://localhost:8080`

### **2. Acessar as Interfaces**
- **Dashboard Principal**: http://localhost:8080/
- **Dashboard Balcões**: http://localhost:8080/balcao.html

### **3. Executar o ESP32**
- Abra o Wokwi
- Carregue o arquivo `sketch.ino`
- Clique em "Start Simulation"
- O ESP32 se conectará automaticamente ao servidor local

### **4. Testar Funcionalidades**

#### No Dashboard Principal:
- Insira uma temperatura e clique "Simular"
- Clique "Simular porta aberta" para teste de porta
- Clique "Acionar alarme manual" para teste de alerta
- Observe o gráfico atualizar em tempo real

#### No Dashboard Balcões:
- Insira temperatura e selecione um balcão
- Clique "Aplicar temperatura manual"
- Clique "Simular falha de refrigeração" para testar limite crítico
- Observe os cards mudarem de cor e o gráfico atualizar

---

## 📊 Limiares de Temperatura

| Status | Faixa de Temperatura | Cor | Ação |
|--------|----------------------|-----|------|
| **Normal** | -25°C a 2°C | 🟢 Verde | Operação segura |
| **Atenção** | 2°C a 4°C | 🟠 Âmbar | Alerta amarelo, log registrado |
| **Crítico** | > 4°C | 🔴 Vermelho | Alarme ativo, LEDs e Buzzer |

---

## 📂 Estrutura de Arquivos

```
CarneCertaIoT/
├── server.js                 # Servidor Node.js com WebSocket
├── sketch.ino               # Código do ESP32 (Arduino)
├── main.ino                 # Arquivo desativado (não compilar)
├── package.json             # Dependências Node.js
├── wokwi.toml              # Configuração Wokwi
├── diagram.json            # Diagrama de conexão Wokwi
├── libraries.txt           # Bibliotecas Arduino necessárias
├── public/
│   ├── index.html          # Dashboard Principal
│   └── balcao.html         # Dashboard Balcões
├── interface_web/          # (Legado, não utilizado)
└── README.md               # Esta documentação
```

---

## 🔌 Dependências

### **Node.js**
```json
{
  "dependencies": {
    "express": "^4.18.0",
    "ws": "^8.13.0"
  }
}
```

### **Arduino/ESP32**
```cpp
#include <WiFi.h>                    // Conectividade Wi-Fi
#include <WebSocketsClient.h>        // Cliente WebSocket
#include <DHT.h>                     // Sensor DHT22
```

---

## ✅ Checklist de Funcionalidades

- ✅ Monitoramento de temperatura em tempo real
- ✅ Detecção de status de porta
- ✅ Alertas automáticos por temperatura/porta
- ✅ Alertas manuais via botão remoto
- ✅ Dashboard principal com gráfico
- ✅ Dashboard balcões com 4 câmaras
- ✅ Simulação de falhas e oscilações
- ✅ Histórico de alertas com timestamp
- ✅ Interface responsiva e moderna
- ✅ WebSocket bidirecional funcionando
- ✅ LEDs e Buzzer no ESP32
- ✅ Comunicação Wokwi-Servidor integrada

---

## 🛠️ Status Atual

| Componente | Status | Notas |
|-----------|--------|-------|
| **Servidor Node.js** | ✅ Funcionando | Porta 8080, todos endpoints ativos |
| **Dashboard Principal** | ✅ Funcionando | Todos botões e gráfico OK |
| **Dashboard Balcões** | ✅ Funcionando | Todos botões e simulação OK |
| **ESP32 Wokwi** | ✅ Pronto | Conecta automaticamente ao servidor |
| **Comunicação WebSocket** | ✅ Bidirecional | Dados e comandos sincronizando |
| **Interface Web** | ✅ Responsiva | Design moderno com Tailwind CSS |
| **Alertas e LEDs** | ✅ Funcionando | Acionados por temperatura/porta |

---

## 📝 Exemplos de Uso

### Cenário 1: Temperatura Elevada
1. Temperatura sobe para 5°C
2. ESP32 envia dados ao servidor
3. Servidor broadcast para browsers
4. Dashboard muda para vermelho
5. LED vermelho acende
6. Buzzer toca
7. Alerta aparece no painel

### Cenário 2: Porta Aberta
1. Porta abre (botão GPIO18 ativado)
2. ESP32 detecta mudança
3. Envia `"porta": "aberta"` ao servidor
4. Dashboard atualiza para "ABERTA"
5. Card muda de cor para rosso
6. Alarme ativado automaticamente

### Cenário 3: Comando Remoto
1. Usuário clica "Acionar Alarme Manual"
2. Browser envia comando via WebSocket
3. Servidor roteia para ESP32
4. ESP32 recebe `manualAlert: true`
5. LED e Buzzer acionados
6. Alerta registrado no histórico

---

## 🎓 Tecnologias Aprendidas

Este projeto integra conhecimentos em:
- **IoT e Sensores**: Leitura de DHT22, GPIO
- **Comunicação em Tempo Real**: WebSocket bidireccional
- **Arquitetura Cliente-Servidor**: Roteamento de mensagens
- **Frontend Moderno**: HTML5, CSS3, JavaScript ES6+
- **Backend Node.js**: Express e gerenciamento de conexões
- **Prototipagem**: Simulação com Wokwi
- **Design Responsivo**: Interface adaptável a diferentes telas

---

## 📞 Suporte e Próximas Melhorias

### Possíveis Melhorias Futuras:
- [ ] Persistência em banco de dados (MongoDB, PostgreSQL)
- [ ] Autenticação e autorização de usuários
- [ ] Dashboard em aplicação mobile
- [ ] Notificações por email/SMS
- [ ] Integração com sistemas HVAC
- [ ] Armazenamento de histórico na nuvem
- [ ] API REST adicional
- [ ] Múltiplos ESP32 simultâneos
- [ ] Configuração dinâmica de limiares

---

## 📄 Licença

Projeto CarneCerta IoT - 2026

---

**Versão da Documentação**: 1.0  
**Data de Atualização**: 16 de Junho de 2026  
**Status**: Sistema Operacional Completo ✅
=======
# 🥩 CarneCerta IoT

Sistema Inteligente de Monitoramento de Câmaras Frias e Balcões Refrigerados utilizando Internet das Coisas (IoT).

O **CarneCerta IoT** foi desenvolvido para auxiliar açougues, frigoríficos e estabelecimentos que trabalham com produtos refrigerados no monitoramento contínuo da temperatura e do status operacional de equipamentos críticos para a conservação dos alimentos.

A solução utiliza sensores conectados a um ESP32 para identificar condições inadequadas de armazenamento, emitindo alertas visuais e sonoros em tempo real e fornecendo informações através de uma interface web moderna e intuitiva.

---

# Objetivo

Garantir a conservação adequada dos alimentos através do monitoramento inteligente de ambientes refrigerados, permitindo a identificação rápida de falhas que possam comprometer a qualidade dos produtos armazenados.

---

# Funcionalidades

## 🌡️ Monitoramento de Temperatura

- Leitura contínua da temperatura da câmara fria
- Identificação automática de temperaturas fora da faixa segura
- Exibição dos dados em tempo real

##     Controle de Abertura de Porta

- Simulação de sensor de abertura de porta
- Detecção imediata de acessos não autorizados ou prolongados

##  Sistema de Alertas

- Alerta visual através de LEDs
- Alerta sonoro através de buzzer
- Acionamento automático em situações críticas

##  Histórico de Dados

- Registro das últimas leituras
- Visualização gráfica da evolução da temperatura
- Acompanhamento de eventos e ocorrências

##  Monitoramento de Balcões Refrigerados

- Controle da temperatura dos balcões de exposição
- Classificação por níveis de risco
- Simulação de falhas de refrigeração

## 🌐 Dashboard Web

- Interface moderna e responsiva
- Atualização de informações em tempo real
- Visualização simplificada para operadores e gestores
---
#  Tecnologias Utilizadas

## Hardware

- ESP32 DevKit V1
- Sensor DHT22
- LEDs de sinalização
- Buzzer
- Botões de simulação

## Front-end

- HTML5
- CSS3
- JavaScript (ES6+)
- Chart.js
- Responsive Design

## Back-end

- Node.js
- Express.js

## IoT e Sistemas Embarcados

- ESP32
- Arduino Framework
- C++
- Internet das Coisas (IoT)
- Sensoriamento em Tempo Real
- Automação de Processos

## Simulação e Hospedagem

- Wokwi Simulator
- Render
- GitHub

---

#  Demonstração

## Dashboard Principal

<img width="500" alt="Dashboard Principal" src="https://github.com/user-attachments/assets/d838a176-a83b-4bd9-ae5e-a4609d04d894">

---

## Monitoramento da Câmara Fria

<img width="600" alt="Monitoramento da Câmara Fria" src="https://github.com/user-attachments/assets/bd74613c-f75e-4385-9e69-21793c4c6018">

---

## Monitoramento de Balcões Refrigerados

<img width="500" alt="Monitoramento de Balcões Refrigerados" src="https://github.com/user-attachments/assets/d7771672-b901-4e00-9386-75c6618e7037">

---

# 🔗 Acessar Projeto

## 🌐 Aplicação Web

https://carne-certa-iot.onrender.com

## 🔧 Simulação ESP32 (Wokwi)

https://wokwi.com/projects/464368907720179713

---
# Contexto Acadêmico

Projeto desenvolvido para a disciplina de Projeto Integrador com foco em Internet das Coisas (IoT), explorando conceitos de automação, monitoramento remoto, sensores inteligentes, sistemas embarcados e dashboards interativos aplicados ao setor alimentício.

---
>>>>>>> 1cd98a16935cb5f0289ee835a4494f5c8378a6ed
