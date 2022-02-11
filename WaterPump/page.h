#ifndef __PAGE_H__
#define __PAGE_H__

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
                border-radius: 40px
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
                border-radius: 30px
            }
            input:checked+.slider
            {
                background-color: green
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
                border-radius: 30px;
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
                border-radius: 30px;
            }
            .dropdownSelector
            {
                border-radius: 30px;
                height: 70px;
                font-size: 30px;
                border-width: 5px;
                border-color: black;
                background: white;
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
                setInterval(getData, 1000);
                setInterval(connection, 2000);

                var counter = 0;
                var counterOld = 0;

                function connection()
                {
                    if(counter == counterOld)
                    {
                        document.getElementById("connection").innerHTML = "Pripojenie: NEAKTIVNE";
                        document.getElementById("connection").style.color = "red";
                    }
                    else
                    {
                        document.getElementById("connection").innerHTML = "Pripojenie: AKTIVNE";
                        document.getElementById("connection").style.color = "green";
                        counterOld = counter;
                    }
                }
                
                function getData()
                {
                    var xhr = new XMLHttpRequest();
                    xhr.onreadystatechange = function()
                    {
                        if (this.readyState == 4 && this.status == 200)
                        {
                            var message = this.responseText.split("/");
                            
                            //  perioda polievania
                            document.getElementById("period").value = message[0];
                            
                            //  aktualny cas
                            document.getElementById("currTime").innerHTML = message[1];
                            
                            //  tlacitko jednorazoveho spustania polievania
                            if(message[2] === '1')
                                document.getElementById("toggleButton").innerHTML = "<a onclick=\"togglePumping()\"><button class=\"buttonRunNow\" style=\"background: red \">STOP</button></a>";
                            else
                                document.getElementById("toggleButton").innerHTML = "<a onclick=\"togglePumping()\"><button class=\"buttonRunNow\" style=\"background: green \">START</button></a>";

                            //  toggle pravidelne polievanie
                            var isTrueSet = (message[3] === '1');
                            document.getElementById("regularPump").checked = isTrueSet;
                            document.getElementById("period").disabled = !isTrueSet;
                            document.getElementById("pumpHour").disabled = !isTrueSet;
                            document.getElementById("pumpMinute").disabled = !isTrueSet;
                            document.getElementById("pumpIntervalReg").disabled = !isTrueSet;
                            if(isTrueSet)
                            {
                                document.getElementById("periodLabel").style.color = "black";
                                document.getElementById("periodLabelReg").style.color = "black";
                                document.getElementById("beginLabel").style.color = "black";
                            }
                            else
                            {
                                document.getElementById("periodLabel").style.color = "lightgrey";
                                document.getElementById("periodLabelReg").style.color = "lightgrey";
                                document.getElementById("beginLabel").style.color = "lightgrey";
                            }

                            //  interval jednorazoveho pumpovania
                            document.getElementById("pumpInterval").value = message[4];

                            //  interval pravidelneho pumpovania
                            document.getElementById("pumpIntervalReg").value = message[5];


                            //  pociatocny cas polievania hodina
                            document.getElementById("pumpHour").value = message[6];

                            //  pociatocny cas polievania minuta
                            document.getElementById("pumpMinute").value = message[7];

                            //  pripojenie na ESP
                            counter = message[8];

                            //  stav cerpadla
                            if(message[9] === '1')
                            {
                                document.getElementById("statusInfo").style.color = "red";
                                document.getElementById("statusInfo").innerHTML = "Cerpadlo teraz CERPA vodu";
                            }
                            else
                            {
                                document.getElementById("statusInfo").style.color = "black";
                                document.getElementById("statusInfo").innerHTML = "Cerpadlo teraz NECERPA vodu";
                            }
                        }
                    };
                    xhr.open("GET", "/data", true);
                    xhr.send();
                }
                
                function sendTime(form)
                {
                    var dt = new Date();
                    var xhr = new XMLHttpRequest();
                    xhr.open("GET", "/setTime?h=" + dt.getHours().toString() + "&m="+dt.getMinutes().toString() + "&s="+dt.getSeconds().toString(), true);
                    xhr.send();
                }
               
               
                function togglePumping()
                {
                    var xhr = new XMLHttpRequest();
                    xhr.open("GET", "/toggleButton", true);
                    xhr.send();
                }

                function runPumpInterval()
                {
                    var xhr = new XMLHttpRequest();
                    var timeVal = document.getElementById("pumpInterval").value;
                    xhr.open("GET", "/runPumpInterval?t=" + timeVal.toString(), true);
                    xhr.send();
                }

                function runPumpIntervalRegular()
                {
                    var xhr = new XMLHttpRequest();
                    var timeVal = document.getElementById("pumpIntervalReg").value;
                    xhr.open("GET", "/runPumpIntervalReg?t=" + timeVal.toString(), true);
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
                
                function runPumpPeriod()
                {
                    var xhr = new XMLHttpRequest();
                    var timeVal = document.getElementById("period").value;
                    xhr.open("GET", "/runPumpPeriod?p=" + timeVal.toString(), true);
                    xhr.send();
                }

                function pumpHour()
                {
                    var xhr = new XMLHttpRequest();
                    var hour = document.getElementById("pumpHour").value;
                    xhr.open("GET", "/pumpHour?h=" + hour.toString(), true);
                    xhr.send();
                }
                
                function pumpMinute()
                {
                    var xhr = new XMLHttpRequest();
                    var minute = document.getElementById("pumpMinute").value;
                    xhr.open("GET", "/pumpMinute?m=" + minute.toString(), true);
                    xhr.send();
                }
                
            </script>
    </head>
    
    <body>
        <div>
            <h3 id="connection">Pripojenie</h3>
        </div>

        <div>
            <h3>Aktualny cas v riadiacej jednotke<h3>
            <h3 id="currTime">0:0:0</h3>
        </div>

        <div>
            <a onclick="sendTime()"><button class="buttonSyc">Synchronizovat cas</button></a>
        </div>
        
        <p></p>
        <hr>

        <div>
            <h3 id="statusInfo"> </h3>
        </div>

        <hr>
        <p></p>
        
        <div>
            <h3>Jednorazove polievanie<h3>
        </div>
        
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
                </select>
            </div>
        </div>

        <p></p>
        <hr>
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

        <div class="container-grid2">
            <div class="col col-1">
                <h3 id="periodLabelReg">Dlzka polievania:<h3>
            </div>
            <div class="col col-2">
                <select id="pumpIntervalReg" onchange="runPumpIntervalRegular()" class="dropdownSelector">
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
                </select>
            </div>
        </div>
        
        <div>
            <h3 id="beginLabel">Zaciatok polievania:<h3>
        </div>
        <div class="container-grid2">
            <div class="col col-1">
                <select id="pumpHour" onchange="pumpHour()" class="dropdownSelector">
                     <option value="0">0 hod</option>
                     <option value="1">1 hod</option>
                     <option value="2">2 hod</option>
                     <option value="3">3 hod</option>
                     <option value="4">4 hod</option>
                     <option value="5">5 hod</option>
                     <option value="6">6 hod</option>
                     <option value="7">7 hod</option>
                     <option value="8">8 hod</option>
                     <option value="9">9 hod</option>
                     <option value="10">10 hod</option>
                     <option value="11">11 hod</option>
                     <option value="12">12 hod</option>
                     <option value="13">13 hod</option>
                     <option value="14">14 hod</option>
                     <option value="15">15 hod</option>
                     <option value="16">16 hod</option>
                     <option value="17">17 hod</option>
                     <option value="18">18 hod</option>
                     <option value="19">19 hod</option>
                     <option value="20">20 hod</option>
                     <option value="21">21 hod</option>
                     <option value="22">22 hod</option>
                     <option value="23">23 hod</option>

                </select>
            </div>
            <div class="col col-2">
                <select id="pumpMinute" onchange="pumpMinute()" class="dropdownSelector">
                     <option value="0">00 min</option>
                     <option value="10">10 min</option>
                     <option value="20">20 min</option>
                     <option value="30">30 min</option>
                     <option value="40">40 min</option>
                     <option value="50">50 min</option>
                </select>
            </div>
        </div>
    </body>
</html>
)rawliteral";


#endif /*__PAGE_H__*/
