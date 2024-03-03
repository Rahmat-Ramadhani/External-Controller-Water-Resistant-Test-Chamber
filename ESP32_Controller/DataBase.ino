// Json Variable to Hold Sensor Readings
JSONVar receivedPackage;
JSONVar myDataBase;
JSONVar myHistory;
JSONVar program;
JSONVar test;
JSONVar history;
JSONVar logger;
JSONVar dates;
JSONVar idProgramList;
JSONVar dataProgram;
JSONVar memoryBuffer;
JSONVar nullObject;
//JSONVar idProgram;
//JSONVar remainingTime;
//JSONVar currentIdTest;
//JSONVar countingTimeCurrentTest;
unsigned long totalTime = 0;
bool stopProgram = false;
bool openMenu = false;
String pathIdProgram = "/program/idProgramList.txt";
String pathDates = "/history/Dates.txt";
String pathOngoingTest = "/history/dataOngoingTest.txt";

void initDataBase() {
//  program["program0"]["programName"] = "Trial 1";
//  program["program0"]["test"]["Test 0"]["ipx"] = "IPX5";
//  program["program0"]["test"]["Test 0"]["hour"] = 0;
//  program["program0"]["test"]["Test 0"]["minute"] = 0;
//  program["program0"]["test"]["Test 0"]["second"] = 30;
//  program["program0"]["test"]["Test 0"]["total"] = 30;
//
//  program["program0"]["test"]["Test 1"]["ipx"] = "IPX6";
//  program["program0"]["test"]["Test 1"]["hour"] = 0;
//  program["program0"]["test"]["Test 1"]["minute"] = 0;
//  program["program0"]["test"]["Test 1"]["second"] = 30;
//  program["program0"]["test"]["Test 1"]["total"] = 30;
//
//  program["program1"]["programName"] = "Trial 2";
//  program["program1"]["test"]["test0"]["ipx"] = "IPX5";
//  program["program1"]["test"]["test0"]["hour"] = 0;
//  program["program1"]["test"]["test0"]["minute"] = 0;
//  program["program1"]["test"]["test0"]["second"] = 30;
//  program["program1"]["test"]["test0"]["total"] = 30;

  history["2024-01-17"]["history0"]["idProgram"] = "program0";
  history["2024-01-17"]["history0"]["start"] = "13:00";
  history["2024-01-17"]["history0"]["stop"] = "15:00";
  history["2024-01-17"]["history0"]["status"] = "Finish";
  history["2024-01-17"]["history0"]["logger"]["13:00"] = "IPX5 Test";
  history["2024-01-17"]["history0"]["logger"]["14:00"] = "IPX6 Test";
  history["2024-01-17"]["history0"]["logger"]["15:00"] = "Completed";

  history["2024-01-17"]["history1"]["idProgram"] = "program0";
  history["2024-01-17"]["history1"]["start"] = "18:00";
  history["2024-01-17"]["history1"]["stop"] = "20:00";
  history["2024-01-17"]["history1"]["status"] = "Finish";
  history["2024-01-17"]["history1"]["logger"]["18:00"] = "IPX5 Test";
  history["2024-01-17"]["history1"]["logger"]["19:00"] = "IPX6 Test";
  history["2024-01-17"]["history1"]["logger"]["20:00"] = "Completed";

  history["2024-01-18"]["history1"]["idProgram"] = "program0";
  history["2024-01-18"]["history1"]["start"] = "18:00";
  history["2024-01-18"]["history1"]["stop"] = "20:00";
  history["2024-01-18"]["history1"]["status"] = "Finish";
  history["2024-01-18"]["history1"]["logger"]["18:00"] = "IPX5 Test";
  history["2024-01-18"]["history1"]["logger"]["19:00"] = "IPX6 Test";
  history["2024-01-18"]["history1"]["logger"]["20:00"] = "Completed";

  dates[0] = "2024-01-17";
  dates[1] = "2024-01-18";

  
  //myDataBase["program"]["idProgramList"] = idProgramList;
  myDataBase["program"]["dataProgram"]["test"] = dataProgram;
  myDataBase["program"]["dataProgram"]["idProgram"] = dataProgram;
  //myDataBase["history"]["information"] = history;
  //myDataBase["history"]["dates"] = dates;
  myDataBase["running"]["idProgram"] = "";  
  myDataBase["running"]["ipx"] = "";
  myDataBase["running"]["pause"] = false;
  myDataBase["running"]["remainingTime"] = 0;
  myDataBase["running"]["currentIdTest"] = "";
  myDataBase["running"]["countingTimeCurrentTest"] = 0;
  myDataBase["running"]["progress"] = 0;
  myDataBase["running"]["totalTest"] = 0;
  myDataBase["ipx5Status"] = "off";
  myDataBase["ipx6Status"] = "off";
  myDataBase["takeOver"] = false;


}

unsigned long initialRemainingTime(String idProgram) {
  unsigned long totalTime = 0;
  JSONVar keys = myDataBase["running"]["dataProgram"]["test"].keys();
 
  for (int x = 0; x < keys.length(); x++) {
    unsigned long testTime = myDataBase["running"]["dataProgram"]["test"][keys[x]]["total"];
    totalTime += testTime;
  }
  
  return totalTime;
}

String generateProgramId(){
  int programLength = myDataBase["program"]["idProgramList"].keys().length();
  String lastProgram = myDataBase["program"]["idProgramList"].keys()[programLength - 1];
  String numberPart = "";
  
  for (int i = 0; i < lastProgram.length(); i++) {
    char currentChar = lastProgram.charAt(i);

    if (isDigit(currentChar)) {
      numberPart += currentChar;
    }

  }
  int numericValue = numberPart.toInt();
  Serial.print("last numeric : ");
  Serial.println(numberPart);
  numericValue++;
  String idProgram = "program" + String(numericValue); 
  Serial.print("new numeric : ");
  Serial.println(numericValue);
  return idProgram;
}

String generateTestid(String programId) {
  String programPath = "/program/" + programId + ".txt";
  memoryAccsess("read", programPath, myDataBase["program"]["dataProgram"]);
  int testLength = myDataBase["program"]["dataProgram"]["test"].keys().length();
  String lastTest = myDataBase["program"]["dataProgram"]["test"].keys()[testLength - 1];
  String newIdTest = "";
  String numberPart = "";
  
  if (testLength == -1){
    newIdTest = "Test 0";
  } else {
    for (int i = 0; i < lastTest.length(); i++) {
      char currentChar = lastTest.charAt(i);
  
      if (isDigit(currentChar)) {
        numberPart += currentChar;
      }
  
    }
    int numericValue = numberPart.toInt();
    Serial.print("last program : ");
    Serial.println(lastTest);
    Serial.print("test lengtg : ");
    Serial.println(testLength);
    numericValue++;
    newIdTest = "Test " + String(numericValue); 
    Serial.print("new numeric : ");
    Serial.println(numericValue);          
  }
  return newIdTest;
}



String generateHistoryid(String idHistoryListPath) {
  //String historyDatePath = "/history/" + getFormattedDate() + ".txt";
  memoryAccsess("read", idHistoryListPath, myHistory["ongoingTest"]["idHistoryList"]);
  int historyLength = myHistory["ongoingTest"]["idHistoryList"].keys().length();
  String lastHistory = myHistory["ongoingTest"]["idHistoryList"].keys()[historyLength - 1];
  String newIdHistory = "";
  String numberPart = "";
  
  if (historyLength == -1){
    newIdHistory = "History 0";
  } else {
    for (int i = 0; i < lastHistory.length(); i++) {
      char currentChar = lastHistory.charAt(i);
  
      if (isDigit(currentChar)) {
        numberPart += currentChar;
      }  
    }
    int numericValue = numberPart.toInt();
    Serial.print("last history : ");
    Serial.println(lastHistory);
    Serial.print("history length : ");
    Serial.println(historyLength);
    numericValue++;
    newIdHistory = "History " + String(numericValue); 
    Serial.print("new numeric : ");
    Serial.println(numericValue);    
  }
  return newIdHistory;
}


String generateLoggerid() {
  //String historyDatePath = "/history/" + getFormattedDate() + ".txt";
  memoryAccsess("read", myHistory["ongoingTest"]["pathData"], myHistory["todayTest"]);
  int loggerLength = myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["logger"].keys().length();
  
  String newIdLogger = "";
  String numberPart = "";
    
  if (loggerLength == -1){
    newIdLogger = "Log0";
       
  } else {
    String lastLogger = myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["logger"].keys()[loggerLength - 1];
    
    for (int i = 0; i < lastLogger.length(); i++) {
      char currentChar = lastLogger.charAt(i);
  
      if (isDigit(currentChar)) {
        numberPart += currentChar;
      }
  
    }
    int numericValue = numberPart.toInt();
    numericValue++;
    newIdLogger = "Log" + String(numericValue); 
  }
  return newIdLogger;
}


void initMemory() {
  String programFolder = "/program";
  String historyFolder = "/history";
  createDir(SD, programFolder);
  createDir(SD, historyFolder);
  JSONVar kosong;


  Serial.print("Read content : ");
  Serial.println(readFile(SD, pathIdProgram));
  Serial.print("Read content : ");
  Serial.println(readFile(SD, pathDates));
  memoryBuffer = JSON.parse(readFile(SD, pathIdProgram));
  myDataBase["program"]["idProgramList"] = memoryBuffer;
  memoryBuffer = JSON.parse(readFile(SD, pathDates));
  myDataBase["history"]["dates"] = memoryBuffer;
//  //myDataBase["history"]["dates"] = JSON.parse(readFile(SD, pathDates, "dates"));
  Serial.print("Read history content: ");
  Serial.println(JSON.stringify(myDataBase["history"]["dates"]));
  Serial.print("Read history length: ");
  Serial.println(myDataBase["history"]["dates"] .keys().length());
//  Serial.print("Read myDataBase[program][idProgramList] : ");
//  Serial.println(myDataBase["program"]["idProgramList"]);
  
}

void memoryAccsess(String task, const String &path, JSONVar myObject) {
  //Serial.println("=============== Accessing Memory ================");
 
  if(task == "read") {
    memoryBuffer = JSON.parse(readFile(SD, path));
    
    
  } else if (task == "write") {
    memoryBuffer = JSON.parse(writeFile(SD, path, JSON.stringify(myObject)));
    
  }

  if(readFile(SD, path) == ""){
    //Serial.println("Object is empty");
    JSONVar nullObject;
    myObject = undefined;
  } else {
    //Serial.println("Object is not empty");
    myObject = memoryBuffer;
  }
  
//  Serial.print("Read myobject : ");
//  Serial.println(myObject);
//  Serial.println("=============== Finished Accessing Memory ================");
// 
}

void updateMyDataBase(){
//------------------------------------------------------------------------
  if(receivedPackage.hasOwnProperty("pause")) {
    myDataBase["running"]["pause"] = (bool) receivedPackage["pause"];
  }

//------------------------------------------------------------------------
  if(receivedPackage.hasOwnProperty("takeOver")) {
    myDataBase["takeOver"] = (bool) receivedPackage["takeOver"];
    bool takeOverStatus = (bool) myDataBase["takeOver"];
    String currentIdRunningProgram = myDataBase["running"]["idProgram"];
    
    if(receivedPackage.hasOwnProperty("ipx5Status")){
      myDataBase["ipx5Status"] = (const char*) receivedPackage["ipx5Status"];
    }
    if(receivedPackage.hasOwnProperty("ipx6Status")){
      myDataBase["ipx6Status"] = (const char*) receivedPackage["ipx6Status"];
    }

    if(!takeOverStatus && currentIdRunningProgram != "") {
      myDataBase["running"]["pause"] = true;
    }
    
    Serial.print("Take Over : ");
    Serial.println((bool) myDataBase["takeOver"]);
  }
//------------------------------------------------------------------------
  if(receivedPackage.hasOwnProperty("ipx5Status")) {
    myDataBase["ipx5Status"] = (const char*) receivedPackage["ipx5Status"];
    
    Serial.print("myDataBase[\"ipx5Status\"] : ");
    Serial.println((const char*) myDataBase["ipx5Status"]);
  }
//------------------------------------------------------------------------
  if(receivedPackage.hasOwnProperty("ipx6Status")) {
    myDataBase["ipx6Status"] = (const char*) receivedPackage["ipx6Status"];
    
    Serial.print("myDataBase[\"ipx6Status\"] : ");
    Serial.println((const char*) myDataBase["ipx6Status"]);
  }
//------------------------------------------------------------------------
  if(receivedPackage.hasOwnProperty("program")) {
    memoryAccsess("read", pathIdProgram, myDataBase["program"]["idProgramList"]);
    int programLength = myDataBase["program"]["idProgramList"].keys().length();
    Serial.print("Length Program : ");
    Serial.println(programLength);
    
    if(programLength == -1){
    
      myDataBase["program"]["idProgramList"]["program0"] = (const char*) receivedPackage["program"]["idProgram"]["programName"];
      memoryAccsess("write", pathIdProgram, myDataBase["program"]["idProgramList"]);
      myDataBase["program"]["dataProgram"]["idProgram"] = "program0";
      myDataBase["program"]["dataProgram"]["test"] = undefined;
      myDataBase["program"]["dataProgram"]["test"] = nullObject;
      String programPath = "/program/program0.txt";
      memoryAccsess("write", programPath, myDataBase["program"]["dataProgram"]);
      
    } else {
      int x;
      bool found = false;
      for (x = 0 ; x <= programLength ; x++) {
        String key = myDataBase["program"]["idProgramList"].keys()[x];
        if (receivedPackage["program"].hasOwnProperty(key)) {
          if (receivedPackage["program"][key].hasOwnProperty("programName")) {
            myDataBase["program"]["idProgramList"][key] = (const char*) receivedPackage["program"][key]["programName"];
            memoryAccsess("write", pathIdProgram, myDataBase["program"]["idProgramList"]);
            
            found = true;
          } //else {masuk test}
        }
      }

      if (!found) {
        Serial.println("Make new program");
        String newIdProgram = generateProgramId();
        Serial.print("new id : ");
        Serial.println(newIdProgram);
        myDataBase["program"]["idProgramList"][newIdProgram] = (const char*) receivedPackage["program"]["idProgram"]["programName"];
        
        memoryAccsess("write", pathIdProgram, myDataBase["program"]["idProgramList"]);
        String programPath = "/program/" + newIdProgram + ".txt";
        myDataBase["program"]["dataProgram"]["idProgram"] = newIdProgram;
        myDataBase["program"]["dataProgram"]["test"] = undefined;
        myDataBase["program"]["dataProgram"]["test"] = nullObject;
        memoryAccsess("write", programPath, myDataBase["program"]["dataProgram"]);
        

      }
    }
  }
//------------------------------------------------------------------------
    if(receivedPackage.hasOwnProperty("delete")) {
      String type = (const char*) receivedPackage["delete"]["type"];
      String idList = (const char*) receivedPackage["delete"]["id"];
      String category = (const char*) receivedPackage["delete"]["category"];

      if(category == "program") {
        
       
        if(type == "idProgramList") {
          memoryAccsess("read", pathIdProgram, myDataBase["program"]["idProgramList"]);
          
          if(myDataBase["program"]["idProgramList"].keys().length() == 1){
            myDataBase["program"]["idProgramList"] = undefined;
            myDataBase["program"]["idProgramList"] = nullObject;
          } else {
            myDataBase["program"]["idProgramList"][idList] = undefined;
          }
         
          memoryAccsess("write", pathIdProgram, myDataBase["program"]["idProgramList"]);
          String programPath = "/program/" + idList + ".txt";
          deleteFile(SD, programPath);
        } else {
          String programPath = "/program/" + type + ".txt";
          memoryAccsess("read", programPath, myDataBase["program"]["dataProgram"]);   
          if(myDataBase["program"]["dataProgram"]["test"].keys().length() == 1){
            myDataBase["program"]["dataProgram"]["test"] = undefined;
            myDataBase["program"]["dataProgram"]["test"] = nullObject;
          } else {
            myDataBase["program"]["dataProgram"]["test"][idList] = undefined;
          }    
          Serial.print("--------Test length : ");
          Serial.println(myDataBase["program"]["dataProgram"]["test"].keys().length());
          memoryAccsess("write", programPath, myDataBase["program"]["dataProgram"]); 
        }
      } else if(category == "history") {
        //============Unused Program Line===================
        memoryAccsess("read", pathDates, myDataBase["history"]["dates"]); 
        
        if(type == "dates") {
//          if(myDataBase["history"]["dates"].keys().length() == 1) {
//            myDataBase["history"]["dates"] = undefined;
//            myDataBase["history"]["dates"] = nullObject;
//          } else {
//            myDataBase["history"]["dates"][idList] = undefined;
//          }
//          
//          memoryAccsess("write", pathDates, myDataBase["history"]["dates"]);
//          String datePath = "/history/" + idList + ".txt";
//          deleteFile(SD, datePath);
        } else {
          String dateFolder = "/history/history on " + type;
          String HistoryListFilePath = dateFolder + "/idHistoryList.txt";
          String reportFilePath = dateFolder + "/" + idList + ".txt";
          memoryAccsess("read", HistoryListFilePath, myDataBase["history"]["idHistoryList"]);
          
          if(myDataBase["history"]["idHistoryList"].keys().length() == 1){
            myDataBase["history"]["idHistoryList"] = undefined;
            myDataBase["history"]["idHistoryList"] = nullObject;
            memoryAccsess("write", HistoryListFilePath, myDataBase["history"]["idHistoryList"]);
            deleteFile(SD, reportFilePath);
            deleteFile(SD, HistoryListFilePath);
            deleteDirectory(SD, dateFolder);
            memoryAccsess("read", pathDates, myDataBase["history"]["dates"]); 
            if(myDataBase["history"]["dates"].keys().length() == 1) {
              myDataBase["history"]["dates"] = undefined;
              myDataBase["history"]["dates"] = nullObject;
            } else {
              myDataBase["history"]["dates"][type] = undefined;
            }
          } else {
            myDataBase["history"]["idHistoryList"][idList] = undefined;
            memoryAccsess("write", HistoryListFilePath, myDataBase["history"]["idHistoryList"]);
            deleteFile(SD, reportFilePath);
          }
          
          
        }
      }

      
    }

//  if(receivedPackage.hasOwnProperty("newDataBase")) {
//    JSONVar a = receivedPackage["newDataBase"];
//    myDataBase = a;
//  }
//------------------------------------------------------------------------
  if(receivedPackage.hasOwnProperty("openProgram")) {
    String idProgram = (const char*) receivedPackage["openProgram"]["idProgram"];
    String programPath = "/program/" + idProgram + ".txt";
    Serial.print("read program from : ");
    Serial.println(programPath);
    memoryAccsess("read", programPath, myDataBase["program"]["dataProgram"]);
    
  }

//------------------------------------------------------------------------
  if(receivedPackage.hasOwnProperty("openDateHistory")) {
    String idDate = (const char*) receivedPackage["openDateHistory"]["idDate"];
    String historyListFile = "/history/history on " + idDate + "/idHistoryList.txt";
    Serial.print("read program from : ");
    Serial.println(historyListFile);
    memoryAccsess("read", historyListFile, myDataBase["history"]["idHistoryList"]);
    
  }

//------------------------------------------------------------------------
  if(receivedPackage.hasOwnProperty("openReport")) {
    String idHistory = (const char*) receivedPackage["openReport"]["idHistory"];
    String idDate = (const char*) receivedPackage["openReport"]["idDate"];
    String reportFile = "/history/history on " + idDate + "/" + idHistory + ".txt";
    Serial.print("read program from : ");
    Serial.println(reportFile);
    memoryAccsess("read", reportFile, myDataBase["history"]["information"]);
    
  }
//------------------------------------------------------------------------
  if(receivedPackage.hasOwnProperty("openMenu")) {
    openMenu = (bool) receivedPackage["openMenu"];
    Serial.print("Open menu : ");
    Serial.println(openMenu);
  }

//------------------------------------------------------------------------
  if(receivedPackage.hasOwnProperty("adjustTime")) {
    int secondSet = receivedPackage["adjustTime"]["second"];
    int minuteSet = receivedPackage["adjustTime"]["minute"];
    int hourSet = receivedPackage["adjustTime"]["hour"];
    int dayOfWeekSet = receivedPackage["adjustTime"]["day"];
    int dayOfMonthSet = receivedPackage["adjustTime"]["date"];
    int monthSet = receivedPackage["adjustTime"]["month"];
    int yearSet = receivedPackage["adjustTime"]["year"];

    adjustTime(secondSet, minuteSet, hourSet, dayOfWeekSet, dayOfMonthSet, monthSet, yearSet);
    Serial.println("----Time Adjusted-----");
    
  }


//------------------------------------------------------------------------
  if(receivedPackage.hasOwnProperty("newTest")) {
    String idProgram = (const char*) receivedPackage["newTest"]["idProgram"];
    String idTest = generateTestid(idProgram);
    String programPath = "/program/" + idProgram + ".txt";
    memoryAccsess("read", programPath, myDataBase["program"]["dataProgram"]);

    
    myDataBase["program"]["dataProgram"]["test"][idTest]["ipx"] = (const char*) receivedPackage["newTest"]["test"]["ipx"];
    myDataBase["program"]["dataProgram"]["test"][idTest]["hour"] = (int) receivedPackage["newTest"]["test"]["hour"]; 
    myDataBase["program"]["dataProgram"]["test"][idTest]["minute"] = (int) receivedPackage["newTest"]["test"]["minute"];
    myDataBase["program"]["dataProgram"]["test"][idTest]["second"] = (int) receivedPackage["newTest"]["test"]["second"];
    myDataBase["program"]["dataProgram"]["test"][idTest]["total"] = (long) receivedPackage["newTest"]["test"]["total"];
    Serial.print(">>> data program test : ");
    Serial.println(myDataBase["program"]["dataProgram"]["test"]);

    memoryAccsess("write", programPath, myDataBase["program"]["dataProgram"]);
    
//    myDataBase["program"][idProgram]["test"][idTest]["ipx"] = (const char*) receivedPackage["newTest"]["test"]["ipx"];
//    myDataBase["program"][idProgram]["test"][idTest]["hour"] = (int) receivedPackage["newTest"]["test"]["hour"]; 
//    myDataBase["program"][idProgram]["test"][idTest]["minute"] = (int) receivedPackage["newTest"]["test"]["minute"];
//    myDataBase["program"][idProgram]["test"][idTest]["second"] = (int) receivedPackage["newTest"]["test"]["second"];
//    myDataBase["program"][idProgram]["test"][idTest]["total"] = (long) receivedPackage["newTest"]["test"]["total"];
    
  }
//------------------------------------------------------------------------
  if(receivedPackage.hasOwnProperty("editTest")) {
    String idProgram = (const char*) receivedPackage["editTest"]["idProgram"];
    String idTest = (const char*) receivedPackage["editTest"]["idTest"];
    String programPath = "/program/" + idProgram + ".txt";
    memoryAccsess("read", programPath, myDataBase["program"]["dataProgram"]);

    myDataBase["program"]["dataProgram"]["test"][idTest]["ipx"] = (const char*) receivedPackage["editTest"]["test"]["ipx"];
    myDataBase["program"]["dataProgram"]["test"][idTest]["hour"] = (int) receivedPackage["editTest"]["test"]["hour"]; 
    myDataBase["program"]["dataProgram"]["test"][idTest]["minute"] = (int) receivedPackage["editTest"]["test"]["minute"];
    myDataBase["program"]["dataProgram"]["test"][idTest]["second"] = (int) receivedPackage["editTest"]["test"]["second"];
    myDataBase["program"]["dataProgram"]["test"][idTest]["total"] = (long) receivedPackage["editTest"]["test"]["total"];

    memoryAccsess("write", programPath, myDataBase["program"]["dataProgram"]);
    
//    myDataBase["program"][idProgram]["test"][idTest]["ipx"] = (const char*) receivedPackage["editTest"]["test"]["ipx"];
//    myDataBase["program"][idProgram]["test"][idTest]["hour"] = (int) receivedPackage["editTest"]["test"]["hour"]; 
//    myDataBase["program"][idProgram]["test"][idTest]["minute"] = (int) receivedPackage["editTest"]["test"]["minute"];
//    myDataBase["program"][idProgram]["test"][idTest]["second"] = (int) receivedPackage["editTest"]["test"]["second"];
//    myDataBase["program"][idProgram]["test"][idTest]["total"] = (long) receivedPackage["editTest"]["test"]["total"];
  }
//------------------------------------------------------------------------
  if(receivedPackage.hasOwnProperty("run")) {
    String idRunningProgram = (const char*) receivedPackage["run"];
    String currentIdRunningProgram = myDataBase["running"]["idProgram"];
    Serial.print("running program : ");
    Serial.println(idRunningProgram);
    if (currentIdRunningProgram == "") {

      //Adding new date and create new date directory
      myDataBase["history"]["dates"][getFormattedDate()] = getFormattedDate();
      memoryAccsess("write", pathDates, myDataBase["history"]["dates"]);
      
      String startTime = getFormattedTime();
      String dateFolder = "/history/history on " + getFormattedDate();
      createDir(SD, dateFolder);
      String historyListFile = dateFolder + "/idHistoryList.txt";

      //Write first time tracker
      myHistory["ongoingTest"]["timeTracker"] = getFormattedTimeHourAndMinute();
      memoryAccsess("write", pathOngoingTest, myHistory["ongoingTest"]);
      Serial.println("---------Time Tracker-----------");
      
      
      myHistory["ongoingTest"]["idHistory"] = generateHistoryid(historyListFile);
      myHistory["ongoingTest"]["idHistoryList"][(const char*) myHistory["ongoingTest"]["idHistory"]]["nameProgram"] = (const char*) myDataBase["program"]["idProgramList"][idRunningProgram];
      myHistory["ongoingTest"]["idHistoryList"][(const char*) myHistory["ongoingTest"]["idHistory"]]["idProgram"] = idRunningProgram;
      //Write id of running program and their name
      Serial.println("---------------write historyListFile-----------------------");
      memoryAccsess("write", historyListFile, myHistory["ongoingTest"]["idHistoryList"]);
      myHistory["ongoingTest"]["pathData"] = dateFolder + "/" + (const char*) myHistory["ongoingTest"]["idHistory"] + ".txt";
      //Write information of current running progaram
      Serial.println("---------------write pathOngoingTest-----------------------");
      memoryAccsess("write", pathOngoingTest, myHistory["ongoingTest"]);
      
      
      Serial.println("---------------Read myHistory-----------------------");
      memoryAccsess("write", myHistory["ongoingTest"]["pathData"], myHistory["todayTest"]);
      memoryAccsess("read", myHistory["ongoingTest"]["pathData"], myHistory["todayTest"]);
      myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["idProgram"] = idRunningProgram;
      myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["name"] = (const char*) myDataBase["program"]["idProgramList"][idRunningProgram];
      myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["status"] = "Running";
      myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["date"] = getFormattedDate();
      myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["start"] = startTime;
      myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["stop"] = "-";
      myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["logger"] = nullObject;
      Serial.println("---------------write myHistory-----------------------");
      memoryAccsess("write", myHistory["ongoingTest"]["pathData"], myHistory["todayTest"]);

      Serial.print("myHistory content : ");
      Serial.println(myHistory["todayTest"]);
     
      String programPath = "/program/" + idRunningProgram + ".txt";
      memoryAccsess("read", programPath, myDataBase["running"]["dataProgram"]);
      totalTime = initialRemainingTime(idRunningProgram);
      myDataBase["running"]["idProgram"] = idRunningProgram;
      myDataBase["running"]["remainingTime"] = totalTime;

      Serial.print("-------Total Running Time Test : ");
      Serial.println(totalTime);
      
      Serial.print("Total time : ");
      Serial.println(myDataBase["running"]["remainingTime"]);
      
      Serial.print("current test : ");
      Serial.println(myDataBase["running"]["dataProgram"]["test"].keys()[0]);
      
    } else if (idRunningProgram == ""){
      
      stopProgram = true;
    }
    
  }
  
}
//----------------------------------------------------------------------------------


int i = 0;
unsigned long lastElapsedTime = 0;
unsigned long testTime = 0;
unsigned long previousTime = 0;
unsigned long currentTime = 0;
unsigned long elapsedTime = 0;
unsigned long pauseElapsedTime = 0;
unsigned long totalPauseElapsedTime = 0;
unsigned long pausePreviousTime = 0;
unsigned long countdownTime = 0;
const char* currentIdTest = "";
const char* typeTest = "";
const char* ipx5Status = "";
const char* ipx6Status = "";
bool runningStatus = false;
bool initPause = true;
String idLogger = "";


void mainProgram() {
  String currentIdRunningProgram = myDataBase["running"]["idProgram"];
  bool pause = myDataBase["running"]["pause"];
  ipx5Status = myDataBase["ipx5Status"];
  ipx6Status = myDataBase["ipx6Status"];
  if(currentIdRunningProgram != "" && !pause) {
    timeTracker();
    
    JSONVar keys =  myDataBase["running"]["dataProgram"]["test"].keys();

    if(!initPause){
      Serial.print("Paused on ");
      Serial.print(previousTime);
      Serial.println(" second");
      Serial.print("Test paused for ");
      Serial.print(pauseElapsedTime);
      Serial.println(" second");    

      Serial.print("elapsedTime ");
      Serial.print(elapsedTime);
      Serial.println(" second"); 
      Serial.print("testTime ");
      Serial.print(testTime);
      Serial.println(" second"); 

      //History
      idLogger = generateLoggerid();
      myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["logger"][idLogger] = getFormattedTime() + " The test was Resumed";
      myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["status"] = "Running";
      memoryAccsess("write", myHistory["ongoingTest"]["pathData"], myHistory["todayTest"]);
        
      Serial.println("===========Resumed===========");
      totalPauseElapsedTime += pauseElapsedTime;
      pauseElapsedTime = 0;
      relayControl(typeTest);
      initPause = true;
      sendDataBase();
    }
    
    currentTime = millis();
    elapsedTime = ((currentTime - previousTime)/1000) - totalPauseElapsedTime;

    
    
    
    if(i < keys.length()){
      runningStatus = true;
      
      if(elapsedTime >= testTime){
        //send and execute
        Serial.println("=======If condition was satisfied=======");
        lastElapsedTime += testTime;
        //currentIdTest = (const char*) keys[index];
        typeTest = (const char*) myDataBase["running"]["dataProgram"]["test"][keys[i]]["ipx"];
        testTime = myDataBase["running"]["dataProgram"]["test"][keys[i]]["total"];
        myDataBase["running"]["currentIdTest"] = (const char*) keys[i];
        myDataBase["running"]["ipx"] = typeTest;
        relayControl(typeTest);
        previousTime = millis();
        totalPauseElapsedTime = 0;
        Serial.print("running test : ");
        Serial.println(myDataBase["running"]["currentIdTest"]);

        //History
        
        idLogger = generateLoggerid();
        
        myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["logger"][idLogger] = getFormattedTime() + " " + typeTest + " test";
        memoryAccsess("write", myHistory["ongoingTest"]["pathData"], myHistory["todayTest"]);
        
        i ++;
        myDataBase["running"]["progress"] = i;
        myDataBase["running"]["totalTest"] = keys.length();
        elapsedTime = 0;
        myDataBase["running"]["countingTimeCurrentTest"] = (long) elapsedTime;

        
        
        sendDataBase();
      }
      
      
    } else {
      
      if (runningStatus && elapsedTime >= testTime) {
        
        //History
        idLogger = generateLoggerid();
        Serial.println("============finish================");
        myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["logger"][idLogger] = getFormattedTime() + " The test has been completed";
        myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["stop"] = getFormattedTime();
        myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["status"] = "Finish";
        memoryAccsess("write", myHistory["ongoingTest"]["pathData"], myHistory["todayTest"]);

        resetRunningProgram();
        
        sendDataBase();
      }
      
    }

    myDataBase["running"]["countingTimeCurrentTest"] = (long)elapsedTime;
    countdownTime = totalTime - (elapsedTime + lastElapsedTime);
    myDataBase["running"]["remainingTime"] = (long) countdownTime;

    
    
  } else if(pause) {
    if(initPause) {
      pausePreviousTime = millis();
      initPause = false;
      relayControl("off");
      sendDataBase();

      //History
      idLogger = generateLoggerid();
      myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["logger"][idLogger] = getFormattedTime() + " The test was paused";
      myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["status"] = "Paused";
      memoryAccsess("write", myHistory["ongoingTest"]["pathData"], myHistory["todayTest"]);
      Serial.println("===========Paused===========");
    }
    currentTime = millis();
    pauseElapsedTime = (currentTime - pausePreviousTime)/1000; 
  }
  
  if (stopProgram) {
      
      stopProgram = false;
      //History
      idLogger = generateLoggerid();
      myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["logger"][idLogger] = getFormattedTime() + " The test intentionally stopped";
      myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["stop"] = getFormattedTime();
      myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["status"] = "Stopped";
      memoryAccsess("write", myHistory["ongoingTest"]["pathData"], myHistory["todayTest"]);
      Serial.println("===========Stopped===========");
      resetRunningProgram();
      sendDataBase();
  }


  if ((bool)myDataBase["takeOver"] == true) {
    digitalWrite(relayTakeOver, LOW);
  } else {
    digitalWrite(relayTakeOver, HIGH);
  }

  if (strcmp(ipx5Status, "on") == 0) {
    digitalWrite(relayIPX5, LOW);
  } else {
    digitalWrite(relayIPX5, HIGH);

  }

  if (strcmp(ipx6Status, "on") == 0) {
    digitalWrite(relayIPX6, LOW);
  } else {
    digitalWrite(relayIPX6, HIGH);
  }

  if(strcmp(ipx6Status, "on") == 0 || strcmp(ipx5Status, "on") == 0) {
    digitalWrite(relayPump, LOW);
    digitalWrite(relayTable, LOW);
  } else {
    digitalWrite(relayPump, HIGH);
    digitalWrite(relayTable, HIGH);
  }
}

void checkingDiscontinuedAttempt(){
  Serial.println("---------Checking discontinued test----------");
  memoryAccsess("read", pathOngoingTest, myHistory["ongoingTest"]);
  Serial.print("Stopped path : ");
  Serial.println(myHistory["ongoingTest"]["pathData"]);
  Serial.print("Stopped path length : ");
  Serial.println(myHistory["ongoingTest"]["pathData"].keys().length());

  if(myHistory["ongoingTest"]["pathData"] != null) {
    Serial.println("---------Found discontinued test----------");
    memoryAccsess("read", myHistory["ongoingTest"]["pathData"], myHistory["todayTest"]);
    String timeTracker = myHistory["ongoingTest"]["timeTracker"];
    Serial.print("time tracker : ");
    Serial.println(timeTracker);
    myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["status"] = "The test was not completed because the controller stopped working.";
    myHistory["todayTest"][(const char*) myHistory["ongoingTest"]["idHistory"]]["stop"] = "Stopped around " + timeTracker;
    memoryAccsess("write", myHistory["ongoingTest"]["pathData"], myHistory["todayTest"]);
    resetRunningProgram();
  }
}

void resetRunningProgram() {
  i = 0;
  runningStatus = false;
  myDataBase["running"]["idProgram"] = "";
  myDataBase["running"]["ipx"] = "";
  myDataBase["running"]["remainingTime"] = 0;
  myDataBase["running"]["currentIdTest"] = "";
  myDataBase["running"]["countingTimeCurrentTest"] = 0;
  myDataBase["running"]["pause"] = false;
  myDataBase["running"]["dataProgram"] = undefined;
  myDataBase["running"]["progress"] = 0;
  myDataBase["running"]["totalTest"] = 0;

  myHistory["ongoingTest"]["idHistory"] = undefined;
  myHistory["ongoingTest"]["pathData"] = undefined;
  myHistory["ongoingTest"]["idHistory"] = nullObject;
  myHistory["ongoingTest"]["pathData"] = nullObject;
  memoryAccsess("write", pathOngoingTest, myHistory["ongoingTest"]);
      
  lastElapsedTime = 0;
  testTime = 0;
  totalPauseElapsedTime = 0;
  initPause = true;
  relayControl("off");
}

unsigned long prevTimeTracker = 0;
unsigned long elapsedTimeTracker = 0;

void timeTracker(){
  elapsedTimeTracker = millis() - prevTimeTracker;
  if(elapsedTimeTracker > 900000) {
    myHistory["ongoingTest"]["timeTracker"] = getFormattedTimeHourAndMinute();
    memoryAccsess("write", pathOngoingTest, myHistory["ongoingTest"]);
    Serial.println("---------Time Tracker-----------");
    prevTimeTracker = millis();
    
  }
}

void relayControl(String ipx) {
  if(ipx == "IPX5") {
    myDataBase["ipx5Status"] = "on";
    myDataBase["ipx6Status"] = "off";
  } else if(ipx == "IPX6") {
    myDataBase["ipx5Status"] = "off";
    myDataBase["ipx6Status"] = "on";
  } else if(ipx == "off") {
    myDataBase["ipx5Status"] = "off";
    myDataBase["ipx6Status"] = "off";
  }
}


unsigned long prevSendCurrentTime = 0;
unsigned long elapsedSendCurrentTime = 0;
void sendCurrentTime(){
  if(openMenu) {
    elapsedSendCurrentTime = millis() - prevSendCurrentTime;
    if(elapsedSendCurrentTime > 1000) {
      myDataBase["currentTime"] = getFormattedTime();
      sendDataBase();
      prevSendCurrentTime = millis();
      Serial.println("=======send current time======");
    }
  }
}
