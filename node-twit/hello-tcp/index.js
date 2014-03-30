// Say hello to tcp port

var net = require('net');
var port = 3001;

var numMsgs = 0;

net.createServer(function(socket) {
    socket.on('connection', function() {
        socket.write("yo\n", "utf8");
    });

    socket.on('data', function(data) {

        numMsgs++;

        var msg = data.toString();

        if(msg !== "\n") {
            console.log(msg);
        }

        socket.write(numMsgs + '\n');
    });

    socket.on('error', function(error) {
        console.log(error);
    });

    socket.on('close', function() {
        socket.end();
    });

}).listen(port);

console.log("Saying 'yo' to connections on port " + port);