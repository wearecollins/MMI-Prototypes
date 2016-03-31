var LogHandler = {};

LogHandler.log = function log(a_Logger){
  var Logger = a_Logger;
  var logger = Logger.getLogger('remote');
  return function handleLogPost(req, res, next){
    if (req.method !== 'POST'){
      next();
    } else {
      try{
        logger[req.body.level.toLowerCase()](req.body.message);
      }catch(e){
        logger.error('problem logging from POST: ',e);
      }
      res.send(' ');
    }
  };
};

module.exports = LogHandler;
