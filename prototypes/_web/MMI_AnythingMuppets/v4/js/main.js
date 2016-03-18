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

	// Scriptz
	var scriptGrid;

	// spacebrew
	var spacebrew 		= new SpacebrewManager();

	// timeouts
	var modeTimeout		= null;
	var modeSwitchTime	= 120000;

	this.setup = function(){
		// load home template
		setMode( STATE_INTRO );

		// setup sb
		spacebrew.setup();

		window.addEventListener("drawer", this.onDrawer.bind(this) );
		window.addEventListener("spin", this.onSpin.bind(this) );

		spacebrew.addListener("drawer", window );
		spacebrew.addListener("spin", window );

		// setup script grid
		scriptGrid = new ScriptGrid(); // doesn't actuall do anything

		// not needed ATM
		// this.animate();
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
					setMode( STATE_ROLLIE );
				})

				stateManager.reset();
				dataManager.reset();
			}
			break;

			case STATE_ROLLIE:
			{
				var template = Handlebars.getTemplate("rollie", "templates");
				var html = template({ video :"videos/rollie_fpo.mp4" });
				$(htmlContainer).html(html);

				try {
					document.getElementById("rollie").play();
				} catch(e){}

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
				var html = template({"scripts":scripts});
				$(htmlContainer).html(html);
				scriptGrid.setup();
				scriptGrid.onOpen = function( data ){
					// drawer/other prompt timeouts
					// setScriptTimeouts();

					// silly
					var name = data.object.object.name;
					$("#selected_character").html("Design <strong>"+name + "</strong>");
					dataManager.name = name;
				}

				scriptGrid.onTypeDone = function( ){
					// drawer/other prompt timeouts
					setScriptTimeouts();
				}

				scriptGrid.onClose = function(){
					console.log("close");
					try {
						for ( var i=0; i<timeouts.length; i++){
							clearTimeout(timeouts[i]);
						}
						timeouts = [];
					} catch(e){}
					closeScriptPrompts();
					$("#selected_character").html("Select a Character");
					dataManager.name = "Character";
				}
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
		       	 	// dest_width: window.innerWidth,
		        	// dest_height: "100%"
		    	});

				Webcam.attach( '#capture_element' );

				// disappear buttons
				$("#share_container").css("visibility", "hidden");
				$("#share_container").css("display", "none");

				$("#button_containerA").css("visibility", "visible");
				$("#button_containerA").css("display", "block");

				$("#button_containerB").css("visibility", "hidden");
				$("#button_containerB").css("display", "none");
				
				// $("#capture_element").css("width", "100%");
				// $("#capture_element").css("height", "100%");
				$("#capture_element").find("video").attr("style", "");

				$("#capture_element").css("visibility", "visible");
				$("#capture_element").css("display", "block");

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

				$("#button_containerA").css("visibility", "hidden");
				$("#button_containerA").css("display", "none");

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

				$("#button_containerB").css("visibility", "visible");
				$("#button_containerB").css("display", "block");

				if ( stateManager.retakeCount > MAX_RETAKES ){
					$("#b_retake").css("visibility", "hidden");
					$("#b_retake").css("display", "none");
				}

				$("#b_share").click(function(){
					$("#share_set").css("visibility", "visible");
					$("#share_set").css("display", "flex");

					$("#email_set").css("visibility", "hidden");
					$("#email_set").css("display", "none");
				})
				
				$("#rname").html( dataManager.name + " " +Math.round(Math.random() * 100 ));	

				$("#b_email").click(function(){
					$("#share_set").css("visibility", "hidden");
					$("#share_set").css("display", "none");

					$("#email_set").css("visibility", "visible");
					$("#email_set").css("display", "flex");
				})

				$("#b_share_go").click(function(){
					$("#i_text").css("visibility", "hidden");
					$("#i_text").css("display", "none");
					$("#e_text").css("visibility", "visible");
					$("#e_text").css("display", "block");

					$("#b_retake").css("visibility", "hidden");
					$("#b_retake").css("display", "none");
				})

				// display buttons
				$("#button_container").css("visibility", "visible");
				$("#button_container").css("display", "flex");

				$("#b_done_final").click(function(){
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

				// $("#b_startover").click(function(){
				// 	setMode( STATE_INTRO );
				// });

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
				document.getElementById("sound").pause();
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

		}, 500);
	}

	function closeScriptPrompts(){
		$("#spin_prompt").css("top", "100%");
		$("#drawer_prompt").css("top", "100%");
		var spin = document.getElementById("a_spin");
		var drawer = document.getElementById("a_drawer");
		spin.pause();
		drawer.pause();
		spin.currentTime = 0;
		drawer.currentTime = 0;
		stateManager.havePromptedDrawers = false;
		stateManager.havePromptedSpin = false;
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
			switch( stateManager.state ){
				case STATE_INTRO:
				case STATE_ROLLIE:
				case STATE_SCRIPT:
					// nothing. why are we here?
					break;

				case STATE_CAMERA_A:
				case STATE_CAMERA_B:
				case STATE_CAMERA_C:
				case STATE_NAME:
				case STATE_SHARE:
				{
					var newState = stateManager.lastState == STATE_NULL ? STATE_INTRO : stateManager.lastState;
					if ( newState > STATE_SCRIPT ){
						newState = STATE_INTRO;
					}
					setMode( newState );
				}
					break;

				case STATE_THANKS:
					setMode( STATE_INTRO );
					break;
			}
		}
	}
}


// states
var STATE_NULL	 	= -1;
var STATE_INTRO 	= 0;
var STATE_ROLLIE 	= 1;
var STATE_SCRIPT 	= 2;
var STATE_CAMERA_A	= 3;
var STATE_CAMERA_B 	= 4;
var STATE_CAMERA_C 	= 5;
var STATE_NAME 		= 6;
var STATE_SHARE 	= 7;
var STATE_THANKS 	= 8;

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
		var val = parseInt(value) / 100.;
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