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
				"content":"3 Performers"
			},
			{
				"name":"Ole Black Magic",
				"id":"magic",
				"video":"video/black_magic.mp4",
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
		// spacebrew.setup();

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
						// video:data, id:"video"
						video:"uploads/ghyqc41rok9gd4w31x6_merged.webm", id:"video"
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


	    // global variables
	    var currentBrowser = !!navigator.mozGetUserMedia ? 'gecko' : 'chromium';

	    // Firefox can record both audio/video in single webm container
	    // Don't need to create multiple instances of the RecordRTC for Firefox
	    // You can even use below property to force recording only audio blob on chrome
	    // var isRecordOnlyAudio = true;
	    isRecordOnlyAudio = !!navigator.mozGetUserMedia;


	    // if recording only audio, we should replace "HTMLVideoElement" with "HTMLAudioElement"
	    if (isRecordOnlyAudio && currentBrowser == 'chromium') {
	        var parentNode = videoElement.parentNode;
	        parentNode.removeChild(videoElement);

	        videoElement = document.createElement('audio');
	        videoElement.style.padding = '.4em';
	        videoElement.controls = true;
	        parentNode.appendChild(videoElement);
	    }

	    // setTimeout(function(){
	    
		// set title
		$(".title").html("Perform");

		    // start recording
		    captureUserMedia(function(stream) {

	            mediaStream = stream;

	            videoElement.src = window.URL.createObjectURL(stream);
	            videoElement.play();
	            videoElement.muted = true;
	            videoElement.controls = false;

	            // it is second parameter of the RecordRTC
	            // var audioConfig = {};

	            // if (!isRecordOnlyAudio) {
	            //     audioConfig.onAudioProcessStarted = function() {
	            //     	console.log("START!");
	            //         // invoke video recorder in this callback
	            //         // to get maximum sync
	            //     };
	            // }

	            // audioRecorder = RecordRTC(stream, audioConfig);

	            if (!isRecordOnlyAudio) {
	                // it is second parameter of the RecordRTC
	                var videoConfig = {
	                    type: 'video',
	                    width:640,
	                    height:480
	                };
	                videoRecorder = RecordRTC(stream, videoConfig);
	            }

	            currentCount = 3;
	            setupCountdown(startPerform);
	        })
		// }, 500);
	}

	function startPerform(){
		var video = document.getElementById(dataManager.id);
		video.currentTime = 0;
		video.play();

		// audioRecorder.startRecording();
        videoRecorder.startRecording();

		video.addEventListener('ended',function(){
			// if (isRecordOnlyAudio) {
	  //           audioRecorder.stopRecording(function(){
	  //           	console.log("yes");
	  //           	onStopRecording( function( video ){setMode( STATE_SHARE, video )} );
	  //           });				        	
	  //       }

	        if (!isRecordOnlyAudio) {
	            // audioRecorder.stopRecording(function() {
	                videoRecorder.stopRecording(function() {
	                    onStopRecording( function( video ){
	                    	setMode( STATE_SHARE, video )
	                    } );
	                });
	            // });
	        }
		},false);

		progInterval = setInterval(function(){
			var video = document.getElementById(dataManager.id);
			var pct = video.currentTime / video.duration;
			$("#timer").css("width", 100 - (pct * 100) + "%");
		}, 250);
	}

    // reusable helpers

    // this function submits both audio/video or single recorded blob to nodejs server
    function postFiles(audio, video, onComplete) {
        // getting unique identifier for the file name
        fileName = generateRandomString();

        // this object is used to allow submitting multiple recorded blobs
        var files = {};

        // recorded audio blob
        // files.audio = {
        //     name: fileName + '.' + audio.blob.type.split('/')[1],
        //     type: audio.blob.type,
        //     contents: audio.dataURL
        // };

        if (video) {
            files.video = {
                name: fileName + '.' + video.blob.type.split('/')[1],
                type: video.blob.type,
                contents: video.dataURL
            };
        }
        files.clip = dataManager.video.split("/")[1].split(".")[0] +".webm";
        files.uploadOnlyAudio = !video;

        // this isn't really necessary, yeah?
        // videoElement.src = '';
        // videoElement.poster = '/ajax-loader.gif';

        xhr('/upload', JSON.stringify(files), function(_fileName) {
            var href = location.href.substr(0, location.href.lastIndexOf('/') + 1);
            onComplete(href + 'uploads/' + _fileName);
        });

        if (mediaStream) mediaStream.stop();
    }

    // XHR2/FormData
    function xhr(url, data, callback) {
        var request = new XMLHttpRequest();
        request.onreadystatechange = function() {
            if (request.readyState == 4 && request.status == 200) {
                callback(request.responseText);
            }
        };

        request.upload.onprogress = function(event) {
            // progressBar.max = event.total;
            // progressBar.value = event.loaded;
            // progressBar.innerHTML = 'Upload Progress ' + Math.round(event.loaded / event.total * 100) + "%";
        };

        request.upload.onload = function() {
            // percentage.style.display = 'none';
            // progressBar.style.display = 'none';
        };
        request.open('POST', url);
        request.send(data);
    }

    // generating random string
    function generateRandomString() {
        if (window.crypto) {
            var a = window.crypto.getRandomValues(new Uint32Array(3)),
                token = '';
            for (var i = 0, l = a.length; i < l; i++) token += a[i].toString(36);
            return token;
        } else {
            return (Math.random() * new Date().getTime()).toString(36).replace(/\./g, '');
        }
    }

    // when btnStopRecording is clicked
    function onStopRecording(onComplete) {
    	console.log("Stopping");
        // audioRecorder.getDataURL(function(audioDataURL) {
        //     var audio = {
        //         blob: audioRecorder.getBlob(),
        //         dataURL: audioDataURL
        //     };

            // if record both wav and webm
            if (!isRecordOnlyAudio) {
                videoRecorder.getDataURL(function(videoDataURL) {
                    var video = {
                        blob: videoRecorder.getBlob(),
                        dataURL: videoDataURL
                    };

                    // console.log("POST!");

                    //postFiles(null, video, onComplete);
                    // postFiles(audio, video, onComplete);
                    onComplete();
                });
            }

            // if record only audio (either wav or ogg)
            // if (isRecordOnlyAudio) postFiles(audio);
        // });
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