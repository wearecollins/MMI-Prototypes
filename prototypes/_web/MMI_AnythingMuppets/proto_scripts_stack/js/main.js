Handlebars.getTemplate = function(name, folder) {
    if (Handlebars.templates === undefined || Handlebars.templates[name] === undefined) {
        $.ajax({
            url : folder + '/' + name + '.handlebars',
            success : function(data) {
                if (Handlebars.templates === undefined) {
                    Handlebars.templates = {};
                }
                Handlebars.templates[name] = Handlebars.compile(data);
            },
            async : false
        });
    }
    return Handlebars.templates[name];
};

var app;

$(window).ready(function(){
	app = new App();
})

function random(x, y) {
	var high = Math.max(x, y);
	var low = Math.min(x, y);
	return Math.max(low, (low + (high - low) * Math.random() ));
}

var App = function(){

	var scriptObjects = [];

	var currentDiv = null;
	var currentData = null;

	var SCRIPT_HEIGHT = 50;

	var interactInterval;

	this.setup = function(){
		var y = 0;

		for ( var i=0; i<scripts.length; i++ ){
			scriptObjects.push( new Script( scripts[i] ) );
			var div = scriptObjects[i].getDOMElement();
			document.getElementById("scriptContainer").innerHTML += div;

			var id = "#script_"+scripts[i].name_clean;
			$(id).css("top", y);
			scriptObjects[i].realDiv = $(id)[0];
		}

		y = 0;

		for ( var i=0; i<scripts.length; i++ ){
			var name = "#script_" + scripts[i].name_clean + "_click";
			var div = $(name)[0];
			jQuery.data( div, "data", {
				scriptObject: scriptObjects[i],
				parentDiv: $("#script_"+scripts[i].name_clean),
				top: y
			} );

			var w = this;

			$(name).click(
				function(){
					clearInterval(interactInterval);
					interactInterval = setInterval(w.shake.bind(w), 15000);

					var data = jQuery.data(this, "data");
					w.sortScripts(this, data);
				}
			);

			var id = "#script_"+scripts[i].name_clean;
			$(id).css("top", y + random(-50, 50));
			$(id).css("left", random(-50, 50));
			$(id).css("transform", "rotate(" + random(-15, 15) + "deg)");
			scriptObjects[i].realDiv = $(id)[0];

			// y += SCRIPT_HEIGHT;
		}

		$("#overlay").click(function(){
			this.sortScripts(currentDiv, currentData);
			$("#overlay").css("background-color", "rgab(0,0,0,0)");
		}.bind(this));

		interactInterval = setInterval(this.shake.bind(this), 15000);

		this.animate();
	}

	this.animate = function(){
		window.requestAnimationFrame(this.animate.bind(this));
		this.update();
		this.draw();
	}

	this.update 	= function(){

	}

	this.draw 	= function(){

	}

	this.shake  = function(){
		for ( var i=0; i<scriptObjects.length; i++ ){
			var t = i * 50;
			var amt = 600;

			colorStart = getRandomColorHS();

			setTimeout(function(d, t, amt){
				d.fill = getNextColor()
				d.realDiv.style.backgroundColor = d.getFill();
			}, t, scriptObjects[i], t, amt);
		}
	}

	this.shakeFrom  = function(index){
		for ( var i=index; i<scriptObjects.length; i++ ){
			var t = i * 50;
			var amt = 600;

			colorStart = getRandomColorHS();

			setTimeout(function(d, t, amt){
				d.fill = getNextColor()
				d.realDiv.style.backgroundColor = d.getFill();
			}, t, scriptObjects[i], t, amt);
		}

		for ( var i=index-1; i>=0; i-- ){
			var t = i * 50;
			var amt = 600;

			colorStart = getRandomColorHS();

			setTimeout(function(d, t, amt){
				d.fill = getNextColor()
				d.realDiv.style.backgroundColor = d.getFill();
			}, t, scriptObjects[i], t, amt);
		}
	}

	this.sortScripts = function( newSelect, data ){
		// first sort old
		var y = 0;
		var h_o = 400;
		var ind = 0;

		// reset css
		for ( var i=0; i<scriptObjects.length; i++ ){
			if ( scriptObjects[i] == data.scriptObject ){
				ind = i;
			} else {
				scriptObjects[i].realDiv.style["box-shadow"] = "0px 0px 10px rgba(0,0,0,.5)";
				scriptObjects[i].realDiv.style.left = random(-50, 50) +"px";
				scriptObjects[i].realDiv.style.top = (y + random(-50, 50)) +"px";
				scriptObjects[i].realDiv.style.transform = "rotate(" + random(-15, 15) + "deg)";
				scriptObjects[i].realDiv.style.zIndex = i;
			}
			// y += SCRIPT_HEIGHT;
		}

		if ( newSelect == currentDiv && currentDiv != null ){
			// shhhh
			currentDiv = null;
			
			data.parentDiv.css("transform", "rotate(" + random(-15, 15) + "deg), translateZ(0)");
			data.parentDiv.css("box-shadow", "0px 0px 10px rgba(0,0,0,.5)");
			data.parentDiv.css("left", random(-50, 50) +"px");
			data.parentDiv.css("top", (y + random(-50, 50)) +"px");
			$("#overlay").css("background-color", "rgba(0,0,0,0)");
			$("#overlay").css("z-index", -1);

		} else {

			for ( var i=0; i<scriptObjects.length; i++ ){
				// scriptObjects[i].realDiv.style.backgroundColor = scriptObjects[i].getFillDark();
			}
			
			data.parentDiv.css("transform", "rotate(0deg), translateZ(0)");
			data.parentDiv.css("z-index", scriptObjects.length+1);
			data.parentDiv.css("box-shadow", "0px 0px 100px black");
			$("#overlay").css("z-index",scriptObjects.length);
			$("#overlay").css("background-color", "rgba(0,0,0,.5)");

			// need to shift?
			// var found = false;
			// for ( var i=0; i<scriptObjects.length; i++ ){
			// 	if ( scriptObjects[i] == data.scriptObject ){
			// 		found = true;
			// 	} else if ( found ){
			// 		// scriptObjects[i].realDiv.style.top = newY +"px";
			// 		// y += SCRIPT_HEIGHT;
			// 	}

			// }
		}

		currentDiv = newSelect;
		currentData = data;
	}

	this.setup();
}

// pass in el from array
var Script = function( el ){
	var html;

	this.realDiv =null; //jquery sel for now!

	this.setup = function( el ){
		this.name = el.name;
		this.fill = el.color;
		this.text = el.text;

		el.colorFill = this.getFill();

		var template = Handlebars.getTemplate("script", "templates");
		html = template(el)
	}

	this.getDOMElement = function(){
		return html;
	}

	this.getFill = function(){
		return "rgb("+ this.fill.r + "," + this.fill.g + "," + this.fill.b + ")";
	}

	this.getFillDark = function(){
		var dark = {r:0, g:0, b:0};
		dark.r = Math.round(dark.r * .5 + this.fill.r * .5);
		dark.g = Math.round(dark.g * .5 + this.fill.g * .5);
		dark.b = Math.round(dark.b * .5 + this.fill.b * .5);

		return "rgb("+ dark.r + "," + dark.g + "," + dark.b + ")";
	}

	this.setup(el);
}