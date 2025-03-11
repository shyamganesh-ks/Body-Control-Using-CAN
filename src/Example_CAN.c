/* CAN Initialization */
void can1_init() {

        VPBDIV = 1;  // Set PCLK = CCLK (default system clock configuration)
    
    /* Configure P0.0 as RD1 (CAN Receive) and P0.1 as TD1 (CAN Transmit) */
        PINSEL1 |= (1 << 18);  // Set P0.0 as RD1 (CAN Receive)
        PINSEL1 &= ~(1 << 19); // Clear P0.1 to ensure it's set as TD1 (CAN Transmit)

    /* Step 1: Enter CAN Reset Mode (Required for Configuration) */
        C1MOD = 0x01;  // Enter Reset Mode

    /* Step 2: Acceptance Filter Mode (AFMR) — Disable filtering (Bypass mode) */
        AFMR = 0x02;  // Bypass mode to accept all messages (for testing or simple communication)

    /* Step 3: Bit Timing Configuration for 125 Kbps */
        C1BTR = 0x001C001D;  // Bit timing settings for 125 Kbps (based on clock settings)

    /* Step 4: Enable CAN Interrupt (Optional) */
    // C1IER = 0x01; // Enable CAN1 RX Interrupt (optional if interrupt handling is required)

    /* Step 5: Exit Reset Mode (Enable Normal Mode) */
        C1MOD = 0x00;  // Normal Operation Mode
}

/* CAN Transmission */
void can1_tx(u32 id, u32 rtr, u32 dlc, u32 byteA, u32 byteB) {

    /* Step 1: Load Identifier */
        C1TID1 = id;  // Set CAN ID

    /* Step 2: Configure Frame Information */
        C1TFI1 = (dlc << 16);  // Set Data Length Code (DLC)

    /* Step 3: Frame Type Selection */
        if (rtr == 0) {           // Data frame
            C1TFI1 &= ~(1 << 30); // Clear RTR bit (Data Frame)
            C1TDA1 = byteA;       // Load data byte A
            C1TDB1 = byteB;       // Load data byte B
        } 
        else 
            C1TFI1 |= (1 << 30);  // Set RTR bit (Remote Frame)

    /* Step 4: Send the Frame */
        C1CMR = (1 << 0) | (1 << 5);  // TR = 1 (Transmission Request) | AT = 1 (Abort if failed)

    /* Step 5: Wait for Successful Transmission */
        while ((C1SR & (1 << 3)) == 0);  // Wait until TX buffer is empty
}

/* CAN Reception */
void can1_rx(u32 *id, u32 *rtr, u32 *dlc, u32 *byteA, u32 *byteB) {
    /* Step 1: Wait for Data in RX Buffer */
        while ((C1SR & 0x01) == 0);  // Wait until a message is received

    /* Step 2: Read Received Message Details  */
        *id = C1RID;            // Store received CAN ID
        *rtr = (C1RFS >> 30) & 1; // Extract RTR bit (1 = Remote, 0 = Data)
        *dlc = (C1RFS >> 16) & 0xF; // Extract DLC (Data Length Code)
        *byteA = C1RDA;         // Extract data byte A
        *byteB = C1RDB;         // Extract data byte B

    /* Step 3: Release RX Buffer (Ready to receive next message) */
        C1CMR = (1 << 2);  // Release RX buffer
}