/*globals Page, Loader*/
function StateHandler(){
  var pages;
  var transitions;
  var activeStateI = 0;
  this.ACTIONS = ['next','prev','cancel','admin'];

  this.init = function init(a_pages, a_transitions, container){
    if (!a_transitions ||
        typeof(a_transitions) !== 'object'){
      log.warn('using default transitions');
      a_transitions = {};
    }

    pages = a_pages;
    transitions = a_transitions;

    return loadPageClass().
      then( loadStates.bind(this, container) ).
      then( result => (pages = result) ).
      then( initState );
  };

  this.handleAction = function handleAction(action, evt){
    if (this.ACTIONS.indexOf(action) >= 0){
      var targetStateI = getTargetStateIndex(activeStateI, action);
      var activeStateName = pages[activeStateI].getName();
      var targetStateName = pages[targetStateI].getName();
      log.info(activeStateName,'->',targetStateName);

      pages[activeStateI].exit(evt);
      activeStateI = targetStateI;
      pages[targetStateI].enter(evt);
    }
  };

  this.isOnAdmin = function isOnAdmin(){
    return activeStateI === (pages.length - 1);
  };

  this.isOnAttract = function isOnAttract(){
    return activeStateI === 0;
  };

  function loadPageClass(){
    return Loader.loadJS('js/Page.js');
  }

  function loadStates(container){
    var statePromises = [];
    for(var stateI = 0;
        stateI < pages.length;
        stateI++){
      statePromises.push(
        loadState(pages[stateI], stateI, container).
          //catch any rejection so we wait for all states
          // to finish (whether they fail or succeed, we don't care here)
          catch( () => undefined ));
    }
    return Promise.all(statePromises);
  }

  function initState(){
    pages[activeStateI].enter();
  }

  function loadState(stateKey, stateI, pageContainer){
    pages[stateKey] = new Page();
    var state = pages[stateKey];
    state.index = stateI;
    return state.load(stateKey, pageContainer, Loader);
  }

  /*eslint-disable complexity*/
  //cyclomatic complexity is pretty high for case statements
  // making this method a lower cyclomatic complexity will not help
  // readability.
  function getDefaultTargetStateIndex(activeStateIndex, type){
    switch(type){
      case 'next':
        if (activeStateIndex >= (pages.length - 2)){
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
        return pages.length - 1;
      case 'cancel':
      default:
        return 0;
    }
  }
  /*eslint-enable*/

  function getTargetStateIndex(activeStateIndex, type){
    var activeStatePath = pages[activeStateIndex].getPath();
    var nextStatePath;
    if (transitions[activeStatePath] && 
        transitions[activeStatePath][type] !== undefined){
      nextStatePath = transitions[activeStatePath][type];
    } else if (transitions['_globals_'] &&
               transitions['_globals_'][type] !== undefined){
      nextStatePath = transitions['_globals_'][type];
    }
    if (nextStatePath !== undefined){
      for(var stateI = pages.length - 1;
          stateI >= 0;
          stateI--){
        if (pages[stateI].getPath() === nextStatePath){
          return stateI;
        }
      }
      log.warn('can not find state',nextStatePath,
                   'defined as target of',type,
                   'event from',activeStatePath);
    }

    log.debug('using default state transition');
    return getDefaultTargetStateIndex(activeStateIndex, type);
  }
}
