#include "utils.h"

void printMac(const uint8_t * mac)
{
  for (int i = 0; i < 6; i++) {
    Serial.print(mac[i], HEX);
    if( i!=5)
        Serial.print(":");
  }
}
bool equalMacs(const uint8_t * mac1, const uint8_t * mac2)
{
        // Hopefully the compiler will optimize this a bit...
        
        for(int i=0; i<6; i++)
                if (mac1[i] != mac2[i])
                        return false;
        return true;
}
