var Express = require('express');
var Facebook = require('fb');
var FileSystem = require('fs');
var BodyParser = require('body-parser');
var Path = require('path');
var Logger = require('log4js');

var configs = require(process.argv[2]);
Logger.configure(require(Path.join(__dirname, 'log4js_conf.json')));
var logger = Logger.getLogger('postServer');

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
  logger.info('[post::post::/photo] posting', client_req.body.url);
  var configPromise = getGraphConfig();

  configPromise.
    //construct options object
    then(
      function (data){
        var options = 
          {access_token:             data.page.token,
           //no_story seems to have no affect when published: false
           no_story:                 true,
           published:                false,
           unpublished_content_type: 'DRAFT',
           url:                      client_req.body.url};
        return {config:data,
                options};
      }).
    //send API POST
    then(
      function upload(data){
        var rootId;
        //if we have an Album ID, upload to the Album
        // otherwise upload to the Page
        if (data.config.album){
          rootId = data.config.album.id;
          logger.debug('[post::post::/photo::upload] to album');
        } else {
          rootId = data.config.page.id;
          logger.debug('[post::post::/photo::upload] to page');
        }
        return postAPI(rootId+PublishType.PHOTO.fb_edge, data.options);
      }).
    //respond to client
    then(function done(/*response*/){
      logger.info('[post::post::/photo::done]');
      client_res.send(' ');
    });
});
app.post(PublishType.VIDEO.in_path, function(client_req, client_res){
  //TODO: validate post data
  logger.info('[post::post::/video] posting', client_req.body.url);
  var configPromise = getGraphConfig();

  configPromise.
    //construct options object
    then(
      function (data){
        var options = 
          {access_token:             data.page.token,
           //no_story seems to have no affect when published: false
           no_story:                 true,
           published:                false,
           unpublished_content_type: 'DRAFT',
           file_url:                 client_req.body.url};
        return {config:data,
                options};
      }).
    //send API POST
    then(
      function upload(data){
        //cannot load videos directly to list
        logger.debug('[post::post::/video::upload] to page');
        var rootId = data.config.page.id;
        return postAPI(rootId+PublishType.VIDEO.fb_edge, data.options).
          then(function(response){
            data.videoId = response.id;
            return data;
          });
      }).
    //link Video with List
    then(
      function link(data){
        if (data.config.videoList){
          logger.debug('[post::post::/video::link]');
          return postAPI(data.config.videoList.id+'/videos', 
                         {access_token: data.config.page.token,
                          video_ids:    JSON.stringify([data.videoId])});
        } else {
          return;
        }
      }).
    //respond to client
    then(function done(){
      logger.info('[post::post::/video::done]');
      client_res.send(' ');
    });
});

function postAPI(endpoint, options){
  logger.debug('[post::postAPI]', endpoint, options);
  return new Promise(function(resolve, reject){
    Facebook.api(
      endpoint,
      'post',
      options,
      function posted(res){
        logger.debug('[post::postAPI]', res);
        if (!res || res.error){
          reject( res ? res.error : undefined );
        } else {
          resolve(res);
        }
      });
  });
}

var port = 8013;
/*var server = */app.listen(port, function(){
  logger.info('listening on port',port);
});

function getGraphConfig(){
  return new Promise(function(resolve, reject){
    var filename = Path.join(__dirname, 'token.json');
    FileSystem.readFile(
      filename, 
      'utf8', 
      function loadedFile(err, data){
        if (err){
          reject(err);
        } else {
          try {
            var json = JSON.parse(data);
            resolve(json);
          } catch (e){
            reject(e);
          }
        }
      });
  });
}
