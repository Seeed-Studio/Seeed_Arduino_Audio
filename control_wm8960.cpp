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
  if (! this->Write(0x0f, 0x0000))return false;
  delay(100);
  // Serial.println("WM8960 reset completed !!\r\n");

  //Set Power Source
  this->Write(Power_Management_1, 1<<8 | 1<<7 | 1<<6);
  this->Write(Power_Management_2, 1<<8 | 1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<3);
  this->Write(Power_Management_3, 1<<3 | 1<<2);
  //Configure clock
  //MCLK->div1->SYSCLK->DAC/ADC sample Freq = 24MHz(MCLK) / 2*256 = 46.875kHz
  this->Write(Clocking_1, 1<<4);
  
  //Configure ADC/DAC
  this->Write(ADC_and_DAC_Control_1, 1 << 2 | 1 << 1 );
  
  //Configure audio interface
  //I2S format 16 bits word length
  this->Write(0x07, 0x0002);
  
  //Configure HP_L and HP_R OUTPUTS
  this->Write(0x02, 0x006F | 0x0100);  //LOUT1 Volume Set
  this->Write(0x03, 0x006F | 0x0100);  //ROUT1 Volume Set
  
  //Configure SPK_RP and SPK_RN
  this->Write(Left_Speaker_Volume, 0x7F | 1<<8 ); //Left Speaker Volume
  this->Write(Right_Speaker_Volume, 0x7F | 1<<8 ); //Right Speaker Volume
  
  //Enable the OUTPUTS
  this->Write(0x31, 0x00F7); //Enable Class D Speaker Outputs
  
  //Configure DAC volume
  this->Write(0x0a, 0x00FF | 0x0100);
  this->Write(0x0b, 0x00FF | 0x0100);
  
  //3D
//  this->Write(0x10, 0x001F);
  
  //Configure MIXER
  this->Write(Left_Out_Mix, 1<<8);
  this->Write(Right_Out_Mix, 1<<8);
  
  //Jack Detect
  this->Write(0x18, 1<<6 | 0<<5);
  this->Write(0x17, 0x01C3);
  this->Write(0x30, 0x0009);//0x000D,0x0005
  
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
  this->Write(Left_DAC_Volume, (uint8_t)L_volume | 0x0100);
  R_volume = R_volume * 0xff;
  this->Write(Right_DAC_Volume, (uint8_t)R_volume | 0x0100);
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
  this->Write(Power_Management_1, 0);
  this->Write(Power_Management_2, 0);
  this->Write(Power_Management_3, 0);
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




