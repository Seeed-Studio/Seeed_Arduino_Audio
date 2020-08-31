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
  if (! this->Write(RESET, 0x0000))return false;
  delay(100);
  // Serial.println("WM8960 reset completed !!\r\n");
#ifdef DEFAULT_MODE
  uint8_t reg_len = sizeof(wm8960_reg_defaults) / sizeof(wm8960_reg_defaults[0]);
  for(uint8_t i=0 ;i < reg_len; i++)
  {
    this->Write(wm8960_reg_defaults[i][0],wm8960_reg_defaults[i][1]);
  }
#else
  //Set Power Source
  this->Write(POWER_MANAGEMENT_1, 1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<3 | 1<<2 | 1<<1);
  this->Write(POWER_MANAGEMENT_2, 1<<8 | 1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<3 | 1<<0); 
  this->Write(POWER_MANAGEMENT_3, 1<<3 | 1<<2 | 1<<4 | 1<<5);

  //Configure clock
  //MCLK->div1->SYSCLK->DAC/ADC sample Freq = 24MHz(MCLK) / 2*256 = 46.875kHz
  this->Write(CLOCKING_1, 0x0091);
  this->Write(CLOCKING_2, 0x01ca);

  //Configure ADC/DAC
  this->Write(ADC_AND_DAC_CONTROL_1, 1 << 2 | 1 << 1);
  //Configure audio interface
  //I2S format 16 bits word length and set to master mode
  // this->Write(DIGITAL_AUDIO_INTERFACE, 1<<1 | 1<<6);
  this->Write(DIGITAL_AUDIO_INTERFACE, 1<<1);

  //Configure HP_L and HP_R OUTPUTS
  this->Write(LOUT1_VOLUME, 0x007F | 0x0100);  //LOUT1 Volume Set
  this->Write(ROUT1_VOLUME, 0x007F | 0x0100);  //ROUT1 Volume Set
  
  //Configure SPK_RP and SPK_RN
  this->Write(LOUT2_VOLUME, 0x7F | 1<<8 ); //Left Speaker Volume
  this->Write(ROUT2_VOLUME, 0x7F | 1<<8 ); //Right Speaker Volume

  //Enable the OUTPUTS
  this->Write(CLASS_D_CONTROL_1, 0x00F7); //Enable Class D Speaker Outputs
  //Configure DAC volume 
  this->Write(LEFT_DAC_VOLUME, 0X00FF | 0x0100);
  this->Write(RIGHT_DAC_VOLUME, 0x00FF | 0x0100);
  //3D
  //this->Write(0x10, 0x001F);

  // enable Left DAC to Left Output Mixer
  this->Write(LEFT_OUT_MIX, 1<<8);
  this->Write(RIGHT_OUT_MIX, 1<<8);

  // connect LINPUT1 to PGA and set PGA Boost Gain.
  this->Write(ADCL_SIGNAL_PATH, 0X0020 | 1<<8 | 1<<3);
  this->Write(ADCR_SIGNAL_PATH, 0X0020 | 1<<8 | 1<<3);

  // set Input PGA Volume
  this->Write(LEFT_INPUT_VOLUME, 0X0027 | 0X0100 );
  this->Write(RIGHT_INPUT_VOLUME, 0X0027 | 0X0100 );

  // set ADC Volume
  this->Write(LEFT_ADC_VOLUME, 0X00c3 | 0X0100 );
  this->Write(RIGHT_ADC_VOLUME,0X00c3 | 0X0100 );

  // disable bypass switch
  this->Write(BYPASS_1,0x0000);
  this->Write(BYPASS_2,0x0000);

  // enable bypass switch
  // this->Write(BYPASS_1,0x0000 | 1<<7);
  // this->Write(BYPASS_2,0x0000 | 1<<7);
  //enable ALC 
  this->Write(ALC1, 0X007B);
  this->Write(ALC2, 0X0100);
  this->Write(ALC3, 0X0032);

  //speaker select
  this->Write(ADDITIONAL_CONTROL_2, 1<<6 | 0<<5); //0 speaker out 1 headphone out 
  //enable thermal shutdown
  this->Write(ADDITIONAL_CONTROL_1, 0x01C3);
  this->Write(ADDITIONAL_CONTROL_4, 0x0009 | 1<<6);//0x000D,0x0005
  //config f clock to 44100 hz
  this->Write(PLL_N, 0x0037);
  this->Write(PLL_K_1, 0x0086);
  this->Write(PLL_K_2, 0x00C2);
  this->Write(PLL_K_3, 0x0027);
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
  this->Write(LEFT_DAC_VOLUME, (uint8_t)L_volume | 0x0100);
  R_volume = R_volume * 0xff;
  this->Write(RIGHT_DAC_VOLUME, (uint8_t)R_volume | 0x0100);
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
  this->Write(POWER_MANAGEMENT_1, 0);
  this->Write(POWER_MANAGEMENT_2, 0);
  this->Write(POWER_MANAGEMENT_3, 0);
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




