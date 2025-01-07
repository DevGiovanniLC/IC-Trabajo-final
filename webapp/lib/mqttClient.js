const { Client } = require('paho-mqtt');

/* Definition of MQTTClient class:
  1. Constructor: Initializes the MQTT client with the given options
  2. Connect: Attempts to connect to the MQTT broker
  3. OnConnect: If successfully connected, subscribes to the given topic
  4. OnConnectionLost: Logs error message when connection is lost
  5. Subscribe: Subscribes the client to the given topic
  6. OnSubscribe: Logs a message when successfully subscribed
  7. Publish: Publishes a message to the given topic
  8. OnMessageArrived: Logs the message received from the broker
*/
class MQTTClient {
    constructor(options) {
        this.client = new Client(
            options.host,
            options.port,
            options.path,
            options.clientId
        )
    }

    connect() {
        this.client.connect({
            onSuccess: this.onConnect,
            reconnect: true
        })
    }

    onConnect() {
        console.log("Connected to MQTT broker")
        this.subscribe()
    }

    onConnectionLost(responseObject) {
        if (responseObject.errorCode !== 0) {
            console.log("onConnectionLost:" + responseObject.errorMessage);
        }
    }

    subscribe() {
        this.client.subscribe(options.parkingTopic, {
            qos: 1, 
            onSuccess: this.onSubscribe
        })
    }

    onSubscribe() {
        console.log("Subscribed to topic")
    }

    publish(message) {
        this.client.publish(
            options.configureTopic,
            message,
            { qos: 1 }
        )
    }

    onMessageArrived(message) {
        console.log("onMessageArrived:" + message.payloadString);
    }
}

module.exports = MQTTClient;