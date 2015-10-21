var VO = function () {
	this.dom = null;
	this.parent = null;

	var playing = false;

	this.load = function( src, parent ){
		this.dom = document.createElement("audio");
		if ( parent === undefined ){
			parent = document.body;
		}
		this.parent = parent;
		this.dom.src = src;
		this.parent.appendChild(this.dom);
		this.dom.preload;
	}

	this.play = function(){
		if ( playing ) return;
		this.dom.play();
		playing = true;
	}

	this.stop = function(){
		if ( !playing ) return;
		this.dom.pause();
		this.dom.currentTime = 0;
		playing = false;
	}

	this.isPlaying = function(){
		var ret = playing ? true : false;
		return ret;
	}
}