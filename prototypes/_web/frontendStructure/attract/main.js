function attract(){
  document.getElementById('attract').
    getElementsByClassName('buttons')[0].
    getElementsByClassName('next')[0].
    onclick = function(){
      window.dispatchEvent(new Event('attract_next'));
    };
  this.enter = function(){
  };
  this.exit = function(){
  };
};
