var express = require('express');
var bodyParser = require('body-parser');
var http = require('http');
var Configer = require('./configer.js');
var Logger = require('log4js');
var WebSocket = require('./websockets.js');
var RemoteLogs = require('./log4javascript_to_log4js.js');

// set up logging
Logger.configure(require('./log4js_conf.json'));
//var logger = Logger.getLogger('server');

// load this app's configs
var config = new Configer(Logger.getLogger('Configer'));
config.load('static/demo/config.json');

// set up HTTP handlers
var app = express();
app.use(bodyParser.urlencoded({extended:true}));
app.use(RemoteLogs.log(Logger));
app.use(express.static('static'));

/**
 * server for accepting connections on the given port
 * @type {http.Server}
 */
var server = http.createServer(app);

/*eslint-disable no-unused-vars*/
/**
 * Websocket server to handle websocket communication
 */
var wss = new WebSocket(server, config, Logger.getLogger('websocket'));
/*eslint-enable*/

// listen to connections on the given port/interface
server.listen(8080, '127.0.0.1');
