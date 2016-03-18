
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
		text:'SESAME STREET -- SHOW 0005<br /><br />"Muppet Sneeze"<br />Come up in limbo on a <u>blank</u> Muppet face.<br /><br />Muppet (starting to sneeze): "Ahh... Ahh..."<br /><br />Gordon\s hand reaches in and puts on the Muppet\'s face, piece by piece. Now everything\'s in place, except the Muppet\'s nose.<br /><br />Muppet (continues to build to a sneeze): "Ahh..."<br /><br />The hand places the nose.<br /><br />Muppet: "Thank you."<br /><br />The Muppet sneezes: "..Choo!.."<br /><br />Gordon (to camera): "When you have to sneeze, a nose comes in very handy."'
	},
	{"name":"Old Lady",
		name_clean:"old",
color:getNextColor(),
text:'SESAME STREET -- SHOW 0005<br /><br />"Tongue Twister Tag"<br />Muppet set.<br /><br /><b>Old Lady Muppet</b> (one of 5 Anything Muppets) enters right with heavy canvas sack over her shoulder. Gordon enters left. The Old Lady Muppet addresses Gordon.<br /><br />Old Lady Muppet: "Hey, Sonny."<br />Gordon: "Yeah?"<br /><br />Old Lady Muppet: "Is this the way to the sea shore"<br /><br />Gordon: "The sea shore?"<br /><br />Old Lady Muppet: "Yes. I\'ve got these seashells to see there."<br /><br />Gordon: "You\'re going to sell sea shells by the sea shore?"<br /><br />Old Lady Muppet: "What\'s it to you?"<br />Gordon: "Nothing. I just bet I know what your name is---"<br />Old Lady Muppet: "Penelope Pepperpot."<br /><br />Gordon (to camera): "Penelope Pepperpot sells sea shells by the sea shore?"'
	},
	{"name":"Little Boy",
		name_clean:"boy",
color:getNextColor(),
text:'SESAME STREET -- SHOW 0135<br /><br />"Lost Kid and Policeman"<br /><b>Little boy</b> Anything Muppet enters, crying. He is lost.<br /><br />Little Boy: Oh I am lost. I can\'t find my way home. Oh what will I do... My poppy told me that if I ever got lost I should find a policeman and he would help me get home... But I don\'t see any polcemen... Oh wait, there\'s a polceman! Oh Mr. Policeman!<br /><br />PM: (Enters, looks at the kid) Why Charlie! Hello, Charlie! What are you doing here?<br /><br />Little Boy: Oh Mr. Policeman, I am lost and can\'t find my way home.<br /><br />PM: Why Charlie, don\'t you recognize me? I\'m your Uncle Louie. Look at me."'
	},
	{"name":"Foreman",
		name_clean:"fore",
color:getNextColor(),
text:'SESAME STREET -- SHOW 0779<br /><br />"Oscar Gets a Job"<br />Oscar: Look at this old boot! It\'s only the left foot... but who knows... maybe if I\'m on this job long enough a right one will turn up. I\'d better take this home.<br /><br />PUTS THE BOOT ASIDE. <b>A.M. FOREMAN</b> COMES IN WITH ANOTHER FULL CAN.<br /><br />Foreman: Here ya go, Oscar. Here\'s another can. Hey, haven\'t you finished emptying the first can yet? You\'ll have to work faster than that if you wanna work around here. Let\'s speed it up, OK?"'
	},
	{"name":"Young Macdonald",
		name_clean:"ym",
color:getNextColor(),
text:"SESAME STREET -- SHOW 2502<br /><br /><br />AM:<br />(OC) Tum-de-dum-de-dum. tum-de-dum-de-dum... Yup (AN AM MAN COMES INTO FRAME. HE IS DRESSED LIKE A COWBOY WITH A GRIZZLY BLACK BEARD AND HE HOLDS 3 LARGE X-ES. HE NOTICES CAM)   Heh-heh-heh.  (HE DUCKS OUT OF FRAME. HE COMES BACK INTO FRAME.)  Heh-heh-heh.   (CUT TO REVEAL 3 X-ES IN A ROW ON THE FIXIT SHOW WINDOW. HE ADMIRES HIS WORK.)  Three X-es. Lovely, lovely, lovely. Heh-heh-heh."
	},
	{"name":"Hippie",
		name_clean:"Hippie",
color:getNextColor(),
text:'SESAME STREET -- SHOW 0060<br /><br />"Up and Down"<br /><b>Hippie Muppets</b> in front of "SILLY SONG TIME" art card.<br /><br />Leader: "Do you know what it\'s time for?"<br /><br />Others: "Yes!"<br /><br />Leader: "What\'s it time for?"<br /><br />Others: "It\'s Silly Song Time!"<br /><br />Leader: "That\'s right! And today our silly song is all about: Up and Down!"<br /><br />Others: "All right! Terrific!" (Etc.)<br /><br />Leader: "All right, then! What are we waiting for! Let\'s go!"'
	},
	{
		"name":"Max the Magnificent",
		name_clean:"max",
color:getNextColor(),
text:'SESAME STREET -- SHOW 1971<br /><br />"Max and Big Bird"<br />BB: "Hey! It says \'Max the Magnificent\' on <u>my</u> door too! And that person said \'magnificent day\' to me. And he was carrying a piece of chalk!! (TURNS AROUND) Hey! Yes, you! Are you Max the Magnificent?"<br /><br />Max: (ENTERING FRAME) Indeed I am. But you can call me Max.<br /><br />BB: (SHARP) Good. I\'m Big Bird and I have a question for you. What does magnificent mean?<br /><br />Max: (CHEERFUL, ENTHUSIASTIC; A NICE CHILD; MICKEY ROONEY-ISH) It means great, wonderful, good as good can be, beautiful, inspiring, glorius. Isn\'t it a magnificent word? Mag-ni-fi-cent.<br /><br />BB: Oh yea. It\'s a nice word. But it makes me angry.'
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
		text:'SESAME STREET -- SHOW 1971<br /><br />"Super Grover and the Computer"<br />AM GIRL:  (VO) Oh no!! <br /><br />SUPER GROVER: What is that I hear? The familiar sounds of someone in distress.  Never fear! Super Grover is on the way!! <br /><br />CUT TO AM GIRL IN HER LIVING ROOM. SHE IS IN FRONT OF A COMPUTER. <br /><br />AM GIRL: Oh no! My computer isn\'t working! Something\' s wrong, I don\'t know how to make it work. (SHE  PRESSES KEYS) Nothing\'s happening. <br /><br />SFX: DIVE BOMBING SUPER GROVER. SHE LOOKS UP. <br /><br />AM GIRL: Uh Oh. What\'s that? A bird? A plane?<br /><br />SUPER GROVER, YELLING, DIVES RIGHT THROUGH THE OPEN WINDOW, IF POSSIBLE. IF NOT, HE FALLS PAST THE WINDOW, THEN PEAKS OVER THE LEDGE, WITH HIS CAPE OVER HIS FACE. <br /><br />AM: (SIGHS) It\'s Super Grover.'
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