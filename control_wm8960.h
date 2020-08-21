#ifndef __CONTROL_WM8960_H__
#define __CONTROL_WM8960_H__

#include "AudioControl.h"

#define WM8960_ADDRESS  0x1A //AUDIO CODEC ADDRESS
#define Left_DAC_Volume 0x0A
#define Right_DAC_Volume 0x0B
#define Power_Management_1 0x19
#define Power_Management_2 0x1A
#define Power_Management_3 0x2F


//resgister value
static uint16_t WM8960_REG_VAL[56] =
{  
  0x0097, 0x0097, 0x0000, 0x0000, 0x0000, 0x0008, 0x0000, 0x000A,
  0x01C0, 0x0000, 0x00FF, 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x007B, 0x0100, 0x0032, 0x0000, 0x00C3, 0x00C3, 0x01C0,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0100, 0x0100, 0x0050, 0x0050, 0x0050, 0x0050, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0040, 0x0000, 0x0000, 0x0050, 0x0050, 0x0000,
  0x0000, 0x0037, 0x004D, 0x0080, 0x0008, 0x0031, 0x0026, 0x00ED
};

class AudioControlWM8960 : public AudioControl
{
public:
    AudioControlWM8960(void) : i2c_addr(WM8960_ADDRESS) {}
    bool enable(void);
	bool disable(void);
	bool volume(float volume);     // volume 0.0 to 1.0
    bool volume_LR(float L_volume,float R_volume);
	bool inputLevel(float volume) {}  // volume 0.0 to 1.0
	bool inputSelect(int n) {}
private:
    bool Write(uint8_t reg, uint16_t dat);
    uint16_t Read(uint8_t reg);
    uint8_t i2c_addr;
};

#endif
