// utilz
function shuffle(array) {
  var currentIndex = array.length, temporaryValue, randomIndex ;

  // While there remain elements to shuffle...
  while (0 !== currentIndex) {

    // Pick a remaining element...
    randomIndex = Math.floor(Math.random() * currentIndex);
    currentIndex -= 1;

    // And swap it with the current element.
    temporaryValue = array[currentIndex];
    array[currentIndex] = array[randomIndex];
    array[randomIndex] = temporaryValue;
  }

  return array;
}

// duh
app.activate();

// get the comp

app.beginUndoGroup("audio script"); {
	var comp = null;
	for ( var i=1; i<app.project.items.length; i++){
		var item = app.project.items[i];
		if ( item.name == "Full Loop"){
			comp = item;
			break;
		}
	}

	// cheel
	if ( comp != null ){
		comp.openInViewer();
		
		// length of audio snippet
		var transLength = 15.;

		// duration in seconds
		var duration = comp.duration;
		var numTransforms = duration / transLength;

		var nLayers = comp.numLayers;
		var indices = [];

		for ( var i=1; i<=nLayers; i++){
			indices.push(i);
		}
		shuffle(indices);

		var ind = 0;
		var t = 10;

		for ( var i=0; i<numTransforms; i++){
			var l = comp.layer( indices[ind] );
			l.selected = true;

			// for ( var p in l.audioLevels.parentProperty){
			// 	// alert(l.audioLevels.parentProperty.enabled)
			// 	alert(p, l.audioLevels.parentProperty[p]);
			// }

			try {
				l.enabled = true;
				var prop = l.property("Effects").property("Stereo Mixer");

				// prop.setValueAtTime(t,[-48.,-48.,]);
				// prop.setValueAtTime(t + 1.,[0,0]);
				// prop.setValueAtTime(t + transLength - 1, [0,0]);
				// prop.setValueAtTime(t + transLength,[-48.,-48.,]);

				// prop.property("Left Level").setValueAtTime(t,0);
				prop.property("Right Level").setValueAtTime(t,0);

				// prop.property("Left Level").setValueAtTime(t + 1,50);
				prop.property("Right Level").setValueAtTime(t + 1,50);

				// prop.property("Left Level").setValueAtTime(t + transLength - 1,50);
				prop.property("Right Level").setValueAtTime(t + transLength - 1,50);

				// prop.property("Left Level").setValueAtTime(t + transLength,0);
				prop.property("Right Level").setValueAtTime(t + transLength,0);
			} catch(e){
				alert(e);
			}
			ind++;
			ind %= nLayers;
			if ( ind == 0 ) ind = 1;

			t += transLength;
		}
	}

}

app.endUndoGroup();