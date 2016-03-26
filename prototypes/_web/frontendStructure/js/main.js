/*eslint no-use-before-define: [2, "nofunc"]*/
/*global Handlebars, log4javascript*/

function Manager(states, transitions){
  if (states === undefined ||
      !Array.isArray(states)){
    throw 'no states defined, or states is not an array.';
  }
  var ws;
  this.configs = {'timeout':30};
  var streamTargets = [];
  var serverName = 'localhost';
  var serverPort = 9091;
  var prevURL;
  var activeStateI = 0;
  var thisManager = this;
  
  function init(){
    //first we need to load up the logging framework
    initLogging().
      then(processTransitions).
      then(initStructure).
      then(loadStates).
      then(function resolve(){
        log.info('done loading all states');
        //I should probably lock `activeStateI` until now
        //maybe wait to establish websocket connection until now?
        //  but what about the configs managed by the Node server?
        //maybe keep body.style.display = 'none' until now?
      });
    createWebsocket();
    registerAllEvents();
    document.body.onkeydown = handlekey;
  }
  
  window.onload = init;

  function processTransitions(){
    if (!transitions ||
        typeof(transitions) !== 'object'){
      log.warn('using default transitions');
      transitions = {};
    }
  }

  function initLogging(){
    window.log = console;
    var loggingPromise = loadJS('js/vendor/log4js-1.4.13.js');
    return loggingPromise.
      then(function resolve(){
        //reference the logger from the window for global availability
        window.log = log4javascript.getLogger();
        var browserConsoleAppender = 
          new log4javascript.BrowserConsoleAppender();
        var timestampedLayout = 
          new log4javascript.PatternLayout('%d{HH:mm:ss,SSSS} - %m%n');
        browserConsoleAppender.setLayout(timestampedLayout);
        window.log.addAppender(browserConsoleAppender);
      }).
      then(function resolve(){
        log.info('loaded Log4JS');
      });
  }

  function initStructure(){
    var structurePromise = loadHTML('structure.html',
                                    Promise.resolve());

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
    var statePromises = [];
    for(var stateI = 0;
        stateI < states.length;
        stateI++){
      statePromises.push(
        loadState(states[stateI], stateI, container).
          //catch any rejection so we wait for all states
          // to finish (whether they fail or succeed, we don't care here)
          catch( () => undefined ));
    }
    return Promise.all(statePromises);
  }

  
  function handlekey(evt){
    var key = evt.keyCode || evt.which;
    var keychar = String.fromCharCode(key);
    if (keychar === 'a' || keychar === 'A') {
      if (activeStateI < states.length - 1){
        window.dispatchEvent(new Event('admin'));
      }
    }
  }

  /*eslint-disable complexity*/
  //cyclomatic complexity is pretty high for case statements
  // making this method a lower cyclomatic complexity will not help
  // readability.
  function getDefaultTargetStateIndex(activeStateIndex, type){
    switch(type){
      case 'next':
        if (activeStateIndex >= (states.length - 2)){
          return 0;
        } else {
          return activeStateIndex + 1;
        }
      case 'prev':
        if (activeStateIndex === 0){
          return 0;
        } else {
          return activeStateIndex - 1;
        }
      case 'admin':
        return states.length - 1;
      case 'cancel':
      default:
        return 0;
    }
  }
  /*eslint-enable*/

  function getTargetStateIndex(activeStateIndex, type){
    var activeStateKey = states[activeStateIndex];
    var nextStateKey;
    log.debug(transitions);
    if (transitions[activeStateKey] && 
        transitions[activeStateKey][type] !== undefined){
      nextStateKey = transitions[activeStateKey][type];
    } else if (transitions['_globals_'] &&
               transitions['_globals_'][type] !== undefined){
      nextStateKey = transitions['_globals_'][type];
    }
    if (nextStateKey !== undefined){
      for(var stateI = states.length - 1;
          stateI >= 0;
          stateI--){
        if (states[stateI] === nextStateKey){
          return stateI;
        }
      }
      log.warn('can not find state',nextStateKey,
                   'defined as target of',type,
                   'event from',activeStateKey);
    }

    log.debug('using default state transition');
    return getDefaultTargetStateIndex(activeStateIndex, type);
  }
  
  var activeTimeouts = [];
  
  function registerEvent(eventType){
    window.addEventListener(eventType, function(evt){
      var targetStateI = getTargetStateIndex(activeStateI, eventType);
      var activeStateKey = states[activeStateI];
      var targetStateKey = states[targetStateI];
      var activeStateName = states[activeStateKey].data.name;
      var targetStateName = states[targetStateKey].data.name;
      log.info(activeStateName,'->',targetStateName);
  
      //clean up previous state
      while(activeTimeouts.length > 0){
        clearTimeout(activeTimeouts.pop());
      }
      document.body.classList.remove('timingOut');
      document.getElementById(activeStateName).classList.add('disabled');
      states[activeStateKey].obj.exit(evt);
  
      //go to new state
      if (targetStateI !== 0 &&
          targetStateI !== states.length - 1){
        activeTimeouts.push(
          setTimeout(
            function(){
              window.dispatchEvent(new Event('cancel'));
            }, 
            thisManager.configs.timeout * 1000));
        activeTimeouts.push(
          setTimeout(
            function(){
              document.body.classList.add('timingOut');
            },
            Math.max(1, (thisManager.configs.timeout - 5)) * 1000));
      }
      activeStateI = targetStateI;
      document.getElementById(targetStateName).classList.remove('disabled');
      states[targetStateKey].obj.enter(evt);
    });
  }
  
  function registerAllEvents(){
    var events = ['next','prev','cancel','admin'];
    for(var eventI = events.length - 1;
        eventI >= 0;
        eventI--){
      var eventType = events[eventI];
      registerEvent(eventType);
    }
  }
  
  function createWebsocket(){
    //connect to the camera's websocket
    ws = new WebSocket('ws://'+serverName+':'+serverPort);
    //receive binary data as an arraybuffer
    ws.binaryType = 'arraybuffer';
    //handle binary and text messages from server
    ws.onmessage = function(msg){
      if (msg.data instanceof ArrayBuffer){
        handleImage(msg.data);
      } else {
        handleJson(msg.data);
      }
    };
    //handle reconnect logic
    ws.onclose = function(){
      ws.close();
      ws = undefined;
      setTimeout(createWebsocket, 3*1000);
    };
    ws.onopen = function(){
      log.info('connected to '+serverName+':'+serverPort);
    };
  }
  
  //images come as binary from the server
  // lets just dump them into an <img>
  function handleImage(buf){
    var view = new Uint8Array(buf);
    var blob = new Blob([view], {type: 'image/jpeg'});
    var url = window.URL.createObjectURL(blob);
    for(var funcI = streamTargets.length - 1;
        funcI >= 0;
        funcI--){
      streamTargets[funcI](url, blob);
    }
    if (prevURL !== undefined){
      window.URL.revokeObjectURL(prevURL);
    }
    prevURL = url;
  }
  
  //the server sends back the current camera config
  // when we first connect, and whenever we
  // try to make a change
  function handleJson(msg){
    var data;
    try{
      data = JSON.parse(msg);
    }catch(e){
      log.error('error parsing', msg, e);
      return;
    }
    //if this is an event message
    if (data.event !== undefined){
      //trigger the event
      var evt;
      if (data.event.detail){
        evt = new CustomEvent(data.event.name, {detail:data.event.detail});
      } else {
        evt = new Event(data.event.name); 
      }
      window.dispatchEvent(evt);
    }
  }
  
  this.registerStreamTarget = function registerStreamTarget(func){
    for(var targetI = streamTargets.length - 1;
        targetI >= 0;
        targetI--){
      if(streamTargets[targetI] === func){
        return;
      }
    }
    streamTargets.push(func);
  };
  
  this.unregisterStreamTarget = function unregisterStreamTarget(func){
    for(var targetI = streamTargets.length - 1;
        targetI >= 0;
        targetI--){
      if(streamTargets[targetI] === func){
        streamTargets.splice(targetI, 1);
      }
    }
  };
  
  function loadState(stateKey, stateI, pageContainer){
    states[stateKey] = {};
    var state = states[stateKey];
    state.index = stateI;
    var dataPath = stateKey+'/data.json';
    var markupPath = stateKey+'/template.hbr';
    var scriptPath = stateKey+'/main.js';
    var stylePath = stateKey+'/style.css';
    var options =  {data:{path:stateKey}};
  
    //load the remote Javascript file
    // and then instantiate the object therein
    function doLoadJS(){
      return loadJS(scriptPath).
        then(function(){
          try{
            //the class in this Javascript file should
            // match the name of the state
            state.obj = new window[state.data.name](thisManager);
            //if this is the current state, enter it
            if (stateI ===  activeStateI){
              state.obj.enter();
            }
          } catch(e){
            log.error('no constructor for',state.data.name,e);
          }
          log.debug('done loading',stateKey);
        });
    }
  
    //load the remote JSON data
    var jsonPromise = loadJSON(dataPath).
      catch(function(reason){
        log.warn('problem loading JSON',reason);
        //error loading JSON
        // let's provide some defaults
        return {name:stateKey};
      }).
      then(function resolve(json){
        //save the data, and keep passing it down the Promise chain
        state.data = json;
        return json;
      });
    //remote CSS and HTML files can be Handlebars templates,
    // so they take the eventual JSON result as their
    // Handlebars context
    var cssPromise = loadCSS(stylePath, jsonPromise, options);
    var htmlPromise = loadHTML(markupPath, jsonPromise, options).
      then(function(markup){
        //put the markup into the page
        var div = document.createElement('div');
        div.innerHTML = markup;
        var elem = div.getElementsByClassName('page')[0];
        pageContainer.appendChild(elem);
        //if this is not the currently active state
        // disable it
        if (stateI !== activeStateI){
          elem.classList.add('disabled');
        }
        return elem;
      });
    //load the remote Javascript after all other remote files have
    // loaded. This way we guarantee the Javascript has access to all its
    // DOM elements and JSON data.
    return Promise.all([jsonPromise,
                        //we don't care if these promises succeed or fail
                        // we just want to wait until they complete
                        cssPromise.catch( () => undefined ),
                        htmlPromise.catch( () => undefined )]).
                   then( () => doLoadJS() );
  }
  
  function loadJSON(path){
    return new Promise(function(resolve, reject){
      loadType(path, 'application/json').
        then(
          function(result){
            try{
              resolve(JSON.parse(result));
            } catch(e){
              log.error('error parsing', path, e);
              reject(e);
            }
          }
        );
    });
  }
  
  function loadHTML(path, contextPromise, options){
    return loadTemplate(path).
      then(function(result){
        return contextPromise.
          then(function(context){
            var template = Handlebars.compile(result);
            var markup = template(context, options);
            return markup;
          });
      });
  }
  
  function loadTemplate(path){
    return loadType(path, 'text/x-handlebars-template');
  }
  
  function loadCSS(path, contextPromise, options){
    return loadTemplate(path).
      then(function(result){
        contextPromise.then(function(context){
          var template = Handlebars.compile(result);
          var styles = template(context, options);
          var elem = document.createElement('style');
          elem.innerHTML = styles;
          document.getElementsByTagName('head')[0].
                   appendChild(elem);
        });
      });
  }
  
  function loadJS(path) {
    return new Promise(function(resolve/*, reject*/){
      log.debug('loading',path);
      var script = document.createElement('script');
      script.type = 'text/javascript';
      script.src = path;
      script.onload = function(){
        resolve();
      };
      //not really sure how to reject...
      document.getElementsByTagName('head')[0].appendChild(script);
    });
  }
  
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
  
  function loadType(path, mimeType){
    return new Promise(function(resolve, reject){
      log.debug('loading',path);
      var xobj = new XMLHttpRequest();
      xobj.overrideMimeType(mimeType);
      xobj.open('GET', path, true);
      xobj.send();
      xobj.onload = function(){
        if (this.status >= 200 && this.status < 300){
          resolve(this.response);
        } else {
          reject(this.statusText);
        }
      };
      xobj.onerror = function(){
        reject(this.statusText);
      };
     // try{
     //   xobj.open('GET', path, true);
     //   xobj.send();
     // } catch(e){
     //   log.error('error retrieving', path, mimeType, e);
     //   reject(e);
     // }
    });
  }
}
