#include "communication.h"
#include "fifo.h"
#include "softuart.h"
void Reflect_LIN(LIN_Header header, LIN_Response response)
{
  Push(&sw_transmit, header.pid);
  Push(&sw_transmit, header.size);
  for (int i = 0; i < header.size; ++i)
  {
    Push(&sw_transmit, response.data[i]);
  }
  Push(&sw_transmit, response.CRC);
}