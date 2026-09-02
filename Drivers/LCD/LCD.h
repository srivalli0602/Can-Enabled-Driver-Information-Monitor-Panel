//LCD.h
#include "types.h"
void Write_LCD(u8 data);
void CharLCD(u8 ascii);
void CmdLCD(u8 cmd);
void Init_LCD(void);
void StrLCD(char* str);
void U32LCD(u32 n);
void S32LCD(s32 n);
void BuildCGRAM(u8* p, u32 nbytes,int);
void F32LCD(f32 f, u8 ndp);




