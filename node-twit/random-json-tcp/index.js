// Periodicaly send a random number to port 3001

var net = require('net');
var port = 3001;

net.createServer(function(socket) {

    var numInterval = setInterval(function() {
        var testJSON = {
            x : Math.random(),
            y : Math.random(),
            z : Math.random()
        };
        socket.write(JSON.stringify(testJSON) + '\n');
    }, 1000);

    socket.on('connection', function() {
        console.log('client connected');
    });

    socket.on('data', function(msg) {
        // msg comes through as a buffer
        console.log(msg.toString());
    });

    socket.on('error', function(error) {
        console.log(error);
    });

    socket.on('close', function() {
        socket.end();
        clearInterval(numInterval);
    });

}).listen(port);

console.log("Periodically sending random numbers to port " + port);