#ifndef __PAGE_H__
#define __PAGE_H__

#include"variable.h"

const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML><html>
    <head>
        <title>ESP32 AccessPoint</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="icon" href="data:,">
        <style>
            html {font-family: Arial; display: inline-block; text-align: center;}
            h2 {font-size: 3.0rem;}
            p {font-size: 3.0rem;}
            body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
            .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
            .switch input {display: none}
            .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
            .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
            input:checked+.slider {background-color: #b30000}
            input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
            .buttonSyc
            {
                background-color: green;
                border: none;
                color: white;
                padding: 16px 40px;
                text-decoration: none;
                font-size: 30px;
                margin: 2px;
                cursor: pointer;
                border-radius: 10px;
            }
            .buttonRunNow
            {
                border: none;
                color: white;
                padding: 16px 40px;
                text-decoration: none;
                font-size: 30px;
                margin: 2px;
                cursor: pointer;
                border-radius: 10px;
            }
        </style>
            <script>
                setInterval(getPumpInterval, 1000);
                setInterval(getButtonColor, 1000);
                setInterval(getCurrTime, 1000);


                function sendTime(form)
                {
                    var dt = new Date();
                    var xhr = new XMLHttpRequest();
                    xhr.open("GET", "/setTime?h=" + dt.getHours().toString() + "&m="+dt.getMinutes().toString() + "&s="+dt.getSeconds().toString(), true);
                    xhr.send();
                }
               
                function getCurrTime()
                {
                    var xhr = new XMLHttpRequest();
                    xhr.onreadystatechange = function()
                    {
                        if (this.readyState == 4 && this.status == 200)
                        {
                            document.getElementById("currTime").innerHTML = this.responseText;
                        }
                    };
                    xhr.open("GET", "/currTime", true);
                    xhr.send();
                }
                
                function togglePumping()
                {
                    var xhr = new XMLHttpRequest();
                    xhr.open("GET", "/toggleButton", true);
                    xhr.send();
                    getButtonColor();
                }

                function getButtonColor()
                {
                    var xhr = new XMLHttpRequest();
                    xhr.onreadystatechange = function()
                    {
                        if (this.readyState == 4 && this.status == 200)
                        {
                            document.getElementById("toggleButton").innerHTML = this.responseText;
                        }
                    };
                    xhr.open("GET", "/toggleButtonGet", true);
                    xhr.send();
                }

                function runPumpInterval()
                {
                    var xhr = new XMLHttpRequest();
                    var timeVal = document.getElementById("pumpInterval").value;
                    xhr.open("GET", "/runPumpInterval?t=" + timeVal.toString(), true);
                    xhr.send();
                    getPumpInterval();
                }

                function getPumpInterval()
                {
                    var xhr = new XMLHttpRequest();
                    xhr.onreadystatechange = function()
                    {
                        if (this.readyState == 4 && this.status == 200)
                        {
                            document.getElementById("pumpInterval").value = this.responseText;
                        }
                    };
                    xhr.open("GET", "/runPumpIntervalGet", true);
                    xhr.send();
                }
                
            </script>
    </head>
    
    <body>
        <div>
            <h3>Aktualny cas v riadiacej jednotke<h3>
            <h3 id="currTime">0:0:0</h3>
        </div>

        <div>
            <a onclick="sendTime()"><button class="buttonSyc">Synchronizovat cas</button></a>
        </div>

        <div id="toggleButton">
            <a onclick="togglePumping()"><button class="buttonRunNow" style="background: green">START</button></a>
        </div>

        <select id="pumpInterval" onchange="runPumpInterval()">
             <option value="1">1 min</option>
             <option value="2">2 min</option>
             <option value="3">3 min</option>
             <option value="4">4 min</option>
             <option value="5">5 min</option>
             <option value="6">6 min</option>
             <option value="7">7 min</option>
             <option value="8">8 min</option>
             <option value="9">9 min</option>
             <option value="10">10 min</option>
             <option value="15">15 min</option>
             <option value="20">20 min</option>
        </select>

    </body>
</html>
)rawliteral";


#endif /*__PAGE_H__*/
