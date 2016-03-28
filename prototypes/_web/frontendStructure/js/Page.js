function Page(){

  var data;
  var instance;
  var domElem;
  var path;

  this.load = function load(a_path, container, loader){
    path = a_path;
    var dataPath = path+'/data.json';
    var markupPath = path+'/template.hbr';
    var scriptPath = path+'/main.js';
    var stylePath = path+'/style.css';
    var options =  {data:{path:path}};
  
    //load the remote JSON data
    var jsonPromise = loadJSON(dataPath, loader);

    //remote CSS and HTML files can be Handlebars templates,
    // so they take the eventual JSON result as their
    // Handlebars context
    var cssPromise = loader.loadCSS(stylePath, jsonPromise, options);
    var htmlPromise = loadHTML(markupPath, jsonPromise, 
                               options, container, loader);

    //load the remote Javascript after all other remote files have
    // loaded. This way we guarantee the Javascript has access to all its
    // DOM elements and JSON data.
    return Promise.all([jsonPromise,
                        //we don't care if these promises succeed or fail
                        // we just want to wait until they complete
                        cssPromise.catch( () => undefined ),
                        htmlPromise.catch( () => undefined )]).
                   then( () => loadJS(scriptPath, loader) ).
                   then( ( () => this ).bind(this) );
  };

  this.enter = function enter(){
    if(domElem){
      domElem.classList.remove('disabled');
    }
    if (instance){
      instance.enter();
    }
  };

  this.exit = function exit(){
    if(domElem){
      domElem.classList.add('disabled');
    }
    if (instance){
      instance.exit();
    }
  };

  this.getName = function getName(){
    return data.name;
  };

  this.getPath = function getPath(){
    return path;
  };

  function loadJS(path, loader){
    return loader.loadJS(path).
      then(function(){
        try{
          //the class in this Javascript file should
          // match the name of the state
          instance = new window[data.name]();
        } catch(e){
          log.error('no constructor for',data.name,e);
        }
        log.debug('done loading',path);
      });
  }

  function loadJSON(path, loader){
    return loader.loadJSON(path).
      catch(function(reason){
        log.warn('problem loading JSON',reason);
        //error loading JSON
        // let's provide some defaults
        return {name:path};
      }).
      then(function resolve(json){
        //save the data, and keep passing it down the Promise chain
        data = json;
        return json;
      });
  }

  function loadHTML(path, contextPromise, options, container, loader){
    return loader.loadHTML(path, contextPromise, options).
      then(function(markup){
        //put the markup into the page
        var div = document.createElement('div');
        div.innerHTML = markup;
        var elem = div.getElementsByClassName('page')[0];
        container.appendChild(elem);
        //start with all states disabled
        elem.classList.add('disabled');
        domElem = elem;
        return elem;
      });
  }
}
