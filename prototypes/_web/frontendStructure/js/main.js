/*global Handlebars, log4javascript, Loader, 
 *       Page, StreamHandler, StateHandler, 
 *       WebsocketHandler, EventHandler*/

function Manager(states, transitions){
  if (states === undefined ||
      !Array.isArray(states)){
    throw 'no states defined, or states is not an array.';
  }
  var ws;
  this.configs = {'timeout':30};
  var serverName = 'localhost';
  var serverPort = 9091;
  var streamHandler;
  var eventHandler;
  var stateHandler;
  
  function init(){
    //first we need to load up the logging framework
    var loggingPromise = initLogging();

    //load library in parallel
    var libraryPromise = loggingPromise.then(loadLibrary);

    //initialize everything
    libraryPromise.
      then(initStructure).
      then(loadStates).
      then(createWebsocket).
      then(createStreamHandler).
      then(createEventHandler).
      then( () => log.info('[Manager::init] done') );  
      //could also wait to connect to websockets until very end...
  }
  
  window.onload = init;

  function loadLibrary(){
    var promises = [];
    var path = 'js';
    var files = ['Page.js','StateHandler.js','EventHandler.js',
                 'WebsocketHandler.js','StreamHandler.js'];
    for (var fileI = files.length - 1;
         fileI >= 0;
         fileI--){
      promises.push(Loader.loadJS(path+'/'+files[fileI]));
    }

    return Promise.all(promises);
  }

  function createStreamHandler(){
    streamHandler = new StreamHandler();
    streamHandler.init(ws, document.getElementById('liveView'));
  }

  function createEventHandler(){
    eventHandler = new EventHandler();
    eventHandler.init(ws, stateHandler);
  }

  function initLogging(){
    window.log = console;
    var loggingPromise = Loader.loadJS('js/vendor/log4js-1.4.13.js');
    return loggingPromise.
      then(function resolve(){
        //reference the logger from the window for global availability
        window.log = log4javascript.getLogger();
        var browserConsoleAppender = 
          new log4javascript.BrowserConsoleAppender();
        var timestampedLayout = 
          new log4javascript.PatternLayout('%-5p %d{HH:mm:ss,SSSS} - %m%n');
        browserConsoleAppender.setLayout(timestampedLayout);
        window.log.addAppender(browserConsoleAppender);
      }).
      then(function resolve(){
        log.info('loaded Log4JS');
      });
  }

  function initStructure(){
    var structurePromise = Loader.loadHTML('structure.html');

    return structurePromise.
      then( function resolve(html){
        document.body.innerHTML = html;
        var pageContainer = document.getElementById('pages');
        return pageContainer;
      }).
      catch(function reject(reason){
        log.warn('structure not loaded',reason);
        return document.body;
      });
  }

  function loadStates(container){
    stateHandler = new StateHandler();
    return stateHandler.init(states, transitions, container);
  }
  
  function createWebsocket(){
    ws = new WebsocketHandler();
    ws.connect('ws://'+serverName+':'+serverPort);
  }
  
/*
  //from
  // http://stackoverflow.com/a/32362233/5953457
  Promise.prototype.finally = function(cb) {
    const res = () => this;
    return this.then(value =>
                       Promise.resolve(cb({state:'fulfilled', value})).
                               then(res),
                     reason =>
                       Promise.resolve(cb({state:'rejected', reason})).
                               then(res));
  };
*/
}
