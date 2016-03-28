function EventHandler(){
  var activeTimeouts = [];
  var stateHandler;

  this.init = function init(textInput, a_stateHandler){
    stateHandler = a_stateHandler;
    textInput.addTextHandler(handleJson);
    registerAllEvents();
    document.body.onkeydown = handlekey;
  };

  function registerAllEvents(){
    var events = ['next','prev','cancel','admin'];
    for(var eventI = events.length - 1;
        eventI >= 0;
        eventI--){
      var eventType = events[eventI];
      registerEvent(eventType);
    }
  }

  function handleEvent(eventType, evt){
    //clean up timeouts
    while(activeTimeouts.length > 0){
      clearTimeout(activeTimeouts.pop());
    }
    document.body.classList.remove('timingOut');

    //do state transition
    stateHandler.handleAction(eventType, evt);
  
    //set up timeouts
    if (!(stateHandler.isOnAdmin() ||
          stateHandler.isOnAttract())){
      activeTimeouts.push(
        setTimeout(
          function(){
            window.dispatchEvent(new Event('cancel'));
          }, 
          30 * 1000));
      activeTimeouts.push(
        setTimeout(
          function(){
            document.body.classList.add('timingOut');
          },
          Math.max(1, (30 - 5)) * 1000));
    }
  }
  
  function registerEvent(eventType){
    window.addEventListener(eventType, handleEvent.bind(this, eventType));
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

  function handlekey(evt){
    var key = evt.keyCode || evt.which;
    var keychar = String.fromCharCode(key);
    if (keychar === 'a' || keychar === 'A') {
      handleEvent('admin', evt);
    }
  }
}
