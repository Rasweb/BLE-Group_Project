## Table of contents
- [Overview](#overview)
  - [BLE_Group_Project](#ble-group-project)
  - [Built with](#built-with)

## Overview
### BLE Group Project
A ble group project connect to my embedded education at STI, from the course "data communication". 
Contributors on the project: [Gustav](https://github.com/Gboettge), [Richard](https://github.com/richekalen), and me. 

This project emphasizes the physical separation between the input and output devices. It comprises of two MCUs(microcontroller): one serves as the input for a specific pattern (e.g., push buttons) and, upon successful verification, wirelessly communicates via BLE to the second MCU, which then performs an action, such as (e.g., opening a door). It operates on a battery power, requiring the first MCU to monitor battery status. In the event of low battery, it will automatically open to faciliate battery replacement.

In the beginning of the project we planed on using nimBLE for the bluetooth but couldn't get it to work as it should with our project. Therefore we swtiched to Bluedroid a bluetooth version created by google for the android devices but even though the code was much larger, the connection between the MCUs began working without major issues. Also it was alot easier to troubleshoot the bluedroid issues because of it being an older and tested version of bluetooth.

The folders are named accordingly to their specific MCU and in the S3 (ESP32-S3) folder most of the code is stored. This project is focused on the BLE side though which is stored in the respective folders for the specific MCUs 

### Built with

- C
- ESP-IDF
- ESP32-C6
- ESP32-S3
- Buttons (to test)
- Battery (to test)
