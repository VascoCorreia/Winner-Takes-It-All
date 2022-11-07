# Winner Takes It All Server

This is the matchmaking server of the Winner Takes It All Game.
It is responsible for spawning servers, managing thier lifecycle and connecting players together to play a game.

## Setup

You need the following:
 - **Node.js** (the server was built and tested on the current LTS version - 16.x.x)
 - **npm** - comes with Node.js
 - It is advised to install Node.js through [NVM for Windows](https://github.com/coreybutler/nvm-windows). You can also check this guide - https://docs.microsoft.com/en-us/windows/dev-environment/javascript/nodejs-on-windows
 
### Config

 - create a file `config.js` inside the `src` directory
 - copy-paste everything from `src/config.example.js`
 - update the paths for `UE4_EXECUTABLE_PATH_NORMAL` , `UE4_EXECUTABLE_PATH_FROM_GITHUB` and `UPROJECT_PATH` if needed.

### Installing dependencies

When you have Node already setup:
 1. Open the Windows CMD in this folder
 2. Run `npm install`
 3. Run the development version of the server `npm run dev`
    - If you get an error about **nodemeon** unrecognozied, you can install it globally with `npm install -g nodemon`. This might be the case when using Windows CMD.
 
 ### Caveats
  - You should run the server with Windows CMD. If you are using WSL (Linux on Windows), spawning game dedicated servers will not work correctly.
