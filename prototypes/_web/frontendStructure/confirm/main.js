function confirm(){
  var buttons = document.getElementById('confirm').
                  getElementsByClassName('buttons')[0];
  buttons.getElementsByClassName('next')[0].
    onclick = function(){
      window.dispatchEvent(new Event('confirm_next'));
    };
  buttons.getElementsByClassName('prev')[0].
    onclick = function(){
      window.dispatchEvent(new Event('confirm_prev'));
    };
  this.enter = function(evt){
    document.getElementById('confirm').
      getElementsByClassName('content')[0].
      getElementsByTagName('img')[0].src = 
        window.URL.createObjectURL(evt.detail);
  };
  this.exit = function(){
    window.URL.revokeObjectURL(
      document.getElementById('confirm').
        getElementsByClassName('content')[0].
        getElementsByTagName('img')[0].src);
  };
};
