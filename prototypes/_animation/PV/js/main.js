var app;

$(document).ready(function() {
	app = new App();
});

var App = function(){
	var state = 0;
	var tween = null;
	var coords = { x: 0, y: 0 };
	var coordsTo = { x: 100, y: 100 };

	this.setup = function(){
		for ( var i=1; i<5;i++){
			$("#container"+i).click(this.flip.bind(this))
		}

		requestAnimationFrame(this.update.bind(this));
	}

	this.update = function(){
		requestAnimationFrame(this.update.bind(this));
		TWEEN.update();
	}

	this.flip = function(){
		if ( tween != null ){
			tween.stop();
		}

		switch(state){
			case 0:
			coords = { x: 100, y: 100 };
			coordsTo = { x: 0, y: 100 };
			break;

			case 1:
			coords = { x: 100, y: 100 };
			coordsTo = { x: 100, y: 0 };
			break;

			case 2:
			coords = { x: 100, y: 100 };
			coordsTo = { x: 100, y: 0 };
			break;

			case 3:
			coords = { x: 0, y: 0 };
			coordsTo = { x: 100, y: 100 };
			break;

			case 4:
			coords = { x: 0, y: 0 };
			coordsTo = { x: 100, y: 100 };
			break;
		}
		if ( state < 3 ){	
			tween = new TWEEN.Tween(coords)
		    .to(coordsTo, 500)
		    .onUpdate(function() {
				$("#container"+(state+1)).css("width", this.x + "%");
				$("#container"+(state+1)).css("height", this.y + "%");
				// $("#container1").css("width", "0%");
		    })
		    .easing( TWEEN.Easing.Quadratic.InOut )
		    .start()
		    .onComplete(function(){
				state++;
				if ( state == 3 ){
					$("#masked").css("width", "100%");
					$("#masked").css("height", "100%");
				}
		    })
		} else {
			tween = new TWEEN.Tween(coords)
		    .to(coordsTo, 500)
		    .onUpdate(function() {
		    	for ( var i=1; i<4; i++){
					$("#container"+(i)).css("width", this.x + "%");
					$("#container"+(i)).css("height", this.y + "%");
		    	}
				// $("#container1").css("width", "0%");
		    })
		    .easing( TWEEN.Easing.Quadratic.InOut )
		    .start()
		    .onComplete(function(){
				state++;
				if (state >= 4){
					state = 0;
				}
		    })
		}
	}

	this.setup();
}