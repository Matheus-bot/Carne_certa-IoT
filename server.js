const express = require('express');
const http = require('http');
const WebSocket = require('ws');

const app = express();
const port = process.env.PORT || 8080;

app.use(express.static('public'));

const server = http.createServer(app);
const wss = new WebSocket.Server({ server });
let lastReading = null;
const browserClients = new Set();
let espClient = null;

function broadcastToBrowsers(data) {
  browserClients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(JSON.stringify(data));
    }
  });
}

wss.on('connection', (ws, req) => {
  ws.clientType = null;
  console.log('WebSocket client connected:', req.socket.remoteAddress);

  if (lastReading) {
    ws.send(JSON.stringify(lastReading));
  }

  ws.on('message', (message) => {
    try {
      const payload = JSON.parse(message.toString());

      if (payload.type === 'register' && payload.client === 'esp32') {
        ws.clientType = 'esp32';
        espClient = ws;
        console.log('ESP32 conectado via WebSocket');
        return;
      }

      if (payload.type === 'register' && payload.client === 'browser') {
        ws.clientType = 'browser';
        browserClients.add(ws);
        console.log('Browser conectado via WebSocket');
        return;
      }

      if (payload.source === 'esp32' && payload.type === 'sensor') {
        ws.clientType = 'esp32';
        espClient = ws;

        const data = {
          source: 'esp32',
          type: 'sensor',
          temperatura: Number(payload.temperatura),
          porta: String(payload.porta),
          manualAlert: Boolean(payload.manualAlert),
          timestamp: new Date().toISOString(),
        };

        lastReading = data;
        console.log('Leitura do ESP32:', data);
        broadcastToBrowsers(data);
        return;
      }

      if (payload.source === 'browser' && payload.type === 'command') {
        ws.clientType = 'browser';
        browserClients.add(ws);

        if (espClient && espClient.readyState === WebSocket.OPEN) {
          espClient.send(JSON.stringify(payload));
          ws.send(JSON.stringify({ type: 'status', message: 'Comando enviado ao ESP32.' }));
        } else {
          ws.send(JSON.stringify({ type: 'error', message: 'ESP32 não está conectado.' }));
        }
        return;
      }

      if (
        Object.prototype.hasOwnProperty.call(payload, 'temperatura') &&
        Object.prototype.hasOwnProperty.call(payload, 'porta')
      ) {
        ws.clientType = 'browser';
        browserClients.add(ws);

        const data = {
          temperatura: Number(payload.temperatura),
          porta: String(payload.porta),
          timestamp: new Date().toISOString(),
        };
        lastReading = data;
        console.log('Dados simulados recebidos do browser:', data);
        broadcastToBrowsers(data);
      }
    } catch (error) {
      console.warn('Mensagem WebSocket inválida:', error.message);
    }
  });

  ws.on('close', () => {
    if (ws.clientType === 'esp32' && espClient === ws) {
      espClient = null;
    }
    if (ws.clientType === 'browser') {
      browserClients.delete(ws);
    }
    console.log('WebSocket client disconnected');
  });
});

server.listen(port, () => {
  console.log(`Servidor rodando na porta ${port}`);
  console.log('Servindo arquivos estáticos de /public');
});
