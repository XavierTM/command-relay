

const express = require('express');
const morgan = require('morgan');

console.clear();

const app = express();

app.use(morgan('dev'));

const commandAvailable = {
	'1': false,
	'2': false
};


app.get('/post-command/:id', function(req, res) {
	const { id } = req.params;
	commandAvailable[id] = true;
	res.send("OK");
});

app.get('/check-command/:id', function(req, res) {
	const { id } = req.params;
	res.send(`||${commandAvailable[id]}||`);
	commandAvailable[id] = false;
});


app.listen(8080, function() {
	console.log('Server started.');
});

