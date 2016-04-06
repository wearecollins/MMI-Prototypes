/*globals Handlebars*/

/**
 * groups methods for loading different types of files
 * @namespace
 */
var Loader = {};

/**
 * List of MIME types for reference
 */
Loader.MIME = {txt:'text/plain',
               js:'text/javascript',
               css:'text/css',
               json:'application/json',
               html:'text/html',
               hbr:'text/x-handlebars-template'};

/**
 * Loads a JSON file
 * @param {string} path the file location
 * @returns {Promise<Object|SyntaxError>} 
 *  resolves to the resulting parsed JSON
 */
Loader.loadJSON = function loadJSON(path){
  return this.loadType(path, Loader.MIME.json).
    then(function resolve(result){
      return JSON.parse(result);
    });
};

/**
 * utility method for turning a non-Promise value into 
 *  a Promise that resolves that value.
 * @param {Promise|*} val the value to wrap in a Promise
 * @returns {Promise} A Promise which resolves to the provided value.
 *  If the provided value IS a Promise, 
 *  then this just passes through that value
 */
Loader.promisify = function promisify(val){
  if (val instanceof Promise){
    return val;
  } else {
    return Promise.resolve(val);
  }
};
  
/**
 * loads an HTML Handlebars template
 * @param {string} path filepath to the file
 * @param {Promise<Object>|Object} [contextPromise] 
 *  the context to pass to the Handlebars template
 * @param {Object} [options] options to pass to the Handlebars template
 * @returns {Promise<string>} resulting HTML markup generated by 
 *  the loaded template and provided context/options
 */
Loader.loadHTML = function loadHTML(path, contextPromise, options){
  contextPromise = Loader.promisify(contextPromise);
  return this.loadTemplate(path).
    then(function(result){
      return contextPromise.
        then(function(context){
          var template = Handlebars.compile(result);
          var markup = template(context, options);
          return markup;
        });
    });
};

/**
 * convenience method for loading Handlebars templates
 * @param {string} path filepath to the file
 * @returns {Promise<string>} resolves to the contents of the file
 */
Loader.loadTemplate = function loadTemplate(path){
  return this.loadType(path, Loader.MIME.hbr);
};

/**
 * loads a CSS Handlebars template into a style tag in the DOM head
 * @param {string} path filepath to the file
 * @param {Promise<Object>|Object} [contextPromise] 
 *  the context to pass to the Handlebars template
 * @param {Object} [options] options to pass to the Handlebars template
 * @returns {Promise<Element>} resolves to the generated style tag
 *  that the CSS was loaded into
 */
Loader.loadCSS = function loadCSS(path, contextPromise, options){
  contextPromise = Loader.promisify(contextPromise);
  return this.loadTemplate(path).
    then(function(result){
      return contextPromise.
        then(function(context){
          var template = Handlebars.compile(result);
          var styles = template(context, options);
          var elem = document.createElement('style');
          elem.innerHTML = styles;
          document.getElementsByTagName('head')[0].
                   appendChild(elem);
          return elem;
        });
    });
};

/**
 * loads a Javascript file
 * @param {string} path filepath to the file
 * @returns {Promise<undefined,Event>}
 *  resolves when the specified Javascript file has finished loading
 *  rejects if the Javascript file fails to load
 */
Loader.loadJS = function loadJS(path) {
  return new Promise(function(resolve, reject){
    log.debug('loading',path);
    var script = document.createElement('script');
    script.type = Loader.MIME.js;
    script.src = path;
    script.onload = function(){
      resolve();
    };
    script.onerror = function(evt){
      reject(evt);
    };
    document.getElementsByTagName('head')[0].appendChild(script);
  });
};

/**
 * Utility method for loading the contents of various file types
 * @param {string} path the path to the file to load
 * @param {string} mimeType the mimeType of the file to load. 
 *  see {@link Loader.MIME}
 * @returns {Promise<string,string|Error>} 
 *  resolves to the contents of the file.
 *  rejects with the provided rejection status, or thrown Error.
 */
Loader.loadType = function loadType(path, mimeType){
  return new Promise(function(resolve, reject){
    log.debug('loading',path);
    var xobj = new XMLHttpRequest();
    xobj.overrideMimeType(mimeType);
    xobj.open('GET', path, true);
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
    try{
      xobj.send();
    } catch(e){
      log.error('error retrieving', path, mimeType, e);
      reject(e);
    }
  });
};
