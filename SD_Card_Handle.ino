#include <SPI.h>
#include <SdFat.h>

SdFat sd;
SdFile file;

// SD card chip select pin
const uint8_t chipSelect = SS;

// Function declarations
bool initializeSDCard();
void printMenu();
void listFiles();
void createFile();
void writeToFile();
void readFile();
void deleteFile();
void getCardInfo();
String readSerialInput(char endChar = '\n');


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  Serial.println(F("╔═════════════════════════╗"));
  Serial.println(F("║        XTX XTSD04GLGEAG SD Card         ║"));
  Serial.println(F("║           Interface System              ║"));
  Serial.println(F("║       --* BY DIY GUY Chris *--          ║"));
  Serial.println(F("╚═════════════════════════╝"));
  Serial.println();
  
  // Initialize SD card
  if (!initializeSDCard()) {
    Serial.println(F("┌─────────────────────────┐"));
    Serial.println(F("│          🚫 SD CARD UNAVAILABLE        │"));
    Serial.println(F("│          System cannot continue        │"));
    Serial.println(F("└─────────────────────────┘"));
    Serial.println(F("💡 Please:"));
    Serial.println(F("   1. Check hardware connections"));
    Serial.println(F("   2. Restart the Arduino"));
    Serial.println(F("   3. Ensure SD card is formatted properly"));
    while(true) {
      // Hang here since SD card is essential
      delay(1000);
    }
  }
  
  Serial.println(F("┌─────────────────────────┐"));
  Serial.println(F("│          ✅ SD CARD READY FOR USE       │"));
  Serial.println(F("└─────────────────────────┘"));
  Serial.println();
  printMenu();
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    
    switch (command) {
      case '1':
        Serial.println(F("\n📁 You selected: List all files"));
        listFiles();
        break;
      case '2':
        Serial.println(F("\n📄 You selected: Create a new file"));
        createFile();
        break;
      case '3':
        Serial.println(F("\n✏  You selected: Write to a file"));
        writeToFile();
        break;
      case '4':
        Serial.println(F("\n👁  You selected: Read from a file"));
        readFile();
        break;
      case '5':
        Serial.println(F("\n🗑  You selected: Delete a file"));
        deleteFile();
        break;
      case '6':
        Serial.println(F("\n📋 You selected: Show menu"));
        printMenu();
        break;
      case '7':
        Serial.println(F("\n💾 You selected: Show SD card info"));
        getCardInfo();
        break;
      case '8':
        Serial.println(F("\n🔄 You selected: Format operations"));
        formatOperations();
        break;
      default:
        if (command != '\n' && command != '\r') {
          Serial.println(F("\n❌ Invalid option! Please choose 1-8"));
          printMenu();
        }
        break;
    }
  }
}

bool initializeSDCard() {
  int retryCount = 0;
  const int maxRetries = 3;
  
  while (retryCount < maxRetries) {
    if (sd.begin(chipSelect, SPI_FULL_SPEED)) {
      return true; // Success
    }
    
    retryCount++;
    
    Serial.println(F("┌─────────────────────────┐"));
    Serial.println(F("│          ❌ INITIALIZATION FAILED       │"));
    Serial.println(F("└─────────────────────────┘"));
    Serial.println(F("🔍 Please check:"));
    Serial.println(F("   1. 📋 SD card is properly inserted"));
    Serial.println(F("   2. 🔌 Chip select pin is correct"));
    Serial.println(F("   3. ⚡ Wiring is proper"));
    Serial.println(F("   4. 🔄 Power cycle the device"));
    Serial.println();
    
    if (retryCount < maxRetries) {
      Serial.print(F("🔄 Attempt "));
      Serial.print(retryCount + 1);
      Serial.print(F(" of "));
      Serial.print(maxRetries);
      Serial.println(F(" - Retrying in 3 seconds..."));
      delay(3000);
    }
  }
  
  // If we get here, all retries failed
  Serial.println();
  Serial.print(F("🔄 Would you like to retry connection? (y/n): "));
  
  String response = readSerialInput();
  response.toLowerCase();
  
  if (response == "y" || response == "yes") {
    Serial.println(F("🔄 Retrying SD card initialization..."));
    Serial.println();
    return initializeSDCard(); // Recursive retry
  }
  
  return false;
}

String readSerialInput(char endChar) {
  String input = "";
  unsigned long startTime = millis();
  
  while (millis() - startTime < 30000) { // 30 second timeout
    if (Serial.available()) {
      char c = Serial.read();
      
      if (c == endChar) {
        break;
      }
      
      if (c == '\b' || c == 127) { // Handle backspace
        if (input.length() > 0) {
          input.remove(input.length() - 1);
          Serial.print(F("\b \b")); // Erase the character
        }
      } else if (c >= 32 && c <= 126) { // Printable characters
        input += c;
        Serial.print(c); // Echo the character
      }
    }
  }
  
  Serial.println(); // New line after input
  return input;
}

void printMenu()
{
  Serial.println(F("╔═════════════════════════╗"));
  Serial.println(F("║              📋 MAIN MENU               ║"));
  Serial.println(F("╠═════════════════════════╣"));
  Serial.println(F("║  1 📁  List all files                   ║"));
  Serial.println(F("║  2 📄  Create a new file                ║"));
  Serial.println(F("║  3 ✏  Write to a file                  ║"));
  Serial.println(F("║  4 👁  Read from a file                 ║"));
  Serial.println(F("║  5 🗑  Delete a file                    ║"));
  Serial.println(F("║  6 📋  Show this menu                   ║"));
  Serial.println(F("║  7 💾  Show SD card info                ║"));
  Serial.println(F("║  8 🔄  Format operations                ║"));
  Serial.println(F("╠═════════════════════════╣"));
  Serial.println(F("║     💡 Tip: Use backspace to edit       ║"));
  Serial.println(F("╚═════════════════════════╝"));
  Serial.print(F("🎯 Enter your choice (1-8): "));
}

void listFiles() {
  Serial.println(F("\n┌────────────────────────┐"));
  Serial.println(F("│             📁 FILE LISTING            │"));
  Serial.println(F("└────────────────────────┘"));
  Serial.println(F("Files Name: "));
  Serial.println(F("─────────────────────────"));
  sd.ls(LS_R);
  Serial.println(F("─────────────────────────"));
  Serial.println(F("✅ Listing completed!"));
  printMenu();
}

void createFile() {
  Serial.println(F("\n┌────────────────────────┐"));
  Serial.println(  F("│            📄 CREATE NEW FILE          │"));
  Serial.println(  F("└────────────────────────┘"));
  
  Serial.println(F("📋 Available files:"));
  Serial.println(F("─────────────────────────"));
  sd.ls(LS_R);
  Serial.println(F("─────────────────────────"));
  
  Serial.print(F("💡 Enter filename to create: "));
  String filename = readSerialInput();
  
  if (filename.length() == 0) {
    Serial.println(F("❌ Error: No filename provided!"));
    printMenu();
    return;
  }
  
  // Add .txt extension if not present
  if (!filename.endsWith(".txt")) {
    filename += ".txt";
  }
  
  if (sd.exists(filename.c_str())) {
    Serial.println(F("❌ Error: File already exists!"));
  } else {
    if (file.open(filename.c_str(), O_CREAT | O_WRITE)) {
      file.close();
      Serial.println(F("✅ File created successfully!"));
      Serial.print(F("📁 Created: "));
      Serial.println(filename);
    } else {
      Serial.println(F("❌ Error: Failed to create file!"));
    }
  }
  
  printMenu();
}

void writeToFile() {
  Serial.println(F("\n┌─────────────────────────┐"));
  Serial.println(  F("│             ✏  WRITE TO FILE           │"));
  Serial.println(  F("└─────────────────────────┘"));
  
  Serial.println(F("📋 Available files:"));
  Serial.println(F("─────────────────────────"));
  sd.ls(LS_R);
  Serial.println(F("─────────────────────────"));
  
  Serial.print(F("📝 Enter filename to write to: "));
  String filename = readSerialInput();
  
  if (filename.length() == 0) {
    Serial.println(F("❌ Error: No filename provided!"));
    printMenu();
    return;
  }
  
  // Add .txt extension if not present
  if (!filename.endsWith(".txt")) {
    filename += ".txt";
  }
  
  if (!sd.exists(filename.c_str())) {
    Serial.println(F("❌ Error: File doesn't exist!"));
    printMenu();
    return;
  }
  
  Serial.println(F("💬 Enter text to write (type # on new line to finish):"));
  Serial.println(F("─────────────────────────"));
  String text = readSerialInput('#');
  
  if (file.open(filename.c_str(), O_WRITE | O_APPEND)) {
    file.println(text);
    file.close();
    Serial.println(F("─────────────────────────"));
    Serial.println(F("✅ Text written successfully!"));
    Serial.print(F("📁 Updated: "));
    Serial.println(filename);
  } else {
    Serial.println(F("❌ Error: Failed to open file for writing!"));
  }
  
  printMenu();
}

void readFile() {
  Serial.println(F("\n┌─────────────────────────┐"));
  Serial.println(  F("│            👁  READ FROM FILE           │"));
  Serial.println(  F("└─────────────────────────┘"));
  
  Serial.println(F("📋 Available files:"));
  Serial.println(F("─────────────────────────"));
  sd.ls(LS_R);
  Serial.println(F("─────────────────────────"));
  
  Serial.print(F("🔍 Enter filename to read: "));
  String filename = readSerialInput();
  
  if (filename.length() == 0) {
    Serial.println(F("❌ Error: No filename provided!"));
    printMenu();
    return;
  }
  
  // Add .txt extension if not present
  if (!filename.endsWith(".txt")) {
    filename += ".txt";
  }
  
  if (!sd.exists(filename.c_str())) {
    Serial.println(F("❌ Error: File doesn't exist!"));
    printMenu();
    return;
  }
  
  Serial.println(F("\n┌─────────────────────────┐"));
  Serial.println(  F("│             📖 FILE CONTENT             │"));
  Serial.println(  F("└─────────────────────────┘"));
  
  if (file.open(filename.c_str(), O_READ)) {
    int data;
    int lineCount = 0;
    while ((data = file.read()) >= 0) {
      Serial.write(data);
      if (data == '\n') lineCount++;
    }
    file.close();
    Serial.println(F("\n─────────────────────────"));
    Serial.println(F("✅ End of file reached"));
    Serial.print(F("📊 Lines read: "));
    Serial.println(lineCount);
  } else {
    Serial.println(F("❌ Error: Failed to open file for reading!"));
  }
  
  printMenu();
}

void deleteFile() {
  Serial.println(F("\n┌─────────────────────────┐"));
  Serial.println(  F("│             🗑  DELETE FILE              │"));
  Serial.println(  F("└─────────────────────────┘"));
  
  Serial.println(F("📋 Available files:"));
  Serial.println(F("─────────────────────────"));
  sd.ls(LS_R);
  Serial.println(F("─────────────────────────"));
  
  Serial.print(F("⚠️  Enter filename to delete: "));
  String filename = readSerialInput();
  
  if (filename.length() == 0) {
    Serial.println(F("❌ Error: No filename provided!"));
    printMenu();
    return;
  }
  
  // Add .txt extension if not present
  if (!filename.endsWith(".txt")) {
    filename += ".txt";
  }
  
  if (!sd.exists(filename.c_str())) {
    Serial.println(F("❌ Error: File doesn't exist!"));
  } else {
    Serial.print(F("🔒 Are you sure you want to delete '"));
    Serial.print(filename);
    Serial.print(F("'? (y/n): "));
    String confirmation = readSerialInput();
    
    if (confirmation == "y" || confirmation == "Y") {
      if (sd.remove(filename.c_str())) {
        Serial.println(F("✅ File deleted successfully!"));
      } else {
        Serial.println(F("❌ Error: Failed to delete file!"));
      }
    } else {
      Serial.println(F("🔒 Deletion cancelled!"));
    }
  }
  
  printMenu();
}

void getCardInfo() {
  Serial.println(F("\n┌─────────────────────────┐"));
  Serial.println(  F("│            💾 SD CARD INFO              │"));
  Serial.println(  F("└─────────────────────────┘"));
  
  cid_t cid;
  if (sd.card()->readCID(&cid)) {
    Serial.println(F("📇 Card Identification Data:"));
    Serial.println(F("─────────────────────────"));
    Serial.print(F("🏭 Manufacturer ID: 0x"));
    Serial.println(cid.mid, HEX);
    Serial.print(F("🔧 OEM ID: "));
    Serial.print(char(cid.oid[0]));
    Serial.println(char(cid.oid[1]));
    Serial.print(F("📦 Product: "));
    for (uint8_t i = 0; i < 5; i++) {
      Serial.print(char(cid.pnm[i]));
    }
    Serial.println();
    Serial.print(F("🔄 Version: "));
    Serial.print(cid.prvN());
    Serial.print(F("."));
    Serial.println(cid.prvM());
    Serial.print(F("🔢 Serial number: 0x"));
    Serial.println(cid.psn(), HEX);
    Serial.println(F("─────────────────────────"));
  }
  
  uint32_t totalSectors = sd.card()->sectorCount();
  uint32_t freeClusters = sd.vol()->freeClusterCount();
  uint32_t sectorsPerCluster = sd.vol()->sectorsPerCluster();
  
  Serial.print(F("💾 Total capacity: "));
  Serial.print(totalSectors * 512E-9);
  Serial.println(F(" GB"));
  
  Serial.print(F("📊 Used space: "));
  Serial.print((totalSectors - (freeClusters * sectorsPerCluster)) * 512E-9);
  Serial.println(F(" GB"));
  
  Serial.print(F("💿 Free space: "));
  Serial.print(freeClusters * sectorsPerCluster * 512E-9);
  Serial.println(F(" GB"));
  
  Serial.print(F("📈 Usage: "));
  Serial.print(100.0 - ((freeClusters * sectorsPerCluster * 100.0) / totalSectors));
  Serial.println(F(" %"));
  
  Serial.println(F("✅ Card info retrieved successfully!"));
  printMenu();
}

void formatOperations() {
  Serial.println(F("\n┌─────────────────────────┐"));
  Serial.println(  F("│           🔄 FORMAT OPERATIONS          │"));
  Serial.println(  F("└─────────────────────────┘"));
  
  Serial.println(F("⚠ Warning: These operations modify the file system"));
  Serial.println(F("📋 Available format operations:"));
  Serial.println(F("─────────────────────────"));
  Serial.println(F("1. 📊 Show file system status"));
  Serial.println(F("2.    Quick format (delete all files)"));
  Serial.println(F("3. ↩ Back to main menu"));
  Serial.println(F("─────────────────────────"));
  Serial.print(F("🎯 Enter your choice (1-3): "));
  
  String choice = readSerialInput();
  
  if (choice == "1") {
    showFileSystemStatus();
  } else if (choice == "2") {
    quickFormat();
  } else if (choice == "3") {
    Serial.println(F("↩ Returning to main menu..."));
    printMenu();
  } else {
    Serial.println(F("❌ Invalid choice!"));
    printMenu();
  }
}

void showFileSystemStatus() {
  Serial.println(F("\n┌─────────────────────────┐"));
  Serial.println(F("│          📊 FILE SYSTEM STATUS          │"));
  Serial.println(F("└─────────────────────────┘"));
  
  uint32_t clusterCount = sd.vol()->clusterCount();
  uint32_t freeClusters = sd.vol()->freeClusterCount();
  uint32_t usedClusters = clusterCount - freeClusters;
  
  Serial.print(F("📁 Total clusters: "));
  Serial.println(clusterCount);
  Serial.print(F("✅ Used clusters: "));
  Serial.println(usedClusters);
  Serial.print(F("💿 Free clusters: "));
  Serial.println(freeClusters);
  Serial.print(F("📈 Usage: "));
  Serial.print((usedClusters * 100.0) / clusterCount);
  Serial.println(F(" %"));
  
  Serial.println(F("\n📋 Files on card:"));
  Serial.println(F("─────────────────────────"));
  sd.ls(LS_R | LS_SIZE);
  Serial.println(F("─────────────────────────"));
  
  printMenu();
}

void quickFormat() {
  Serial.print(F("🚨 ARE YOU SURE? This will delete ALL files! (y/n): "));
  String confirmation = readSerialInput();
  
  if (confirmation == "y" || confirmation == "Y") {
    Serial.println(F("🔄 Deleting all files..."));
    
    SdFile root;
    root.openRoot(sd.vol());
    
    SdFile file;
    char fileName[13];
    int deletedCount = 0;
    
    while (file.openNext(&root, O_READ)) {
      file.getName(fileName, sizeof(fileName));
      file.close();
      
      if (sd.remove(fileName)) {
        Serial.print(F("✅ Deleted: "));
        Serial.println(fileName);
        deletedCount++;
      }
    }
    
    root.close();
    Serial.println(F("─────────────────────────"));
    Serial.print(F("🎉 Format completed! Deleted "));
    Serial.print(deletedCount);
    Serial.println(F(" files."));
  } else {
    Serial.println(F("🔒 Format cancelled!"));
  }
  
  printMenu();
}
