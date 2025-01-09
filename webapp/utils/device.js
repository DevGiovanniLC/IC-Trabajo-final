class Device {
    constructor(address, status, distance, frequency) {
        this.address = address
        this.status = status
        this.distance = distance.meters + 0.1 * distance.decimeters
        this.frequency = frequency.minutes * 60 + frequency.seconds
    }
}

export {Device}