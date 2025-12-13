# XTX XTSD04GLGEAG SD Card Interface
A complete solution for interfacing the XTX XTSD04GLGEAG SD card chip with Arduino using SPI communication. This project includes PCB design files, Arduino code with a user-friendly serial interface, and comprehensive documentation.

![Project Banner](/Images/Arduino%20SD%20Card%20Chip%20Hacking.png)

## 📋 Features
- **Direct PCB Integration**: Chip can be soldered directly to your PCB
- **Arduino-Compatible**: Works with any Arduino board with SPI
- **User-Friendly Interface**: Serial menu for file operations
- **Plug & Play**: No drivers needed - works like standard SD cards
- **Compact Design**: Breakout board for easy prototyping

## 🛠️ Hardware Requirements
- XTX XTSD04GLGEAG chip
- Arduino board (Uno, Nano, Mega, etc.)
- Custom PCB (files included) or breadboard setup
- SD card reader (for initial testing)

## 📁 Repository Structure
- `/Arduino` - Complete Arduino sketch with serial interface
- `/PCB` - Gerber files, schematic, and BOM for breakout board
- `/Documentation` - Wiring diagrams and assembly guide
- `/Examples` - Additional example sketches

## 🔧 Quick Start
1. **Order PCB**: Use Gerber files in `/PCB/Gerber_Files/`
2. **Assemble**: Solder XTSD chip and SIL connectors
3. **Wire to Arduino**: Follow wiring diagram in `Documentation/`
4. **Upload Code**: Use Arduino IDE with SdFat library
5. **Test**: Open Serial Monitor (9600 baud)

## 📊 Arduino Code Features
- File creation, reading, writing, deletion
- File listing with sizes
- SD card information display
- Automatic reconnection with retry logic
- User-friendly serial menu system

## 🎥 Video Tutorial
Watch the complete build and demo on YouTube: [Link to your video]

## 🤝 Contributing
Feel free to fork, modify, and submit pull requests. Issues and feature requests are welcome!

## 📄 License
MIT License - see LICENSE file for details

## 🙏 Acknowledgments
- XTX for the XTSD04GLGEAG chip
- Bill Greiman for the SdFat Arduino library
- JLCPCB for PCB manufacturing
