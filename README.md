# Medi-Box
The Smart MediBox is an intelligent device designed to ensure timely medication intake by providing audible and visual reminders.
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    
</head>
<body>

<h1>Smart MediBox</h1>

<h2>Project Description</h2>
<p>The Smart MediBox is an intelligent and interactive device designed to help users manage their medication schedule efficiently. It provides timely reminders to take medications, monitors environmental conditions, and integrates with a web-based dashboard for remote management and control. By combining audio and visual alerts with IoT capabilities, the Smart MediBox ensures that users never miss a dose and that their medications are stored in optimal conditions.</p>

<h2>Features</h2>
<ul>
    <li><strong>Medication Reminders:</strong> Rings an alarm at scheduled times to remind users to take their medication.</li>
    <li><strong>Environmental Monitoring:</strong> Uses a DHT11 sensor to monitor temperature and humidity levels inside the MediBox, ringing an alarm if the conditions are outside the recommended range.</li>
    <li><strong>Display Information:</strong> Shows the current time, date, and alarm information on an OLED display.</li>
    <li><strong>User Interaction:</strong> Allows users to set the time, date, and alarms via push buttons on startup.</li>
    <li><strong>Remote Management:</strong> Provides a web-based dashboard for monitoring and controlling the MediBox settings over Wi-Fi.</li>
</ul>

<h2>Components</h2>
<ul>
    <li><strong>Microcontroller:</strong> ESP32</li>
    <li><strong>Sensors:</strong> DHT11 for temperature and humidity</li>
    <li><strong>Display:</strong> OLED display for visual feedback</li>
    <li><strong>Audio Alert:</strong> Buzzer for sound notifications</li>
    <li><strong>Indicators:</strong> LED bulbs for visual alerts</li>
    <li><strong>Input Controls:</strong> Push buttons for setting time, date, and alarms</li>
    <li><strong>Connectivity:</strong> Wi-Fi for communication with the web-based dashboard</li>
</ul>

<h2>How It Works</h2>
<ol>
    <li><strong>Initialization:</strong> On startup, the user sets the current time, date, and medication alarm times using the push buttons.</li>
    <li><strong>Monitoring:</strong> The MediBox continuously monitors the internal temperature and humidity using the DHT11 sensor.</li>
    <li><strong>Alerts:</strong> 
        <ul>
            <li>Rings an alarm to remind the user when it's time to take their medication.</li>
            <li>Rings an alarm if the temperature or humidity goes outside the recommended range to ensure proper medication storage conditions.</li>
        </ul>
    </li>
    <li><strong>Display:</strong> The OLED display shows the current time, date, and upcoming alarm information.</li>
    <li><strong>Remote Access:</strong> The device connects to a Wi-Fi network, allowing users to monitor and control the MediBox through a web-based dashboard.</li>
</ol>

<h2>Usage</h2>
<ol>
    <li><strong>Setup:</strong> Connect the MediBox to a power source and configure the initial settings using the push buttons.</li>
    <li><strong>Operation:</strong> The device will automatically provide reminders and monitor environmental conditions.</li>
    <li><strong>Web Dashboard:</strong> Access the web dashboard from any internet-enabled device to manage settings and view alerts.</li>
</ol>


</body>
</html>
