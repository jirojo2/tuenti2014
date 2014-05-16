#!/usr/bin/env node

if ((process.version.split('.')[1]|0) < 10) {
	console.log('Please, upgrade your node version to 0.10+');
	process.exit();
}

var net = require('net');
var util = require('util');
var crypto = require('crypto');

var options = {
	'port': 6969,
	'host': '54.83.207.90',
}

var KEYPHRASE = 'YOUR KEYPHRASE';

var dh, secret, serverkey, state = 0;

function attack() {

	var dh, secret, serverkey;

	var socket = net.connect(options, function() {
		//console.log('Connected to backdoor service.');
		state++;
	});

	socket.on('data', function(data) {

		var pkg = data.toString().trim().split(':');
		
		var hdr = pkg[0].split('->');
		var src = hdr[0];
		var dst = hdr[1];

		//console.log(data.toString().trim());

		if (pkg.length == 1) return;

		var body = pkg[1].split('|');

		if (src == 'CLIENT') {
		  	if (body[0] == 'hello?') {
				socket.write(pkg[1] + '\n');
			}
			if (body[0] == 'key') {
				dh = crypto.createDiffieHellman(256);
				dh.generateKeys();
				socket.write(util.format('key|%s|%s\n', dh.getPrime('hex'), dh.getPublicKey('hex')));
			}
			if (body[0] == 'keyphrase') {
				secret = dh.computeSecret(serverkey, 'hex');
				var cipher = crypto.createCipheriv('aes-256-ecb', secret, '');
				var keyphrase = cipher.update(KEYPHRASE, 'utf8', 'hex') + cipher.final('hex');
				socket.write(util.format('keyphrase|%s\n', keyphrase));	
			}

		}

		if (src == 'SERVER') {
			socket.write(pkg[1] + '\n'); // Tell the client the server sends its regards 
			if (body[0] == 'key') {
				serverkey = body[1];
				//console.log(util.format("Server key: %s", serverkey));
			}
			if (body[0] == 'result') {
				var decipher = crypto.createDecipheriv('aes-256-ecb', secret, '');
				var message = decipher.update(body[1], 'hex', 'utf8') + decipher.final('utf8');
				console.log(message);
				socket.end();
			}
		}
	});
}

var readline = require('readline');

var rl = readline.createInterface({
	input: process.stdin,
	output: process.stdout,
	terminal: false
});

rl.on('line', function (cmd) {
	KEYPHRASE = cmd;
	attack();
});
