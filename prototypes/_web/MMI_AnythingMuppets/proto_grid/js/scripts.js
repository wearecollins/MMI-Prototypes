
function getRandomColor(){
	var r = Math.round(Math.random() * 200);
	var g = Math.round(Math.random() * 200);
	var b = Math.round(Math.random() * 200);
	// return "rgb("+ r + "," + g + "," + b + ")";
	return {r:r, g:g, b:b};
}

function getRandomColorHS(){
	var color = Color.HSLtoRGB([Math.random(), .5, .5]);
	// console.log(color);
	return {r:Math.round(color[0]), g:Math.round(color[1]), b:Math.round(color[2])};
}

var colorStart = getRandomColorHS();

function getNextColor( inc ){
	var increment = inc === undefined ? .5/13. : inc;

	var c = new Color.rgb(colorStart.r, colorStart.g, colorStart.b);
	var hsl = Color.RGBtoHSL(c.channels);

	hsl[0] += increment;


	var next = Color.HSLtoRGB(hsl);
	var ret = {r:Math.round(next[0]), g:Math.round(next[1]), b:Math.round(next[2])};

	colorStart = ret;

	return ret;
}

function getColorFill(c){
	return "rgb(" + c.r + "," + c.g + "," + c.b + ")";
}

var scripts = 
[
	{
		"name":"Old Lady",
		"name_clean":"old",
		"color":getNextColor(),
		"text":'<em>Old Lady Muppet enters right with heavy canvas sack over her shoulder. The Old Lady Muppet addresses Gordon:</em><br>"Hey, Sonny."',
		"img":"img/old_lady.jpg"
	},
	{
		"name":"Young Macdonald",
		"name_clean":"ym",
		"color":getNextColor(),
		"text":"<em>An outgoing farmgirl with a Scottish accent</em>",
		"img":"img/young_mac.jpg"
	},
	{
		"name":"AM Boy",
		"name_clean":"max",
		"color":getNextColor(),
		"text":'<em>Cheerful, enthusiastic; a nice child; Mickey Rooney-ish</em><br>"You can call me Max"',
		"img":"img/am_boy.jpg"
	},
	{
		"name":"French Businessman",
		"name_clean":"french",
		"color":getNextColor(),
		"text":'"The sign says ‘Win a free lunch!’...I thought there was no such thing as a free lunch."',
		"img":"img/french.jpg"
	},
	{
		"name":"AM Girl",
		name_clean:"little_girl",
		color:getNextColor(),
		text:'<em>AM Girl in her living room. She is in front of a computer.</em><br/>"Oh no! My computer isn’t working!"',
		"img":"img/am_girl.jpg"
	},
	{
		"name":"Salesman",
		name_clean:"salesman",
		color:getNextColor(),
		text:'<em>An AM Man enters dressed as a salesman, carrying his briefcase. The AM Man is in his thirties, has brown hair, a pointy nose, a friendly face, and speaks with a kind of nasal, slightly annoying voice.</em><br/>"Hello there. I’m your friendly neighborhood vacuum cleaner salesman."',
		"img":"img/salesman.jpg"
	},
];