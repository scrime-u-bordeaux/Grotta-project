#include <Audio.h>
#include <SD.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav2;     //xy=158,194
AudioPlaySdWav           playSdWav1;     //xy=164,301
AudioMixer4              mixer1;         //xy=419,130
AudioMixer4              mixer2;         //xy=425,359
AudioOutputAnalogStereo  dacs;           //xy=640,236
AudioConnection          patchCord1(playSdWav2, 0, mixer1, 0);
AudioConnection          patchCord2(playSdWav2, 1, mixer2, 0);
AudioConnection          patchCord3(playSdWav1, 0, mixer1, 1);
AudioConnection          patchCord4(playSdWav1, 1, mixer2, 1);
AudioConnection          patchCord5(mixer1, 0, dacs, 0);
AudioConnection          patchCord6(mixer2, 0, dacs, 1);
// GUItool: end automatically generated code

// Use these with the Teensy 3.5 & 3.6 SD card
#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  11  // not actually used
#define SDCARD_SCK_PIN   13  // not actually used

int lightsens;
Bounce bouncer1 = Bounce(35, 20);
///////////////////version 24 mai 2019 ==> Bouton + MIXER////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  pinMode(35, INPUT_PULLUP);
  pinMode(37, OUTPUT);
  digitalWrite(37, LOW);
  AudioMemory(20);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  while (!(SD.begin(SDCARD_CS_PIN))) {
    digitalWrite(13, LOW);
    delay (100);
    digitalWrite(13, HIGH);
    delay (100);
    if (digitalRead(35) == LOW) break;
  }
  delay (5000); ////////////////5 seconde de delai, pour post synchro long.wav
  digitalWrite(13, LOW); // Led orange off
  delay (5000); //////////////delai total avant declanchement capteur = 10 secondes
}

void loop() {
  lightsens = analogRead(A9);
  Serial.println(lightsens);
  if (playSdWav1.isPlaying() == false && lightsens > 175) { //////////////////////////////////////// 175 => seuil lumière déclanchement "long.wav"
    playSdWav1.play("long.wav");
    delay(5000); // 5 secondes de delai, pour ne pas trigger "short.wav" de suite
  }
  if (playSdWav1.isPlaying() == true && playSdWav2.isPlaying() == false && lightsens > 600 ) { ///// 600 => seuil lumière déclanchement "short.wav"
    playSdWav2.play("short.wav");
    delay(1000); // 1 seconde de delai
  }
  bouncer1.update();
  if ( bouncer1.fallingEdge()) {  ///////////////////////////////////////////////////////////////// Detection boutton Play/Stop sur "long.wav"
    if (playSdWav1.isPlaying() == false) playSdWav1.play("long.wav");
    else playSdWav1.stop;
  }
  delay(15);
}
