# twitter_timelapse
Code for [twitter-timelapse.tumblr.com](http://twitter-timelapse.tumblr.com) image rendering.

A Raspberry Pi was used to to render the images from this app over a number of days last year.

The whole process didn't perform well on the Pi alone so a small netbook was also used with data made available using OSC.

## Processes
1. node.js
    - Push the geodata coordinates from the streaming twitter API to port 3001 using OSC protocol
2. ofApp
    - Read the OSC data from the same port and use it in the running visual render
    - Periodically save the image to a shared folder
3. node.js
    - Wait for a file to appear in the incoming images folder
    - Post the image to tumblr usin the API
    - Move the image into a completed uploads folder

### Step 1
Connect to the twitter stream API and push the geocoordinates of each matching tweet to a local port.

```
    $ node node-twit/index.js -h

    -t, --text [string or #string]  Filter tweets containing the specified text or hashtag. Comma separated.
    -r, --retain [number]           Use every x tweet to limit rate. Defaults to 10
    -d, --debug                     Print to stdout

    $ node node-twit/index.js -t  -r 10
```

### Step 2
With the correct ip and port set run the openFrameworks app.

### Step 3
In a separate process:

```
    node tumblr-post/index.js
```