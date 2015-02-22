/* Our version connecting Edison to the Wunderbar Bridge Module
** Author: Ronnie Nigash and Grayson Sinclair
*/

#include "Arduino.h"
#include "WunderbarBridge.h"


//#include <SoftwareSerial.h>
//#define BRIDGE_DEBUG true


//SoftwareSerial mySerial(10, 11);

Bridge::Bridge(){
      useDebugOutput = false;
}

Bridge::Bridge(uint8_t rx_pin, uint8_t tx_pin, int32_t baudrate){
      _baudrate = baudrate;
      _rx_pin = rx_pin;
      _tx_pin = tx_pin;
      useDebugOutput = true;
//      SoftwareSerial mySerial(_rx_pin, _tx_pin); // RX (default 10), TX (default 11)*/
}

//------------------------------------------------------------------------------
/**
 *    Initialize Serial port and print welcome message
 *
 * \param
 *
 * \return True if a Bridge Connection was detected
 *         False if no answer received from the Bridge
 */
bool Bridge::begin()
{
  /* 115200 is the default baudrate for the Bridge
    (could be changed in the BLE Config char)*/
  //Serial.begin(115200);

  if (useDebugOutput){
    //SoftwareSerial mySerial(_rx_pin, _tx_pin); // RX (default 10), TX (default 11)
    //initialize Soft Serial UART:
    //mySerial.begin(_baudrate);

  	//mySerial.println("\n\r\n\r------------------------------- \
                          \n\r relayr - bring things to life  \
                          \n\r Arduino / WunderBar-Bridge lib \
                          \n\r-------------------------------\n\r");
  }
  return checkConnection();
}
//------------------------------------------------------------------------------
/**
 * Sends a packet over the Hardware Serial port (TX)
 *
 * \param: byte array with the payload and payload size
 *
 * \return
 */
bool Bridge::sendData(uint8_t payload[], int size)
{
  //prepares the packet
  bridge_comm_t outPacket = createUpPacket(payload, sizeof(payload), outputBuffer);

  if (!_bridgeConnected){
    if (!checkConnection()) return false;
  }

   // memcpy((char *) &outPacket, outputBuffer, sizeof(outPacket));//outPacket.length + 4);
  if (useDebugOutput){
    //mySerial.println("\n\r==>>> OutPacket:");
    dumpPacket(outPacket);

    //mySerial.print("output Buffer: ");
    for (char i=0; i < outPacket.length+4; i++)
    {
      //mySerial.print(outputBuffer[i], HEX);
      //mySerial.print(",");
    }
  }
  //sends the packet to the UART
  //Serial.write((uint8_t *)outputBuffer, outPacket.length + 4);

  return true;
}

//------------------------------------------------------------------------------
/**
 *    Calculate a CRC (16 bits) of a packet
 *
 * \params: pointer to the data, data size, initialization word (NULL by default)
 *
 * \return: the calculated CRC
 */
uint16_t Bridge::crc16Compute(uint8_t * p_data, int size, uint8_t * p_crc)
{
  uint32_t i;
  uint16_t crc = (p_crc == NULL) ? 0xffff : *p_crc;

  for (i = 0; i < size; i++)
  {
    crc = (uint8_t)(crc >> 8) | (crc << 8);
    crc ^= p_data[i];
    crc ^= (uint8_t)(crc & 0xff) >> 4;
    crc ^= (crc << 8) << 4;
    crc ^= ((crc & 0xff) << 4) << 1;
  }
  return crc;
}

//------------------------------------------------------------------------------
/**
 *   Creates an Up Data Packet
 *
 * \params: pointer to the payload, payload length, pointer to the output buffer.
 *
 * \return: the bridge_comm object
 */
bridge_comm_t Bridge::createUpPacket(uint8_t * payload, int length, uint8_t * outBuffer)
{
  bridge_comm_t packet;

  packet.command = BRIDGE_COMM_WRITE_UP_CHANNEL;
  packet.length = length;

  memcpy(packet.payload, payload, length);

  outBuffer[0] = packet.command;
  outBuffer[1] = packet.length;

  for (char i = 2; i < length + 2; i++)
  {
    outBuffer[i] = packet.payload[i-2];
  }

  packet.crc = crc16Compute(outBuffer, length + 2, NULL);

  outBuffer[length + 2] = (packet.crc & 0xff);
  outBuffer[length + 3] = (packet.crc >> 8);

  return packet;
}

//------------------------------------------------------------------------------
/**
 *   Returns a struct with received payload
 * \param: A pointer to the array where to put the data
 *
 * \return none
 */
bridge_payload_t Bridge::getData(void)
{
  bridge_payload_t rx_payload;

  memcpy(rx_payload.payload, down.channel.payload, down.channel.length);
  rx_payload.length = down.channel.length;
  newData = false;

  return rx_payload;
}

bool Bridge::checkConnection()
{
  if (!commandReceived || (down.channel.command != BRIDGE_COMM_ACK))
  {
  _bridgeConnected = false;
   return false;
  }
  _bridgeConnected = true;
  return true;
}
