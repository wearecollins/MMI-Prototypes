function c(){
	return {
		x: window.innerWidth/2.0,
		y: window.innerHeight/2.0
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

var ScriptGrid = function (container) {
	this.container = container;

	var gridX = 3;
	var gridY = 2;

	this.setup = function(){
		this.setGeometries();
		clearScripts();

		var startX = layout.x;

		var row = 0;
		for (var i=0; i<scripts.length; i++){
			var s = new Script(scripts[i]);
			scriptObjects.push(s);

			var div = s.getDOMObject();
			this.container.appendChild(div);

			jQuery.data( div, "data", {
				object: s,
				bOpen: false
			});

			s.div = div;

			div.style.left = layout.x +"px";
			div.style.top = layout.y +"px";

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
			var div = e.target.parentNode.parentNode;
			openDiv(div);
		})

		$("#overlay").click(function(){
			if (currentDiv != null){
				openDiv(currentDiv);
			}
		})
	}

	this.windowResize = function(){
		this.setup();
	}

	this.setGeometries = function(){
		layout.w = (window.innerWidth * .8) / gridX;
		layout.margin = layout.w * .1;
		layout.w -= (layout.margin);
		layout.h = (layout.w / 11) * 17;

		layout.x = c().x - (layout.w * gridX + layout.margin * (gridX-1))/2;
		layout.y = c().y - (layout.h * gridY + layout.margin * (gridY-1))/2;
	}

	function clearScripts(){
		for ( var i=0; i<scriptObjects.length; i++){
			this.container.removeChild(scriptObjects[i].div);
		}
		scriptObjects = [];
	}

	this.setup();
}

var Script = function( obj ){
	this.object = null;
	this.div = null;

	var div;

	this.setup = function(obj){
		this.object = obj;

		div = document.createElement("div");
		div.className = "scriptContainer";
		div.id = obj.name_clean;
		this.object.colorFill = getColorFill(obj.color);

		var template = Handlebars.getTemplate("script", "templates");
		var html = template(obj)
		div.innerHTML = html;
	}

	this.getDOMObject = function(){
		return div;
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
			}
		}.bind(this), 25);
	}

	this.stopType = function(){
		clearInterval(typeInterval);
		this.div.querySelector("#text").innerHTML = "";
	}

	this.setup( obj );
}

function openDiv(div){
	var data = jQuery.data( div, "data");

	// close oldies
	for (var i=0; i<scriptObjects.length; i++){
		var div2 = scriptObjects[i].div;
		if ( div != div2 ){
			var d2 = jQuery.data(div2, "data");
			if (d2.bOpen ){
				d2.bOpen = false;
				d2.object.stopType();
				div2.style.width = "197px";
				div2.style.height = "282px";
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

		var scale = (window.innerHeight - (layout.margin*2))/layout.h;
		var cX = (window.innerWidth/2.0) - (layout.w * scale)/2.
		var cY = (window.innerHeight/2.0) - (layout.h * scale)/2.;

		div.style.width = (layout.w * scale) +"px";
		div.style.height = (layout.h * scale) +"px";
		div.style.left = cX +"px";
		div.style.top = cY +"px";
		div.querySelector(".actualTitle").style.top = "10%";
		
		$("#overlay").css("opacity", 1);
		$("#overlay").css("z-index", 4);

		setTimeout(function(obj){
			obj.startType();
		}, 500, data.object)

		currentDiv = div;
	} else {
		data.bOpen = false;
		data.object.stopType();
		div.style.width = "197px";
		div.style.height = "282px";
		div.style.left = data.original.x;
		div.style.top = data.original.y;
		div.querySelector(".actualTitle").style.top = "40%";
		setTimeout(function(div){
			div.style.zIndex = 0;
		}, 500, div);

		$("#overlay").css("opacity", 0);
		$("#overlay").css("z-index", 0);

		currentDiv = null;
	}
}