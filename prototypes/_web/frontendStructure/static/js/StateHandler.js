/*globals Page, Loader*/

/**
 * Manages the current State (aka {@link Page}) of the view. 
 * @constructor
 */
function StateHandler(){
  
  //### Private vars ###
  /** 
   * the list of all pages 
   * @type {Page[]}
   */
  var pages;
  /**
   * custom transitions between pages
   * this has the format
   * {sourcePagePath:{action:destinationPagePath},
   * ...,
   * '_globals_':{action:destinationPagePath}}
   * @type {Object}
   */
  var transitions;
  /** 
   * the currently active page index 
   * @type {number};
   */
  var activeStateI = 0;
  /**
   * list of functions to notify on State changes
   * @type {jsonNotifier[]}
   */
  var jsonNotifiers = [];
  /**
   * list of functions to call when the stream should be enabled/disabled
   * @type {streamNotifier[]}
   */
  var streamNotifiers = [];

  /**
   * A callback for stream enable/disable notifications
   * @callback streamNotifier
   * @param {boolean} enable whether or not to enable the stream
   */

  //### Public methods ###
  
  /**
   * Loades the provided pages, drops them in the provided container
   * and uses the specified custom transitions.
   * @param {string[]} a_pages a list of paths to load {@link Page}s from
   * @param {Object} [a_transitions={}] custom transitions to override defaults.
   *  Expected format described by {@link StateHandler~transitions}
   * @param {Element} container the DOM container to add {@link Page}s to
   * @param {ConfigHandler} a_configHandler
   */
  this.init = function init(a_pages, 
                            a_transitions, 
                            container, 
                            a_configHandler){
    if (!a_transitions ||
        typeof(a_transitions) !== 'object'){
      log.warn('using default transitions');
      a_transitions = {};
    }

    transitions = a_transitions;

    return loadPages(a_pages, container, a_configHandler).
      then( result => (pages = result) ).
      then( initState );
  };

  /**
   * A callback for json notifications
   * @callback jsonNotifier
   * @param {string} json a JSON-encoded Object or map
   */

  /**
   * adds a function to be called when the state transitions
   * @param {jsonNotifier} func 
   */
  this.addJsonNotifier = function addJsonNotifier(func){
    if (typeof(func) === 'function'){
      jsonNotifiers.push(func);
    }
  };

  /**
   * adds a function to be called when 
   *  the video stream should be shown/hidden
   * @param {streamNotifier} func
   */
  this.addStreamNotifier = function addStreamNotifier(func){
    if (typeof(func) === 'function'){
      streamNotifiers.push(func);
    }
  };

  /**
   * Handles transitioning States.
   * @param {string} action the action driving state transition.
   *  must be one of {@link StateHandler.ACTIONS}.
   * @param {Event} [evt] event that triggered this action.
   */
  this.handleAction = function handleAction(action, evt){
    if (StateHandler.ACTIONS.indexOf(action) >= 0){
      var targetStateI = getTargetStateIndex(activeStateI, action);
      var activeStateName = pages[activeStateI].getName();
      var targetStateName = pages[targetStateI].getName();
      log.info(activeStateName,'->',targetStateName);

      pages[activeStateI].exit(evt);
      notifyTransition(activeStateI, targetStateI);
      manageStream(activeStateI, targetStateI);
      activeStateI = targetStateI;
      pages[targetStateI].enter(evt);
    }
  };

  /**
   * check if the stream should be shown or hidden, 
   *  and notify relevant consumers
   * @param {number} fromIndex the index number of the state we are leaving
   * @param {number} toIndex the index number of the state we are entering
   */
  function manageStream(fromStateI, toStateI){
    var didShow = pages[fromStateI].shouldShowStream();
    var shouldShow = pages[toStateI].shouldShowStream();
    if (didShow !== shouldShow){
      for(var notifierI = streamNotifiers.length - 1;
          notifierI >= 0;
          notifierI--){
        streamNotifiers[notifierI](shouldShow);
      }
    }
  }

  /**
   * check if the auto-timeout should be disabled for the current state
   * @returns {boolean} true if the auto-timeout should be disabled
   */
  this.disabledTimeout = function disabledTimeout(){
    return pages[activeStateI].disabledTimeout();
  };

  /**
   * Notifies all registered jsonNotifiers of the state transition
   * @param {number} fromIndex the index number of the state we are leaving
   * @param {number} toIndex the index number of the state we are entering
   */
  function notifyTransition(fromIndex, toIndex){
    var fromStateName = pages[fromIndex].getName();
    var toStateName = pages[toIndex].getName();
    var message = {state:{from:fromStateName, to:toStateName}};
    for(var notifierI = jsonNotifiers.length - 1;
        notifierI >= 0;
        notifierI--){
      jsonNotifiers[notifierI](message);
    }
  }

  /**
   * Loads all the specified pages.
   * @param {string[]} pages list of directories to load {@link Page}s from.
   * @param {Element} container DOM container to load pages into.
   * @param {ConfigHandler} configHandler
   * @returns {Promise<Page[]>}
   */
  function loadPages(pages, container, configHandler){
    var pagePromises = [];
    for(var pageI = 0;
        pageI < pages.length;
        pageI++){
      pagePromises.push(
        loadPage(pages[pageI], container, configHandler).
          //catch any rejection so we wait for all states
          // to finish (whether they fail or succeed, we don't care here)
          catch( () => undefined ));
    }
    return Promise.all(pagePromises);
  }

  /**
   * Used for initializing the current State after loading all Pages
   */
  function initState(){
    pages[activeStateI].enter();
  }

  /**
   * Loads a particular page
   * @param {string} pagePath path to the Page
   * @param {Element} pageContainer DOM element to load Page DOM elements into
   * @param {ConfigHandler} configHandler
   * @returns {Promise<Page>}
   */
  function loadPage(pagePath, pageContainer, configHandler){
    var page = new Page();
    return page.load(pagePath, pageContainer, configHandler, Loader);
  }

  /*eslint-disable complexity*/
  //cyclomatic complexity is pretty high for case statements
  // making this method a lower cyclomatic complexity will not help
  // readability.
  /**
   * Used for determining the default next State. 
   *  can be overridden via {@link StateHandler~transitions}
   * @param {number} activeStateIndex the current state index
   * @param {number} numStates the total number of states
   * @param {string} type the action. 
   *  expected to be one of {@link StateHandler.ACTIONS}
   * @returns {number} the index for the state to transition to
   */
  function getDefaultTargetStateIndex(activeStateIndex, numStates, type){
    switch(type){
      case 'next':
        if (activeStateIndex >= (numStates - 2)){
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
        return numStates - 1;
      case 'cancel':
      default:
        return 0;
    }
  }
  /*eslint-enable*/

  /**
   * Determines which state to transition to
   * @param {number} activeStateIndex the index of the current state
   * @param {string} type the transition {@link StateHandler.ACTIONS|action}
   * @returns {number} the index of the state to transition to
   */
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
    return getDefaultTargetStateIndex(activeStateIndex, pages.length, type);
  }
}

/**
 * The list of actions supported by the StateHandler
 */
StateHandler.ACTIONS = ['next','prev','cancel','admin'];
