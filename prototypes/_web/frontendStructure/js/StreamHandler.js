function StreamHandler(){

  var domElem;
  var prevURL;

  this.init = function init(streamSource, domDestination){
    streamSource.addBinaryHandler(handleImage);
    domElem = document.createElement('img');
    domDestination.appendChild(domElem);
  };

  this.showStream = function showStream(){
    log.warn('TODO: show stream!');
  };

  this.hideStream = function hideStream(){
    log.warn('TODO: hide stream!');
  };

  //images come as binary from the server
  // lets just dump them into an <img>
  function handleImage(buf){
    var view = new Uint8Array(buf);
    var blob = new Blob([view], {type: 'image/jpeg'});
    var url = window.URL.createObjectURL(blob);
    domElem.src = url;
    if (prevURL !== undefined){
      window.URL.revokeObjectURL(prevURL);
    }
    prevURL = url;
  }
}
