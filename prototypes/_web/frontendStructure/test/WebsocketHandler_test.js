/* globals chai, WebsocketHandler*/
var expect = chai.expect;
window.log = console;

describe('WebsocketHandler', function(){

  var ws;
  var handleText;
  var handleBinary;
  var testText = 'test';
  var testBinary = new ArrayBuffer(4);
  //fill the binary buffer with random data
  (new Int32Array(testBinary))[0] = 
    Math.floor(Math.random() * Math.pow(2, 30));
  var serverName = 'echo';
  var serverURL = 'ws://echo.websocket.org/';
  function fwdTextMessage(msg, name){
    handleText(msg, name);
  }
  function fwdBinaryMessage(data, name){
    handleBinary(data, name);
  }

  before(function(done){
    ws = new WebsocketHandler();
    ws.connect(serverURL, serverName);
    ws.addBinaryHandler(fwdBinaryMessage);
    ws.addTextHandler(fwdTextMessage);
    //delay progress to give websocket time to connect
    setTimeout(done, 1800);
  });

  it('receiving text data notifies registered text handler', function(done){
    handleText = function handleText(msg, name){
      expect(msg).to.equal(testText);
      expect(name).to.equal(serverName);
      handleText = undefined;
      done();
    };
    ws.send(testText);
  });
  it('receiving binary data notifies registered binary handler', function(done){
    handleBinary = function handleBinary(msg, name){
      expect(msg).to.deep.equal(testBinary);
      expect(name).to.equal(serverName);
      handleBinary = undefined;
      done();
    };
    ws.send(testBinary);
  });
});
