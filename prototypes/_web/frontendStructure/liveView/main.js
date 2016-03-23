function liveView(manager){
  var frame;
  var countdownTimeout;
  var imgContainer = 
    document.getElementById('liveView').
      getElementsByClassName('background')[0].
      getElementsByTagName('img')[0];
  document.getElementById('liveView').
    getElementsByClassName('buttons')[0].
    getElementsByClassName('liveView_snap')[0].
    onclick = function(){
      document.getElementById('liveView').classList.add('countdown');
      countdownTimeout = setTimeout(function(){
        window.dispatchEvent(new CustomEvent('next', {detail:frame}));
      }, 3000);
    };
  function handleStream(url, blob){
    imgContainer.src = url;
    frame = blob;
  };
  this.enter = function(){
    manager.registerStreamTarget(handleStream);
  };
  this.exit = function(){
    document.getElementById('liveView').classList.remove('countdown');
    manager.unregisterStreamTarget(handleStream);
  };
};
