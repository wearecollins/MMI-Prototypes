function WebsocketHandler(){
  var websockets = [];
  var binaryHandlers = [];
  var textHandlers = [];
  var reconnectDelay = 3 * 1000;

  this.connect = function connect(url, name){
    var self = this;
    var ws = new WebSocket(url);
    //receive binary data as an arraybuffer
    ws.binaryType = 'arraybuffer';
    //handle binary and text messages from server
    ws.onmessage = handleMessage.bind(ws, name);
    //handle reconnect logic
    ws.onclose = function(){
      removeWS(this);
      this.close();
      ws = undefined;
      setTimeout(self.connect.bind(self, url, name), reconnectDelay);
    };
    ws.onopen = function(){
      log.info('connected to',name,url);
      addWS(ws, name);
    };
  };

  this.send = function send(message){
    for(var wsI = websockets.length - 1;
        wsI >= 0;
        wsI--){
      websockets[wsI].ws.send(message);
    }
  };

  this.addBinaryHandler = function addBinaryHandler(func){
    binaryHandlers.push(func);
  };

  this.addTextHandler = function addTextHandler(func){
    textHandlers.push(func);
  };

  function removeWS(ws){
    for(var wsI = websockets.length - 1;
        wsI >= 0;
        wsI--){
      if (websockets[wsI].ws === ws){
        websockets.splice(wsI, 1);
      }
    }
  }

  function addWS(ws, name){
    websockets.push({ws, name});
  }
  
  function handleImage(buffer, name){
    for(var handlerI = 0;
        handlerI < binaryHandlers.length;
        handlerI++){
      binaryHandlers[handlerI](buffer, name);
    }
  }

  function handleText(text, name){
    for(var handlerI = 0;
        handlerI < textHandlers.length;
        handlerI++){
      textHandlers[handlerI](text, name);
    }
  }

  function handleMessage(name, message){
    if (message.data instanceof ArrayBuffer){
      handleImage(message.data, name);
    } else {
      handleText(message.data, name);
    }
  }
}
