#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);


void setup() {
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_16MHZ);
  mcp2515.setNormalMode();
}

uint8_t Utils_ChecksumAdd(const uint8_t *a, int len) {
    int i, sum = 0;
    for (i = 0; i < len; i++) {
        sum += a[i];
    }
    return sum & 0xff;
}

uint8_t uartMsg[13] = {0};
void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    uartMsg[0] = 0xF1;
    uartMsg[1] = 0x05;
    uartMsg[2] = (uint8_t) (canMsg.can_id & 0xFF);
    uartMsg[3] = (uint8_t) (canMsg.can_id >> 8 & 0xFF);
    for (int i = 0; i < 8; i++) {
        uartMsg[i + 4] =canMsg.data[i];
    }
    uartMsg[12] = Utils_ChecksumAdd(uartMsg, 12);
    Serial.write(uartMsg, 13);
  }
}
