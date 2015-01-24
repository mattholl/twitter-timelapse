# twitter_timelapse
Code for twitter-timelapse.tumblr.com image renderings.

Was run over a number of days last year on and a Raspberry Pi to render the images, save to a shared disk.
The whole process didn't perform well on the Raspberry Pi so a small netbook was used
Separarte node process on the pi listen for image dump and uploaded it to tumblr.

## Procedure
1. node.js
    - Push the geodata coordinates from the streaming twitter API to a port with OSC.
2. ofApp
    - Read the OSC data from the same port and use it in the running visual render
    - Periodically save the image to a shared folder
3. node.js
    - Wait for a file to land in the incoming image uploads folder
    - Post the image to the tumblr API
    - Move the file into a completed uploads folder

### Step 1
Connect to the twitter stream API and

node node-twit/index.js -h

    -t, --text [string or #string]  Filter tweets containing the specified text or hashtag. Comma separated.
    -r, --retain [number]           Use every x tweet to limit rate. Defaults to 10
    -d, --debug                     Print to stdout

### Step 2
With the correct ip and port set run the ofApp.

### Step 3
