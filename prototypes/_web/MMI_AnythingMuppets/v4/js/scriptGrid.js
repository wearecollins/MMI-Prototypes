function c(w,h){
	return {
		x: w/2.0,
		y: h/2.0
	}
}

var scriptObjects = [];
var layout = 
{
	w:197,
	h:282,
	margin:25,
	x:0,
	y:0
}

var ScriptGrid = function () {

	var gridX = 3;
	var gridY = 2;

	this.startX = 0;
	this.width = window.innerWidth;
	this.height = window.innerHeight * .90;
	this.startY = window.innerHeight * .10;

	this.setup = function(){
		this.setGeometries();
		clearScripts();

		// assuming HTML already set up

		var startX = layout.x + this.startX;
		var startY = this.startY;
		var row = 0;
		for (var i=0; i<scripts.length; i++){
			var s = new Script(scripts[i]);
			scriptObjects.push(s);

			var div = document.getElementById(scripts[i].name_clean);

			jQuery.data( div, "data", {
				object: s,
				bOpen: false
			});

			s.div = div;

			div.style.left = layout.x +"px";
			div.style.top = startY + layout.y +"px";
			div.style.width = layout.w +"px";
			div.style.height = layout.h +"px";

			layout.x = layout.x + layout.w + layout.margin;
			row++;
			if (row == 3){
				layout.x = startX;
				layout.y += layout.h + layout.margin;
				row = 0;
			} else {
				bAdd = true;
			}
		}

		var currentDiv = null;

		$(".scriptClick").click(function(e){
			var div = e.target.parentNode;
			this.openDiv(div);
		}.bind(this))

		$("#underlay").click(function(){
			if (currentDiv != null){
				this.openDiv(currentDiv);
			}
		}.bind(this));

		if ( !window.shakeInterval )
			window.shakeInterval = setInterval(this.shake.bind(this), 5000);

		this.shake();
	}

	this.windowResize = function(){
		this.setup();
	}

	this.setGeometries = function(){
		if ( this.width > this.height ){
			layout.h = (this.height * .9) / gridY;
			layout.w = (layout.h / 17) * 11;
			layout.margin = layout.h * .1;
			layout.h -= (layout.margin);

		} else {
			layout.w = (this.width * .9) / gridX;
			layout.margin = layout.w * .1;
			layout.w -= (layout.margin);
			layout.h = (layout.w / 11) * 17;
		}

		layout.x = c(this.width, this.height).x - (layout.w * gridX + layout.margin * (gridX-1))/2;
		layout.y = c(this.width, this.height).y - (layout.h * gridY + layout.margin * (gridY-1))/2;

		this.startX = 0;
		this.startY = window.innerHeight * .10;
	}

	function clearScripts(){
		scriptObjects = [];
	}

	this.divIsSelected = false;

	this.openDiv = function(div){
		var data = jQuery.data( div, "data");

		// close oldies
		for (var i=0; i<scriptObjects.length; i++){
			var div2 = scriptObjects[i].div;
			if ( div != div2 ){
				var d2 = jQuery.data(div2, "data");
				if (d2.bOpen ){
					d2.bOpen = false;
					d2.object.stopType();
					div2.style.width = layout.w + "px";
					div2.style.height = layout.h + "px";
					div2.style.left = data.original.x;
					div2.style.top = data.original.y;

					div2.querySelector(".actualTitle").style.top = "40%";

					div2.style.zIndex = 4;
					setTimeout(function(div){
						div.style.zIndex = 0;
					}, 500, div2);
				}
			}
		}

		if (!data.bOpen){
			data.bOpen = true;
			data.original = {x:div.style.left, y:div.style.top};

			div.style.zIndex = 5;

			var scale = (this.height - (layout.margin*2))/layout.h;
			var cX = (this.width/2.0) - (layout.w * scale)/2.
			var cY = this.startY + (this.height/2.0) - (layout.h * scale)/2.;

			div.style.width = (layout.w * scale) +"px";
			div.style.height = (layout.h * scale) +"px";
			div.style.left = cX +"px";
			div.style.top = cY +"px";
			div.querySelector(".actualTitle").style.top = "10%";
			
			$("#underlay").css("opacity", 1);
			$("#underlay").css("z-index", 4);

			setTimeout(function(obj, scope){
				obj.startType();
				obj.typeDone = function(){
					scope.onTypeDone();
				}.bind(scope);
			}, 500, data.object, this)

			currentDiv = div;
			this.divIsSelected = true;

			this.onOpen( data );
		} else {
			data.bOpen = false;
			data.object.stopType();
			div.style.width = layout.w + "px";
			div.style.height = layout.h + "px";
			div.style.left = data.original.x;
			div.style.top = data.original.y;

			div.querySelector(".actualTitle").style.top = "40%";
			setTimeout(function(div){
				div.style.zIndex = 0;
			}, 500, div);

			$("#underlay").css("opacity", 0);
			$("#underlay").css("z-index", 0);

			currentDiv = null;
			this.divIsSelected = false;
			this.onClose();
		}
	}

	this.onOpen = function(){

	}

	this.onClose = function(){
		
	}

	this.onTypeDone = function(){
		
	}

	this.shake = function(){
		if ( this.divIsSelected === true ) return;

		for (var i=0; i<scriptObjects.length; i++){
			var div = scriptObjects[i].div;
			setTimeout(function(time, div){
				div.style.transform = "translateY(-5%)";
				var t = div.querySelector("#title");
				if ( t ) t.style.backgroundColor = getColorFill(getNextColor());

				setTimeout(function(){
					div.style.transform = "translateY(0%)";
				}, time, div);
			}, i * 50, (i+1) * 100, div);
		}
	}
}

var Script = function( obj ){
	this.object = null;
	this.div = null;

	this.setup = function(obj){
		this.object = obj;
	}

	var typeInterval;
	var index = 0;

	this.startType =function(){
		var textDiv = this.div.querySelector("#text");
		index = 0;
		typeInterval = setInterval(function(){
			// to-do: store HTML els, won't let you just do partial zones
			var nextText = this.object.text[index];
			if (nextText == "<"){
				index++;
				var t = this.object.text[index];
				while( t != ">"){
					nextText += t;
					index++;
					t = this.object.text[index];
				}
				nextText += t;
			}
			textDiv.innerHTML += nextText;
			index++;
			if ( index >= this.object.text.length){
				clearInterval(typeInterval);
				this.typeDone();
			}
		}.bind(this), 25);
	}

	this.typeDone = function(){

	}

	this.stopType = function(){
		clearInterval(typeInterval);
		this.div.querySelector("#text").innerHTML = "";
	}

	this.setup( obj );
}