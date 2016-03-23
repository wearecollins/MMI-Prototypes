var ws;
var streamTargets = [];
var serverName = 'localhost';
var serverPort = 9091;
var prevURL;

function init(){
  if (states === undefined){
    console.error('no states defined');
  } else {
    for(var stateI = 0;
        stateI < states.length;
        stateI++){
      loadState(states[stateI], stateI);
    }
  }
  createWebsocket();
  registerEvents();
};

window.onload = init;

function registerEvents(){
  window.addEventListener('attract_next', function(evt){
    document.getElementById('attract').classList.add('disabled');
    states['attract'].obj.exit();
    document.getElementById('liveView').classList.remove('disabled');
    states['liveView'].obj.enter(evt);
  });
  window.addEventListener('liveView_next', function(evt){
    document.getElementById('liveView').classList.add('disabled');
    states['liveView'].obj.exit();
    document.getElementById('confirm').classList.remove('disabled');
    states['confirm'].obj.enter(evt);
  });
  window.addEventListener('confirm_next', function(evt){
    document.getElementById('confirm').classList.add('disabled');
    states['confirm'].obj.exit();
    document.getElementById('attract').classList.remove('disabled');
    states['attract'].obj.enter(evt);
  });
  window.addEventListener('confirm_prev', function(evt){
    document.getElementById('confirm').classList.add('disabled');
    states['confirm'].obj.exit();
    document.getElementById('liveView').classList.remove('disabled');
    states['liveView'].obj.enter(evt);
  });
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

function registerStreamTarget(func){
  for(var targetI = streamTargets.length - 1;
      targetI >= 0;
      targetI--){
    if(streamTargets[targetI] === func){
      return;
    }
  }
  streamTargets.push(func);
};

function unregisterStreamTarget(func){
  for(var targetI = streamTargets.length - 1;
      targetI >= 0;
      targetI--){
    if(streamTargets[targetI] === func){
      streamTargets.splice(targetI, 1);
    }
  }
};

function loadState(stateName, stateI){
  states[stateName] = {};
  var state = states[stateName];
  var dataPath = stateName+'/data.json';
  var templatePath = stateName+'/template.hbr';
  loadCSS(stateName+'/style.css');
  loadJSON(dataPath, function(response){
    state.data = response;
    loadTemplate(templatePath, function(response){
      state.template = response;
      state.script = Handlebars.compile(state.template);
      var div = document.createElement('div');
      div.innerHTML = state.script(state.data);
      var children = div.childNodes;
      for(var childI = 0;
          childI < children.length;
          childI++){
        document.body.appendChild(children[childI]);
      }
      add_script(stateName+'/main.js', function(){
        try{
          state.obj = new window[stateName]();
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

