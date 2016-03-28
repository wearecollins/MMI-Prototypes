/*globals Handlebars*/

var Loader = {};

Loader.MIME = {txt:'text/plain',
               js:'text/javascript',
               css:'text/css',
               json:'application/json',
               html:'text/html',
               hbr:'text/x-handlebars-template'};

Loader.loadJSON = function loadJSON(path){
  return this.loadType(path, Loader.MIME.json).
    then(function resolve(result){
      return JSON.parse(result);
    });
};

Loader.promisify = function promisify(val){
  if (val instanceof Promise){
    return val;
  } else {
    return Promise.resolve(val);
  }
};
  
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

Loader.loadTemplate = function loadTemplate(path){
  return this.loadType(path, Loader.MIME.hbr);
};

Loader.loadCSS = function loadCSS(path, contextPromise, options){
  contextPromise = Loader.promisify(contextPromise);
  return this.loadTemplate(path).
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
};

Loader.loadJS = function loadJS(path) {
  return new Promise(function(resolve/*, reject*/){
    log.debug('loading',path);
    var script = document.createElement('script');
    script.type = Loader.MIME.js;
    script.src = path;
    script.onload = function(){
      resolve();
    };
    //not really sure how to reject...
    document.getElementsByTagName('head')[0].appendChild(script);
  });
};

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
