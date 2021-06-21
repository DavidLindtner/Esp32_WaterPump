#ifndef __PAGE_H__
#define __PAGE_H__

#include"variable.h"

const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML><html>
    <head>
        <title>Zahradna_Pumpa</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="icon" href="data:,">
        <style>
            html {font-family: Arial; display: inline-block; text-align: center;}
            h2 {font-size: 3.0rem;}
            p {font-size: 3.0rem;}
            body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
            .switch
            {
                position: relative;
                display: inline-block;
                width: 120px;
                height: 68px
            } 
            .switch input
            {
                display: none
            }
            .slider
            {
                position: absolute;
                top: 0;
                left: 0;
                right: 0;
                bottom: 0;
                background-color: grey;
                border-radius: 6px
            }
            .slider:before
            {
                position: absolute;
                content: "";
                height: 52px;
                width: 52px;
                left: 8px;
                bottom: 8px; 
                background-color: white;
                -webkit-transition: .4s;
                transition: .4s;
                border-radius: 3px
            }
            input:checked+.slider
            {
                background-color: #2196F3
            }
            input:checked+.slider:before
            {
                -webkit-transform: translateX(52px);
                -ms-transform: translateX(52px);
                transform: translateX(52px)
            }
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
            .dropdownSelector
            {
                border-radius: 10px;
                height: 70px;
                font-size: 30px;
                border-width: 5px;
                border-color: black;
            }
            .container-grid
            {
                display: grid;
                grid-template-columns: 60% 40%;
            }
            .col-1
            {
                padding: 10px;
                text-align: end;
            }
            .col-2
            {
                padding: 10px;
                text-align: start;
            }

            .container-grid2
            {
                display: grid;
                grid-template-columns: 50% 50%;
            }

        </style>
            <script>
                setInterval(getPumpInterval, 1000);
                setInterval(getButtonColor, 1000);
                setInterval(getCurrTime, 1000);
                setInterval(getRegularPumping, 1000);
                setInterval(getPeriod, 1000);


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
                
                function regularPumping(element)
                {
                    var xhr = new XMLHttpRequest();
                    if(element.checked)
                    { 
                        xhr.open("GET", "/regularPumping?r=1", true);
                    }
                    else
                    {
                        xhr.open("GET", "/regularPumping?r=0", true);
                    }
                    xhr.send();
                }
                
                function getRegularPumping()
                {
                    var xhr = new XMLHttpRequest();
                    xhr.onreadystatechange = function()
                    {
                        if (this.readyState == 4 && this.status == 200)
                        {
                            var isTrueSet = (this.responseText === 'true');
                            document.getElementById("regularPump").checked = isTrueSet;
                            document.getElementById("period").disabled = !isTrueSet;
                            if(isTrueSet)
                            {
                                document.getElementById("periodLabel").style.color = "black"
                            }
                            else
                            {
                                document.getElementById("periodLabel").style.color = "lightgrey"
                            }
                        }
                    };
                    xhr.open("GET", "/regularPumpingGet", true);
                    xhr.send();
                }

                function runPumpPeriod()
                {
                    var xhr = new XMLHttpRequest();
                    var timeVal = document.getElementById("period").value;
                    xhr.open("GET", "/runPumpPeriod?p=" + timeVal.toString(), true);
                    xhr.send();
                }
                
                function getPeriod()
                {
                    var xhr = new XMLHttpRequest();
                    xhr.onreadystatechange = function()
                    {
                        if (this.readyState == 4 && this.status == 200)
                        {
                            document.getElementById("period").value = this.responseText;
                        }
                    };
                    xhr.open("GET", "/periodGet", true);
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
        
        <p></p>
        
        <div class="container-grid">
            <div class="col col-1">
                <a onclick="togglePumping()" id="toggleButton"><button class="buttonRunNow" style="background: green">START</button></a>
            </div>
            <div class="col col-2">
                <select id="pumpInterval" onchange="runPumpInterval()" class="dropdownSelector">
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
            </div>
        </div>

        <p></p>

        <div class="container-grid2">
            <div class="col col-1">
                <h3>Spustit pravidelne polievanie<h3>
            </div>
            <div class="col col-2">
                <label class="switch" ><input type="checkbox" onchange="regularPumping(this)" id="regularPump" checked>
                    <span class="slider">
                    </span>
                </label>
            </div>
        </div>

        
        <div class="container-grid2">
            <div class="col col-1">
                <h3 id="periodLabel">Perioda polievania:<h3>
            </div>
            <div class="col col-2">
                <select id="period" onchange="runPumpPeriod()" class="dropdownSelector">
                     <option value="1">12 hod</option>
                     <option value="2">1 den</option>
                     <option value="3">2 dni</option>
                     <option value="4">3 dni</option>
                     <option value="5">4 dni</option>
                     <option value="6">5 dni</option>
                     <option value="7">6 dni</option>
                     <option value="8">1 tyzden</option>
                </select>
            </div>
        </div>

    </body>
</html>
)rawliteral";


#endif /*__PAGE_H__*/
