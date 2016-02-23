function setupScriptsStack(container, dataManager, scripts){
	var type = "stack"
	var template = Handlebars.getTemplate("scripts_"+type, "templates");
	var html = template({"scripts":scripts});
	$(container).html(html);

	dataManagerGlobal = dataManager; //oof

	for ( var i=0; i<scripts.length; i++ ){
		scriptObjects.push( new Script( scripts[i] ) );
		var id = "#script_"+scripts[i].name_clean;
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

		$(name).click(
			function(){
				clearInterval(interactInterval);
				interactInterval = setInterval(randomizeStack, 15000);

				var data = jQuery.data(this, "data");
				sortScripts(this, data);
			}
		);


		var id = "#script_"+scripts[i].name_clean;
		$(id).css("top", y);
		scriptObjects[i].realDiv = $(id)[0];

		// y += SCRIPT_HEIGHT;
	}

	$("#no_selected_character").css("visibility", "visible");
	$("#no_selected_character").css("display", "block");
	
	$("#selected_character").css("visibility", "hidden");
	$("#selected_character").css("display", "none");

	setTimeout(function(){
		var id = "#script_"+scripts[Math.floor(Math.random() * scripts.length)].name_clean  + "_click";
		var data = jQuery.data($(id)[0], "data");
		sortScripts($(id)[0], data);
	}, 500)

	interactInterval = setInterval(randomizeStack, 15000);
}

function randomizeStack(){

}

function sortScripts( newSelect, data ){
	// first sort old
	var y = 0;
	var h_o = 400;
	var ind = 0;

	if ( currentData != null ){		
		$("#scriptButtons_"+currentData.scriptObject.name_clean).css("visibility", "hidden");
		$("#more_"+currentData.scriptObject.name_clean).css("visibility", "hidden");
		currentData.parentDiv.css("flex", "0 1 auto");
	}

	// reset css
	for ( var i=0; i<scriptObjects.length; i++ ){
		scriptObjects[i].realDiv.style.height = (SCRIPT_HEIGHT - 10) +"px";
		scriptObjects[i].realDiv.style.top = y +"px";
		scriptObjects[i].realDiv.style.transform = "rotateX(-15deg) rotateY(0deg)";
		scriptObjects[i].realDiv.style.zIndex = "0";
		// scriptObjects[i].realDiv.style.backgroundColor = scriptObjects[i].getFill();
		// scriptObjects[i].realDiv.style["box-shadow"] = "none";

		// y += SCRIPT_HEIGHT;
		if ( scriptObjects[i] == data.scriptObject ){
			ind = i;
		}
	}

	if ( newSelect == currentDiv ){

		currentData = jQuery.data(currentDiv, "data");
		$("#scriptButtons_"+currentData.scriptObject.name_clean).css("visibility", "hidden");
		$("#more_"+currentData.scriptObject.name_clean).css("visibility", "hidden");
		$("#more_"+currentData.scriptObject.name_clean).unbind("click");

		// shhhh
		currentDiv = null;
		currentData = null;

		$("#no_selected_character").css("visibility", "visible");
		$("#no_selected_character").css("display", "block");
		$("#selected_character").css("visibility", "hidden");
		$("#selected_character").css("display", "none");

		return;
	} else {
		var realY = SCRIPT_HEIGHT * ind;
		var newY = window.innerHeight/2.0-(h_o);

		if (realY + newY < 0 ){
			newY = -realY;
		}

		for ( var i=0; i<scriptObjects.length; i++ ){
			// scriptObjects[i].realDiv.style.backgroundColor = scriptObjects[i].getFillDark();
		}
		
		data.parentDiv.css("top", newY + "px");
		data.parentDiv.css("height", h_o);
		data.parentDiv.css("z-index", 1);
		data.parentDiv.css("transform", "translate3d(0px, 20px, 50px) rotateX(0deg)");

		$("#scriptButtons_"+data.scriptObject.name_clean).css("visibility", "visible");
		$("#more_"+data.scriptObject.name_clean).css("visibility", "visible");

		// need to shift?

		$("#script_" + data.scriptObject.name_clean + "_can").click(function(){
			sortScripts(newSelect, data)
		});

		$("#more_"+data.scriptObject.name_clean).click(openCurrentScript);

		if ( newY != data.top - h_o){
			var found = false;

			for ( var i=0; i<scriptObjects.length; i++ ){
				if ( scriptObjects[i] == data.scriptObject ){
					found = true;
				} else if ( found ){
					scriptObjects[i].realDiv.style.top = newY +"px";
					// y += SCRIPT_HEIGHT;
				}

			}
		}
	}

	currentDiv = newSelect;

	currentData = jQuery.data(currentDiv, "data");
	dataManagerGlobal.name = currentData.scriptObject.name;
	$("#info_name").html( dataManagerGlobal.name );

	$("#selected_character").css("visibility", "visible");
	$("#selected_character").css("display", "block");
	$("#no_selected_character").css("visibility", "hidden");
	$("#no_selected_character").css("display", "none");
}

function openCurrentScript(){
	currentData.parentDiv.css("height", window.innerHeight * .7);
	currentData.parentDiv.css("flex", "none");
		
	var newY = -((window.innerHeight * .85) - (window.innerHeight * .7))/2;
	currentData.parentDiv.css("top", newY + "px");
	$("#more_"+currentData.scriptObject.name_clean).css("visibility", "hidden");

	// and tell something to main app?
}
