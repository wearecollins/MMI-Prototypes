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

	// for tracking swipe
	var mouseDistance = {x:0, y:0};
	var mouseLast = {x:0, y:0};
	var mouseStart = {x:0, y:0};
	var moveTarget = {x:0, y:0};

	// TODO: original touch offset

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
			var div = $("#script_"+scripts[i].name_clean)[0];
			jQuery.data( div, "data", {
				scriptObject: scriptObjects[i],
				parentDiv: $("#script_"+scripts[i].name_clean),
				top: y
			} );

			var id = "#script_"+scripts[i].name_clean;
			$(id).css("top", y + random(-100, 100));
			$(id).css("left", random(-100, 100));
			$(id).css("transform", "rotate(" + random(-15, 15) + "deg)");
			scriptObjects[i].realDiv = $(id)[0];

			y -= 15;
		}

		$("#overlay").click(function(){
			// hm
		}.bind(this));

		function down(event){
			event.preventDefault();

			clearInterval(interactInterval);

			var cX = event.clientX;
			var cY = event.clientY;

			if ( event.touches ){
				cX = event.touches[0].clientX;
				cY = event.touches[0].clientY;
			}

			interactInterval = setInterval(app.shake.bind(app), 15000);

			var object = scriptObjects[scriptObjects.length-1];

			currentData = jQuery.data(object.realDiv, "data");
			currentDiv  = object.realDiv;

			currentData.isDown = true;

			mouseStart.x = cX;
			mouseStart.y = cY;
			mouseLast.x  = cX;
			mouseLast.y  = cY;
		}

		function move(event){
			event.preventDefault();
			return;
			var cX = event.clientX;
			var cY = event.clientY;

			if ( event.touches ){
				cX = event.touches[0].clientX;
				cY = event.touches[0].clientY;
			}

			if ( currentData && currentData.isDown){
				mouseDistance.x = (mouseLast.x - cX) * -20.;
				mouseDistance.y = (mouseLast.y - cY) * -20.;

				var l = currentData.parentDiv.position().left + mouseDistance.x;
				var t = currentData.parentDiv.position().top + mouseDistance.y;
				currentData.parentDiv.css("left", l );
				currentData.parentDiv.css("top", t );

				mouseLast.x = cX;
				mouseLast.y = cY;

				moveTarget.x = l;
			}
		}


		function end(evt){
			evt.preventDefault();
			if ( currentData == null ){
				return;
			}
			// var object = scriptObjects[scriptObjects.length-1];
			// currentData = jQuery.data(object.realDiv, "data");
			// currentDiv  = object.realDiv;

			var l = currentData.parentDiv.offset().left;
			var t = currentData.parentDiv.offset().top;

			console.log( t );

			if ( l > 300 || l < 0 || t < 100 || t > 500 ){
				setTimeout( 
					function(el){
						el.css("top", random(-200, 100));
						el.css("left", random(-100, 100));
						el.css("transform", "rotate(" + random(-15, 15) + "deg)");

						// swap!
						var obj = scriptObjects[scriptObjects.length-1];
						scriptObjects.pop();
						scriptObjects.splice(obj, 0, obj);

						var object = scriptObjects[scriptObjects.length-1];

						currentData = jQuery.data(object.realDiv, "data");
						currentDiv  = object.realDiv;

						for ( var i=0;i<scriptObjects.length; i++){
							scriptObjects[i].realDiv.style.zIndex = i;
						}

						setTimeout(function(el){
							el.css("transition", "transform ease-in-out .5s, width ease-in-out .25s,height ease-in-out .5s, background-color ease-in-out .5s, box-shadow ease-in-out .5s, top ease-in-out, left ease-in-out");
						}, 200, el);

					}, 5, currentData.parentDiv
				)
				currentData.parentDiv.css("transition", "transform ease-in-out .5s, width ease-in-out .25s,height ease-in-out .5s, background-color ease-in-out .5s, box-shadow ease-in-out .5s, top ease-in-out .2s, left ease-in-out .2s")
			}

			if ( currentData ){
				currentData.isDown = false;
			}
			currentDiv = null;
			currentData = null;
		}

		// $(window).mousedown(down);
		// $(window).mousemove(move);
		// $(window).mouseup(end);

		// window.addEventListener("touchstart", down, false);
		window.addEventListener("touchmove", move, false);
		window.addEventListener("touchend", end, false);
		
		$(function() {
		    $( ".script" ).draggable({
		    	start: function(e) {
		    		currentDiv = this;
					currentData = jQuery.data(currentDiv, "data");
				},
				drag: function(e) {
				},
				stop: function(e) {
				},
				stack: ".script"
      		});
		});

		interactInterval = setInterval(this.shake.bind(this), 15000);

		this.shake();
		// this.animate();
	}

	this.onMouseUp = function(){

	}

	this.animate = function(){
		window.requestAnimationFrame(this.animate.bind(this));
		this.update();
		this.draw();
	}

	this.update 	= function(){

	}

	this.draw 	= function(){
		if (currentDiv != null){
			// currentDiv.left = window.
		}
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