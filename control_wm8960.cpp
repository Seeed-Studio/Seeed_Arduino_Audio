#include "control_wm8960.h"
#include <Arduino.h>
#include "Wire.h"
/**
  * @brief  enable WM8960 voice card.
  * @return true or false.
  */
bool AudioControlWM8960::enable(void)
{
  Wire.begin();

  // Serial.println("WM8960 INIT...");
  //Reset Device
  if (! Write(RESET, 0x0000))return false;
  delay(100);
  // Serial.println("WM8960 reset completed !!\r\n");
#ifdef DEFAULT_MODE
  uint8_t reg_len = sizeof(wm8960_reg_defaults) / sizeof(wm8960_reg_defaults[0]);
  for(uint8_t i=0 ;i < reg_len; i++)
  {
    Write(wm8960_reg_defaults[i][0],wm8960_reg_defaults[i][1]);
  }
#else
  //Set Power Source
  Write(POWER_MANAGEMENT_1, 1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<3 | 1<<2 | 1<<1);
  Write(POWER_MANAGEMENT_2, 1<<8 | 1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<3 | 1<<0); 
  Write(POWER_MANAGEMENT_3, 1<<3 | 1<<2 | 1<<4 | 1<<5);

  //Configure clock
  //MCLK->div1->SYSCLK->DAC/ADC sample Freq = 24MHz(MCLK) / 2*256 = 46.875kHz
  Write(CLOCKING_1, 0x0091);
  Write(CLOCKING_2, 0x01ca);

  //Configure ADC/DAC
  Write(ADC_AND_DAC_CONTROL_1, 1 << 2 | 1 << 1);
  //Configure audio interface
  //I2S format 16 bits word length and set to master mode
  // Write(DIGITAL_AUDIO_INTERFACE, 1<<1 | 1<<6);
  Write(DIGITAL_AUDIO_INTERFACE, 1<<1);

  //Configure HP_L and HP_R OUTPUTS
  Write(LOUT1_VOLUME, 0x007F | 0x0100);  //LOUT1 Volume Set
  Write(ROUT1_VOLUME, 0x007F | 0x0100);  //ROUT1 Volume Set
  
  //Configure SPK_RP and SPK_RN
  Write(LOUT2_VOLUME, 0x7F | 1<<8 ); //Left Speaker Volume
  Write(ROUT2_VOLUME, 0x7F | 1<<8 ); //Right Speaker Volume

  //Enable the OUTPUTS
  Write(CLASS_D_CONTROL_1, 0x00F7); //Enable Class D Speaker Outputs
  //Configure DAC volume 
  Write(LEFT_DAC_VOLUME, 0X00FF | 0x0100);
  Write(RIGHT_DAC_VOLUME, 0x00FF | 0x0100);
  //3D
  //Write(0x10, 0x001F);

  // enable Left DAC to Left Output Mixer
  Write(LEFT_OUT_MIX, 1<<8);
  Write(RIGHT_OUT_MIX, 1<<8);

  // connect LINPUT1 to PGA and set PGA Boost Gain.
  Write(ADCL_SIGNAL_PATH, 0X0020 | 1<<8 | 1<<3);
  Write(ADCR_SIGNAL_PATH, 0X0020 | 1<<8 | 1<<3);

  // set Input PGA Volume
  Write(LEFT_INPUT_VOLUME, 0X0027 | 0X0100 );
  Write(RIGHT_INPUT_VOLUME, 0X0027 | 0X0100 );

  // set ADC Volume
  Write(LEFT_ADC_VOLUME, 0X00c3 | 0X0100 );
  Write(RIGHT_ADC_VOLUME,0X00c3 | 0X0100 );

  // disable bypass switch
  Write(BYPASS_1,0x0000);
  Write(BYPASS_2,0x0000);

  // enable bypass switch
  // Write(BYPASS_1,0x0000 | 1<<7);
  // Write(BYPASS_2,0x0000 | 1<<7);
  //enable ALC 
  Write(ALC1, 0X007B);
  Write(ALC2, 0X0100);
  Write(ALC3, 0X0032);

  //speaker select
  Write(ADDITIONAL_CONTROL_2, 1<<6 | 0<<5); //0 speaker out 1 headphone out 
  //enable thermal shutdown
  Write(ADDITIONAL_CONTROL_1, 0x01C3);
  Write(ADDITIONAL_CONTROL_4, 0x0009 | 1<<6);//0x000D,0x0005
  //config f clock to 44100 hz
  Write(PLL_N, 0x0037);
  Write(PLL_K_1, 0x0086);
  Write(PLL_K_2, 0x00C2);
  Write(PLL_K_3, 0x0027);
#endif
  return true;
}
/** 
 * @brief control the output volume of WM8960.
 * @param L_volume: Left volume ,R_volume : Right volume 0.0 to 1.0.
 * @return true
 */
bool AudioControlWM8960::volume_LR(float L_volume,float R_volume)
{
  L_volume = L_volume * 0xff;
  Write(LEFT_DAC_VOLUME, (uint8_t)L_volume | 0x0100);
  R_volume = R_volume * 0xff;
  Write(RIGHT_DAC_VOLUME, (uint8_t)R_volume | 0x0100);
  return true;
}
/** 
 * @brief control the output volume of WM8960.
 * @param volume: the volume of Left volume and Right volume.
 * @return true
 */
bool AudioControlWM8960::volume(float volume)
{
  volume_LR(volume,volume);
  return true;
}
/** 
 * @brief disable the WM8960.
 * @return true
 */
bool AudioControlWM8960::disable(void)
{
  Write(POWER_MANAGEMENT_1, 0);
  Write(POWER_MANAGEMENT_2, 0);
  Write(POWER_MANAGEMENT_3, 0);
  return true;
}
/** 
 * @brief input select.
 * @param n: input type AUDIO_INPUT_LINEIN,AUDIO_INPUT_MIC
 * @return true or false
 */
bool AudioControlWM8960::inputSelect(int n)
{
  //respekaer cannot support linein input. 
  if (AUDIO_INPUT_LINEIN == n)
    return false;
  return true;
}
/** 
 * @brief enable bypass recommend use headphone to use this function.
 * @param status :true = enable , false = disable default : disbale.
 * @return true or false
 */
bool AudioControlWM8960::bypassContorl(bool status)
{
  // enable bypass switch
  uint16_t reg1,reg2;
  reg1 = Read(BYPASS_1);
  reg2 = Read(BYPASS_2);
  if (status)
  {
    Write(BYPASS_1, reg1 | 1<<7); //enable left input to output
    Write(BYPASS_2, reg2 | 1<<7); //enable right input to output
  }
  else
  {
    Write(BYPASS_1, reg1 & ~(1<<7)); //disable left input to output
    Write(BYPASS_2, reg2 & ~(1<<7)); //disable right input to output   
  }
  return true;
}
/** 
 * @brief control the input volume of WM8960.
 * @param volume: the volume of Left volume and Right volume.
 * @return true
 */
bool AudioControlWM8960::inputLevel(float volume)
{
  volume = volume * 0x3f;
  Write(LEFT_INPUT_VOLUME, (uint8_t)volume | 0X0100 );
  Write(RIGHT_INPUT_VOLUME, (uint8_t)volume | 0X0100 );
  return true;
}
/** 
 * @brief output select.
 * @param Interface: outputInterface type SPEAKER,HEADPHONE
 * @return true or false
 */
bool AudioControlWM8960::outputSelect(outputInterface Interface)
{
  uint16_t reg;
  reg = Read(ADDITIONAL_CONTROL_2);
  if (Interface)
    Write(ADDITIONAL_CONTROL_2, reg | 1<<5);
  else
    Write(ADDITIONAL_CONTROL_2, reg & ~(1<<5));
  return true;
}
/** 
 * @brief Write data to register of wm8960
 * @param reg: register will be write ,dat : write
 * @return true or false
 */
bool AudioControlWM8960::Write(uint8_t reg, uint16_t dat)  {
	Wire.beginTransmission(i2c_addr);
	Wire.write((reg<<1)|((uint8_t)((dat>>8)&0x0001)));
  Wire.write((uint8_t)(dat&0x00FF));
  if (0 == Wire.endTransmission()){
    WM8960_REG_VAL[reg] = dat;
    return true;
  }
  return false;
}

/**
  * @brief  Read register of WM8960.
  * @param  reg: The number of resigter which to be read.
  * @return The value of regsiter.
  */
uint16_t AudioControlWM8960::Read(uint8_t reg) {
    return WM8960_REG_VAL[reg];
}




