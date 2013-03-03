var addon = require('./build/Release/leap');
var EventEmitter = require('events').EventEmitter;
var shoe = require('shoe');
var emitStream = require('emit-stream');
var JSONStream = require('JSONStream');
var http = require('http');

var server = http.createServer(require('ecstatic')(__dirname));

var sock = shoe(function(stream) {
  var ev = new EventEmitter();
  emitStream(ev)
    .pipe(JSONStream.stringify())
    .pipe(stream);

  setInterval(function() {
    var frame = addon.leap();
    ev.emit('frame', frame);
  }, 1e3 / 60);
});

server.listen(1337);

console.log('Server running on http://localhost:1337');

sock.install(server, '/sock');
