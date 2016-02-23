// globals
var scriptObjects = [];
var dataManagerGlobal;
var SCRIPT_HEIGHT = 38;
var currentDiv = null;
var currentData = null;

// pass in el from array
var Script = function( el ){
	var html;

	this.realDiv =null; //jquery sel for now!

	this.setup = function( el ){
		this.name = el.name;
		this.name_clean = el.name_clean;
		this.fill = el.color;
		this.text = el.text;

		el.colorFill = this.getFill();
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