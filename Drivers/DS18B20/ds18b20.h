#ifndef _DS18B20_H_
#define _DS18B20_H_

/**DS18B20 FUNCTION DECLARATION**/
unsigned char ResetDS18b20(void);
unsigned char ReadBit(void);
void WriteBit(unsigned char);
unsigned char ReadByte(void);
void WriteByte(unsigned char);
int ReadTemp(void);

#endif
