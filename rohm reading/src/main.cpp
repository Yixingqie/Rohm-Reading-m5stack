#include <Arduino.h>
#include <M5Stack.h>
const uint8_t RPR_ADDRESS = 0x38;

//function declarations
void writeReg(uint8_t address, uint8_t reg, uint8_t value);
uint16_t readReg16Bit(uint8_t address, uint8_t reg);


void setup()
{
  Serial.begin(9600);
  m5.begin();
  m5.Speaker.mute();
  m5.lcd.begin();
  m5.lcd.setTextSize(4);
  Wire.begin();
  
  //sets up the correct registers in the rohm sensor
  writeReg(RPR_ADDRESS, 0x43, 0b100000);
  writeReg(RPR_ADDRESS, 0x42, 0b000011);
  writeReg(RPR_ADDRESS, 0x41, 0b11101011);
  m5.lcd.setBrightness(255);
}

void loop()
{
  m5.lcd.clearDisplay();
  m5.update();
  uint16_t val = readReg16Bit(RPR_ADDRESS, 0x44);
  m5.lcd.setCursor(0, 0);
  m5.lcd.setTextSize(3);
  m5.lcd.print("PROX ADC: ");
  m5.lcd.println(val);
  m5.lcd.println();
  m5.lcd.print("IR ADC: ");
  val = readReg16Bit(RPR_ADDRESS, 0x48);
  m5.lcd.println(val);
  m5.lcd.println();
  m5.lcd.print("VIS ADC: ");
  val = readReg16Bit(RPR_ADDRESS, 0x46);
  m5.lcd.print(val);
  delay(100);

}


void writeReg(uint8_t address, uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}




uint16_t readReg16Bit(uint8_t address, uint8_t reg)
{
  uint16_t value;

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(address, (uint8_t)2);
  value = Wire.read();                 // value low byte
  value |= (uint16_t)Wire.read() << 8; // value high byte

  return value;
}
