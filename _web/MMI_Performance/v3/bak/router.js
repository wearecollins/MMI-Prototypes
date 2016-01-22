// Muaz Khan     - www.MuazKhan.com
// MIT License   - www.WebRTC-Experiment.com/licence
// Source Code   - github.com/muaz-khan/WebRTC-Experiment/tree/master/RecordRTC/RecordRTC-to-Nodejs

function respondWithHTTPCode(response, code) {
    response.writeHead(code, { 'Content-Type': 'text/plain' });
    response.end();
}

function route(handle, pathname, response, postData, request) {

    var extension = pathname.split('.').pop();

    var staticFiles = {
        js: 'js',
        gif: 'gif',
        jpg: 'jpg',
        jpeg: 'jpeg',
        png: 'png',
        css: 'css',
        webm: 'webm',
        mp4: 'mp4',
        mp3: 'mp3',
        wav: 'wav',
        ogg: 'ogg',
        handlebars: 'handlebars'
    };

    if ('function' === typeof handle[pathname]) {
        console.log( postData );
        handle[pathname](response, postData);
    } /*else if (staticFiles[extension]) {
        handle._static(response, pathname, request, postData);
    } else {
        respondWithHTTPCode(response, 404);
    }*/
}

exports.route = route;
