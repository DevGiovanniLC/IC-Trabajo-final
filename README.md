

# Propuesta de Proyecto IoT: 
# Sistema de Detección de Plazas de Aparcamiento

***Cardona Candil, Adrián***

***Chen Zhou, Naixin***

***León Corujo, Giovanni***

***Ortega Abelleira, Miguel Ángel***

---
## Introducción

El presente proyecto tiene como objetivo desarrollar un sistema IoT que permita detectar y monitorizar en tiempo real el estado de ocupación de plazas de aparcamiento mediante sensores ultrasónicos y tecnología LoRa. Este sistema estará compuesto por:

- **4 placas Arduino.**
- **4 antenas LoRa.**
- **3 sensores ultrasónicos.**
- **1 portátil para procesamiento y visualización.**

El sistema integrará la comunicación inalámbrica LoRa para transmitir los datos de ocupación desde los sensores hasta un gateway central, que posteriormente enviará la información a un portátil. Los datos serán procesados y enviados a un broker MQTT, permitiendo su visualización a través de una interfaz web.

---

## Descripción del Sistema

### Componentes Principales

1. **Sensores de Ultrasonido:**
    * Cada sensor estará conectado a un Arduino y será responsable de detectar si una plaza de aparcamiento está ocupada o libre.
2. **Antenas LoRa:**
    * Utilizadas para establecer la comunicación inalámbrica entre los Arduinos de los sensores y el gateway.
3. **Arduino Gateway:**
    * Recibe los datos de los otros tres Arduinos y los transmite al portátil mediante conexión USB.
4. **Portátil:**
    * Actúa como intermediario para procesar y enviar los datos al broker MQTT.
5. **Broker MQTT:**
    * Servidor de mensajería donde los datos serán almacenados temporalmente para su consulta.
6. **Aplicación web:**
    * Cliente que consume los datos del broker MQTT y muestra el estado de las plazas en un panel gráfico.

### Arquitectura del Sistema

1. **Sensores y Detección:**
    * Tres Arduinos estarán equipados con un sensor ultrasónico cada uno.
    * Cada sensor medirá la distancia a un objeto (vehículo) sobre su plaza correspondiente.
    * Si la distancia medida indica ocupación, el Arduino generará un mensaje indicando que la plaza está ocupada.
    * Desde el cliente será posible configurar la distancia a partir de la cual el Arduino considera que la plaza está ocupada o no.
2. **Comunicación LoRa:**
    * Los Arduinos con sensores enviarán la información de ocupación (plaza libre/ocupada) a través de sus antenas LoRa.
    * El Arduino Gateway recibirá estos datos.
3. **Gateway y Comunicación con el Portátil:**
    * El Arduino Gateway actuará como concentrador, recopilando los datos de los sensores y transmitiéndolos al portátil mediante conexión USB.
4. **Procesamiento en el Portátil:**
    * Un programa en el portátil leerá los datos enviados por el Arduino Gateway.
    * Utilizando un cliente MQTT, los datos serán publicados en un broker.
5. **Visualización web:**
    * La aplicación web consumirá los datos del broker MQTT y mostrará un panel con el estado de las plazas de aparcamiento.

---
## Metodología

1. **Configuración de Sensores:**
    * Programar cada Arduino para leer los datos del sensor ultrasónico y determinar el estado de la plaza.
2. **Implementación de la Comunicación LoRa:**
    * Configurar las antenas LoRa en los Arduinos para la transmisión y recepción de datos.
3. **Diseño del Gateway:**
    * Desarrollar un sketch de Arduino para que el Gateway recopile los datos y los envíe al portátil.
4. **Desarrollo del Cliente MQTT:**
    * Implementar un script en el portátil para leer los datos del Gateway y publicarlos en el broker MQTT.
5. **Integración con la aplicación web:**
    * Diseñar una aplicación web que consuma los datos del broker y muestre el estado de las plazas.

**Resultados Esperados**

- Detección precisa y en tiempo real del estado de ocupación de tres plazas de aparcamiento.
- Transmisión inalámbrica eficiente de los datos mediante LoRa.
- Visualización intuitiva en un panel gráfico que permita monitorear las plazas de forma remota.
- Infraestructura escalable para añadir más sensores o plazas en el futuro.

---
## Conclusión

Este proyecto combina tecnologías IoT, comunicación inalámbrica, y visualización de datos para desarrollar una solución innovadora y práctica de gestión de aparcamientos. Su implementación puede servir como base para futuros sistemas más complejos en entornos urbanos o privados.
