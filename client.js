var _ = require('lodash');
var shoe = require('shoe');
var emitStream = require('emit-stream');
var through = require('through');
var JSONStream = require('JSONStream');

var parser = JSONStream.parse([true]);
var stream = parser.pipe(shoe('/sock')).pipe(parser);
var ev = emitStream(stream);

// Dump frame
var log = document.createElement('pre');
log.id = 'log';
document.body.appendChild(log);

ev.on('frame', function(frame) {
  log.innerHTML = JSON.stringify(frame, null, 2);
});

// Drawing surface
var canvas = document.createElement('canvas');
document.body.appendChild(canvas);
canvas.width = document.width;
canvas.height = document.height;
var context = canvas.getContext('2d');

// Show fps
var fpsDisplay = document.createElement('pre');
fpsDisplay.id = 'fps';
document.body.appendChild(fpsDisplay);
stream.pipe(epicStream()).pipe(through(function(fps) {
  fpsDisplay.innerHTML = fps + ' fps';
}));

// Pipe to finger stream
stream.pipe(fingerDrawStream(context));

function epicStream(interval) {
  if (!interval) interval = 1000;
  var start = +new Date();
  var events = 0;
  return through(function() {
    events++;
    if (+new Date() > start + interval) {
      this.emit('data', events);
      start = +new Date();
      events = 0;
    }
  });
}

function fingerDrawStream(context) {
  return through(function(data) {
    if (data[0] !== 'frame') return;
    var frame = data[1];
    var intersections = frame.screens[0].intersections;
    _.each(frame.screens[0].intersections, function(i) {
      var x = i.position[0] * canvas.width;
      var y = canvas.height - i.position[1] * canvas.height;
      var l = 10;
      var hue = 255 / intersections.length * i.id;
      context.fillStyle = 'hsla(' + hue + ',100%,50%, 0.7)';
      context.fillRect(x, y, l, l);
    });
  });
}
