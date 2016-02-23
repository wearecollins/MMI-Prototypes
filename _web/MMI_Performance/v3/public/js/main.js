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
				"video":"video/cest_ci_bon.mp4",
				"content":"3 Performers",
				"cleanName":"cest_bon"
			},
			{
				"name":"Ole Black Magic",
				"id":"magic",
				"video":"video/black_magic.mp4",
				"content":"2 Performers",
				"cleanName":"black_magic"
			},

			{
				"name":"A Horse Named Bill",
				"id":"horse",
				"video":"video/Kermit_banjo.mp4",
				"content":"1 Performer",
				"cleanName":"horse_named_bill"
			},

		]
	}

	this.setup = function(){
		// load home template
		setMode( STATE_INTRO );

		// setup sb
		spacebrew.setup();

		window.addEventListener("recording", this.onGotRecording.bind(this) );
		// window.addEventListener("spin", this.onSpin.bind(this) );

		spacebrew.addListener("recording", window );
		// spacebrew.addListener("spin", window );

		this.animate();
	}

	this.update = function(){

	}

	this.draw 	= function(){
		
	}

	this.onGotRecording = function( data ){
		console.log( data );
		var URL = data.detail;
		console.log("GOT IT! "+data.detail);

		setMode( STATE_SHARE, URL )
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

	var progInterval = null;

	function setMode( mode, data ){
		if (stateManager.state == mode ) return;
		clearInterval(progInterval);

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

				openWebCam(true);

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

				openWebCam(true, false);

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
						// background = close
						$("#select_overlay").css("visibility", "visible");
						$("#select_overlay").css("display", "block");

						// create video
						var video = document.getElementById("big_video");
						var src = "";
						video.src = src;

						// set which clip
						for ( var j=0; j<clipData.clips.length; j++ ){
							if (clipData.clips[j].id == this.id){
								dataManager.clipIndex = j;
								src = clipData.clips[j].video;
								break;
							}
						}

						video.src = src;
						video.play();

						$("#select").click(function(){
							// set which clip
							for ( var j=0; j<clipData.clips.length; j++ ){
								if (clipData.clips[j].id == this.id){
									dataManager.clipIndex = j;
									break;
								}
							}

							setMode( STATE_PRACTICE );
						});

						$("#back").click(function(){
							$("#select_overlay").css("visibility", "hidden");
							$("#select_overlay").css("display", "none");
							video.src = "";
						})

					});
				}
			}
			break;

			case STATE_PRACTICE:
			{
				var template = Handlebars.getTemplate("practice", "templates");
				var vid = clipData.clips[dataManager.clipIndex];
				var html = template({video:vid.video, id:vid.id});
				$(htmlContainer).html(html);
				
				dataManager.id = vid.id;
				dataManager.video = vid.video;

				setupPractice();
			}
			break;

			case STATE_PERFORM:
			{
				// var template = Handlebars.getTemplate("perform", "templates");
				// var vid = clipData.clips[dataManager.clipIndex];
				// var html = template({video:vid.video, id:vid.id});
				// $(htmlContainer).html(html);
				
				setupPerform();

				// document.getElementById(vid.id).pause();
				// to-do: countdown timeout
				
				
			}
			break;

			case STATE_SHARE:
			{
				var template = Handlebars.getTemplate("share", "templates");
				var html = template(
					{
						video:data, id:"video"
						// video:"uploads/ghyqc41rok9gd4w31x6_merged.webm", id:"video"
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

				// $("#b_email").click(function(){
				// 	showHideShares("#interface_email");

				// });

				// $("#b_instagram").click(function(){
				// 	showHideShares("#interface_instagram");
				// });

				// $("#b_tumblr").click(function(){
				// 	showHideShares("#interface_tumblr");
				// });

				// $("#b_done").click(function(){
				// 	setMode( STATE_THANKS );
				// })
				
				$("#b_startover").click(function(){
					setMode( STATE_INTRO );
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

	/**
	 * This assumes a 640x480 texture with 320x240 stacked videos.
	 * The right 320x480 is tossed away
	 */

	function openWebCam( fullscreen, top ){
		var isTop = top === undefined ? true : top;
		var width = 320;
		var height = 480;
		var s 		= 2;

		if ( fullscreen !== undefined && fullscreen ){
			s = window.innerWidth / 640;
			width = window.innerWidth;
			height= height * s;
		}
		
		// mask height

		captureUserMedia(function(stream) {

            mediaStream = stream;
            videoElement = document.getElementById('wc');
            videoElement.src = window.URL.createObjectURL(stream);
            videoElement.play();
            videoElement.muted = true;
            videoElement.controls = false;
        });

		$("#wc").css("width", width * 2 + "px");
		// $("#wc").css("height", height + "px");
		$("#wc").css("transform-origin", width/2 + "px 0px 0px");
		// "scaleX(" + s + ") scaleY(" + s + ")" + 
		$("#wc").css("transform", (isTop ? "" : "translateY(-" + (height) +"px)"));

		$("#webcam").css("width", width + "px");
		$("#webcam").css("height", height + "px");
		$("#webcam").css("overflow","hidden");
	}

	/*******************************************************
		PERFORM
	**********************************	*********************/

    var fileName;
    var audioRecorder;
    var videoRecorder;
    var videoElement;
    var isRecordOnlyAudio;

    var currentCount = 3;
    var countdownInterval = null;

    function setupPractice(){
		spacebrew.sendGetReady( clipData.clips[dataManager.clipIndex].cleanName );

    	$("#b_next").click(function(){
    		$("#b_next").css("visibility", "hidden");
    		setupCountdown(function(){
				var video = document.getElementById(dataManager.id);
				video.currentTime = 0;
				video.play();

				video.addEventListener('ended',function(){
					setMode( STATE_PERFORM );
				},false);

				progInterval = setInterval(function(){
					var video = document.getElementById(dataManager.id);
					var pct = video.currentTime / video.duration;
					$("#timer").css("width", 100 - (pct * 100) + "%");
				}, 250);
			});
    	});

		openWebCam(true);
    }

    function setupCountdown( onComplete ){
    	currentCount = 3;
    	console.log("Countdown!");
    	if ( countdownInterval != null){
    		clearInterval(countdownInterval);
    	}
		
		$("#countdown_text").html(currentCount);
		$("#countdown").css("visibility","visible");

    	// to-do: countdown timeout
		countdownInterval = setInterval(function(){

			currentCount = currentCount - 1;
			if ( currentCount > 0 )
				$("#countdown_text").html(currentCount);
			else {
				$("#countdown").css("visibility","hidden");

		    	if ( countdownInterval != null){
		    		clearInterval(countdownInterval);
		    	}

				onComplete();
			}
		}, 1000);
    }

	function setupPerform(){

		var video = document.getElementById(dataManager.id);

		// rewind video
		video.currentTime = 0;

		// set title
		$(".title").html("Perform");

		openWebCam(true);

        currentCount = 3;
        setupCountdown(startPerform);
	}

	function startPerform(){
		spacebrew.sendRecord( clipData.clips[dataManager.clipIndex].cleanName );

		var video = document.getElementById(dataManager.id);
		video.currentTime = 0;
		video.play();

		video.addEventListener('ended',function(){
			// send note to spacebrew

			// 'video'
			// setMode( STATE_SHARE, video )
		},false);

		progInterval = setInterval(function(){
			var video = document.getElementById(dataManager.id);
			var pct = video.currentTime / video.duration;
			$("#timer").css("width", 100 - (pct * 100) + "%");
		}, 250);
	}

    var mediaStream = null;
    // reusable getUserMedia
    function captureUserMedia(success_callback) {
        var session = {
            audio: true,
            video: true
        };

        navigator.getUserMedia(session, success_callback, function(error) {
            alert(JSON.stringify(error));
        });
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
		host = host === undefined ? "sandbox.spacebrew.cc" : host;
		sb = new Spacebrew.Client();
		var app_name = "Arch Display"
		sb.name(app_name);
		sb.description("");
		// sb.server = host;

		// configure the publication and subscription feeds
		sb.addPublish("startRecording", "string");
		sb.addPublish("getReady", "string");

		sb.addSubscribe("recording", "string");
		// sb.addSubscribe("drawer", "boolean");

		// override Spacebrew events - this is how you catch events coming from Spacebrew
		sb.onStringMessage = this.onStringMessage.bind(this);
		// sb.onOpen = this.onOpen.bind(this);

		// connect to spacbrew
		sb.connect();

		// setup events
	}

	this.sendGetReady = function( clip ){
		sb.send("getReady", "string", clip)
	}

	this.sendRecord = function( clip ){
		sb.send("startRecording", "string", clip)
	}

	var listeners = {};

	this.addListener = function( event, obj ){
		if ( !listeners.hasOwnProperty(event) ){
			listeners[event] = [];
		}

		listeners[event].push( obj );
	}

	var lastOpen = false;

	this.onStringMessage = function( name, value ){
		var newEvent = new CustomEvent( name, {"detail":value});
		
		if ( listeners.hasOwnProperty(name) ){
			for ( var l in listeners[name] ){
				listeners[name][l].dispatchEvent(newEvent);
			}
		}
	}

	var lastBoolean = false;

	this.onBooleanMessage = function( name, value ){
	}
}