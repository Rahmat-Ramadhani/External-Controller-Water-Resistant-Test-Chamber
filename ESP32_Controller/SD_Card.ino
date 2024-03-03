
//#include "FS.h"
//#include "SD.h"
//#include "SPI.h"

void initSDCard(){
    if(!SD.begin(32)){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    } else {
      // Initialize main data
      initMemory();
      checkingDiscontinuedAttempt();
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    
}

String objectToString(const JSONVar &myObject, String propertieName){
  JSONVar object;
  object[propertieName] = myObject;
  String result = JSON.stringify(object);
  return result;
}


void createDir(fs::FS &fs, const String &path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path.c_str())){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

int attemptCounter = 0;


String writeFile(fs::FS &fs, const String &path, const String &message){
    Serial.printf("Writing file: %s\n", path);
  
    File file = fs.open(path.c_str(), FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return "";
    }
    if(file.print(message)){
        Serial.println("File written");
        String writtenContent = "";
        writtenContent = readFile(SD, path);
        
        return writtenContent;
    } else {
        Serial.println("Write failed");
        return "";
        
    }
    file.close();
}

String readFile(fs::FS &fs, const String &path){
    //Serial.printf("Reading file: %s\n", path.c_str());
    String readContent = "";
    File file = fs.open(path.c_str());

    while(file.available()){
        //Serial.write(file.read());
        char c = static_cast<char>(file.read());
        readContent += c;
        attemptCounter = 0;
    }
    
    if(!file){
        Serial.println("Failed to open file for reading");
        if(attemptCounter < 3) {
          Serial.println("Try to make a new file");
          attemptCounter++;
          JSONVar nullTopic;
//          JSONVar object;
//          object = nullTopic;
          String textContent = JSON.stringify(nullTopic);
//          String textContent = "";
          readContent = writeFile(SD, path, textContent); 
          
        }else{
          Serial.println("Check your SD Card condition");
          attemptCounter = 0;
        }

        
    }

    //Serial.print("Read from file: ");
    
    
    file.close();

    return readContent;
}





//void renameFile(fs::FS &fs, const String * path1, const String * path2){
//    Serial.printf("Renaming file %s to %s\n", path1.c_str(), path2.c_str());
//    if (fs.rename(path1.c_str(), path2.c_str())) {
//        Serial.println("File renamed");
//    } else {
//        Serial.println("Rename failed");
//    }
//}

void deleteFile(fs::FS &fs, const String &path){
    Serial.printf("Deleting file: %s\n", path.c_str());
    if(fs.remove(path.c_str())){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void deleteDirectory(fs::FS &fs, const String &path){
  // Delete the directory 
    if (fs.rmdir(path.c_str())) {
        Serial.println("Deleted directory: " + path);
    } else {
        Serial.println("Failed to delete directory: " + path);
    }
}

//void initMemory() {
//  createDir(SD, "/program");
//  createDir(SD, "/history");
//
//  String idProgramList = readFile(SD, pathIdProgram, "idProgramList");
//
//  Serial.print("Read content : ");
//  Serial.println(idProgramList);
//  Serial.print("Read content : ");
//  Serial.println(readFile(SD, pathDates, "dates"));
//  JSONVar wadah = JSON.parse(idProgramList);
//  myDataBase["program"]["idProgramList"] = wadah["idProgramList"];
//  myDataBase["history"]["dates"] = JSON.parse(readFile(SD, pathDates, "dates"));
//  Serial.println(JSON.stringify(wadah));
//  
//}




//void setup(){
//    Serial.begin(115200);
//    
//
//    listDir(SD, "/", 0);
//    createDir(SD, "/mydir");
//    listDir(SD, "/", 0);
//    removeDir(SD, "/mydir");
//    listDir(SD, "/", 2);
//    writeFile(SD, "/hello.txt", "Hello ");
//    appendFile(SD, "/hello.txt", "World!\n");
//    readFile(SD, "/hello.txt");
//    deleteFile(SD, "/foo.txt");
//    renameFile(SD, "/hello.txt", "/foo.txt");
//    readFile(SD, "/foo.txt");
//    testFileIO(SD, "/test.txt");
//    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
//    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
//}
//
//void loop(){
//
//}
