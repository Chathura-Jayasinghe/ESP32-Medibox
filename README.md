# ESP32-Medibox
This repo includes the simulation of the Medibox using ESP32.

The **ESP32 Medibox** is a smart medicine box designed to help users effectively manage their medication schedules. Utilizing an ESP32 microcontroller, it integrates various sensors, actuators, and communication modules to offer a comprehensive medication management solution.

## Overview
  <em>Built using the following tools and technologies:</em>
</p>
<p>
 <img src="https://img.shields.io/badge/VS_Code-007ACC.svg?style=flat&logo=visual-studio-code&logoColor=white" alt="VS Code">
 <img src="https://img.shields.io/badge/C++-00599C.svg?style=flat&logo=c%2B%2B&logoColor=white" alt="C++">
 <img src="https://img.shields.io/badge/Node_Red-8F0000.svg?style=flat&logo=node-red&logoColor=white" alt="Node-RED">
 <img src="https://img.shields.io/badge/Wokwi-18A497.svg?style=flat&logo=google&logoColor=white" alt="Wokwi">
</p>

## Table of Contents

- [ESP32 Medibox](#esp32-medibox)
  - [Overview](#overview)
  -   - [Table of Contents](#table-of-contents)
  - [Circuit Diagram](#circuit-diagram)
  - [Features](#features)
  - [Components](#components)
  - [How It Works](#how-it-works)
  - [Node-RED Flow](#node-red-flow)
    - [Running Node-RED](#running-node-red)
    - [MQTT Topics](#mqtt-topics)
  - [Getting Started](#getting-started)
    
## Circuit Diagram

You can view the complete circuit diagram and simulation on [Wokwi]https://wokwi.com/projects/391493988516700161).

![image](https://github.com/user-attachments/assets/c0a6f793-b663-46f2-b8e4-d74c86372aa3)

## Features

- **WiFi Connectivity:** Enables remote monitoring and control via MQTT.
- **Temperature and Humidity Monitoring:** Uses the DHT22 sensor to track environmental conditions.
- **Light Intensity Measurement:** Detects ambient light levels with LDRs and adjusts the servo motor accordingly.
- **Servo Motor Control:** Modulates the position of the shaded window based on light intensity.
- **Buzzer Notifications:** Alerts users for scheduled medication times.
- **Button Interface:** Allows user interaction for canceling alarms or confirming actions.
- **Customizable Settings:** Adjusts the servo motor’s minimum angle and control factor through MQTT.

## Components

- **ESP32:** The central microcontroller for the system.
- **DHT22:** Sensor for measuring temperature and humidity.
- **LDRs:** Light-dependent resistors to measure light intensity.
- **Servo Motor:** Controls the shaded window's position.
- **Buzzer:** Emits audio notifications.
- **Buttons:** Facilitates user interaction.

## How It Works

The ESP32 Medibox assists with medication management through:

1. **Monitoring:** Continuously tracks temperature, humidity, and light intensity.
2. **Notifications:** Provides timely reminders via a buzzer for medication.
3. **Remote Control:** Enables remote adjustment and monitoring via MQTT.

The system is adaptable, allowing users to customize parameters like the servo motor’s minimum angle and control factor.

## Node-RED Flow

You can find the Node-RED flow for this project in the `node-red-flow.json` file. Import this file into your Node-RED setup to interact with the ESP32 Medibox through MQTT.

![image](https://github.com/user-attachments/assets/5cab2cdd-8747-41ed-ab33-2cb5837c67c7)

### Running Node-RED

To set up Node-RED:

1. **Install Node-RED**:
   - First, install [Node.js](https://nodejs.org/).
   - Then, run this command to install Node-RED:
     ```bash
     npm install -g --unsafe-perm node-red
     ```

2. **Start Node-RED**:
   - Use the command:
     ```bash
     node-red
     ```
   - Access Node-RED at [http://localhost:1880](http://localhost:1880).

3. **Create and Deploy Flows**:
   - Design and deploy flows in the Node-RED interface to interact with your ESP32.

Node-RED allows you to build interactive dashboards and control mechanisms for enhanced functionality.

### MQTT Topics

This project utilizes the following MQTT topics:

- **MQTT-ON-OFF:** Controls the buzzer (ON/OFF).
- **MQTT-SCH-ON:** Manages buzzer notification schedules.
- **MQTT-TEMP:** Receives temperature readings.
- **MQTT-LDR:** Receives light intensity data.
- **MQTT-MIN-ANG:** Sets the servo motor’s minimum angle.
- **MQTT-CTRL-FAC:** Adjusts the servo motor’s control factor.
- **MQTT-DROP-DOWN:** Chooses predefined settings for the servo motor.

## Getting Started

To begin with the ESP32 Medibox:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Chathura-Jayasinghe/ESP32-Medibox
