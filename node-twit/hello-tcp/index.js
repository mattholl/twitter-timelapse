// Say hello to tcp port

var net = require('net');
var port = 3001;

net.createServer(function(socket) {
    socket.on('connect', function() {
        socket.write("yo");
    });

    socket.on('data', function(data) {
        var msg = data.toString();

        if(msg !== "\n") {
            console.log(msg);
        }
    });

    socket.on('error', function(error) {
        console.log(error);
    });

    socket.on('close', function() {
        socket.end();
    });

}).listen(port);

console.log("Saying 'yo' to connections on port " + port);