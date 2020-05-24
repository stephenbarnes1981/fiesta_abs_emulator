#include <mcp_can.h>
#include <SPI.h>

/*SAMD core*/
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;

int count = 0x00;
int data0 = 0x5D; // set wheel speed hibyte
int data1 = 0x00; // set wheel speed lobyte

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup() {
    SERIAL.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS)) {            // init can bus : baudrate = 500k
        SERIAL.println("CAN BUS Shield init fail");
        SERIAL.println(" Init CAN BUS Shield again");
        delay(100);
    }
    SERIAL.println("CAN BUS Shield init ok!");
}


unsigned char stmp1[8] = {data0, data1, data0, data1, data0, data1, data0, data1};

void loop() {

  if (count < 0xFF){
    count++;}
    else count = 0x00;

  unsigned char stmp[7] = {0xFF, 0xFF, 0x30, 0x28, 0x90, 0x00, count};
  
    // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
    stmp[6] = stmp[6] + 1;
    if (stmp[6] == 100) {
        stmp[6] = 0;
        stmp[5] = stmp[5] + 1;

        if (stmp[5] == 100) {
            stmp[5] = 0;
            stmp[4] = stmp[5] + 1;
        }
    }

    SERIAL.println("sending message: 0x210");

    CAN.sendMsgBuf(0x210, 0, 7, stmp);

    SERIAL.println("message sent");
    
    delay(100);                       // send data per 100ms

    stmp1[7] = stmp1[7] + 1;
    if (stmp1[7] == 100) {
        stmp1[7] = 0;
        stmp1[6] = stmp1[6] + 1;

        if (stmp1[6] == 100) {
            stmp1[6] = 0;
            stmp1[5] = stmp1[6] + 1;
        }
    }

    SERIAL.println("sending message: 0x4B0");
    
    CAN.sendMsgBuf(0x4B0, 0, 8, stmp1);

    SERIAL.println("message sent");

    SERIAL.print("0x");
    
    if (count < 0x10){
      SERIAL.print('0');
    }
    SERIAL.println(count, HEX);
    
    delay(14);                       // send data per 14ms
}

// END FILE
