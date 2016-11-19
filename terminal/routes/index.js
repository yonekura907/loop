var express = require('express');
// var osc = require('osc');
var osc = require('node-osc');

var router = express.Router();

var oscClient = [];

// var udpBroadcast = new osc.UDPPort({
//     localAddress: '127.0.0.1',
//     localPort: 7100,
//     // remoteAddress: '172.17.11.116',
//     // remotePort: 7100,
//     // broadcast: true
// });
//
// // Listen for incoming OSC bundles.
// udpBroadcast.on("bundle", function (oscBundle, timeTag, info) {
//     console.log("An OSC bundle just arrived for time tag", timeTag, ":", oscBundle);
//     console.log("Remote info is: ", info);
// });
//
// //Open the socket.
// udpBroadcast.open();


//デフォルトルーティング
router.get('/', function(request, response) {
    response.redirect('/public');
});

router.get('/start/:ip1/:ip2', function(request, response) {
    console.log('Sending message start');
    oscClient.push(new osc.Client(request.params.ip1, 7100));
    oscClient.push(new osc.Client(request.params.ip2, 7100));

    var oscMsg = new osc.Message('/start');
    oscClient[0].send(oscMsg, 200);
    oscClient[1].send(oscMsg, 200);

    response.send("success start");
});


router.get('/sync', function(request, response) {
    console.log('Sending message sync');
    oscClient.push(new osc.Client(request.params.ip1, 7100));
    oscClient.push(new osc.Client(request.params.ip2, 7100));

    var oscMsg = new osc.Message('/sync');
    oscClient[0].send(oscMsg, 200);
    oscClient[1].send(oscMsg, 200);

    response.send("success sync");
});

router.get('/sync', function(request, response) {
    console.log('Sending message stop');
    oscClient.push(new osc.Client(request.params.ip1, 7100));
    oscClient.push(new osc.Client(request.params.ip2, 7100));

    var oscMsg = new osc.Message('/stop');
    oscClient[0].send(oscMsg, 200);
    oscClient[1].send(oscMsg, 200);

    response.send("success stop");
});

module.exports = router;
