/**
 * @module server
 */

var express = require('express');
var bodyParser = require('body-parser');
var http = require('http');
var Configer = require('./configer.js');
var Logger = require('log4js');
var WebSocket = require('./websockets.js');
var RemoteLogs = require('./log4javascript_to_log4js.js');

//TODO: configurable via param
/**
 * The station this is serving and configured for
 * @type {string}
 */
var station = 'demo';

// set up logging
Logger.configure(require('./log4js_conf.json'));

/**
 * handles persistent configs
 * @type {Configer}
 */
var config = new Configer(Logger.getLogger('Configer'));
//load this station's config
config.load('static/'+station+'/config.json');

// set up HTTP handlers
/**
 * Express server for simple middleware-based message handling
 * @type {Express}
 */
var app = express();
app.use(bodyParser.urlencoded({extended:true}));
app.use(RemoteLogs.log(Logger));
app.use(express.static('static', {index:station+'.html'}));

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
