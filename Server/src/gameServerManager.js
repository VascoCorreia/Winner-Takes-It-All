const { spawn } = require('child_process');
const path = require('path');
const config = require('./config');

const UE4ExecutablePath = config.USE_UE4_CORE_BUILD ? 
  config.UE4_EXECUTABLE_PATH_FROM_GITHUB : 
  config.UE4_EXECUTABLE_PATH_NORMAL;
const gameServerFilePath = config.IS_TEST ? 
`"${UE4ExecutablePath}" "${config.UPROJECT_PATH}"` :  
`"${path.join(__dirname, '../gameServerBuild/WindowsServer/projectServer.exe')}"`;

function spawnGameServer(port) {
  console.log(`Attempting to spawn using release ${gameServerFilePath}`);
  
  const gameServer = config.IS_TEST ? 
    spawn(gameServerFilePath, ['/Game/TopDownCPP/Maps/World', '-server', '-log', `-port=${port}`], { shell: true }) :
    spawn(gameServerFilePath, ['-log', `-PORT=${port}`], { shell: true }) ;

  console.log(`Created a new game server on port ${port}\n`);

  gameServer.stdout.on('data', (data) => {
    console.log(`GameServer | ${port} | stdout: ${data}`);
  });
  
  gameServer.stderr.on('data', (data) => {
    console.error(`GameServer | ${port} | stderr: ${data}`);
  });
  
  gameServer.on('close', (code) => {
    console.log(`GameServer | ${port} | exited with code ${code}`);
  });

  return gameServer;
}

function killGameServer (gameServer) {
  // Workaround for Windows - https://stackoverflow.com/questions/23706055/why-can-i-not-kill-my-child-process-in-nodejs-on-windows
  spawn("taskkill", ["/pid", gameServer.pid, '/f', '/t']);
  return true;
}

module.exports = {
  spawnGameServer,
  killGameServer
};
