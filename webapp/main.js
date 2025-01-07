const options = {
    host: "brokermqtt.duckdns.org",
    port: 1883,
    clientId: "web-" + parseInt(Math.random() * 100, 10),
    parkingTopic: "PARKING-SPACE",
    configureTopic: "PARKING-CONFIGURE"
}