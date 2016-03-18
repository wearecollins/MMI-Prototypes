// Muaz Khan     - www.MuazKhan.com
// MIT License   - www.WebRTC-Experiment.com/licence
// Source Code   - github.com/muaz-khan/WebRTC-Experiment/tree/master/RecordRTC/RecordRTC-to-Nodejs

var config = require('./config'),
    http = require('http-server'),
    url = require('url');

function start(route, handle) {

    function onRequest(request, response) {

        var pathname = url.parse(request.url).pathname,
            postData = '';

        if ( handle[pathname] ){

            request.setEncoding('utf8');
            
            request.addListener('data', function(postDataChunk) {
                postData += postDataChunk;
            });
            
            request.addListener('end', function() {
                route(handle, pathname, response, postData, request);
                // response.emit('next');
            });
        } else {
            response.emit('next');
        }
    }

    http.createServer({before:[onRequest]}).listen(config.port);
}

exports.start = start;
