
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
	var increment = inc === undefined ? .2/13. : inc;

	var c = new Color.rgb(colorStart.r, colorStart.g, colorStart.b);
	var hsl = Color.RGBtoHSL(c.channels);
	console.log(hsl);

	hsl[0] += increment;


	var next = Color.HSLtoRGB(hsl);
	var ret = {r:Math.round(next[0]), g:Math.round(next[1]), b:Math.round(next[2])};

	colorStart = ret;

	return ret;
}

var scripts = 
[
	{
		"name":"???",
		name_clean:"mystery",
		color:getNextColor(),
		text:"Design your own character!"
	},
	{
		"name":"Sneezing AM",
		name_clean:"sneeze",
		color:getNextColor(),
		text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	},
	{"name":"Old Lady",
		name_clean:"old",
color:getNextColor(),
text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	},
	{"name":"Little Boy",
		name_clean:"boy",
color:getNextColor(),
text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	},
	{"name":"Foreman",
		name_clean:"fore",
color:getNextColor(),
text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	},
	{"name":"Young Macdonald",
		name_clean:"ym",
color:getNextColor(),
text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	},
	{"name":"Hippie",
		name_clean:"Hippie",
color:getNextColor(),
text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	},
	{
		"name":"Max the Magnificent",
		name_clean:"max",
color:getNextColor(),
text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	},
	{
		"name":"French Businessman",
		name_clean:"french",
color:getNextColor(),
text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	},
	{
		"name":"Little Girl",
		name_clean:"little_girl",
color:getNextColor(),
text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	},
	{
		"name":"Enthusiastic Man",
		name_clean:"enthused",
color:getNextColor(),
text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	},
	{
		"name":"Upset woman",
		name_clean:"upsets",
color:getNextColor(),
text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	},
	{
		"name":"Cowboy",
		name_clean:"cowboy",
color:getNextColor(),
text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	},
	{
		"name":"Salesman",
		name_clean:"salesman",
		color:getNextColor(),
		text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	},
	{
		"name":"Farmer",
		name_clean:"farmer",
color:getNextColor(),
text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	}
];