// handlebars stuff
// http://stackoverflow.com/questions/23013447/how-to-define-handlebar-js-templates-in-an-external-file
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

/**
 * 
 */
var app;

$(document).ready(function() {
	app = new App();
	app.setup();
});

var App = function(){

	// where HTML goes
	var htmlContainer = "#container";

	// state manager
	var stateManager 	= new StateManager();
	var dataManager 	= new DataManager();

	// spacebrew
	var spacebrew 		= new SpacebrewManager();

	// timeouts
	var modeTimeout		= null;
	var modeSwitchTime	= 120000;

	var scriptData = 
	{ 
		title : 'Find Your Character',  
		scripts : 
		[
			{
				name:"Sad Girl",
				img:"img/script_a.jpg",
				id:"sg"
			},
			{
				name:"Old Man",
				img:"img/script_b.jpg",
				id:"om"
			},
			{
				name:"Scary Monster",
				img:"img/script_c.jpg",
				id:"sm"
			}
		]
	};

	this.setup = function(){
		// load home template
		setMode( STATE_INTRO );

		// setup sb
		spacebrew.setup();

		// ?

		window.addEventListener("drawer", this.onDrawer.bind(this) );
		window.addEventListener("spin", this.onSpin.bind(this) );

		spacebrew.addListener("drawer", window );
		spacebrew.addListener("spin", window );

		this.animate();
	}

	this.update = function(){

	}

	this.draw 	= function(){

	}

	this.animate = function(){
		window.requestAnimationFrame( this.animate.bind(this) );
		this.update();
		this.draw();
	}

	/*******************************************************
		UTILS
	*******************************************************/

	var timeouts = [];

	function timeout( fun, time ){
		timeouts.push( setTimeout(fun, time ) );
	}

	/*******************************************************
		MODE SWITCH
	*******************************************************/

	function setMode( mode ){
		if (stateManager.state == mode ) return;

		for ( var i=0; i<timeouts.length; i++){
			clearTimeout(timeouts[i]);
		}
		timeouts = [];

		if ( modeTimeout != null ) clearTimeout( modeTimeout );

		switch( mode ){
			case STATE_INTRO:
			{
				var template = Handlebars.getTemplate("intro", "templates");
				var html = template({ video :"videos/consider_yourself.mp4" });
				$(htmlContainer).html(html);
				$("#b_getstarted").click(function(){
					setMode( STATE_SCRIPT );
				})

				stateManager.reset();
				dataManager.reset();
			}
			break;

			case STATE_SCRIPT:
			{
				var template = Handlebars.getTemplate("scripts", "templates");
				var html = template(scriptData);
				$(htmlContainer).html(html);
				dataManager.name = scriptData.scripts[0].name;
				$("#info_name").html( dataManager.name );
				timeout( function(){
					var x = 0;
					for (var i=0; i<scriptData.scripts.length; i++){
						var s = scriptData.scripts[i];
						$("#"+s.id).css("left", x);
						x += $("#"+s.id).width();
					}
				}, 0);
				$("#prev").click(nextScript);
				$("#next").click(prevScript);

				setScriptTimeouts();
			}
			break;

			case STATE_CAMERA_A:
			{
				camera_countdown = CAMERA_COUNTDOWN_MAX;
				var template = Handlebars.getTemplate("camera", "templates");
				var html = template(
					{ 
						name :dataManager.name
					}
				);

				$(htmlContainer).html(html);
				
				// setup web camera
				Webcam.set({
					width:640, 
					height:480, 
		       	 	dest_width: 640,
		        	dest_height: 480
		    	});

				Webcam.attach( '#capture_element' );

				// disappear buttons
				$("#share_container").css("visibility", "hidden");
				$("#share_container").css("display", "none");

				$("#button_container").css("visibility", "visible");
				$("#button_container").css("display", "flex");
				
				$("#capture_element").css("width", "100%");
				$("#capture_element").css("height", "100%");

				$("#capture_element").css("visibility", "visible");
				$("#capture_element").css("display", "block");

				$("video").css("width", "100%");
				$("video").css("height", "100%");

				// setup buttons
				$("#b_photo").click(function(){
					setMode(STATE_CAMERA_B);
				});

				$("#b_done").click(function(){
					stateManager.didInteract = false;
					setMode( STATE_THANKS );
				});

				$("#b_retake").click(function(){
					stateManager.didInteract = false;
					stateManager.retakeCount++;
					setMode( STATE_CAMERA_B );
				});

				$("#b_share").click(function(){
					stateManager.didInteract = false;
					setMode( STATE_NAME );
				});

				modeTimeout = setTimeout(function(){
					setMode( STATE_THANKS );
				}, modeSwitchTime);
			}
			break;

			case STATE_CAMERA_B:
			{
				camera_countdown = CAMERA_COUNTDOWN_MAX;

				// disappear buttons
				$("#share_container").css("visibility", "hidden");
				$("#share_container").css("display", "none");
				$("#button_container").css("visibility", "hidden");
				$("#button_container").css("display", "none");

				// disappear capture (just in case)
				$("#capture_output").css("visibility", "hidden");
				$("#capture_output").css("display", "none");
				
				$("#capture_element").css("visibility", "visible");
				$("#capture_element").css("display", "block");

				// show countdown
				$("#countdown").css("visibility", "visible");
				$("#countdown").css("display", "flex");
				$("#countdown").html(camera_countdown);

				// setup countdown
				var interval = setInterval(function(){
					camera_countdown--;
					if (camera_countdown <= 0 ){
						clearInterval(interval);

						// capture
						Webcam.snap( function(data_uri) {
				            document.getElementById('capture_output').innerHTML = '<img src="'+data_uri+'"/>';
				        	$("#capture_output").css("visibility", "visible");
							$("#capture_output").css("display", "block");

							dataManager.url = data_uri;

				        	setMode( STATE_CAMERA_C );
				        } );
					} else {
						$("#countdown").html(camera_countdown);
					}
				}, 1000);
			}
			break;

			case STATE_CAMERA_C:
			{
				$("#countdown").css("visibility", "hidden");
				$("#countdown").css("display", "none");
				$("#capture_element").css("visibility", "hidden");
				$("#capture_element").css("display", "none");

				$("#share_container").css("visibility", "visible");
				$("#share_container").css("display", "flex");

				if ( stateManager.retakeCount > MAX_RETAKES ){
					$("#b_retake").css("visibility", "hidden");
					$("#b_retake").css("display", "none");
				}

				// display buttons
				$("#share_container").css("visibility", "visible");
				$("#share_container").css("display", "flex");
			}
			break;

			case STATE_NAME:
			{
				var template = Handlebars.getTemplate("name", "templates");
				var html = template(
					{ 
						name 	: dataManager.name,
						img 	: dataManager.url
					}
				);
				$(htmlContainer).html(html);

				// setup keyboard
				$('#c_name').keyboard({
					language     : null, // string or array
					layout       : 'qwerty',
					customLayout : { 'normal': ['{cancel}'] },
					usePreview   : false,

					// Append the keyboard to a desired element. This can be a jQuery selector
					// string or object
					appendTo: '#keyboard',
					// maxLength    : false,

					// initialized   : function(e, keyboard, el) {},
					// beforeVisible : function(e, keyboard, el) {},
					// visible       : function(e, keyboard, el) {},
					// change        : function(e, keyboard, el) {},
					// beforeClose   : function(e, keyboard, el, accepted) {},
					// accepted      : function(e, keyboard, el) {},
					// canceled      : function(e, keyboard, el) {},
					// hidden        : function(e, keyboard, el) {}
				});

				$('#y_name').keyboard({
					language     : null, // string or array
					layout       : 'qwerty',
					customLayout : { 'normal': ['{cancel}'] },
					usePreview   : false,

					// Append the keyboard to a desired element. This can be a jQuery selector
					// string or object
					appendTo: '#keyboard',
					// maxLength    : false,
				});

				$("#b_next").click(function(){
					setMode( STATE_SHARE );
				})
			}
			break;

			case STATE_SHARE:
			{
				var template = Handlebars.getTemplate("share", "templates");
				var html = template(
					{ 
						name :dataManager.name,
						img 	: dataManager.url
					}
				);
				$(htmlContainer).html(html);

				function showHideShares( id ){
					$("#interface_email").css("visibility", "hidden");
					$("#interface_email").css("display", "none");
					$("#interface_tumblr").css("visibility", "hidden");
					$("#interface_tumblr").css("display", "none");
					$("#interface_instagram").css("visibility", "hidden");
					$("#interface_instagram").css("display", "none");

					$(id).css("visibility", "visible");
					$(id).css("display", "block");
				}

				$("#b_email").click(function(){
					showHideShares("#interface_email");

				});

				$("#b_instagram").click(function(){
					showHideShares("#interface_instagram");
				});

				$("#b_tumblr").click(function(){
					showHideShares("#interface_tumblr");
				});

				$("#b_done").click(function(){
					setMode( STATE_THANKS );
				})
			}
			break;

			case STATE_THANKS:
			{
				var template = Handlebars.getTemplate("thanks", "templates");
				var html = template(
					{ 
						name :dataManager.name,
						didInteract: stateManager.didInteract,
						img  : dataManager.url
					}
				);
				$(htmlContainer).html(html);

				$("#b_startover").click(function(){
					setMode( STATE_INTRO );
				});

				modeTimeout = setTimeout(function(){
					setMode( STATE_INTRO );
				}, modeSwitchTime);
			}
			break;
		}

		stateManager.lastState = stateManager.state;
		stateManager.state = mode;
	}

	/*******************************************************
		HOME
	*******************************************************/

		// crickets

	/*******************************************************
		SCRIPTS
	*******************************************************/

	var currentScript = 0;

	function setScriptTimeouts(){
		// for now, set these timeouts here
		timeout( function(){
			if ( !stateManager.haveOpenedDrawers ){
				$("#drawer_prompt").css("top", "0%");
				document.getElementById("a_drawer").play();
			}

			// reset
			timeout( function(){
				$("#drawer_prompt").css("top", "100%");
			}, 10000);

			stateManager.havePromptedDrawers = true;

			// next prompt
			timeout( function(){
				if ( !stateManager.havePromptedSpin ){
				
					$("#spin_prompt").css("top", "0%");
					document.getElementById("a_spin").play();
				}
				stateManager.havePromptedSpin = true;

				// reset
				timeout( function(){
					$("#spin_prompt").css("top", "100%");
				}, 10000);

			}, 30000);

		}, 30000);
	}

	function nextScript(){
		currentScript++;
		if ( currentScript >= scriptData.scripts.length ){
			currentScript = 0;
		}
		setWhichScript(currentScript);
	}
	function prevScript(){
		currentScript--;
		if ( currentScript < 0 ){
			currentScript = scriptData.scripts.length - 1;
		}
		setWhichScript(currentScript);
	}

	function setWhichScript(index){
		dataManager.name = scriptData.scripts[index].name;

		$("#info_name").html(dataManager.name);
		var width = $("#"+scriptData.scripts[index].id).width();

		console.log(width);

		var x = -( (index) * width);

		for (var i=0; i<scriptData.scripts.length; i++){
			var s = scriptData.scripts[i];
			$("#"+s.id).css("left", x);
			x += $("#"+s.id).width();
		}

	}

	/*******************************************************
		CAMERA
	*******************************************************/

	// countdown
	var CAMERA_COUNTDOWN_MAX = 3;
	var camera_countdown = CAMERA_COUNTDOWN_MAX;

	// retakes
	var MAX_RETAKES 		 = 2;

	// debug
	this.camera =function (){
		setMode( STATE_CAMERA_A );
	}

	/*******************************************************
		EVENTS
	*******************************************************/

	this.onDrawer = function( e ){
		var data = e.detail;

		if ( value == true ){
			stateManager.haveOpenedDrawers = true;

			if ( !stateManager.havePromptedDrawers ){
				document.getElementById("a_drawer").play();
			}
		} else {
			// crickets
		}
	}

	this.onSpin = function( e ){
		var data = e.detail;
		if ( data.open == true ){
			if ( stateManager.state < STATE_CAMERA_A )	
				setMode( STATE_CAMERA_A );
		} else {
			setMode( stateManager.lastState == STATE_NULL ? STATE_INTRO : stateManager.lastState );
		}
	}
}


// states
var STATE_NULL	 	= -1;
var STATE_INTRO 	= 0;
var STATE_SCRIPT 	= 1;
var STATE_CAMERA_A	= 2;
var STATE_CAMERA_B 	= 3;
var STATE_CAMERA_C 	= 4;
var STATE_NAME 		= 5;
var STATE_SHARE 	= 6;
var STATE_THANKS 	= 7;

/**
 * @class
 */
var StateManager = function(){	
	this.reset = function(){
		this.state = STATE_NULL;
		this.lastState = STATE_NULL;

		this.haveOpenedDrawers = false;
		this.havePromptedDrawers = false;

		this.havePromptedSpin 	= false;

		// determines end state
		this.didInteract		= false;

		// retakes
		this.retakeCount 		= 0;
	}

	this.reset();
}

/**
 * @class
 */
var DataManager = function(){
	this.reset = function(){
		this.name = "Character";
		this.image 		 = "";
	}

	this.reset();
}

/**
 * @class
 */
var SpacebrewManager = function(){
	var sb;

	this.setup = function( host ){
		host = host === undefined ? "localhost" : host;
		sb = new Spacebrew.Client();
		var app_name = "Arch Display"
		sb.name(app_name);
		sb.description("");
		sb.server = host;

		// configure the publication and subscription feeds
		sb.addSubscribe("spin", "range");
		sb.addSubscribe("drawer", "boolean");

		// override Spacebrew events - this is how you catch events coming from Spacebrew
		sb.onRangeMessage = this.onRangeMessage.bind(this);
		sb.onBooleanMessage = this.onBooleanMessage.bind(this);
		// sb.onOpen = this.onOpen.bind(this);

		// connect to spacbrew
		sb.connect();

		// setup events
	}

	var listenersSpin = [];
	var listenersDrawer = [];

	this.addListener = function( event, obj ){
		switch(event){
			case "spin":
			listenersSpin.push( obj );
			break;

			case "drawer":
			listenersDrawer.push( obj );
			break;
		}
	}

	var lastOpen = false;

	this.onRangeMessage = function( name, value ){
		var val = parseInt(value) / 180.;
		if ( name == "spin" ){
			var data = {
				value: val,
				open: (val > .5 ) // to-do: threshold
			}

			// this may change!
			if ( data.open != lastOpen ){
				var onSpinEvent = new CustomEvent( "spin", {"detail":data});
				for ( var l in listenersSpin ){
					listenersSpin[l].dispatchEvent(onSpinEvent);
				}
			}

			lastOpen = data.open;
		}
	}

	var lastBoolean = false;

	this.onBooleanMessage = function( name, value ){
		if (name == "drawer"){
			var val = value == "true";
			if ( lastBoolean != val){
				var onDrawerEvent = new CustomEvent( "drawer", {"detail":{value:val, index:0}});
				for ( var l in listenersDrawer ){
					listenersDrawer[l].dispatchEvent(onDrawerEvent);
				}
			}
			lastBoolean = val;

		}
	}
}