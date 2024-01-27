#include <Arduino_JSON.h>
#include <LEAmDNS.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "secrets.h"

const char *ssid = STASSID;
const char *password = STAPSK;

extern WebServer server(80);

const String htmlContent =
    "<!DOCTYPE html>"
    "<html lang=\"en\">"
    ""
    "<head>"
    "    <meta charset=\"UTF-8\">"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
    "    <script src=\"https://cdn.tailwindcss.com\"></script>"
    "    <title>Conway's Game of Life</title>"
    "    <style>"
    "        body {"
    "            display: flex;"
    "            flex-direction: column;"
    "            justify-content: center;"
    "            align-items: center;"
    "            height: 100vh;"
    "            margin: 0;"
    "        }"
    ""
    "        .grid-container {"
    "            display: grid;"
    "            grid-template-columns: repeat(32, 20px);"
    "        }"
    ""
    "        .cell {"
    "            width: 20px;"
    "            height: 20px;"
    "            border: 1px solid #ccc;"
    "            background-color: white;"
    "        }"
    ""
    "        .buttons {"
    "            margin-top: 20px;"
    "        }"
    "    </style>"
    "</head>"
    ""
    "<body>"
    "    <div class=\"grid-container\" id=\"grid-container\"></div>"
    "    <div class=\"buttons\">"
    "        <button class=\"bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded-full\" onclick=\"startSimulation()\">Start Simulation</button>"
    "        <button class=\"bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded-full\" onclick=\"resetSimulation()\">Reset Simulation</button>"
    "    </div>"
    "    <script>"
    "        function initMatrix() {"
    "            const gridContainer = document.getElementById('grid-container');"
    "            for (let i = 0; i < 32; i++) {"
    "                for (let j = 0; j < 32; j++) {"
    "                    const cell = document.createElement('div');"
    "                    cell.classList.add('cell');"
    "                    cell.addEventListener('click', toggleCell);"
    "                    gridContainer.appendChild(cell);"
    "                }"
    "            }"
    "        }"
    ""
    "        function toggleCell(event) {"
    "            const cell = event.target;"
    "            cell.style.backgroundColor = cell.style.backgroundColor === 'red' ? 'white' : 'red';"
    "        }"
    ""
    "        function encodeMatrix() {"
    "            const cells = document.querySelectorAll('.cell');"
    "            let bitString = '';"
    "            let hexString = '';"
    ""
    "            cells.forEach(cell => {"
    "                const isAlive = cell.style.backgroundColor === 'red';"
    "                bitString += isAlive ? '1' : '0';"
    "            });"
    "            while (bitString.length > 0) {"
    "                const hex = parseInt(bitString.substring(0, 4), 2).toString(16);"
    "                bitString = bitString.substring(4);"
    "                hexString += hex;"
    "            }"
    "            console.log('Encoded Grid Hex:', hexString);"
    "            return hexString;"
    "        }"
    ""
    "        function postMatrixState(hexString) {"
    "            fetch('/send/', {"
    "                method: 'POST',"
    "                headers: {"
    "                    'Content-Type': 'application/json',"
    "                },"
    "                body: JSON.stringify({ gridState: hexString }),"
    "            })"
    "                .then(response => response.json())"
    "                .then(data => {"
    "                    console.log('Server Response:', data);"
    "                })"
    "                .catch(error => {"
    "                    console.error('Error:', error);"
    "                });"
    "        }"
    ""
    "        function resetMatrixState() {"
    "            fetch('/reset/', {"
    "                method: 'POST',"
    "                headers: {"
    "                    'Content-Type': 'application/json',"
    "                },"
    "                body: 'Client request to reset.',"
    "            })"
    "                .then(response => response.json())"
    "                .then(data => {"
    "                    console.log('Server Response:', data);"
    "                })"
    "                .catch(error => {"
    "                    console.error('Error:', error);"
    "                });"
    "        }"
    ""
    "        function startSimulation() {"
    "            const hexString = encodeMatrix();"
    "            postMatrixState(hexString);"
    "            console.log('Simulation started');"
    "        }"
    ""
    "        function resetSimulation() {"
    "            const cells = document.querySelectorAll('.cell');"
    "            cells.forEach(cell => {"
    "                cell.style.backgroundColor = 'white';"
    "            });"
    "            resetMatrixState();"
    "            console.log('Simulation reset');"
    "        }"
    ""
    "        initMatrix();"
    "    </script>"
    ""
    "</body>"
    ""
    "</html>";

void handleRoot() {
    digitalWrite(LED_BUILTIN, HIGH);
    server.send(200, "text/html", htmlContent);
    digitalWrite(LED_BUILTIN, LOW);
}

void handleSend() {
    if (server.method() != HTTP_POST) {
        digitalWrite(LED_BUILTIN, HIGH);
        server.send(405, "text/plain", "Method Not Allowed");
        digitalWrite(LED_BUILTIN, LOW);
    }
    else {
        digitalWrite(LED_BUILTIN, HIGH);
        String str_payload = server.arg(0);
        JSONVar payload = JSON.parse(str_payload);
        String gridState = payload["gridState"];
        Serial.println(gridState);
        server.send(200, "text/plain", gridState);

        startSimulation(gridState);
        digitalWrite(LED_BUILTIN, LOW);
    }
}

void handleReset() {
    if (server.method() != HTTP_POST) {
        digitalWrite(LED_BUILTIN, HIGH);
        server.send(405, "text/plain", "Method Not Allowed");
        digitalWrite(LED_BUILTIN, LOW);
    }
    else {
        digitalWrite(LED_BUILTIN, HIGH);
        String message = server.arg(0);
        Serial.println(message);
        server.send(200, "text/plain", message);

        resetSimulation();
        digitalWrite(LED_BUILTIN, LOW);
    }
}

void handleNotFound() {
    digitalWrite(LED_BUILTIN, HIGH);
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
    digitalWrite(LED_BUILTIN, LOW);
}

void setupHttpServer() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("picow")) {
        Serial.println("MDNS responder started");
    }

    // routing
    server.on("/", handleRoot);
    server.on("/send/", handleSend);
    server.on("/reset/", handleReset);
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}