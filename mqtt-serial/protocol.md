# Protocolo MQTT

## Paquetes entrantes

### Comandos

- device: dirección LoRa del dispositivo
- command: tipo de comando
- value: valor del comando

#### Cambio de tiempo de actualización

- command: period
- value: minutos y segundos

Valores válidos:

| Minutos | Segundos |
| ------- | -------- |
| 0 a 8   | 0 / 10 / 20 / 30 / 40 / 50 |

Ejemplo:

```json
{
  "device": 147,
  "command": "period",
  "value": {
    "minutes": 3,
    "seconds": 30
  }
}
```

#### Cambio de distancia

- command: distance
- value: metros y decimetros

Valores válidos:

| Metros | Decimetros |
| ------ | ---------- |
| 0 a 7 | 0 a 9     |

Ejemplo:

```json
{
    "device": 147,
    "command": "distance",
    "value": {
      "meters": 3,
      "decimeters": 8
    }
}

```

## Paquetes salientes

### Estado del aparcamiento

- device: dirección LoRa del dispositivo
- occupied: 1 si el aparcamiento está ocupado, 0 si no

Ejemplo:

```json
{
  "device": 147,
  "occupied": 1
}
```
