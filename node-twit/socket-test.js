var net = require('net');
var port = 3001;

var client = net.connect({port:port}, function() {
    console.log('connected');
});

client.on('data', function(data) {
    var received = data.toString();
    console.log(received);
});