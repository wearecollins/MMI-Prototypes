function Manager(states, transitions){
  if (states === undefined ||
      !Array.isArray(states)){
    throw 'no states defined, or states is not an array.';
  }
  if (!transitions ||
      typeof(transitions) !== 'object'){
    console.warn('using default transitions');
    transitions = {};
  }
var ws;
var streamTargets = [];
var serverName = 'localhost';
var serverPort = 9091;
var prevURL;
var activeStateI = 0;
var thisManager = this;

function init(){
  for(var stateI = 0;
      stateI < states.length;
      stateI++){
    loadState(states[stateI], stateI);
  }
  createWebsocket();
  registerAllEvents();
  document.body.onkeydown = handlekey;
};

window.onload = init;

function handlekey(evt){
  var key = evt.keyCode || evt.which;
  var keychar = String.fromCharCode(key);
  if (keychar === 'a' || keychar === 'A') {
    if (activeStateI < states.length - 1){
      window.dispatchEvent(new Event('admin'));
    }
  }
};

function getTargetStateIndex(activeStateIndex, type){
  var activeStateKey = states[activeStateIndex];
  var nextStateKey;
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
    console.warn('can not find state',nextStateKey,
                 'defined as target of',type,
                 'event from',activeStateKey);
  }

  switch(type){
    case "next":
      if (activeStateIndex === (states.length - 2)){
        return 0;
      } else {
        return activeStateIndex + 1;
      }
    case "prev":
      if (activeStateIndex === 0){
        return 0;
      } else {
        return activeStateIndex - 1;
      }
    case "admin":
      return states.length - 1;
    case "cancel":
    default:
      return 0;
  }
};

function registerEvent(eventType){
  window.addEventListener(eventType, function(evt){
    var targetStateI = getTargetStateIndex(activeStateI, eventType);
    var activeStateKey = states[activeStateI];
    var targetStateKey = states[targetStateI];
    var activeStateName = states[activeStateKey].data.name;
    var targetStateName = states[targetStateKey].data.name;
    document.getElementById(activeStateName).classList.add('disabled');
    states[activeStateKey].obj.exit(evt);
    activeStateI = targetStateI;
    document.getElementById(targetStateName).classList.remove('disabled');
    states[targetStateKey].obj.enter(evt);
  });
};

function registerAllEvents(){
  var events = ['next','prev','cancel','admin'];
  for(var eventI = events.length - 1;
      eventI >= 0;
      eventI--){
    var eventType = events[eventI];
    registerEvent(eventType);
  }
};

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
    console.log('connected to '+serverName+':'+serverPort);
  };
};

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
};

//the server sends back the current camera config
// when we first connect, and whenever we
// try to make a change
function handleJson(msg){
  var data;
  try{
    data = JSON.parse(msg);
  }catch(e){
    console.error('error parsing', msg, e);
    return;
  }
  //if this is an event message
  if (data.event !== undefined){
    //trigger the event
    var name = data.event.name;
    delete(data.event.name);
    window.dispatchEvent(new CustomEvent(name, data.event));
  }
};

this.registerStreamTarget = function(func){
  for(var targetI = streamTargets.length - 1;
      targetI >= 0;
      targetI--){
    if(streamTargets[targetI] === func){
      return;
    }
  }
  streamTargets.push(func);
};

this.unregisterStreamTarget = function(func){
  for(var targetI = streamTargets.length - 1;
      targetI >= 0;
      targetI--){
    if(streamTargets[targetI] === func){
      streamTargets.splice(targetI, 1);
    }
  }
};

function loadState(stateKey, stateI){
  states[stateKey] = {};
  var state = states[stateKey];
  state.index = stateI;
  var dataPath = stateKey+'/data.json';
  var templatePath = stateKey+'/template.hbr';
  var scriptPath = stateKey+'/main.js';
  var stylePath = stateKey+'/style.css';

  loadCSS(stylePath);
  loadJSON(dataPath, function(response){
    state.data = response;
    var stateName = state.data.name;
    loadTemplate(templatePath, function(response){
      state.templateString = response;
      state.template = Handlebars.compile(state.templateString);
      var div = document.createElement('div');
      div.innerHTML = state.template(state.data);
      var elem = div.getElementsByClassName('page')[0];
      if (stateI !== 0){
        elem.classList.add('disabled');
      }
      document.body.appendChild(elem);
      add_script(scriptPath, function(){
        try{
          state.obj = new window[stateName](thisManager);
          if (stateI ===  0){
            state.obj.enter();
          }
        } catch(e){
          console.error('no constructor for',stateName,e);
        }
      });
    });
  });
};

function loadJSON(path, callback){
  loadType(path, 'application/json', function(result){
    try{
        callback(JSON.parse(result));
    }catch(e){
        console.error('error parsing', path, e);
    }
  });
};

function loadTemplate(path, callback){
  loadType(path, 'text/x-handlebars-template', callback);
};

/* from 
 *
 * https://software.intel.com/en-us/blogs/2010/05/22/
 * dynamically-load-javascript-with-load-completion-notification
 *
 * utility to add a script node to the current document
 * and call the callback function when it is loaded.
 * Should allow scripts to be loaded at any time during document
 * life..and have code be kicked off after they are loaded.
 *
 * Browsers tested: 
 * FF 3.6.x - WORKS
 * Chrome 5.x - WORKS
 * Safari 4.x - WORKS
 * MS IE 8 - WORKS
 * Opera 10.x - WORKS 
 *
 **/
function add_script(scriptURL, onloadCB) {
  var scriptEl    = document.createElement("script");
  scriptEl.type   = "text/javascript";
  scriptEl.src    = scriptURL;
  function calltheCBcmn() {
    onloadCB(scriptURL);
  }
  if(typeof(scriptEl.addEventListener) != 'undefined') {
    /* The FF, Chrome, Safari, Opera way */
    scriptEl.addEventListener('load',calltheCBcmn,false);
  } else {
    /* The MS IE 8+ way (may work with others - I dunno)*/
    function handleIeState() {
      if(scriptEl.readyState == 'loaded'){
        calltheCBcmn(scriptURL);
      }
    }
    var ret = scriptEl.attachEvent('onreadystatechange',handleIeState);
  }
  document.getElementsByTagName("head")[0].appendChild(scriptEl);
}

function loadCSS(path){
  var link = document.createElement('link');
  link.setAttribute('rel', 'stylesheet');
  link.setAttribute('type', 'text/css');
  link.setAttribute('href', path);
  document.getElementsByTagName('head')[0].appendChild(link);
};

function loadType(path, mimeType, callback){
  var xobj = new XMLHttpRequest();
  xobj.overrideMimeType(mimeType);
  xobj.open('GET', path, true);
  xobj.onreadystatechange = function(){
    if (xobj.readyState === 4 && xobj.status === 200){
      callback(xobj.response);
    }
  };
  try{
    xobj.send();
  } catch(e){
    console.error('error retrieving', path, mimeType, e);
  }
};
};
