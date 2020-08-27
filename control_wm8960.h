#ifndef __CONTROL_WM8960_H__
#define __CONTROL_WM8960_H__

#include "AudioControl.h"

#define WM8960_ADDRESS  0x1A //AUDIO CODEC ADDRESS

#define LEFT_INPUT_VOLUME 0X00
#define RIGHT_INPUT_VOLUME 0X01
#define LOUT1_VOLUME 0X02
#define ROUT1_VOLUME 0X03
#define CLOCKING_1 0X04
#define ADC_AND_DAC_CONTROL_1 0X05
#define ADC_AND_DAC_Control_2 0X06
#define DIGITAL_AUDIO_INTERFACE 0X07
#define CLOCKING_2 0X08
#define AUDIO_INTERFACE 0X09
#define LEFT_DAC_VOLUME 0X0A
#define RIGHT_DAC_VOLUME 0X0B
#define RESET 0X0F
#define _3D_CONTROL 0X10
#define ALC1 0X11
#define ALC2 0X12
#define ALC3 0X13
#define NOISE_GATE 0X14 
#define LEFT_ADC_VOLUME 0X15
#define RIGHT_ADC_VOLUME 0X16
#define ADDITIONAL_CONTROL_1 0X17
#define ADDITIONAL_CONTROL_2 0X18
#define POWER_MANAGEMENT_1 0X19
#define POWER_MANAGEMENT_2 0X1A
#define ADDITIONAL_CONTROL_3 0X1B
#define ANTI_POP_1 0X1C
#define ANTI_POP_2 0X1D
#define ADCL_SIGNAL_PATH  0X20
#define ADCR_SIGNAL_PATH  0X21
#define LEFT_OUT_MIX 0X22
#define RIGHT_OUT_MIX 0X25
#define MONO_OUT_MIX_1 0X26
#define MONO_OUT_MIX_2 0X27
#define LOUT2_VOLUME 0X28
#define ROUT2_VOLUME 0X29
#define MONOOUT_VOLUME 0X2A
#define INPUT_BOOST_MIXER_1 0X2B
#define INPUT_BOOST_MIXER_2 0X2C
#define BYPASS_1 0X2D
#define BYPASS_2 0X2E
#define POWER_MANAGEMENT_3 0X2F
#define ADDITIONAL_CONTROL_4 0X30
#define CLASS_D_CONTROL_1 0X31
#define CLASS_D_CONTROL_3 0X33
#define PLL_N 0X34
#define PLL_K_1 0X35
#define PLL_K_2 0X36
#define PLL_K_3 0X37

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
