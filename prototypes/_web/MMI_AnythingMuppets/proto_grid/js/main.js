var app, grid;

window.onload =function () {
	
	// setup scripts
	app = new App();
}

var App = function(){
	var scriptObjects = [];

	var w = 197;
	var h = 282;
	var margin = 25;
	var x = 0;
	var y = 0;

	this.setup = function(){
		x = window.innerWidth/2.0 - (w * 3 + margin * 3)/2;
		y = window.innerHeight/2.0 - (h * 2 + margin * 2)/2;

		var container = document.getElementById("container");

		grid = new ScriptGrid(container);
	}

	this.setup();
}