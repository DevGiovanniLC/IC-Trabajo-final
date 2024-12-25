# Protocolo LoRa

## Paquetes entrantes

| Byte | Descripcion |
| ---- | ----------- |
| 0 | direccion de destino |
| 1 | direccion de origen |
| 2 | contenido entrante |

### Contenido entrante

| Byte |
| ---- |
| 0 (libre) |
| 1 (ocupado) |

## Paquetes salientes

| Byte | Descripcion |
| ---- | ----------- |
| 0 | direccion de destino |
| 1 | direccion de origen |
| 2 | contenido saliente |

### Contenido saliente

| Bit 0 | Bit 1 - 3 | Bit 4 - 7 |
| ----- | ----- | ----- |
| 0 (distancia) | metros | decimetros |
| 1 (tiempo) | segundos (x10) | minutos |