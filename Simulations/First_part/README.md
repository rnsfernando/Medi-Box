<!DOCTYPE html>
<html>
<head>
  
</head>
<body>
  <h1>Medibox Simulation Projects</h1>
  <p>This repository contains the Wokwi simulation projects for the Medibox project. The Medibox project is designed to help users manage their medication schedules by setting alarms and monitoring environmental conditions.</p>
  
  <h2>Project Description</h2>
  <p>The Medibox project uses an ESP32 microcontroller to simulate a medicine reminder box with the following functionalities:</p>
  <ul>
    <li><strong>Time Zone Setting:</strong> Allows users to set the time zone by inputting the offset from UTC.</li>
    <li><strong>Alarm Setting:</strong> Users can set up to three alarms.</li>
    <li><strong>Alarm Disabling:</strong> Option to disable all alarms.</li>
    <li><strong>Time Fetching:</strong> Fetches the current time from an NTP server over Wi-Fi and displays it on an OLED screen.</li>
    <li><strong>Alarm Indication:</strong> Rings an alarm with proper indication when the set alarm times are reached.</li>
    <li><strong>Alarm Stopping:</strong> Allows stopping the alarm using a push button.</li>
    <li><strong>Environmental Monitoring:</strong> Monitors temperature and humidity levels and provides warnings when they exceed healthy limits.</li>
  </ul>
  
  <h2>Features and Implementation</h2>
  <h3>Menu System</h3>
  <p>Allows users to navigate through options to set time zone, set alarms, and disable alarms.</p>
  
  <h3>NTP Server Integration</h3>
  <p>Fetches the current time from the NTP server and displays it on the OLED screen.</p>
  
  <h3>Alarm System</h3>
  <ul>
    <li>Users can set three alarms.</li>
    <li>Proper indications (buzzer, LED, message on OLED) when the alarm rings.</li>
    <li>Option to stop the alarm using a push button.</li>
    <li>Option to disable all alarms.</li>
  </ul>
  
  <h3>Environmental Monitoring</h3>
  <ul>
    <li>Monitors temperature and humidity using sensors.</li>
    <li>Provides warnings on the OLED screen when temperature or humidity exceeds healthy limits (Temperature: 26°C - 32°C, Humidity: 60% - 80%).</li>
  </ul>
  
  <h2>Usage</h2>
  <p>To run the simulations:</p>
  <ol>
    <li>Open the respective folder for the functionality you want to test.</li>
    <li>Load the project files into Wokwi Simulator.</li>
    <li>Run the simulation and observe the behavior on the virtual components (OLED, buzzer, LEDs, etc.).</li>
  </ol>
</body>
</html>

