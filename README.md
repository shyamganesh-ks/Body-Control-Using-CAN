# Body Control Using CAN

## Overview
This project demonstrates an automotive **Body Control Automation System** using the **Controller Area Network (CAN)** protocol. The system efficiently manages vehicle body functions like indicators and wipers, improving reliability and reducing wiring complexity.

The CAN protocol offers robust communication features, including:
- **Multi-master architecture** for seamless communication between multiple nodes.
- **Error detection and recovery** mechanisms to ensure data integrity.
- **Arbitration control** to prioritize critical messages in high-traffic conditions.

---

## Problem Statement
Automotive body functions require efficient control mechanisms. Traditional methods faced challenges like:
- **Complex wiring**: Leading to increased weight and maintenance difficulties.
- **Communication issues**: Data inconsistency across various vehicle modules.
- **Limited scalability**: Difficult to integrate additional sensors or controllers.

## Solution
This system leverages the **CAN protocol** to:
- Enable seamless communication between vehicle modules.
- Implement **error detection** mechanisms for reliable data exchange.
- Introduce scalability by easily adding new modules without significant wiring changes.

By utilizing the CAN protocol, multiple microcontrollers can communicate over a **two-wire differential bus**, significantly improving system reliability while minimizing wiring complexity.

---

## Hardware Requirements
- **ARM7-based LPC2129 Development Board** (for controller logic)
- **CAN Transceivers** (e.g., MCP2551) for hardware-level signal transmission
- **Sensors** (e.g., Proximity, Temperature) to capture environmental data
- **Actuators** (e.g., Motors, Relays) for physical output control
- **Power Supply Module** for stable voltage regulation

## Software Tools
- **Keil uVision** (for coding and debugging)
- **Flash Magic** (for firmware upload)
- **Proteus** (for simulation and circuit design)
- **CAN Analyzer Tool** (for real-time data monitoring and debugging)

---

## System Architecture
Below is a visual representation of the system's architecture:
```
+---------------+       +---------------+
|   Controller  |       |   Controller  |
|    (Node 1)   |       |    (Node 2)   |
|               |       |               |
|  Sensors/Act. |<----->|  Sensors/Act. |
+---------------+       +---------------+
       |                       |
       |                       |
       +--------CAN BUS--------+
```
### Key Features of Architecture
- Each node is an independent ARM controller equipped with sensors and actuators.
- The CAN Bus acts as a shared communication medium, allowing multiple controllers to exchange data efficiently.
- The system is designed to prioritize critical data through **CAN message arbitration**, ensuring essential commands (e.g., wiper control) are processed first.

---

## Code Implementation

### CAN Initialization
The initialization process sets up the CAN controller for communication:
```c
void can1_init() {
    VPBDIV = 1;  // PCLK = CCLK (default)
    PINSEL1 |= (1 << 18);  // P0.0 as RD1
    PINSEL1 &= ~(1 << 19); // P0.1 as TD1

    C1MOD = 0x01;   // Enter Reset Mode
    AFMR = 0x02;    // Bypass mode (accept all messages)
    C1BTR = 0x001C001D; // Bit timing for 125 Kbps
    C1MOD = 0x00;   // Exit Reset Mode (Normal Mode)
}
```

### CAN Transmission
This function transmits CAN messages with specified parameters:
```c
void can1_tx(u32 id, u32 rtr, u32 dlc, u32 byteA, u32 byteB) {
    C1TID1 = id;
    C1TFI1 = (dlc << 16);
    if (rtr == 0) {
        C1TFI1 &= ~(1 << 30);
        C1TDA1 = byteA;
        C1TDB1 = byteB;
    } else {
        C1TFI1 |= (1 << 30);
    }
    C1CMR = (1 << 0) | (1 << 5);
    while ((C1SR & (1 << 3)) == 0);
}
```

### CAN Reception
This function receives incoming CAN messages and extracts relevant data:
```c
void can1_rx(u32 *id, u32 *rtr, u32 *dlc, u32 *byteA, u32 *byteB) {
    while ((C1SR & 0x01) == 0);
    *id = C1RID;
    *rtr = (C1RFS >> 30) & 1;
    *dlc = (C1RFS >> 16) & 0xF;
    *byteA = C1RDA;
    *byteB = C1RDB;
    C1CMR = (1 << 2);
}
```

---

## How to Run
1. Connect the hardware setup with CAN transceivers and ARM boards.
2. Flash the code using **Keil uVision** and **Flash Magic**.
3. Power the system and observe CAN communication between nodes.
4. Use the **CAN Analyzer Tool** to:
   - Monitor data flow between nodes
   - Verify message content and timing
   - Debug transmission issues or message loss

---

## Key Features
✅ Modular design with multiple ARM controllers for scalability  
✅ Reliable communication using CAN protocol with error detection  
✅ Optimized bit timing to ensure accurate data transmission  
✅ Efficient hardware design with minimal wiring complexity  
✅ Supports **Remote Transmission Requests (RTR)** for real-time data polling  
✅ Designed for scalability by allowing easy integration of additional controllers  

---

## Future Improvements
- Implement **CAN Interrupt Handling** for improved efficiency.
- Add advanced filtering mechanisms for selective data processing.
- Integrate **Error Frame Monitoring** for robust fault diagnosis.
- Develop a **Web Interface Dashboard** for real-time data visualization.
