# Medibox Simulation Projects

This repository contains the Wokwi simulation project and Node-RED flow for the extended Medibox project from the EN2853: Embedded Systems and Applications course. The Medibox project is designed to help users manage their medication schedules by setting alarms and monitoring environmental conditions.

## Project Description

The Medibox project uses an ESP32 microcontroller to simulate a medicine reminder box with the following functionalities:

### Basic Features

- **Time Zone Setting**: Allows users to set the time zone by inputting the offset from UTC.
- **Alarm Setting**: Users can set up to three alarms.
- **Alarm Disabling**: Option to disable all alarms.
- **Time Fetching**: Fetches the current time from an NTP server over Wi-Fi and displays it on an OLED screen.
- **Alarm Indication**: Rings an alarm with proper indication when the set alarm times are reached.
- **Alarm Stopping**: Allows stopping the alarm using a push button.
- **Environmental Monitoring**: Monitors temperature and humidity levels and provides warnings when they exceed healthy limits.

### Enhanced Features

- **Light Intensity Monitoring**: Uses two Light Dependent Resistors (LDRs) to monitor light intensity to protect light-sensitive medicines.
  - Displays the highest light intensity on the Node-RED dashboard.
  - Indicates which LDR (left or right) recorded the highest intensity.
  - Uses a gauge to display real-time intensity and a plot for past variations.
- **Shaded Sliding Window**: Controls a servo motor to adjust the window position based on light intensity.
  - Regulates light entering the Medibox to ensure optimal storage conditions.
  - Adjustable minimum angle and controlling factor through the Node-RED dashboard.

## Features and Implementation

### Menu System

- Allows users to navigate through options to set time zone, set alarms, and disable alarms.

### NTP Server Integration

- Fetches the current time from the NTP server and displays it on the OLED screen.

### Alarm System

- Users can set three alarms.
- Provides proper indications (buzzer, LED, message on OLED) when the alarm rings.
- Option to stop the alarm using a push button.
- Option to disable all alarms.

### Environmental Monitoring

- Monitors temperature and humidity using sensors.
- Provides warnings on the OLED screen when temperature or humidity exceeds healthy limits (Temperature: 26°C - 32°C, Humidity: 60% - 80%).

### Light Intensity Monitoring

- Uses two LDRs to monitor light intensity.
- Displays highest light intensity on the Node-RED dashboard.
- Indicates which LDR (left or right) recorded the highest intensity.
- Uses a gauge for real-time intensity and a plot for past variations.

### Shaded Sliding Window

- Controls a servo motor to adjust the window based on light intensity.
- Adjustable minimum angle and controlling factor via Node-RED dashboard.

## Usage

### Wokwi Simulations

1. Open the `WokwiProject_ExtendedFeatures` folder.
2. Load the project files into Wokwi Simulator.
3. Run the simulation and observe the behavior on the virtual components (OLED, buzzer, LEDs, etc.).

### Node-RED Flows

1. Import the respective Node-RED flow from the `NodeRED_Flow` folder.
2. Deploy the flow on the Node-RED dashboard.
3. Interact with the dashboard to observe the light intensity monitoring and servo control functionalities.
