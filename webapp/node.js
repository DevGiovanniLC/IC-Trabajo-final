const MQTTClient = require('./lib/mqttclient')
const { Server } = require('socket.io')

const http = require('http')

const io = http.createServer()
const socket = new Server(io, {
    cors: {
        origin: '*',
        methods: ['GET', 'POST', "PUT", "DELETE"]
    }
  }
);

const options = {
    host: "localhost",
    port: 8000,
    path: "/mqtt",
    clientId: "mqttjs_" + Math.random().toString(16).substr(2, 8),
    parkingTopic: "parking",
    configureTopic: "configure"
}

const client = new MQTTClient(
    options,

    /*
    When the MQTT client receives a message from the broker, a callback function is called
    so the received message gets decoded and sent by the socket to the web client. Note, data
    is the decoded message.
    */

    (data) => {
        socket.emit('information', data)
    }
);

/*
When the web client subscribes and emits a configure event, an object is received with the
new configuration data. Then, this object is emmited to the MQTT broker coded as a message. 
*/

socket.on('connection', (socket) => {
    socket.on('configure', (data) => {
        client.publish(data)
    })
});

client.connect();

io.listen(3000, () => console.log("Socket server running on port 3000"));