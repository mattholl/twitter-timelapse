var chokidar = require('chokidar');
var fs = require('fs');
var path = require('path');
var Tumblr = require('tumblrwks');

var fileSize = 0;
var fileCheckTimer = null;

// http://stackoverflow.com/questions/7665818/tumblr-api-2-where-is-the-oauth-token-and-oauth-token-secret
// https://api.tumblr.com/console/calls/user/info
// https://groups.google.com/forum/#!topic/tumblr-api/gz8Zv-Mhex4

var key = require('./key.json');

var postTestApi = new Tumblr(key.oauth, key.tumblr_blog_url);

var watcher = chokidar.watch(key.READ_FILE_PATH, {ignored: /[\/\\]\./, persistent: true});

watcher.on('add', function(absPath) {
    checkFile(absPath);
});

// Check whether the file
function checkFile(imgPath) {

    clearTimeout(fileCheckTimer);

    // Check whether saving is finished
    fileCheckTimer = setTimeout(function() {

        fs.stat(imgPath, function(err, stat) {
            if(err) console.log(err);

            var currentFileSize = stat.size;

            if(currentFileSize === fileSize) {
                // Still saving recall this function
                checkFile(imgPath);
            } else {
                // Filesize has not increased
                //   - post the image
                //   - clear the timer
                //   - set fileSize back to zero
                postFile(imgPath);
                clearTimeout(fileCheckTimer);
                fileSize = 0;
            }

        });

    }, 1000);
}


// When the file writing is complete post the image to tumblr and move to archive folder
function postFile(filePath) {

    var img = fs.readFileSync(filePath);

    var postObject = {
        type : 'photo',
        data : [img]
    };

    postTestApi.post('/post', postObject, function(err, response) {

        // Logs
        console.log(new Date().getTime().toString());
        console.log('Image : ' + filePath);
        console.log(response);

        var imgName = path.basename(filePath);

        // move the file
        fs.rename(filePath, key.ARCHIVE_PILE_PATH + imgName, function(err) {
            if(err) console.log(error);
        });

    });
}
