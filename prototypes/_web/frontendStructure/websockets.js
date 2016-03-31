var WebSocket = require('ws');

var Websockets = function(server, configer, logger){
  /**
   * Websocket server to handle websocket communication
   * @type {ws}
   */
  var wss = new WebSocket.Server({server});

  /**
   * all active WebSocket connections
   * @type {ws.WebSocket[]}
   */
  var wsConnections = [];
  
  /**
   * handle an incoming message from one of the websocket connections
   */
  function gotMessage(message, flags){
    logger.debug(message);
    var forwardMsg = true;
  
    //if the message is a config type
    if (message && !flags.binary){
      try {
        var json = JSON.parse(message);
        if (json.config){
          forwardMsg = false;
          configer.updateConfig(json).
            then( () => sendMessage(configer.getConfigString()) );
        }
      } catch(err){
        logger.warn(err);
      }
    } 
  
    if (forwardMsg){
      sendMessage(message, this);
    }
  }
  
  function sendMessage(msg, wsToSkip){
    for(var wsI = wsConnections.length - 1;
        wsI >= 0;
        wsI--){
      var ws = wsConnections[wsI];
      if (ws !== wsToSkip){
        wsConnections[wsI].send(msg);
      }
    }
  }
  
  
  //when a new websocket client connects
  wss.on('connection', function(ws){
    //add the new client to our list of connections
    wsConnections.push(ws);
    
    ws.on('message', gotMessage.bind(ws));
  
    //when the connection is closed
    ws.on('close', function(ws){
      //remove it from the list of active connections
      for(var wsI = wsConnections.length - 1;
          wsI >= 0;
          wsI--){
        if (wsConnections[wsI]=== ws){
          wsConnections.splice(wsI, 1);
        }
      }
    });
  });
};

module.exports = Websockets;
