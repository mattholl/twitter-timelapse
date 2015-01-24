var twitter = require('twitter');
var program = require('commander');
var osc = require('omgosc');

// TCP port
var PORT = 3001;

// Parse argv
program
  .version('0.0.0')
  .option('-t, --text [string or #string]', 'Filter tweets containing the specified text or hashtag. Comma separated.')
  .option('-r, --retain [number]', 'Use every x tweet to limit rate.')
  .option('-d, --debug', 'Send to stdout')
  .parse(process.argv);

var timeCheck = {
    timeNow : 0,
    maxDiff : 0,
    lastTime : 0
};

var keep = program.retain;
var tweetCount = 0;

var apiKey = require('./key.json');

console.log(apiKey);

var twitApi = new twitter(apiKey);

var oscSender = new osc.UdpSender('192.168.1.72', PORT);
// var oscSender = new osc.UdpSender('127.0.0.1', PORT);

// get all geo tagged tweets
var filter = {
    locations: '-180,-90,180,90'
};

// if text is specified - uses OR
if(program.text) {
    filter.track = program.text;
}

// Keep variable reference here
var timerID = null;

// Recursive refreshStream function
function refreshStream() {

    // Stop lots of timers from building up.
    clearTimeout(timerID);

    // Filter twitter stream, get the geocoords and write to local TCP port
    twitApi.stream('filter', filter, function(stream) {
        stream.on('data', function(data) {

            if(data.coordinates) {
                tweetCount++;
                if(tweetCount % keep === 0) {

                    // Turn the lat, lon geocoords into Cartesian.
                    // cartObj = {x: float, y: float, z: float}
                    var cartObj = toECEFEarth(data.coordinates.coordinates[0], data.coordinates.coordinates[1]);

                    // Push the Cartesian object to the OSC port for openframeworks
                    // send them as strings - OF will convert them to floats
                    oscSender.send('/coords', 'sss', [cartObj.x.toString(), cartObj.y.toString(), cartObj.z.toString()]);
                    tweetCount = 0;

                    // To send only one coord and then stop
                    // stream.destroy();


                } else {
                    // Discard
                    return;
                }
            }

            // Time now
            timeCheck.timeNow = new Date().getTime();

            // Timestamp of this tweet
            timeCheck.lastTime =  new Date(data.created_at).getTime();

            var currentDiff = timeCheck.timeNow - timeCheck.lastTime;

            // Keep high water mark of time to process
            if(currentDiff > timeCheck.maxDiff) {
                timeCheck.maxDiff = currentDiff;
                if(currentDiff > 10000) {
                    console.log('timeDiff exceeds 10000 ms : ' + timeCheck.maxDiff);
                }
            }

            // Are there warnings / disconnections - reconnect?
            // https://dev.twitter.com/docs/streaming-apis/messages#Limit_notices_limit
            if(!data.created_at) {
                if('disconnect' in data) {
                    console.log(data);
                }
            }

        });

        stream.on('error', function(err) {
            console.log(err);
            // stream.destroy();
        });

        // Register a timeout to destroy the stream after an hour and reconnect
        timerID = setTimeout(function() {
            stream.destroy();
            refreshStream();
        // Rest the connection every hour
        }, 3600000);

    });

    console.log("Send Cartesian coords to port " + PORT);
}

// Kick off the twitter stream
refreshStream();

/**
 * Convert latitude and longitude geo coords into Cartesian
 * @param  {number} lat Latitude
 * @param  {number} lon Longitude
 * @return {object}     Cartesian conversion with x, y, z fields
 */
function toECEFEarth(lat, lon) {
    // Approximate radius of the earth 6371km
    var R = 6378137.0;

    var cosLat = Math.cos(degToRad(lat));
    var sinLat = Math.sin(degToRad(lat));
    var cosLon = Math.cos(degToRad(lon));
    var sinLon = Math.sin(degToRad(lon));

    var f = 1.0 / 298.257224;
    var C = 1.0 / Math.sqrt(cosLat * cosLat + (1 - f) * (1 - f) * sinLat * sinLat);
    var S = (1.0 - f) * (1.0 - f) * C;
    var h = 0.0;

    var x = (R * C + h) * cosLat * cosLon;
    var y = (R * C + h) * cosLat * sinLon;
    var z = (R * S + h) * sinLat;

    return {
        x : x,
        y : y,
        z : z
    };
}

/**
 * Convert degrees to radians
 * @param  {number} degrees Angle in degrees
 * @return {number}         Angle in radians
 */
function degToRad(degress) {
    var radians = degress * (Math.PI/180);
    return radians;
}