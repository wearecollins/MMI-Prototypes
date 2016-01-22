// Muaz Khan     - www.MuazKhan.com
// MIT License   - www.WebRTC-Experiment.com/licence
// Source Code   - github.com/muaz-khan/WebRTC-Experiment/tree/master/RecordRTC/RecordRTC-to-Nodejs

var config = require('./config'),
    fs = require('fs'),
    sys = require('sys'),
    exec = require('child_process').exec;

// this function uploads files

function upload(response, postData) {
    if (!postData){
        console.log("Null object")
        return;
    }
    var files = JSON.parse(postData);

    // writing audio file to disk
    // _upload(response, files.video);

    // if (files.uploadOnlyAudio) {
    //     response.statusCode = 200;
    //     response.writeHead(200, { 'Content-Type': 'application/json' });
    //     response.end(files.audio.name);
    // }

    // if (!files.uploadOnlyAudio) {
        // writing video file to disk
        _upload(response, files.video);

        sliceAndDice(response, files.video, files.clip);
    // }
}

function sliceAndDice(response, file, clip) {
    // its probably *nix, assume ffmpeg is available
    // var audioFile = __dirname + '/uploads/' + files.audio.name;
    // var videoFile = __dirname + '/uploads/' + files.video.name;
    // var mergedFile = __dirname + '/uploads/' + files.audio.name.split('.')[0] + '-merged.webm';
    
    var util = require('util'),
        exec = require('child_process').exec;

    var fileName = file.name.split('.')[0];

    // var command = "ffmpeg -i '" + audioFile + "' -i '" + videoFile + "' -map 0:0 -map 1:0 -vf scale=320x480 " + mergedFile;

    var command = "bash cvt.sh "+ config.upload_dir+" " + fileName + " " + clip;

    exec(command, function (error, stdout, stderr) {
        if (stdout) console.log(stdout);
        if (stderr) console.log(stderr);

        if (error) {
            console.log('exec error: ' + error);
            response.statusCode = 404;
            response.end();

        } else {
            response.statusCode = 200;
            response.writeHead(200, {
                'Content-Type': 'application/json'
            });
            response.end(fileName + '_merged.webm');

            // removing audio/video files
            // fs.unlink(audioFile);
            // fs.unlink(videoFile);
        }

    });
}

function _upload(response, file) {
    var fileRootName = file.name.split('.').shift(),
        fileExtension = file.name.split('.').pop(),
        filePathBase = config.upload_dir + '/',
        fileRootNameWithBase = filePathBase + fileRootName,
        filePath = fileRootNameWithBase + '.' + fileExtension,
        fileID = 2,
        fileBuffer;

    while (fs.existsSync(filePath)) {
        filePath = fileRootNameWithBase + '(' + fileID + ').' + fileExtension;
        fileID += 1;
    }

    file.contents = file.contents.split(',').pop();

    fileBuffer = new Buffer(file.contents, "base64");

    fs.writeFileSync(filePath, fileBuffer);
}

exports.upload = upload;
