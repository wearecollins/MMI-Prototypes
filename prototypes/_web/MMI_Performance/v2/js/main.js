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

// states
var STATE_NULL	 	 	= -1;
var STATE_INTRO 	 	= 0;
var STATE_OVERVIEW_A 	= 1;
var STATE_OVERVIEW_B 	= 2;
var STATE_CLIPS 	 	= 3;
var STATE_PRACTICE 		= 4;
// var STATE_PRACTICE_B	= 6;
var STATE_PERFORM  		= 5;
// var STATE_PERFORM_B 	= 6;
var STATE_PREVIEW	 	= 6;
var STATE_SHARE 	 	= 7;
var STATE_THANKS 	 	= 8;

var CLIP_LENGTH			= 15 * 1000;

/**
 * [App description]
 */
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

	var clipData = {
		clips: [
			{
				"name":"C’est Ci Bon",
				"id":"bon",
				"video":"video/Sam and Friends - C'est Ci Bon.mp4",
				"content":"3 Performers"
			},
			{
				"name":"Ole Black Magic",
				"id":"magic",
				"video":"video/Sam & Friends - Ole Black Magic.mp4",
				"content":"2 Performers"
			},

			{
				"name":"A Horse Named Bill",
				"id":"horse",
				"video":"video/Kermit_banjo.mp4",
				"content":"1 Performer"
			},

		]
	}

	this.setup = function(){
		// load home template
		setMode( STATE_INTRO );

		// setup sb
		spacebrew.setup();

		// window.addEventListener("drawer", this.onDrawer.bind(this) );
		// window.addEventListener("spin", this.onSpin.bind(this) );

		// spacebrew.addListener("drawer", window );
		// spacebrew.addListener("spin", window );

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

		switch( mode ){
			case STATE_INTRO:
			{
				var template = Handlebars.getTemplate("intro", "templates");
				var html = template({});
				$(htmlContainer).html(html);

				$("#b_getstarted").click(function(){
					setMode( STATE_OVERVIEW_A );
				})
			}
			break;

			case STATE_OVERVIEW_A:
			{
				var template = Handlebars.getTemplate("overview_a", "templates");
				var html = template({});
				$(htmlContainer).html(html);

				openWebCam();

				// $("video").css("width", "1220px");
				// $("video").css("height", "");

				$("#b_next").click(function(){
					setMode( STATE_OVERVIEW_B );
				})
			}
			break;

			case STATE_OVERVIEW_B:
			{
				var template = Handlebars.getTemplate("overview_b", "templates");
				var html = template({});
				$(htmlContainer).html(html);

				openWebCam();

				$("#b_next").click(function(){
					setMode( STATE_CLIPS );
				})
			}
			break;

			case STATE_CLIPS:
			{	
				var template = Handlebars.getTemplate("clips", "templates");
				var html = template(clipData);
				$(htmlContainer).html(html);

				for ( var i=0; i<clipData.clips.length; i++ ){
					console.log(clipData.clips +":"+i);
					$("#" + clipData.clips[i].id ).click(function(){
						// set which clip
						for ( var j=0; j<clipData.clips.length; j++ ){
							if (clipData.clips[j].id == this.id){
								dataManager.clipIndex = j;
								break;
							}
						}

						setMode( STATE_PRACTICE );
					})
				}
			}
			break;

			case STATE_PRACTICE:
			{
				var template = Handlebars.getTemplate("practice", "templates");
				var vid = clipData.clips[dataManager.clipIndex];
				var html = template({video:vid.video, id:vid.id});
				$(htmlContainer).html(html);

				document.getElementById(vid.id).currentTime = 10;
				document.getElementById(vid.id).play();
				// to-do: countdown timeout
				setInterval(function(){
					
				});

				setTimeout(function(){
					// reset countdown
					setTimeout(function(){
						setMode( STATE_PERFORM );
						document.getElementById(vid.id).currentTime = 0;
						document.getElementById(vid.id).play();
					}, CLIP_LENGTH);
				}, CLIP_LENGTH);

				openWebCam(true);
			}
			break;

			case STATE_PERFORM:
			{
				var template = Handlebars.getTemplate("perform", "templates");
				var vid = clipData.clips[dataManager.clipIndex];
				var html = template({video:vid.video, id:vid.id});
				$(htmlContainer).html(html);

				document.getElementById(vid.id).pause();
				// to-do: countdown timeout
				
				setTimeout(function(){
					// reset countdown
					setTimeout(function(){
						setMode( STATE_SHARE );
						document.getElementById(vid.id).currentTime = 0;
						document.getElementById(vid.id).play();
					}, CLIP_LENGTH);
				}, CLIP_LENGTH);

				openWebCam(true);
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
		PRACTICE
	*******************************************************/

	function openWebCam( fullscreen ){
		var width = 640;
		var height = 480;
		var s 		= 2;

		if ( fullscreen !== undefined && fullscreen ){
			width = window.innerWidth;
			height= window.innerHeight;
			// s = width / 640;
		}

		Webcam.set({
			width:640, 
			height:480,
			flip_horiz:true
    	});

		Webcam.attach( '#webcam' );

		$("#wc").css("width", width + "px");
		$("#wc").css("height", height + "px");
		$("#wc").css("transform-origin", width/2 + "px 0px 0px");
		$("#wc").css("transform", "scaleX(" + s + ") scaleY(" + s + ")");

		$("#webcam").css("width", width + "px");
		$("#webcam").css("height", height + "px");
	}
}

/**
 * @class
 */
var StateManager = function(){	
	this.reset = function(){
		this.state = STATE_NULL;
		this.lastState = STATE_NULL;
	}

	this.reset();
}

/**
 * @class
 */
var DataManager = function(){
	this.reset = function(){
		this.clipIndex = -1;
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
		// sb.addSubscribe("spin", "range");
		// sb.addSubscribe("drawer", "boolean");

		// override Spacebrew events - this is how you catch events coming from Spacebrew
		sb.onRangeMessage = this.onRangeMessage.bind(this);
		sb.onBooleanMessage = this.onBooleanMessage.bind(this);
		// sb.onOpen = this.onOpen.bind(this);

		// connect to spacbrew
		sb.connect();

		// setup events
	}

	var listeners = {};

	this.addListener = function( event, obj ){
		if ( !listeners.hasOwnProperty(event) ){
			listeners[event] = [];
		}

		listeners[event].push( obj );
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
				var newEvent = new CustomEvent( "spin", {"detail":data});
				
				if ( listeners.hasOwnProperty(name) ){
					for ( var l in listeners[name] ){
						listeners[name][l].dispatchEvent(newEvent);
					}
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
				var newEvent = new CustomEvent( "drawer", {"detail":{value:val, index:0}});
				
				if ( listeners.hasOwnProperty(name) ){
					for ( var l in listeners[name] ){
						listeners[name][l].dispatchEvent(newEvent);
					}
				}
			}
			lastBoolean = val;

		}
	}
}