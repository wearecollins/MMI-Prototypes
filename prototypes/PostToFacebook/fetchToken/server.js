var Express = require('express');
var Facebook = require('fb');
var BodyParser = require('body-parser');
var FileSystem = require('fs');

var configs = require(process.argv[2]);

Facebook.options({appId:     configs.facebook.app.id,
		              appSecret: configs.facebook.app.secret,
		              version:   'v2.5'});

var app = Express();
app.use(Express.static('static'));
app.use(BodyParser.urlencoded({extended:false}));
app.post('/', function (client_req, client_res){
  var shortToken = client_req.body.token;
  var pageId = client_req.body.page;
  console.log('[server::post::/] token:', shortToken);
  console.log('[server::post::/] page:', pageId);
  //elevate token to long-live token
  Facebook.api(
    'oauth/access_token',
    {grant_type:        'fb_exchange_token',
     client_id:         configs.facebook.app.id,
     client_secret:     configs.facebook.app.secret,
     fb_exchange_token: shortToken},
    function gotLongToken(res){
      if (!res || res.error){
        console.error('[server::gotLongToken]', 
                      res ? res.error : 'error');
      } else {
        var longToken = res.access_token
        console.log('[server::gotLongToken] token:', longToken);
        //use long-live token to get long-live page token
        Facebook.setAccessToken(longToken);
        Facebook.api(
          'me/accounts/'+pageId,
          function gotPageToken(res){
            if (!res || res.error){
              console.error('[server::gotPageToken]', 
                            res ? res.error : 'error');
            } else {
              var pageToken = res.data[0].access_token;
              console.log('[server::gotPageToken] token:',
                          pageToken);
              //store token
              var filename = '../token.json';
              FileSystem.writeFile(
                filename, 
                JSON.stringify({page: {token: pageToken,
                                       id:    pageId}}),
                function wroteFile(err){
                  if (err){
                    console.error('[server::wroteFile]', err);
                  } else {
                    console.log('[server::wroteFile] wrote',filename);
                    client_res.redirect('/thanks.html');
                  }
                });
            }
          });
      }
    });
});

var port = 8012;
var server = app.listen(port, function(){
  console.log('server listening on port',port);
});
