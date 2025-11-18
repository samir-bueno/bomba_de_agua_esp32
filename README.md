# 📡 Sistemas Embebidos: Datalogger ESP32 + MQTT + SQL + Grafana

## Integrantes
* Kevin
* Tomy
* Samir

---

## 1. Contexto y Requisitos del Proyecto

Una empresa que se dedica a proveer servicios de IoT determinó que uno de los pedidos más comunes de sus clientes era contar con un dispositivo al cual se le suele llamar **datalogger**. Un datalogger es un equipo que almacena los datos temporales de algún parámetro, para que un usuario o administrador del sistema pueda acceder a los valores históricos de ese parámetro.

### Esquema General del Sistema
El proyecto se basa en la infraestructura de sistemas embebidos propuesta, utilizando una ESP32 como dispositivo de borde y una arquitectura de servidor para el procesamiento y visualización.

![Esquema de Datalogger con Sensor, ESP32, Mosquitto, Node-RED, SQL y Grafana](assets/esquema_general.png)

### Requisitos del Sistema
Se consideraron los siguientes requisitos para la solución:
* **Infraestructura:** La solución debe utilizar una infraestructura (dispositivos y servicios) como la indicada en el esquema.
* **Escalabilidad:** El sistema debe ser escalable, permitiendo funcionar para un sólo dispositivo o para cientos de ellos, y para distintos sensores dentro de un mismo dispositivo.
* **Datos:** Los datos reportados deben obtenerse de una medición real.

### Tabla de Planificación (Resumen de Tareas Realizadas)
| Act. | Descripción | Responsable | Estado |
| :---: | :--- | :---: | :---: |
| A | Descripción/definición del problema a resolver | Samir | Terminado |
| B | Elección/justificación del sensor y accesorios a utilizar | Tomy | Terminado |
| C | Mosquitto: instalación y puesta en marcha del servicio | Kevin | Terminado |
| D-E | SQL: Instalación, puesta en marcha y Creación de base de datos | Tomy | Terminado |
| F-H | ESP32: Obtención del dato y publicación MQTT | Kevin | Terminado |
| I-L | Node-RED: Instalación, suscripción y almacenamiento del dato | Samir | Terminado |
| M, N, O | Grafana: Configuración de servicio y dashboard | los 3 | SIN INICIAR |

---

## 2. Proyecto Implementado: Monitoreo de Nivel de Tanque (Tank-Level)

### (A) Descripción del Problema a Resolver
El objetivo es resolver el control del nivel de agua en tanques. El sistema inteligente de monitoreo con un microcontrolador conectado a Wi-Fi mide el nivel de agua en tiempo real, permitiendo al usuario anticiparse a la falta de agua y evitar desperdicios por rebalse.

### (B) Componentes Utilizados
| Componente | Justificación |
| :--- | :--- |
| **HC SR04** (Sensor Ultrasónico) | Es el sensor ultrasónico más accesible, facilita encontrar información y librerías. |
| **ESP 32 + Módulo WiFi** | Posee conectividad WiFi y Bluetooth integrada, esencial para enviar datos a Internet. |
| **Relé** | Recibe la orden del ESP32 para generar una acción física (encendido/apagado) sobre la bomba o electroválvula. |

### Prototipo
![Prototipo del sistema Tank-Level con tanque y electrónica](assets/prototipo_tank_level.png)

### Diagrama de Flujo (Tank-Level)
El diagrama muestra el flujo de datos y control del sistema, incluyendo los umbrales de acción del relé.

![Diagrama de Bloques del Sistema de Nivel de Tanque de Agua con ESP32, HC-SR04, Relé y Bomba](assets/diagrama_tank_level.png)

---

## 3. Implementación y Configuración

### (C) Mosquitto: Instalación y Puesta en Marcha
Se instaló el broker y el cliente MQTT, y se configuró para recibir datos de forma segura.

**Comandos de Instalación:**

sudo apt update && sudo apt upgrade -y
sudo apt install mosquitto mosquitto-clients -y
# Se configuran el listener 1883, allow_anonymous false y password_file
sudo systemctl restart mosquitto

# Configuración agregada al archivo:

listener 1883
allow_anonymous false
password_file /etc/mosquitto/passwd

# Reiniciar el servicio:

sudo systemctl restart mosquitto
sudo systemctl start mosquitto

### (D) SQL - Instalación y Puesta en Marcha de MariaDB

# Se utiliza Docker para la instalación y ejecución de MariaDB.

mkdir mariadb-data
docker pull mariadb:latest
docker run -d \
--name mariadb-samir \
-p 3307:3306 \
-v mariadb-data:/var/lib/mysql \
-e MYSQL_ROOT_PASSWORD=pancho \
-e MYSQL_DATABASE=appstock \
--restart unless-stopped \
mariadb:latest

# Verificación:

docker ps | grep mariadb-samir

# Acceso a MariaDB:

docker exec -it mariadb-samir mariadb -u root -ppancho

# Acceso: http://10.56.2.15:1880

### (J) Suscripción a Broker MQTT

### (K) Obtención del Dato

### (L) Almacenamiento del Dato

# El dato se procesa en un nodo Function antes de ser insertado en MariaDB, asegurando el formato correcto.

# Función de Procesamiento en Node-RED:

// Procesar los datos del sensor HC-SR04
var distancia = msg.payload;

// Preparar el INSERT para MariaDB
msg.topic = "INSERT INTO pruebas (fecha_hora, mensaje, distancia_cm) VALUES (NOW(), ?, ?)";
msg.payload = ["Dato del sensor HC-SR04 ", distancia];

return msg;

# Datos Almacenados en la Base de Datos:

image

### 4. Repositorios del Proyecto

Código ESP32: https://github.com/samir-bueno/bomba_de_agua_esp32.git

Página Web (Visualización): https://github.com/samir-bueno/bomba_de_agua_pagina.git