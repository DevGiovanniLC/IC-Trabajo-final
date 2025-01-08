const MQTTClient = require('../lib/mqttclient');

const options = {
    host: "localhost",
    port: 8000,
    path: "/mqtt",
    clientId: "mqttjs_" + Math.random().toString(16).substr(2, 8),
}

const client = new MQTTClient(options);
client.connect();