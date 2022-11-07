const express = require('express');
const { spawnGameServer, killGameServer } = require('./gameServerManager');
const app = express();
app.use(express.json());

const http = require('http');
const httpServer = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(httpServer);

io.on('connection', (socket) => {
  console.log('A player is connected to webSocket');

  socket.on('disconnect', () => {
    console.log('A player disconnected from webSocket');
  });
});

let gameServers = [];
const MAX_PORT = 7800;
let lastPort = 7777;

app.post('/api/matchmaking/start', (req, res) => {
  try{
   lastPort++;
  
    // security example, probably not best UX though
    if (lastPort > MAX_PORT) {
      res.status(500).send('No more ports available. Try again later.\n');
      return;
    }

    const gameServerProcess = spawnGameServer(lastPort);
    gameServers.push({ port: lastPort, players: [], process: gameServerProcess });
    // console.log(gameServers);

    res.status(200).json({ "port": lastPort });
  } catch (error) {
    res.status(500).send('Failed creating a new server. Try again later.\n');
  }
});

app.post('/api/matchmaking/:serverId/join', (req, res) => {
  const { serverId } = req.params;
  const { data } = req.body;

  try{
    const foundServer = gameServers.find(server => server.port === parseInt(serverId, 10));
    if (!foundServer) {
      res.status(400).send(`There is no server running on port ${serverId}\n`);
      return;
    }
    
    foundServer.players.push(data);
    
    foundServer.players.forEach(player => {
      console.log('server: ' , foundServer.port, ' | player: ', player.name, ' - sending playerConnected msg');
      io.emit('playerConnected', { name: data.name });
    });

    res.sendStatus(200);
  } catch (error) {
    res.status(500).send('Failed broadcasting joining the server. Try again later.\n');
  }
});

app.post('/api/matchmaking/:serverId/disconnect', (req, res) => {
  const { serverId } = req.params;
  const { data } = req.body;

  try{
    const foundServer = gameServers.find(server => server.port ===  parseInt(serverId, 10));
    if (!foundServer) {
      res.status(400).send(`There is no server running on port ${serverId}\n`);
      return;
    }

    foundServer.players = foundServer.players.filter(player => player.name !== data.name);

    if (foundServer.players.length <= 0) {
      killGameServer(foundServer.process);
      gameServers = gameServers.filter(server => server.port !==  parseInt(serverId, 10));
    } else {
      foundServer.players.forEach(player => {
        console.log('server: ' , foundServer.port, ' | player: ', player.name, ' - sending playerDisconnected msg');
        io.emit('playerDisconnected', { name: data.name });
      });
    }

    res.sendStatus(200);
  } catch (error) {
    console.error('Disconnect failed | ', error);
    res.status(500).send('Failed disconnecting from the server. Try again later.\n');
  }
});

module.exports = httpServer;