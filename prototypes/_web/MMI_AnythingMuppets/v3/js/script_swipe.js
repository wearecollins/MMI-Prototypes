function setupScriptsSwipe(container, dataManager, scripts) {
	var type = "swipe"
	var template = Handlebars.getTemplate("scripts_"+type, "templates");
	var html = template({"scripts":scripts});
	$(container).html(html);

	dataManagerGlobal = dataManager; //oof

	// scripts R built
	var y = 0;

	for ( var i=0; i<scripts.length; i++ ){
		scriptObjects.push( new Script( scripts[i] ) );
		var id = "#script_"+scripts[i].name_clean;
		scriptObjects[i].realDiv = $(id)[0];
	}

	y = 100;

	for ( var i=0; i<scripts.length; i++ ){
		var name = "#script_" + scripts[i].name_clean + "_click";
		var div = $("#script_"+scripts[i].name_clean)[0];
		jQuery.data( div, "data", {
			scriptObject: scriptObjects[i],
			parentDiv: $("#script_"+scripts[i].name_clean),
			top: y
		} );

		var id = "#script_"+scripts[i].name_clean;
		$(id).css("top", y + random(-50, 50));
		$(id).css("left", random(-100, 100));
		$(id).css("transform", "rotate(" + random(-15, 15) + "deg)");
		scriptObjects[i].realDiv = $(id)[0];

		y -= 15;
	}

	// hmmm
	dataManagerGlobal.name = scripts[0].name;
	$("#info_name").html( dataManagerGlobal.name );

	buildScriptInteraction();
}

function buildScriptInteraction(){

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

		if ( l > 300 || l < 0 || t < 100 || t > 500 ){
			setTimeout( 
				function(el){
					el.css("top", random(-50, 200));
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

	// window.addEventListener("touchstart", down, false);
	// window.addEventListener("touchmove", move, false);
	window.addEventListener("touchend", end, false);

	// window.addEventListener("mousedown", move, false);
	// window.addEventListener("mouseup", end, false);
	
	$(function() {
	    $( ".script" ).draggable({
	    	start: function(e) {
	    		currentDiv = this;
				currentData = jQuery.data(currentDiv, "data");

				dataManagerGlobal.name = currentData.scriptObject.name;
				$("#info_name").html( dataManagerGlobal.name );
			},
			drag: function(e) {
			},
			stop: function(e) {
				var l = currentData.parentDiv.offset().left;
				var t = currentData.parentDiv.offset().top;

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
																
							dataManagerGlobal.name = currentData.scriptObject.name;
							$("#info_name").html( dataManagerGlobal.name );

						}, 5, currentData.parentDiv
					)
					currentData.parentDiv.css("transition", "transform ease-in-out .5s, width ease-in-out .25s,height ease-in-out .5s, background-color ease-in-out .5s, box-shadow ease-in-out .5s, top ease-in-out .2s, left ease-in-out .2s")
				}

				if ( currentData ){
					currentData.isDown = false;
				}
				currentDiv = null;
				currentData = null;
			},
			stack: ".script"
  		});
	});

	interactInterval = setInterval(randomizeSwipes, 15000);

	randomizeSwipes();
}

function randomizeSwipes(){
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