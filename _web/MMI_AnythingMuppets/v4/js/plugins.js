// Avoid `console` errors in browsers that lack a console.
(function() {
    var method;
    var noop = function () {};
    var methods = [
        'assert', 'clear', 'count', 'debug', 'dir', 'dirxml', 'error',
        'exception', 'group', 'groupCollapsed', 'groupEnd', 'info', 'log',
        'markTimeline', 'profile', 'profileEnd', 'table', 'time', 'timeEnd',
        'timeline', 'timelineEnd', 'timeStamp', 'trace', 'warn'
    ];
    var length = methods.length;
    var console = (window.console = window.console || {});

    while (length--) {
        method = methods[length];

        // Only stub undefined methods.
        if (!console[method]) {
            console[method] = noop;
        }
    }
}());

// Place any jQuery/helper plugins in here.
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

// color stuffs
// 
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
    var increment = inc === undefined ? .07 : inc;

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

function random(x, y) {
    var high = Math.max(x, y);
    var low = Math.min(x, y);
    return Math.max(low, (low + (high - low) * Math.random() ));
}