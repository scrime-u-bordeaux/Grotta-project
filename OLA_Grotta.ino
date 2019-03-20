
#include <Audio.h>
#include <i2c_t3.h>
//#include <SPI.h>
#include <SD.h>
//#include <SerialFlash.h>
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav;     //xy=429,270
AudioAmplifier           amp1;           //xy=634,233
AudioAmplifier           amp2;           //xy=635,323
AudioOutputAnalogStereo  dacs;          //xy=842,302
AudioConnection          patchCord1(playSdWav, 0, amp1, 0);
AudioConnection          patchCord2(playSdWav, 1, amp2, 0);
AudioConnection          patchCord3(amp1, 0, dacs, 0);
AudioConnection          patchCord4(amp2, 0, dacs, 1);
// GUItool: end automatically generated code

// Use these with the Teensy Audio Shield
//#define SDCARD_CS_PIN    10
//#define SDCARD_MOSI_PIN  7
//#define SDCARD_SCK_PIN   14

// Use these with the Teensy 3.5 & 3.6 SD card
#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  11  // not actually used
#define SDCARD_SCK_PIN   13  // not actually used

// Use these for the SD+Wiz820 or other adaptors
//#define SDCARD_CS_PIN    4
//#define SDCARD_MOSI_PIN  11
//#define SDCARD_SCK_PIN   13

#define DEBUG
float lightsens;
float vol;

void setup() {
  Serial.begin(9600);
  AudioMemory(10);
  pinMode(13, OUTPUT);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  while (!(SD.begin(SDCARD_CS_PIN))) {
    digitalWrite(13, LOW);
    delay (100);
    digitalWrite(13, HIGH);
    delay (100);
  }
}

void loop() {
  lightsens = analogRead(A9);
  vol = map((float) lightsens, 400, 1023, 0, 1000);
  vol = vol / 1000;
  if (vol < 0.0) vol = 0.0;
  amp1.gain(vol);
  amp2.gain(vol);
#ifdef DEBUG
  Serial.println(lightsens);
#endif
  if (playSdWav.isPlaying() == false ) { //&& freeze == 0
    playSdWav.play("piste01.wav");
    delay(5); // brief delay for the library read WAV info
  }
  //playSdWav.stop();
  delay(50);
}
