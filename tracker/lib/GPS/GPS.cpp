#include <Arduino.h>

// void setFlightMode(byte newMode){
//   // Send navigation configuration command
//   unsigned char setNav[] = {
//     0xB5, 0x62, // Header
//     0x06, 0x24, // Message ID
//     0x24, 0x00,
//     // dynModel, fixMode
//     0xFF, 0xFF, // mask
//     // fixedAlt
//     0x06, 0x03, // fixedAltVar
//     0x00,
//     0x00,
//     0x00, 0x00, 0x10, 0x27,
//     0x00, 0x00, 0x05, 0x00, 0xFA, 0x00,
//     0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00,
//     0x16, 0xDC
//   };

//   setNav[8] = newMode;

//   FixUBXChecksum(setNav, sizeof(setNav));
  
//   SendUBX(setNav, sizeof(setNav));
// }

// void FixUBXChecksum(unsigned char *Message, int Length){ 
//   unsigned char CK_A, CK_B;
 
//   CK_A = 0;
//   CK_B = 0;

//   for (int i=2; i<(Length-2); i++){
//     CK_A = CK_A + Message[i];
//     CK_B = CK_B + CK_A;
//   }
  
//   Message[Length-2] = CK_A;
//   Message[Length-1] = CK_B;
// }

// void SendUBX(unsigned char *Message, int Length){
//   LastCommand1 = Message[2];
//   LastCommand2 = Message[3];
  
//   for (int i=0; i<Length; i++){
//     GPS_SERIAL.write(Message[i]);
//   }
// }