
document.addEventListener("DOMContentLoaded", function () {
    const apiUrl = "http://192.168.1.106/data";  // Replace with ESP's actual IP
    const updateInterval = 500; // Update every 500ms
    let angle1 = 0;
    let angle2 = 0;
    let isOn = true;
    let old_angle1 = 0;
    let old_angle2 = 0;
    let old_isOn = true;
    // **Elements for sensor data**
    const gasStatus = document.getElementById("gas-status");
    const flameStatus = document.getElementById("flame-status");
    const rainStatus = document.getElementById("rain-status");
    const tempValue = document.getElementById("temp-value");

    // **Elements for control**
    const relayButton = document.getElementById("relay-toggle");
    const doorToggle = document.getElementById("toggleDoor");   // Door toggle switch
    const windowToggle = document.getElementById("toggleWindow"); // Window toggle switch

    // **Fetch and update data function**
    function fetchData() {
        fetch(apiUrl)
            .then(response => response.json())
            .then(data => {
                console.log("Received Data:", data);
                // **Update sensor text based on values**
                gasStatus.textContent = data.gas ? "🔴" : "🟢";
                flameStatus.textContent = data.flame ? "🔴" : "🟢";
                rainStatus.textContent = data.rain ? "🔴" : "🟢";
                tempValue.textContent = `${data.temperature} °C`;

                // **Update relay button text**
                relayButton.textContent = data.relay ? "Turn Gas OFF" : "Turn Gas ON";
                relayButton.classList.toggle("active", data.relay);

                // **Update toggle switches for door & window**
                // doorToggle.checked = data.servo1 === 90;   // If 90°, door switch ON
                // windowToggle.checked = data.servo2 === 90; // If 90°, window switch ON
            })
            .catch(error => console.error("Error fetching data:", error));
    }

    // **Send control command function**
    function sendCommand(endpoint, params) {
        fetch(`http://192.168.1.106/update`, {
            method: "POST",
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
            body: new URLSearchParams(params)
        })
            .then(response => response.text())
            .then(result => console.log("Response:", result))
            .catch(error => console.error("Error sending command:", error));
    }
    function sendControl() {

        if (old_angle1 != angle1 || old_angle2 != angle2 || old_isOn != isOn) {
            // console.log(angle1);
            // console.log(angle2);
            // console.log(isOn);
            sendCommand("update", {
                servo1: angle1,   // angle for servo1
                servo2: angle2,    // angle for servo2
                state: isOn ? 0 : 1 
            });
            old_angle1 = angle1;
            old_angle2 = angle2;
            old_isOn = isOn;
        }

    }

    // **Event listener for toggle switches**
    doorToggle.addEventListener("change", function () {
        angle1 = doorToggle.checked ? 90 : 0; // ON → 90°, OFF → 0°
        // sendCommand("update", { servo: 1, angle: angle }); // Send to ESP (servo 1 → Door)
    });

    windowToggle.addEventListener("change", function () {
        angle2 = windowToggle.checked ? 90 : 0; // ON → 90°, OFF → 0°
        // sendCommand("update", { servo: 2, angle: angle }); // Send to ESP (servo 2 → Window)
    });

    relayButton.addEventListener("click", () => {
        isOn = !isOn;
    });

    // **Auto-update data every 500ms**
    setInterval(fetchData, updateInterval);
    setInterval(sendControl, updateInterval);
    // **Initial data fetch**
    fetchData();
    sendControl();
});
