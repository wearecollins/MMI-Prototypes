var Express = require('express');
var Facebook = require('fb');
var FileSystem = require('fs');
var BodyParser = require('body-parser');
var Path = require('path');
var configs = require(process.argv[2]);

Facebook.options({appId:     configs.facebook.app.id,
                  appSecret: configs.facebook.app.secret,
                  version:   'v2.5'});

var PublishType = {PHOTO:{in_path:'/photo',
                          fb_edge:'/photos',
                          url_key:'url'},
                   VIDEO:{in_path:'/video',
                          fb_edge:'/videos',
                          url_key:'file_url'}};

var app = Express();
app.use(BodyParser.urlencoded({extended:false}));
app.post(PublishType.PHOTO.in_path, function(client_req, client_res){
  //TODO: validate post data
  console.log('[post::post::/photo] posting', client_req.body.url);
  publish(PublishType.PHOTO, client_req.body.url);
  client_res.send(' ');
});
app.post(PublishType.VIDEO.in_path, function(client_req, client_res){
  //TODO: validate post data
  console.log('[post::post::/video] posting', client_req.body.url);
  publish(PublishType.VIDEO, client_req.body.url);
  client_res.send(' ');
});

var port = 8013;
var server = app.listen(port, function(){
  console.log('listening on port',port);
});

function publish(type, url){
  //load token
  var filename = Path.join(__dirname, 'token.json');
  FileSystem.readFile(filename, 'utf8', function loadedFile(err, data){
    if (err){
      console.error('[post::publish::loadedFile] error loading file', err);
    } else {
      try{
        var json = JSON.parse(data);
        //TODO: validate file data
        //the key used to provide the url is different for videos
        var options = {access_token:             json.page.token,
                       //no_story seems to have no affect when published: false
                       no_story:                 true,
                       published:                false,
                       unpublished_content_type: 'DRAFT'};
        options[type.url_key] = url;
        // send FB API call
        Facebook.api(
          json.page.id+type.fb_edge,
          'post',
          options,
          function uploadedMedia(res){
            if (!res || res.error){
              console.error('[post::publish::uploadedMedia]', 
                            res && res.error ?
                              res.error :
                              'error uploading');
            } else {
              console.log('[post::publish::uploadedMedia] uploaded', 
                          res);
            }
          });
      } catch (e){
        console.error('[post::publish::loadedFile] error parsing json', e);
      } 
    }
  });
}
