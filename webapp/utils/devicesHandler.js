import { Device } from "./device.js"
const webSocket = io("http://localhost:3000")

// Initial device configuration = no detection, 2.5 meters, 60 seconds
const devices = [
    new Device(130, false, {meters: 2, decimeters: 5}, {minutes: 1, seconds: 0}),
    new Device(131, false, {meters: 2, decimeters: 5}, {minutes: 1, seconds: 0}),
    new Device(147, false, {meters: 2, decimeters: 5}, {minutes: 1, seconds: 0})
]

let checkInDevices = (address) => {
    return devices.find(device => device.address === address)
}

let addDevice = (device) => {
    let newDevice = new Device(device.address, device.status == 1 ? true : false, {meters: 2, decimeters: 5}, {minutes: 1, seconds: 0})
    devices.push(newDevice)
    return newDevice
}

let buildDeviceUi = (device) => {
    // Getting the devices list
    let list = document.getElementById("device-list")

    // Creating the device article surface
    let deviceArticle = document.createElement("article")
    deviceArticle.classList.add("device")
    deviceArticle.id = `device-${device.address}`

    // Creatring the device address surface
    let deviceAddressSurface = document.createElement("div")
    deviceAddressSurface.classList.add("device-address")
    deviceAddressSurface.innerHTML = `<p>${device.address}<p>`

    // Creating the device information surface
    let deviceInformationSurface = document.createElement("div")
    deviceInformationSurface.classList.add("device-information")
    deviceInformationSurface.innerHTML = `
        <div class = "device-current">
            <p> Running At: </p>
            <div class = "device-current-distance">
                <p> Distance (m) </p>
                <p> ${device.distance} </p>
            </div>
            <div class = "device-current-frequency">
                <p> Frequency (s) </p>
                <p> ${device.frequency} </p>
            </div>
        </div>
        <div class = "device-status">
            <p> Status: </p>
            <div class = "device-status-off"></div>
        </div>
        <div class = "device-distance-update">
            <p> Change Distance: </p>
            <label for = "device-distance-meters-select"> Meters: </label>
            <select class = "device-distance-meters-select">
                <option value = "0"> 0 </option>
                <option value = "1"> 1 </option>
                <option value = "2" selected = "true"> 2 </option>
                <option value = "3"> 3 </option>
                <option value = "4"> 4 </option>
                <option value = "5"> 5 </option>
                <option value = "6"> 6 </option>
                <option value = "7"> 7 </option>
            </select>
            <label for = "device-distance-decimeters-select"> Decimeters: </label>
            <select class = "device-distance-decimeters-select">
                <option value = "0"> 0 </option>
                <option value = "1"> 1 </option>
                <option value = "2"> 2 </option>
                <option value = "3"> 3 </option>
                <option value = "4"> 4 </option>
                <option value = "5" selected = "true"> 5 </option>
                <option value = "6"> 6 </option>
                <option value = "7"> 7 </option>
                <option value = "8"> 8 </option>
                <option value = "9"> 9 </option>
            </select>
        </div>
        <div class = "device-frequency-update">
            <p> Change Frequency: </p>
            <label for = "device-frequency-minutes-select"> Minutes: </label>
            <select class = "device-frequency-minutes-select">
                <option value = "0"> 0 </option>
                <option value = "1" selected = "true"> 1 </option>
                <option value = "2"> 2 </option>
                <option value = "3"> 3 </option>
                <option value = "4"> 4 </option>
                <option value = "5"> 5 </option>
                <option value = "6"> 6 </option>
                <option value = "7"> 7 </option>
                <option value = "8"> 8 </option>
            </select>
            <label for = "device-frequency-seconds-select"> Seconds: </label>
            <select class = "device-frequency-seconds-select">
                <option value = "0"> 0 </option>
                <option value = "10"> 10 </option>
                <option value = "20"> 20 </option>
                <option value = "30"> 30 </option>
                <option value = "40"> 40 </option>
                <option value = "50"> 50 </option>
            </select>
        </div>
    `

    // Appending the device address and information surfaces to the device article
    deviceArticle.appendChild(deviceAddressSurface)
    deviceArticle.appendChild(deviceInformationSurface)

    // Appending the device article to the devices list
    list.appendChild(deviceArticle)
}

let addDeviceEventListeners = (device) => {
    let metersSelect = document.querySelector(`#device-${device.address} .device-distance-meters-select`)
    let decimetersSelect = document.querySelector(`#device-${device.address} .device-distance-decimeters-select`)
    let minutesSelect = document.querySelector(`#device-${device.address} .device-frequency-minutes-select`)
    let secondsSelect = document.querySelector(`#device-${device.address} .device-frequency-seconds-select`)

    metersSelect.addEventListener("change", (event) => {
        device.distance = parseInt(event.target.value) + 0.1 * parseInt(decimetersSelect.value)
        updateDeviceDistanceUi(device, device.distance)
        webSocket.emit('configure', {
            device: device.address,
            command: "distance",
            value: {
                meters: parseInt(event.target.value),
                decimeters: parseInt(decimetersSelect.value)
            }
        })
    })

    decimetersSelect.addEventListener("change", (event) => {
        device.distance = parseInt(metersSelect.value) + 0.1 * parseInt(event.target.value)
        updateDeviceDistanceUi(device, device.distance)
        webSocket.emit('configure', {
            device: device.address,
            command: "distance",
            value: {
                meters: parseInt(metersSelect.value),
                decimeters: parseInt(event.target.value)
            }
        })
    })

    minutesSelect.addEventListener("change", (event) => {
        device.frequency = parseInt(event.target.value) * 60 + parseInt(secondsSelect.value)
        updateDeviceFrequencyUi(device, device.frequency)
        webSocket.emit('configure', {
            device: device.address,
            command: "period",
            value: {
                minutes: parseInt(event.target.value),
                seconds: parseInt(secondsSelect.value)
            }
        })
    })

    secondsSelect.addEventListener("change", (event) => {
        device.frequency = parseInt(minutesSelect.value) * 60 + parseInt(event.target.value)
        updateDeviceFrequencyUi(device, device.frequency)
        webSocket.emit('configure', {
            device: device.address,
            command: "period",
            value: {
                minutes: parseInt(minutesSelect.value),
                seconds: parseInt(event.target.value)
            }
        })
    })
}

let updateDeviceStatusUi = (device, status) => {
    let deviceStatusSurface = document.querySelector(`#device-${device.address} .device-status div`)
    if (status) {
        deviceStatusSurface.classList.remove("device-status-off")
        deviceStatusSurface.classList.add("device-status-on")
    } else {
        deviceStatusSurface.classList.remove("device-status-on")
        deviceStatusSurface.classList.add("device-status-off")
    }
}

let updateDeviceDistanceUi = (device, distance) => {
    let deviceDistanceSurface = document.querySelector(`#device-${device.address} .device-current-distance p:last-child`)
    deviceDistanceSurface.innerHTML = Math.round(distance * 10) / 10
}

let updateDeviceFrequencyUi = (device, frequency) => {
    let deviceFrequencySurface = document.querySelector(`#device-${device.address} .device-current-frequency p:last-child`)
    deviceFrequencySurface.innerHTML = Math.round(frequency * 10) / 10
}

let buildInitialUi = () => {
    devices.forEach(device => {
        buildDeviceUi(device)
        addDeviceEventListeners(device)
    })
}

webSocket.on('information', (data) => {
    console.log(data)
    let device = checkInDevices(data.address)
    if (device) {
        updateDeviceStatusUi(device, data.status)
    } else {
        let device = addDevice(data)
        buildDeviceUi(device)
        addDeviceEventListeners(device)
    }
})

document.addEventListener("DOMContentLoaded", () => {
    console.log("DOM fully loaded and parsed")
    buildInitialUi()
})

