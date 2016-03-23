function liveView(){
  var frame;
  var countdownTimeout;
  var imgContainer = 
    document.getElementById('liveView').getElementsByTagName('img')[0];
  document.getElementById('liveView').
    getElementsByClassName('buttons')[0].
    getElementsByClassName('next')[0].
    onclick = function(){
      document.getElementById('liveView').classList.add('countdown');
      countdownTimeout = setTimeout(function(){
        window.dispatchEvent(new CustomEvent('liveView_next', {detail:frame}));
      }, 3000);
    };
  function handleStream(url, blob){
    //imgContainer.style.backgroundImage = 'url('+url+')';
    imgContainer.src = url;
    frame = blob;
  };
  this.enter = function(){
    registerStreamTarget(handleStream);
  };
  this.exit = function(){
    document.getElementById('liveView').classList.remove('countdown');
    unregisterStreamTarget(handleStream);
  };
};
