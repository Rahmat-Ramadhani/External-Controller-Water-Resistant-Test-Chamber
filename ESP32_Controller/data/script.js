
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
//var x = document.getElementById("controller-content");


document.addEventListener("DOMContentLoaded", function() {
    selectScreen('release');
    
});



// Init web socket when the page loads
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
    
}

function getData(){
    websocket.send("getData");
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

// When websocket is established, call the getReadings() function
function onOpen(event) {
    console.log('Connection opened');
    getData();
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}
var dataContainer;
var allDataProgram = {};

// Function that receives the message from the ESP32 with the readings
function onMessage(event) {
    console.log(event.data);
    dataContainer = JSON.parse(event.data);
    var keys = Object.keys(dataContainer);
    runningIdTest = dataContainer.running.currentIdTest;

    

    if(dataContainer.program.dataProgram.idProgram != null){
        allDataProgram[dataContainer.program.dataProgram.idProgram] = {};
        allDataProgram[dataContainer.program.dataProgram.idProgram] = dataContainer.program.dataProgram.test;
        console.log("data all program : " + JSON.stringify(allDataProgram));
    }
    
    if (runningIdTest != "") {
        run = true;
        idRunProgram = dataContainer.running.idProgram;
        timerCounter('down', dataContainer.running.remainingTime, "total-remaining");
        timerCounter('up', dataContainer.running.countingTimeCurrentTest, "current-test");
        if(currentScreen == 'CommandScreen' && clickedProgramId == idRunProgram) {
            currentScreen = 'runningCommandScreen';
        }
    } else {
        idRunProgram = "";
        run = false;
        if(currentScreen == 'runningCommandScreen') {
            currentScreen = 'CommandScreen';
        }
    }
    console.log("id run program : " + idRunProgram);
    pause = dataContainer.running.pause;
    console.log("pause : " + pause);
    showController();
    flowMeterStatus();
    deleteAllList();
    console.log("current screen : " + currentScreen);
    selectScreen(currentScreen);
    // switch(currentScreen) {
    //     case 'ProgramScreen':
    //         addProgramToScreen();
    //         break;
    //     case 'CommandScreen':
    //         addCommandToScreen("command-list", "label-command-list");
    //         break;
    //     case 'runningCommandScreen':
    //         addCommandToScreen("running-command-list", "label-running-command-list");
    //         break;
    //     default :
    // }


    

}

var a = true;
var runningIdTest = '';

function showController() {
    var y = document.getElementById("Take-over-description");
    var z = document.getElementById("Take-over-button");
    
    if (dataContainer.takeOver == true && a == true) {
        a = false;
        
        y.textContent = "Press Button to Release The Controller";
        z.textContent = "Release";
        z.style.backgroundColor = 'blue';
        currentScreen = 'ProgramScreen';
        document.getElementById('programToHistory-button').textContent = 'History';
        document.getElementById('programToHistory-button').style.backgroundColor = '#776B5D';
        setClock();
        //selectScreen('ProgramScreen');
    } else if (dataContainer.takeOver == false) {
        a = true;
        
        y.textContent = "Press Button to Take Over The Controller";
        z.textContent = "Take Over";
        z.style.backgroundColor = '#FF0000';
        currentScreen = 'release';
        //selectScreen('release');
    }

    ipx5status = "off";
    ipx6status = "off";
    flowMeterStatus();
}

function takeOverClick() {
    if (dataContainer.takeOver == true) {
        package = {};
        package = {
            takeOver : false,
            ipx5Status : "off",
            ipx6Status : "off"
        };
    } else {
        package = {};
        package = {takeOver : true};
    }
    sendPackage(package);
}

function sendPackage(object) {
    var wrapPackage = JSON.stringify(object);
    websocket.send(wrapPackage);
    console.log("send : " + wrapPackage);
}

//==============================Flow Meter Status=============================
var ipx5status = "off";
var ipx6status = "off";

function ipx5click() {
    if (dataContainer.ipx5Status == "off") {
        package = {};
        package = {ipx5Status : "on"};
    }else{
        package = {};
        package = {ipx5Status : "off"};
    }

    sendPackage(package);
}

function ipx6click() {
    if (dataContainer.ipx6Status == "off") {
        package = {};
        package = {ipx6Status : "on"};
    }else{
        package = {};
        package = {ipx6Status : "off"};
    }

    sendPackage(package);
}

function flowMeterStatus() {
    var fm = document.getElementById("flow-meter")

    if (dataContainer.ipx5Status === "on" && dataContainer.ipx6Status === "on") {
        fm.src = "ON.jpg";
    } else if (dataContainer.ipx5Status === "off" && dataContainer.ipx6Status === "off") {
        fm.src = "OFF.jpg" 
    } else if (dataContainer.ipx5Status === "on" && dataContainer.ipx6Status === "off") {
        fm.src = "IPX5ON.jpg"
    } else if (dataContainer.ipx5Status === "off" && dataContainer.ipx6Status === "on") {
        fm.src = "IPX6ON.jpg"
    }

}

//============================Get Value From Input=======================


var generatedID;
var numberID = 0;


var package = {};

var inputHour;
var inputMinute;
var inputSecond;
var selectedTest;

var editFlag = false;
var idParent;
var idEditButton;

var prevBGColor;

function callBack() {
    inputHour =  document.getElementById("input-hour").value;
    inputMinute =  document.getElementById("input-minute").value;
    inputSecond =  document.getElementById("input-second").value;
    selectedTest = document.getElementById("ipx").value;
    

    if (inputHour === ""){
        inputHour = 0;
    }

    if (inputMinute === ""){
        inputMinute = 0;
    }

    if (inputSecond === ""){
        inputSecond = 0;
    }
}


var myProgramlist = document.getElementsByTagName("LI");
var close = document.getElementsByClassName("close");
var edit = document.getElementsByClassName("edit");




function addCommand() {
    callBack();
    var time = adjustTimeValue('input');

    // alert("realMinute : " + realMinute);
    // alert("realSecond : " + realSecond);

    if (selectedTest == 'none') {
        alert("Please Select Type Test!");
    } else if (inputHour === 0 && inputMinute === 0 && inputSecond === 0) {
        alert("Please Enter Time Value!");
    } else {
        if (!editFlag) {
            package = {};
            package = {
                newTest : {
                    idProgram : clickedProgramId,
                    test : { 
                        ipx : selectedTest,
                        hour : time.hour,
                        minute : time.minute,
                        second : time.second,
                        total : time.total
                    }
                }
            }
        } else {
            package = {};
            package = {
                editTest : {
                    idProgram : clickedProgramId,
                    idTest : idParent,
                    test : { 
                        ipx : selectedTest,
                        hour : time.hour,
                        minute : time.minute,
                        second : time.second,
                        total : time.total
                    }
                }
            }
            editScreenValue("add-button", "Add")
        }

        sendPackage(package);
    }

    document.getElementById("input-hour").value="";
    document.getElementById("input-minute").value="";
    document.getElementById("input-second").value="";
    document.getElementById("ipx").value="none";
    //alert(dataContainer.test[generatedID][hour);
}

function createProgram() {
    var name = document.getElementById("input-name").value;

    if(name == ""){
        alert("Please Enter a Program Name!");
    } else {
        if (!editFlag) {
            package = {};
            package = {
                program : {
                    idProgram : {
                        programName : name
                    }
                }
            };
        } else {
            package = {};
            package.program = {};
            package.program[idParent] = {}
            package.program[idParent].programName = name;
            editScreenValue("create-button", "Create");
        }
    
        sendPackage(package);
        document.getElementById("input-name").value = "";
    }
}

function displayTime(idTime) {
    displayHour = dataContainer.test[idTime].hour;
    displayMinute = dataContainer.test[idTime].minute;
    displaySecond = dataContainer.test[idTime].second;

    if (displayHour < 10) {
        displayHour = "0" + displayHour;
    }

    if (displayMinute < 10) {
        displayMinute = "0" + displayMinute;
    }

    if (displaySecond < 10) {
        displaySecond = "0" + displaySecond;
    }

}



function list(idList, idSection, idButton, className, content){
    const para = document.createElement("li");
    para.id = idList; // key
    para.className = className;

    var text = document.createTextNode(content); // isi list nya

    para.appendChild(text);

    if (className == 'liProgram') {
        if (idList != idRunProgram) {
            var spanClose = document.createElement("SPAN");
            var txtCross = document.createTextNode("\u00D7");
            spanClose.className = "close" ;
            spanClose.id = "close" + idList;
            spanClose.appendChild(txtCross);
            para.appendChild(spanClose);
    
            var spanEdit = document.createElement("SPAN");
            spanEdit.id = "edit" + idList;
            spanEdit.innerHTML = '\u270E';
            spanEdit.className = "edit";
            //spanEdit.appendChild(txtPencil);
            para.appendChild(spanEdit);
        }
    } else {
        if (clickedProgramId != idRunProgram && className != 'liListHistory' && className != 'liHistory' && className != 'liLog' && className != 'liHistoryDate') {
            var spanClose = document.createElement("SPAN");
            var txtCross = document.createTextNode("\u00D7");
            spanClose.className = "close";
            spanClose.id = "close" + idList;
            spanClose.appendChild(txtCross);
            para.appendChild(spanClose);
    
            var spanEdit = document.createElement("SPAN");
            spanEdit.id = "edit" + idList;
            spanEdit.innerHTML = '\u270E';
            spanEdit.className = "edit";
            //spanEdit.appendChild(txtPencil);
            para.appendChild(spanEdit);
        }
    }

    if (className == 'liHistory' || className == 'liListHistory') {
    var spanClose = document.createElement("SPAN");
    var txtCross = document.createTextNode("\u00D7");
    spanClose.className = "close";
    spanClose.id = "close" + idList;
    spanClose.appendChild(txtCross);
    para.appendChild(spanClose);
    }

    

    document.getElementById(idSection).appendChild(para);
    //alert(generatedID);

    for (i = 0; i < close.length; i++) {
        close[i].onclick = function() {
            var div = this.parentElement;
            idParent = div.id;
            

            var deleteConfirm = window.confirm("Do you want to delete this?")

            if(deleteConfirm){
                package = {};
                switch (currentScreen) {
                    case 'listHistoryScreen' :
                        package = {
                            delete : {
                                category : "history",
                                type : date,
                                id : idParent
                            }
                        }
                        break;
                    case 'dateHistoryScreen' :
                        package = {
                            delete : {
                                category : "history",
                                type : "dates",
                                id : idParent
                            }
                        }
                        break;
                    case 'ProgramScreen':
                        package = {
                            delete : {
                                category : "program",
                                type : "idProgramList",
                                id : idParent
                            }
                        }
                        break;
                    case 'CommandScreen':
                        package = {
                            delete : {
                                category : "program",
                                type : clickedProgramId,
                                id : idParent
                            }
                        }
                        break;
            
                    default :
                };

                sendPackage(package);
                
                // if(clickedProgramId == ''){
                //     // Delete Program
                //     var programLength = Object.keys(dataContainer.program).length;

                //     if(programLength == 1) {
                //         dataContainer.program = null;
                //     } else {
                //         delete dataContainer.program[idParent];
                //     }

                // } else {
                //     //Delete Command
                //     var testLength = Object.keys(dataContainer.program[clickedProgramId].test).length;

                //     if (testLength == 1) {
                //         dataContainer.program[clickedProgramId].test = null;
                //     } else {
                //         delete dataContainer.program[clickedProgramId].test[idParent];
                //     }

                    
                // }
                // package = {newDataBase : dataContainer}
                // sendPackage(package);
                // setNumber();
                if (editFlag) {
                    editFlag = false;
                }
            }
            
            
        }
    }

    var addButton = document.getElementById(idButton);
    var originalContent = addButton.textContent;

    for (i = 0; i < edit.length; i++) {
        edit[i].onclick = function() {
            
            //alert(originalContent);
            if (!editFlag){
                editFlag = true;
                var div = this.parentElement;
                idParent = div.id;
                idEditButton = this.id;
                div.querySelector(".number").style.backgroundColor = '#FFFAA0';
                div.style.background = '#FFFAA0';
                this.style.backgroundColor = '#304674';
                this.style.color = 'white';
                addButton.textContent = "Edit";
                addButton.style.backgroundColor = '#304674';
            } else {
                if (this.id == idEditButton) {
                    var div = this.parentElement;
                    div.style.backgroundColor = '';
                    this.style.backgroundColor = '';
                    div.querySelector(".number").style.backgroundColor = '';
                    this.style.color = '';
                    setNumber(className);
                    addButton.textContent = originalContent;
                    addButton.style.backgroundColor = '';
                    editFlag = false;
                }

            }

        }
    }
}

function editProgramName() {
    var programName = document.getElementById("input-name").value;
    dataContainer.program[idParent].programName = programName;
}

function addProgramToScreen() {
    if (dataContainer.program.idProgramList != null) {
        Object.keys(dataContainer.program.idProgramList).forEach(key => {
            //alert(key);
            list(key, "program-list", "create-button", "liProgram", dataContainer.program.idProgramList[key]);
        });
    
        setNumber("liProgram");
    }
    

    
}

function backClick() {

    selectScreen('ProgramScreen');
    clickedProgramId='';
    
}

function deleteAllList() {
    var ulProgram = document.getElementById("program-list");
    var ulCommand = document.getElementById("command-list");
    var ulRunning = document.getElementById("running-command-list"); 
    var ulDateHistory = document.getElementById("dateHistoryScreen-list");
    var ulListHistory = document.getElementById("listHistoryScreen-list");
    var ulReportHistory = document.getElementById("reportHistoryScreen-list");

    while (ulProgram.firstChild) {
        ulProgram.removeChild(ulProgram.firstChild);
    }

    while (ulCommand.firstChild) {
        ulCommand.removeChild(ulCommand.firstChild);
    }

    while (ulRunning.firstChild) {
        ulRunning.removeChild(ulRunning.firstChild);
    }

    while (ulDateHistory.firstChild) {
        ulDateHistory.removeChild(ulDateHistory.firstChild);
    }

    while (ulListHistory.firstChild) {
        ulListHistory.removeChild(ulListHistory.firstChild);
    }

    while (ulReportHistory.firstChild) {
        ulReportHistory.removeChild(ulReportHistory.firstChild);
    }
}

var clickedProgramId = '';
var clickedProgramClass = '';
var date;
var prevcurrentScreen = '';
document.addEventListener('DOMContentLoaded', function() {
    editFlag = false;

    // Select all <ul> elements
    var listItems = document.querySelectorAll('ul');

    // Iterate through each <ul> element
    listItems.forEach(function(listItem) {

        var ulid = listItem.id;

        if (ulid == 'program-list' || ulid == 'dateHistoryScreen-list' || ulid == 'listHistoryScreen-list') {
            // Attach click event listener to each <ul> element
            listItem.addEventListener('click', function(ev) {
                clickedProgramId = ev.target.id;
                clickedProgramClass = ev.target.className;
                console.log("Clicked item ID:", clickedProgramId);
                console.log("Clicked item class:", clickedProgramClass);

                var clickedUlId = ev.currentTarget.id;
                console.log("Clicked UL ID:", clickedUlId);

                if (clickedProgramClass !== "close" && clickedProgramClass !== 'edit' && clickedProgramClass !== 'number') {
                    if (clickedUlId === 'program-list') {
                        //Request program's command list
                        package = {};
                        package = {
                            openProgram : {
                                idProgram : clickedProgramId
                            }
                        }
                        sendPackage(package);
                        
                        //Choose screen 
                        if (!run) {
                            
                            currentScreen = 'CommandScreen'
                            //selectScreen('CommandScreen');
                        } else {
                            if (idRunProgram == clickedProgramId) {
                                currentScreen = 'runningCommandScreen';
                                // selectScreen('runningCommandScreen');
                            } else {
                                currentScreen = 'CommandScreen';
                                // selectScreen('CommandScreen');
                            }
                        }
                    } else if (clickedUlId === 'dateHistoryScreen-list') {
                        package = {};
                        package = {
                            openDateHistory : {
                                idDate : clickedProgramId
                            }
                        }
                        sendPackage(package);
                       
                        currentScreen = 'listHistoryScreen';



                        date = clickedProgramId;
                        // console.log("Clicked history date:", date);
                        // selectScreen('listHistoryScreen');
                    } else if (clickedUlId === 'listHistoryScreen-list') {
                        package = {};
                        package = {
                            openReport : {
                                idHistory : clickedProgramId,
                                idDate : date
                            }
                        }
                        
                        sendPackage(package);
                       
                        currentScreen = 'reportHistoryScreen';
                        
                        //selectScreen('reportHistoryScreen');
                    }
                } else {
                    clickedProgramId = "";
                    console.log("Clicked item ID:", clickedProgramId);
                }
            }, false);

        }

        
    });
});


var currentScreen;
function selectScreen(selectedScreen) {
    editFlag = false;
    deleteAllList();

    if (!run ) {
        document.getElementById('ipx-button-content').style.display = "";
        document.getElementById('current-program').innerHTML = '';
        document.getElementById('running-status').style.display = "none";
      
        document.getElementById('run-button').style.display = "";
        document.getElementById('run-button').textContent = 'Run';
        document.getElementById('pause-button').style.display = "none";
        document.getElementById('stop-button').style.display = "none";
    } else {
        document.getElementById('ipx-button-content').style.display = "none";
        document.getElementById('running-status').style.display = "";
        document.getElementById('current-program').innerHTML = 'Running : ' + dataContainer.program.idProgramList[dataContainer.running.idProgram] + " " + '(' + dataContainer.running.progress +  "/" + dataContainer.running.totalTest + ")";

        if (clickedProgramId == idRunProgram) {
            if(!pause){
                document.getElementById('run-button').style.display = "none";
                document.getElementById('run-button').textContent = 'Run';
                document.getElementById('pause-button').style.display = "";
                document.getElementById('stop-button').style.display = "none";
            } else {
                
                document.getElementById('run-button').style.display = "";
                document.getElementById('run-button').textContent = 'Continue';
                document.getElementById('pause-button').style.display = "none";
                document.getElementById('stop-button').style.display = "";
            }
        } else {
            document.getElementById('run-button').style.display = "none";
            document.getElementById('pause-button').style.display = "none";
            document.getElementById('stop-button').style.display = "none";
        }
    }
    
    
    switch (selectedScreen) {
        case 'menuScreen' :
            document.getElementById('program-section').style.display = "none";
            document.getElementById('command-section').style.display = "none";
            document.getElementById('running-command').style.display = "none";
            document.getElementById('command-button').style.display = "none";

            document.getElementById('menu-history-button').style.display = "none";
            document.getElementById('universal-back-button').style.display = "";
            document.getElementById('dateHistoryScreen').style.display = "none";
            document.getElementById('listHistoryScreen').style.display = "none";
            document.getElementById('reportHistoryScreen').style.display = "none";
            document.getElementById('menu-section').style.display = "";
            currentScreen = 'menuScreen';
            document.getElementById('current-clock').innerHTML = "Current time: " + dataContainer.currentTime;

            break;
        case 'reportHistoryScreen' :
            document.getElementById('program-section').style.display = "none";
            document.getElementById('command-section').style.display = "none";
            document.getElementById('running-command').style.display = "none";
            document.getElementById('command-button').style.display = "none";

            document.getElementById('menu-history-button').style.display = "none";
            document.getElementById('universal-back-button').style.display = "";
            document.getElementById('dateHistoryScreen').style.display = "none";
            document.getElementById('listHistoryScreen').style.display = "none";
            document.getElementById('reportHistoryScreen').style.display = "";
            document.getElementById('menu-section').style.display = "none";
            //document.getElementById('reportHistoryScreen').style.padding = "12px 8px 12px 10px";
            report();
            currentScreen = 'reportHistoryScreen';
            break;
        case 'listHistoryScreen' :
            currentScreen = 'listHistoryScreen';
            document.getElementById('program-section').style.display = "none";
            document.getElementById('command-section').style.display = "none";
            document.getElementById('running-command').style.display = "none";
            document.getElementById('command-button').style.display = "none";

            document.getElementById('menu-history-button').style.display = "none";
            document.getElementById('universal-back-button').style.display = "";
            document.getElementById('dateHistoryScreen').style.display = "none";
            document.getElementById('listHistoryScreen').style.display = "";
            document.getElementById('reportHistoryScreen').style.display = "none";
            document.getElementById('menu-section').style.display = "none";
            document.getElementById('title-listHistoryScreen').innerHTML = 'Test History : ' + date;
            
            if (dataContainer.history.idHistoryList != null) {
                console.log('masuuuuuuuk')
                Object.keys(dataContainer.history.idHistoryList).forEach(key => {
                    //alert(key);
                    console.log('key : ' + key)
                    var currentProgramName = dataContainer.program.idProgramList[dataContainer.history.idHistoryList[key].idProgram];
                    if (currentProgramName != null) {
                        list(key, "listHistoryScreen-list", "create-button", "liListHistory", currentProgramName);
                    } else {
                        list(key, "listHistoryScreen-list", "create-button", "liListHistory", dataContainer.history.idHistoryList[key].nameProgram);
                    }
                    //list(key, "listHistoryScreen-list", "create-button", "liListHistory", dataContainer.history.idHistoryList[key]);
                });
            
                setNumber("liListHistory");
            }
           // document.getElementById('programToHistory-button').textContent = 'Back';
            break;
        case 'dateHistoryScreen' :
            currentScreen = 'dateHistoryScreen';
            document.getElementById('program-section').style.display = "none";
            document.getElementById('command-section').style.display = "none";
            document.getElementById('running-command').style.display = "none";
            document.getElementById('command-button').style.display = "none";

            document.getElementById('menu-history-button').style.display = "none";
            document.getElementById('universal-back-button').style.display = "";
            document.getElementById('dateHistoryScreen').style.display = "";
            document.getElementById('listHistoryScreen').style.display = "none";
            document.getElementById('reportHistoryScreen').style.display = "none";
            document.getElementById('menu-section').style.display = "none";

            if (dataContainer.history.dates != null) {
                Object.keys(dataContainer.history.dates).forEach(key => {
                    //alert(key);
                    list(key, "dateHistoryScreen-list", "create-button", "liHistoryDate", dataContainer.history.dates[key]);
                });
            
                setNumber("liHistoryDate");
            }
            break;
        case 'ProgramScreen':
            currentScreen = 'ProgramScreen';
            document.getElementById('program-section').style.display = "";
            document.getElementById('command-section').style.display = "none";
            document.getElementById('running-command').style.display = "none";
            document.getElementById('command-button').style.display = "none";

            document.getElementById('menu-history-button').style.display = "";
            document.getElementById('universal-back-button').style.display = "none";
            document.getElementById('dateHistoryScreen').style.display = "none";
            document.getElementById('listHistoryScreen').style.display = "none";
            document.getElementById('reportHistoryScreen').style.display = "none";
            document.getElementById('menu-section').style.display = "none";
            addProgramToScreen();
            break;
        case 'CommandScreen':
            currentScreen = 'CommandScreen';
            document.getElementById('program-section').style.display = "none";
            document.getElementById('command-section').style.display = "";
            document.getElementById('running-command').style.display = "none";
            document.getElementById('command-button').style.display = "";

            document.getElementById('menu-history-button').style.display = "none";
            document.getElementById('universal-back-button').style.display = "none";
            document.getElementById('dateHistoryScreen').style.display = "none";
            document.getElementById('listHistoryScreen').style.display = "none";
            document.getElementById('reportHistoryScreen').style.display = "none";
            document.getElementById('menu-section').style.display = "none";
            addCommandToScreen("command-list", "label-command-list");
            break;
        case 'runningCommandScreen':
            currentScreen = 'runningCommandScreen';
            document.getElementById('program-section').style.display = "none";
            document.getElementById('command-section').style.display = "none";
            document.getElementById('running-command').style.display = "";
            document.getElementById('command-button').style.display = "";

            document.getElementById('menu-history-button').style.display = "none";
            document.getElementById('universal-back-button').style.display = "none";
            document.getElementById('dateHistoryScreen').style.display = "none";
            document.getElementById('listHistoryScreen').style.display = "none";
            document.getElementById('reportHistoryScreen').style.display = "none";
            document.getElementById('menu-section').style.display = "none";
            addCommandToScreen("running-command-list", "label-running-command-list");
            
            break;
        case 'release':
            currentScreen = 'release';
            document.getElementById('ipx-button-content').style.display = "none";
            document.getElementById('running-status').style.display = "none";
            document.getElementById('program-section').style.display = "none";
            document.getElementById('command-section').style.display = "none";
            document.getElementById('running-command').style.display = "none";
            document.getElementById('command-button').style.display = "none";

            document.getElementById('menu-history-button').style.display = "none";
            document.getElementById('universal-back-button').style.display = "none";
            document.getElementById('dateHistoryScreen').style.display = "none";
            document.getElementById('listHistoryScreen').style.display = "none";
            document.getElementById('reportHistoryScreen').style.display = "none";
            document.getElementById('menu-section').style.display = "none";
            break;
        default :
    }

    

}

function report() {
    var currentProgramName = dataContainer.program.idProgramList[dataContainer.history.idHistoryList[clickedProgramId].idProgram];
    if (currentProgramName == null) {
        currentProgramName = dataContainer.history.idHistoryList[clickedProgramId].nameProgram;
    } 
    document.getElementById('ran-program').innerHTML = 'Program Name : ' + currentProgramName;
    document.getElementById('ran-status').innerHTML = 'Status : ' + dataContainer.history.information[clickedProgramId].status;
    document.getElementById('ran-date').innerHTML = 'Date : ' + date;
    document.getElementById('start-time').innerHTML = 'Start Time : ' + dataContainer.history.information[clickedProgramId].start;
    document.getElementById('stop-time').innerHTML = 'Stop Time : ' + dataContainer.history.information[clickedProgramId].stop;

    if (dataContainer.history.information[clickedProgramId].logger != null) {
        
        Object.keys(dataContainer.history.information[clickedProgramId].logger).forEach(key => {
            //alert(key);
            console.log('key : ' + key)
            var log = dataContainer.history.information[clickedProgramId].logger[key];
            list(key, "reportHistoryScreen-list", "create-button", "liLog", log);
        });
        setNumber("liLog");
    }

}


function addCommandToScreen(idUl, idLabel) {

    if (clickedProgramId != '') {
        var clickedProgramName = dataContainer.program.idProgramList[clickedProgramId];
        // alert("program name : " + clickedProgramName);
        document.getElementById(idLabel).textContent = "Command List : " + clickedProgramName;
        if (allDataProgram[clickedProgramId] != null) {
            Object.keys(allDataProgram[clickedProgramId]).forEach(key => {
                var idTest = key;
                
    
                displayTest = allDataProgram[clickedProgramId][idTest].ipx;
                displayHour = allDataProgram[clickedProgramId][idTest].hour;       
                displayMinute = allDataProgram[clickedProgramId][idTest].minute;        
                displaySecond = allDataProgram[clickedProgramId][idTest].second;
               
                var textCommandList = displayTest + " " + displayTime(displayHour) + ":" + displayTime(displayMinute) + ":" + displayTime(displaySecond);
    
                list(idTest, idUl, "add-button", "liCommand", textCommandList);
                setNumber("liCommand");
            });
        }
        
    }
    
    
}



function setNumber(className){
    var ulElement = document.getElementById("command-list");
    var liElement = document.getElementsByClassName(className);

    //Delete previous numbering
    document.querySelectorAll(".number").forEach(function(element){
        element.remove();
    });

    //Add new numbering
    var number = 1;
    //alert('jai');
    for (var i = 0; i < liElement.length; i++) {
        var li = liElement[i];
        //alert(li.className);
        
        if (li.style.display == '') {
            var numberElement = document.createElement('li');
            numberElement.className = "number";
            numberElement.id = "numberid";
            
            if (number < 10) {
                numberElement.textContent = "0" + number + ".";
                
            } else {
                numberElement.textContent = number + ".";
            }
            li.appendChild(numberElement);
            
            if ((number%2) != 0){
                li.style.backgroundColor = '#f9f9f9';
                numberElement.style.backgroundColor = '#f9f9f9';
            }else{
                li.style.backgroundColor = '#eee';
                numberElement.style.backgroundColor = '#eee'
            }
            
            //alert(li.className);
             number++;
        }

        
    }
    
}

function editScreenValue(idButton, originText){
    var editElement = document.getElementById(idParent);
    var editButton = document.getElementById(idEditButton);
    var Button = document.getElementById(idButton);

    editElement.style.backgroundColor = '';
    editButton.style.backgroundColor = '';
    editButton.style.color = '';

    Button.textContent = originText;
    Button.style.backgroundColor = '';

    editFlag = false;
}


//Adjust time input value
var hourToSecond;
var minuteToSecond;


var displayTest;
var displayHour;
var remainMinute;
var displayMinute;
var displaySecond;
var remainTime

function adjustTimeValue(type, currentTime) {

    var totalInSecond;

    if (type == 'input') {
        hourToSecond = inputHour * 3600;
        minuteToSecond = inputMinute * 60;
        totalInSecond = parseInt(hourToSecond) + parseInt(minuteToSecond) + parseInt(inputSecond);
    } else if (type == 'count') {
        totalInSecond = currentTime;
    }

    var realHour = Math.floor(totalInSecond / 3600);
    var realMinute = Math.floor((totalInSecond % 3600) / 60);
    var realSecond = totalInSecond % 60;


    // if (totalInSecond >= 3600){
    //     remainTime = totalInSecond % 3600;
    //     realHour = (totalInSecond - remainTime)/3600;

    //     if (remainTime >= 60){
    //         realSecond = remainTime % 60;
    //         realMinute = (remainTime - realSecond)/60;
    //     } else {
    //         realMinute = 0;
    //         realSecond = remainTime;
    //     }
    // } else {
    //     realHour = 0;
    //     if (totalInSecond >= 60){
    //         realSecond = totalInSecond % 60;
    //         realMinute = (totalInSecond - realSecond)/60;
    //     } else {
    //         realMinute = 0;
    //         realSecond = parseInt(inputSecond);
    //     }
    // }

    return{
        hour : realHour,
        minute : realMinute,
        second : realSecond,
        total : totalInSecond
    }
}
//========================Count Down Time Test=========================
var countingUp = false;
var countingDown = false;


function timerCounter(type, duration, idElement) {
    
    
    var durationInMillis = parseInt(duration) * 1000;
    if (type == 'down' && !countingDown) {
        console.log('======counting down==================');
        countingDown = true;
        var countDownDate = new Date().getTime() + durationInMillis;
        var originalContent = document.getElementById(idElement).textContent;
        var x = setInterval(function() {
            var now = new Date().getTime();

            var distance = Math.ceil(((countDownDate - now)/1000));
            // console.log("distance : " + distance);
            var timer = adjustTimeValue('count', distance);
            //console.log("second : " + timer.second);
            //document.getElementById(idElement).innerHTML = dataContainer.program[dataContainer.running.idProgram].programName + ' ' + displayTime()
            document.getElementById(idElement).innerHTML = "Remanining Time: " + displayTime(timer.hour) + ':' + displayTime(timer.minute) + ':' + displayTime(timer.second);
            
            if (distance < 1 || pause) {
                clearInterval(x);
                countingDown = false;
                if(pause) {
                    console.log("timer down killed due to paused")
                }
                //document.getElementById(idElement).innerHTML = originalContent;
                //document.getElementById("demo").innerHTML = "EXPIRED";
            }
        }, 1000);
    } else if (type == 'up' && !countingUp) {
        console.log('======countingup==================');
        var front = "Current Test: " + dataContainer.running.ipx  + ' ';
        var back =   '/' + displayTime(dataContainer.running.dataProgram.test[dataContainer.running.currentIdTest].hour) + ':' + displayTime(dataContainer.running.dataProgram.test[dataContainer.running.currentIdTest].minute) + ':' + displayTime(dataContainer.running.dataProgram.test[dataContainer.running.currentIdTest].second);
        countingUp = true;
        var startTime = new Date().getTime() - (dataContainer.running.countingTimeCurrentTest * 1000);
        var testTime = dataContainer.running.dataProgram.test[dataContainer.running.currentIdTest].total
        var y = setInterval(function() {
            var now = new Date().getTime();

            var elapsed = Math.floor(((now - startTime)/1000));
            //console.log("distance : " + elapsed);
            var timer = adjustTimeValue('count', elapsed);
            //console.log("second : " + timer.second);
            
            document.getElementById(idElement).innerHTML = front + '(' + displayTime(timer.hour) + ':' + displayTime(timer.minute) + ':' + displayTime(timer.second) + back + ')';
            //document.getElementById(idElement).innerHTML = 'hai';
            if (elapsed >= testTime || pause) {
                clearInterval(y);
                countingUp = false;
                console.log("distance : stop");
                if(pause) {
                    console.log("timer up killed due to paused")
                }

                if(runningIdTest != "" && !pause) {
                    timerCounter('up', dataContainer.running.countingTimeCurrentTest, "current-test");
                    
                }
                //document.getElementById(idElement).innerHTML = originalContent;
                //document.getElementById("demo").innerHTML = "EXPIRED";
            }
        }, 1000);
    }

    
    
    
}

function displayTime(time) {
    if (time < 10) {
        time = '0' + time;
    }

    return time;
}

//===============Run/Stop=======================
var run = false;
var pause = false;


var runButton = document.getElementById('run-button');
var pauseButton = document.getElementById('pause-button');
var stopButton = document.getElementById('stop-button');

var idRunProgram = '';

function runClick() {

    if(allDataProgram[clickedProgramId] != null && !pause) {
        //run = true;
        //pause = false;
        idRunProgram = clickedProgramId
        console.log("Running Program : " + idRunProgram);

        //selectScreen('runningCommandScreen');
        

        package = {};
        package = {run : idRunProgram};
        sendPackage(package); 
    } else if (pause) {
        package = {};
        package = {pause : false};
        sendPackage(package);
        
    } else {
        alert('Please add a test!');
    }

   
    console.log("------cliclk program: " + clickedProgramId);
    
}

function pauseClick() {
    //pause = true;
    package = {};
    package = {pause : true};
    sendPackage(package); 
    //selectScreen('runningCommandScreen');

}

function stopClick() {
    

    // console.log("Running Program : " + idRunProgram);
    // selectScreen('CommandScreen');

    package = {};
    package = {run : ""};
    sendPackage(package); 

}

function programHistoryClick() {
    
    selectScreen('dateHistoryScreen');
    console.log("haiiiiiiiiiiiiiiiii");
    // console.log("Current Screen : " + currentScreen)
    // switch(currentScreen) {
    //     case 'ProgramScreen':
    //         selectScreen('dateHistoryScreen');
    //         document.getElementById('programToHistory-button').textContent = 'Back';
    //         document.getElementById('programToHistory-button').style.backgroundColor = '#333399';
    //         break;
    //     case 'dateHistoryScreen':
    //         selectScreen('ProgramScreen');
    //         document.getElementById('programToHistory-button').textContent = 'History';
    //         document.getElementById('programToHistory-button').style.backgroundColor = '#304D30';
    //         break;
        
    //     case 'listHistoryScreen':
    //         selectScreen('dateHistoryScreen');
    //         document.getElementById('programToHistory-button').textContent = 'Back';
    //         document.getElementById('programToHistory-button').style.backgroundColor = '#333399';
    //         break;
    //     case 'reportHistoryScreen':
    //         selectScreen('listHistoryScreen');
    //         document.getElementById('programToHistory-button').textContent = 'Back';
    //         document.getElementById('programToHistory-button').style.backgroundColor = '#333399';
    //         break;
    //     default :
    // }

    // if (currentScreen == 'dateHistoryScreen' || currentScreen == 'listHistoryScreen' || currentScreen == 'reportHistoryScreen') {
    //     selectScreen('ProgramScreen');
    //     document.getElementById('programToHistory-button').textContent = 'History';
    // } 
    // else {
    //     selectScreen('dateHistoryScreen');
    //     document.getElementById('programToHistory-button').textContent = 'Create Program';
        
    // }
}


function menuClick() {
    package = {};
    package = {openMenu : true};
    sendPackage(package); 
    selectScreen('menuScreen');
}

function universalBackClick() {

    switch(currentScreen) {
        case 'menuScreen':
            prevcurrentScreen = 'ProgramScreen';
            package = {};
            package = {openMenu : false};
            sendPackage(package);
            break;
        case 'dateHistoryScreen':
            prevcurrentScreen = 'ProgramScreen';
            break;
        case 'listHistoryScreen':
            prevcurrentScreen = 'dateHistoryScreen';
            break;
        case 'reportHistoryScreen':
            prevcurrentScreen = 'listHistoryScreen';
            break;
        default :
    }

    selectScreen(prevcurrentScreen);
}

function setClock() {
    var now = new Date();

    var year = now.getFullYear() - 2000;
    var month = now.getMonth() + 1; 
    var date = now.getDate();
    var day = now.getDay() + 1; 
    var hour = now.getHours();
    var minute = now.getMinutes();
    var second = now.getSeconds();

    package = {};
    package = {
        adjustTime : {
            year : year,
            month : month,
            date : date,
            day : day,
            hour : hour,
            minute : minute,
            second : second
        } 
    };
    sendPackage(package);
}




// var dataContainer = {

//     program : {
//         program1 : {
//             programName : "Trial 1",
//             totalTime : 80,
//             test : {
//                 test1 : {
//                     ipx : 'IPX5',
//                     hour : 0,
//                     minute : 0,
//                     second : 10,
//                     total : 10
//                 },
                
//                 test2 :{
//                     ipx : 'IPX6',
//                     hour : 0,
//                     minute : 0,
//                     second : 20,
//                     total : 20
//                 },
        
//                 test3 :{
//                     ipx : 'IPX6',
//                     hour : 0,
//                     minute : 0,
//                     second : 50,
//                     total : 50
//                 }
//             }
//         },

//         trial2 : {
//             programName : "Trial 2",
//             test : {
//                 test1 : {
//                     ipx : 'IPX5',
//                     hour : 0,
//                     minute : 0,
//                     second : 40,
//                     total : 40
//                 },
                
//                 test2 :{
//                     ipx : 'IPX6',
//                     hour : 0,
//                     minute : 0,
//                     second : 50,
//                     total : 50
//                 },
        
//                 test3 :{
//                     ipx : 'IPX6',
//                     hour : 0,
//                     minute : 0,
//                     second : 10,
//                     total : 10
//                 }
//             }
//         },

//         trial3 : {
//             programName : "Trial 3",
//             test : {
//                 test1 : {
//                     ipx : 'IPX5',
//                     hour : 0,
//                     minute : 0,
//                     second : 5,
//                     total : 5
//                 },
                
//                 test2 :{
//                     ipx : 'IPX6',
//                     hour : 0,
//                     minute : 0,
//                     second : 1,
//                     total : 1
//                 },
        
//                 test3 :{
//                     ipx : 'IPX6',
//                     hour : 0,
//                     minute : 0,
//                     second : 7,
//                     total : 7
//                 }
//             }
//         }
//     },
    

//     ipx5Status : 'on',
//     ipx6Status : 'off',

//     takeOver : true

// };