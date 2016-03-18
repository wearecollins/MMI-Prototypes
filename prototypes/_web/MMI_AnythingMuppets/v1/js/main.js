var app;
var canShow = true;
var preview 	= false;

$(document).ready(function() {
	app = new App();
	app.setup();

	var showing = true;

	$("#clip").css("visibility", "visible");
	$("#clip").css("display", "block");
	$("#clip").html('<iframe id="ytplayer" type="text/html" width="960" height="700" src="http://www.youtube.com/embed/HRprvU5EzoQ?autoplay=1" frameborder="0"/>');
	
	var inter = setInterval(function(){
		if ( !showing && canShow && !preview ){
			$("#clip").css("visibility", "visible");
			$("#clip").css("display", "block");
			$("#clip").html('<iframe id="ytplayer" type="text/html" width="960" height="700" src="http://www.youtube.com/embed/HRprvU5EzoQ?autoplay=1" frameborder="0"/>');
			showing = true;
		} else {
			$("#clip").css("visibility", "hidden");
			$("#clip").css("display", "none");
			$("#clip").html("");
			showing = false;
		}
	}, 35000 );
});

// shim-shimmany
if (!navigator.getUserMedia) {
    navigator.getUserMedia = navigator.getUserMedia ||
                             navigator.webkitGetUserMedia ||
                             navigator.mozGetUserMedia ||
                             navigator.msGetUserMedia || null;
}

var App = function(){
	var sb;
	var camDiv;

	var promptDiv;
	var prompts = [];

	var promptA = ["Scary", "Funny", "Surprised", "Happy", "Sad", "Angry"];
	var promptB = ["Detective", "Girl", "Monster", "Gentleman", "Alien", "Baby", "Woman", "Man", "Weightlifter"]

	this.setup = function(){
		camDiv = $("#camera")[0];
		promptDiv = $("#prompts")[0];
		Webcam.set({
			width:640, 
			height:480, 
       	 	dest_width: 640,
        	dest_height: 480 
    	});

		Webcam.attach( '#camera' );

		sb = new Spacebrew.Client();
		var app_name = "Arch Display"
		sb.name(app_name);
		sb.description("");
		sb.server = "localhost";

		// configure the publication and subscription feeds
		sb.addSubscribe("spin", "range");
		sb.addSubscribe("window_open", "range");
		sb.addSubscribe("slider3", "range");
		sb.addSubscribe("feature", "string");

		// override Spacebrew events - this is how you catch events coming from Spacebrew
		sb.onRangeMessage = this.onRangeMessage.bind(this);
		sb.onBooleanMessage = this.onBooleanMessage.bind(this);
		sb.onStringMessage = this.onStringMessage.bind(this);
		sb.onOpen = this.onOpen.bind(this);

		// connect to spacbrew
		sb.connect();
		this.randomizePrompt();

		// setup 'script'
		var date = (new Date()).toDateString();
		$("#date").html(date);

		this.draw();
	}

	this.draw = function(){
		window.requestAnimationFrame(this.draw.bind(this));
	}

	this.onOpen = function(){
		console.log("Spacebrew is open");
	}

	var lastOpen = 0;
	var windowThresh = 50;
	var triggerTime = null;
	var triggerInt = null;

	this.onRangeMessage = function( name, value ){
			var val = parseInt(value);
		if (name == "window_open"){
			if ( val < windowThresh && val != lastOpen){
				this.randomizePrompt();
				canShow = false;
			} else {
				canShow = true;
			}
			lastOpen = val;
		} else if ( name == "spin" ){
			if ( val > 0 ){
				$("#preview").css("visibility", "visible");
				$("#preview").css("display", "block");
				preview = true;
			} else {
				$("#preview").css("visibility", "hidden");
				$("#preview").css("display", "none");
				preview = false;
			}
			$("#preview").css("opacity", val / 100.);
			if ( val / 100. >= 1.0 ){
				if ( triggerTime == null ){
					triggerTime = setTimeout(this.startCountdown.bind(this), 500);
				}
			} else {
				if ( triggerTime != null ){
					clearTimeout(triggerTime);
					triggerTime = null;
				}

				if ( triggerInt != null ){
					clearInterval(triggerInt);
					triggerInt = null;
				}
        		$("#capture").css("visibility", "hidden");
				$("#count").html("");
			}
		}
	}

	this.onBooleanMessage = function( name, value ){
		if (name == "window_open"){
			var val = value;
			if ( val == false && val != lastOpen){
				this.randomizePrompt();
			}
			lastOpen = val;
		}
	}

	this.onStringMessage = function( name, value ){
		if ( name == "feature" ){
			$("#nose_interface").visibility("visible");

			setTimeout(function(){
				$("#nose_interface").visibility("hidden");
			}, 3000);
		}
	}

	this.randomizePrompt = function(){
		for (var i=0; i<prompts.length; i++){
			promptDiv.removeChild(prompts[i]);
		}

		prompts = [];

		var prompt1 = document.createElement("div");
		prompt1.className = "prompt left";
		prompt1.innerHTML = promptA[Math.floor(Math.random() * promptA.length)];

		var prompt2 = document.createElement("div");
		prompt2.className = "prompt right";
		prompt2.innerHTML = promptB[Math.floor(Math.random() * promptB.length)];
		promptDiv.appendChild(prompt1);
		promptDiv.appendChild(prompt2);
		prompts.push(prompt1);
		prompts.push(prompt2);
	}

	var count = 3;

	this.startCountdown = function(){
		if ( triggerInt != null ){
			clearInterval(triggerInt);
			triggerInt = null;
		}
		count = 3;

		var a = this;

		triggerInt = setInterval( function(){
			count--;
			if ( count >= 0 )
				$("#count").html(count);
			else {
				clearInterval(triggerInt);
				a.snap();
			}
		}, 1000);

		$("#count").html(count);
	}

	this.snap = function(){
        Webcam.snap( function(data_uri) {
        	$("#capture").css("visibility", "visible");
            document.getElementById('capture').innerHTML = '<img src="'+data_uri+'"/>';
        } );
	}
}