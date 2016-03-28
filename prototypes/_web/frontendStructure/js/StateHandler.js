/*globals Page, Loader*/

/**
 * Manages the current State (aka {@link Page}) of the view. 
 * @constructor
 */
function StateHandler(){
  
  //### Private vars ###
  /** the list of all pages */
  var pages;
  /**
   * custom transitions between pages
   * this has the format
   * {sourcePagePath:{action:destinationPagePath},
   * ...,
   * '_globals_':{action:destinationPagePath}}
   */
  var transitions;
  /** the currently active page index */
  var activeStateI = 0;

  //### Public methods ###
  
  /**
   * Loades the provided pages, drops them in the provided container
   * and uses the specified custom transitions.
   * @param {string[]} a_pages a list of paths to load {@link Page}s from
   * @param {Object} [a_transitions={}] custom transitions to override defaults.
   *  Expected format described by {@link StateHandler~transitions}
   * @param {Element} container the DOM container to add {@link Page}s to
   */
  this.init = function init(a_pages, a_transitions, container){
    if (!a_transitions ||
        typeof(a_transitions) !== 'object'){
      log.warn('using default transitions');
      a_transitions = {};
    }

    transitions = a_transitions;

    return loadPages(a_pages, container).
      then( result => (pages = result) ).
      then( initState );
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
      activeStateI = targetStateI;
      pages[targetStateI].enter(evt);
    }
  };

  /**
   * Whether or not the current State is the Admin state.
   * @returns {boolean} True if the current state is the Admin state.
   */
  this.isOnAdmin = function isOnAdmin(){
    return activeStateI === (pages.length - 1);
  };

  /**
   * Whether or not the current State is the Attract state.
   * @returns {boolean} True if the current state is the Attract state.
   */
  this.isOnAttract = function isOnAttract(){
    return activeStateI === 0;
  };

  /**
   * Loads all the specified pages.
   * @param {string[]} pages list of directories to load {@link Page}s from.
   * @param {Element} container DOM container to load pages into.
   * @returns {Promise<Page[]>}
   */
  function loadPages(pages, container){
    var pagePromises = [];
    for(var pageI = 0;
        pageI < pages.length;
        pageI++){
      pagePromises.push(
        loadPage(pages[pageI], container).
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
   * @returns {Promise<Page>}
   */
  function loadPage(pagePath, pageContainer){
    var page = new Page();
    return page.load(pagePath, pageContainer, Loader);
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
