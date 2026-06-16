const express = require('express');
const http = require('http');
const WebSocket = require('ws');

const app = express();
const port = process.env.PORT || 8080;

app.use(express.static('public'));

const server = http.createServer(app);
const wss = new WebSocket.Server({ server });
let lastReading = null;

function broadcast(data) {
  wss.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(JSON.stringify(data));
    }
  });
}

wss.on('connection', (ws, req) => {
  console.log('WebSocket client connected:', req.socket.remoteAddress);

  if (lastReading) {
    ws.send(JSON.stringify(lastReading));
  }

  ws.on('message', (message) => {
    try {
      const payload = JSON.parse(message.toString());
      if (
        Object.prototype.hasOwnProperty.call(payload, 'temperatura') &&
        Object.prototype.hasOwnProperty.call(payload, 'porta')
      ) {
        const data = {
          temperatura: Number(payload.temperatura),
          porta: String(payload.porta),
          timestamp: new Date().toISOString(),
        };
        lastReading = data;
        console.log('Recebido do ESP32:', data);
        broadcast(data);
      }
    } catch (error) {
      console.warn('Mensagem WebSocket inválida:', error.message);
    }
  });

  ws.on('close', () => {
    console.log('WebSocket client disconnected');
  });
});

server.listen(port, () => {
  console.log(`Servidor rodando na porta ${port}`);
  console.log('Servindo arquivos estáticos de /public');
});
