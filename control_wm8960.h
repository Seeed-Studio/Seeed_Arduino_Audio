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
#define ADC_AND_DAC_CONTROL_2 0X06
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
// #define DEFAULT_MODE
/*
 * wm8960 register cache
 * We can't read the WM8960 register space when we are
 * using 2 wire for device control, so we cache them instead.
 */
static const uint16_t wm8960_reg_defaults[][2] = {
	{  0x0, 0x0127 },
	{  0x1, 0x0127 },
	{  0x2, 0x017f },
	{  0x3, 0x017f },
	{  0x4, 0x0091 },
	{  0x5, 0x0000 },
	{  0x6, 0x0000 },
	{  0x7, 0x0002 },
	{  0x8, 0x01ca },
	{  0x9, 0x0000 }, /* ADCLRC/GPIO1 as GPIO pin */
	{  0xa, 0x01ff },
	{  0xb, 0x01ff },

	{ 0x10, 0x0000 },
	{ 0x11, 0x007b },
	{ 0x12, 0x0100 },
	{ 0x13, 0x0032 },
	{ 0x14, 0x0000 },
	{ 0x15, 0x01c3 },
	{ 0x16, 0x01c3 },
	{ 0x17, 0x01c0 },
	{ 0x18, 0x0000 },
	{ 0x19, 0x00c0 },
	{ 0x1a, 0x01f9 },
	{ 0x1b, 0x0000 },
	{ 0x1c, 0x0008 },
	{ 0x1d, 0x0000 },

	{ 0x20, 0x0138 },
	{ 0x21, 0x0138 },
	{ 0x22, 0x0170 },

	{ 0x25, 0x0150 },
	{ 0x26, 0x0000 },
	{ 0x27, 0x0000 },
	{ 0x28, 0x017f },
	{ 0x29, 0x017f },
	{ 0x2a, 0x0040 },
	{ 0x2b, 0x0000 },
	{ 0x2c, 0x0000 },
	{ 0x2d, 0x0070 },
	{ 0x2e, 0x0020 },
	{ 0x2f, 0x000c },
	{ 0x30, 0x0002 },
	{ 0x31, 0x00f7 },

	{ 0x33, 0x00a5 },
	{ 0x34, 0x0037 },
	{ 0x35, 0x0086 },
	{ 0x36, 0x00c2 },
	{ 0x37, 0x0027 },
};
typedef enum
{
	SPEAKER = 0,
	HEADPHONE
}outputInterface;

class AudioControlWM8960 : public AudioControl
{
public:
    AudioControlWM8960(void) : i2c_addr(WM8960_ADDRESS) {}
    bool enable(void);
	bool disable(void);
	bool volume(float volume);     // volume 0.0 to 1.0
    bool volume_LR(float L_volume,float R_volume);
	bool inputLevel(float volume);  // volume 0.0 to 1.0
	bool inputSelect(int n);
	bool outputSelect(outputInterface Interface);
	bool bypassContorl(bool status);
private:
    bool Write(uint8_t reg, uint16_t dat);
    uint16_t Read(uint8_t reg);
    uint8_t i2c_addr;
};

#endif
