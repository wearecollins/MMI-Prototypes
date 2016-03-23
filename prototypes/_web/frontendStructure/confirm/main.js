function confirm(manager){
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
