#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin Ultrasonik
#define trigPin A1
#define echoPin A0  // pindah agar tidak bentrok dengan LED internal

// Pin Relay Jamu 5 jenis
#define relayJamu1 23
#define relayJamu2 24
#define relayJamu3 25
#define relayJamu4 26
#define relayJamu5 27

// Pin Relay Air
#define relayAirPanas 28
#define relayAirDingin 29

// Pin Flow sensor dan Coin Acceptor
#define flowPin 18
#define coinPin 3

// Tombol Pilih Jamu
#define btnJamu1 4
#define btnJamu2 5
#define btnJamu3 6
#define btnJamu4 7
#define btnJamu5 8

// Pin Tombol Pilih Suhu Air
#define btnDingin 9
#define btnHangat 10
#define btnPanas 11

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Variabel Flow Sensor
volatile int pulseCount = 0;
float calibrationFactor = 4.5;
float volume = 0;

// Variabel Coin Acceptor
volatile int coinPulse = 0;
int credit = 0;
int hargaPerGelas = 5;

// Flag status
int jamuDipilih = 0;
int suhuDipilih = 0;

// Interrupt
void pulseCounter() { pulseCount++; }
void coinCounter() { coinPulse++; }

// Fungsi Baca Ultrasonik
long readUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 60000); // timeout 60ms
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set relay sebagai OUTPUT
  int relays[] = {relayJamu1, relayJamu2, relayJamu3, relayJamu4, relayJamu5, relayAirPanas, relayAirDingin};
  for (int i = 0; i < 7; i++) {
    pinMode(relays[i], OUTPUT);
    digitalWrite(relays[i], HIGH); // off default
  }

  // Pin tombol
  pinMode(flowPin, INPUT);
  pinMode(coinPin, INPUT_PULLUP);
  pinMode(btnJamu1, INPUT_PULLUP);
  pinMode(btnJamu2, INPUT_PULLUP);
  pinMode(btnJamu3, INPUT_PULLUP);
  pinMode(btnJamu4, INPUT_PULLUP);
  pinMode(btnJamu5, INPUT_PULLUP);
  pinMode(btnDingin, INPUT_PULLUP);
  pinMode(btnHangat, INPUT_PULLUP);
  pinMode(btnPanas, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(flowPin), pulseCounter, RISING);
  attachInterrupt(digitalPinToInterrupt(coinPin), coinCounter, FALLING);

  lcd.setCursor(2, 0); lcd.print("SELAMAT DATANG");
  lcd.setCursor(9, 1); lcd.print("DI");
  lcd.setCursor(1, 2); lcd.print("MESIN JAMU OTOMATIS");
  delay(3000);
  lcd.clear();
}

void loop() {
  long jarak = readUltrasonic();
  Serial.print("Jarak: ");
  Serial.println(jarak);

  // Input koin
  if (coinPulse > 0) {
    credit += coinPulse;
    coinPulse = 0;
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Koin masuk!");
    lcd.setCursor(0, 1); lcd.print("Total: "); lcd.print(credit);
    delay(600);
  }

  // Pilih jamu
  if (credit >= hargaPerGelas && jamuDipilih == 0) {
    if (digitalRead(btnJamu1) == LOW) jamuDipilih = 1;
    if (digitalRead(btnJamu2) == LOW) jamuDipilih = 2;
    if (digitalRead(btnJamu3) == LOW) jamuDipilih = 3;
    if (digitalRead(btnJamu4) == LOW) jamuDipilih = 4;
    if (digitalRead(btnJamu5) == LOW) jamuDipilih = 5;

    if (jamuDipilih > 0) {
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("Jamu "); lcd.print(jamuDipilih); lcd.print(" dipilih");
      delay(800);
    }
  }

  // Pilih suhu air
  if (jamuDipilih > 0 && suhuDipilih == 0) {
    if (digitalRead(btnDingin) == LOW) suhuDipilih = 1;
    if (digitalRead(btnHangat) == LOW) suhuDipilih = 2;
    if (digitalRead(btnPanas) == LOW) suhuDipilih = 3;

    if (suhuDipilih > 0) {
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("Suhu: ");
      if (suhuDipilih == 1) lcd.print("Dingin");
      if (suhuDipilih == 2) lcd.print("Hangat");
      if (suhuDipilih == 3) lcd.print("Panas");
      delay(800);
    }
  }

  // Proses jika gelas terdeteksi
  if (jamuDipilih > 0 && suhuDipilih > 0 && jarak > 0 && jarak < 9) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Proses penyajian...");
    delay(1000);

    // Tentukan relay jamu sesuai pilihan
    int relayJamu;
    switch (jamuDipilih) {
      case 1: relayJamu = relayJamu1; break;
      case 2: relayJamu = relayJamu2; break;
      case 3: relayJamu = relayJamu3; break;
      case 4: relayJamu = relayJamu4; break;
      case 5: relayJamu = relayJamu5; break;
    }

    // Tuang jamu 20 ml
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Menuang Jamu...");
    pulseCount = 0;
    volume = 0;
    digitalWrite(relayJamu, LOW);
    while (volume < 20) {
      volume = pulseCount / calibrationFactor;
      lcd.setCursor(0, 1);
      lcd.print("Vol: "); lcd.print(volume); lcd.print(" ml  ");
    }
    digitalWrite(relayJamu, HIGH);
    delay(1000);

    // Tuang air sesuai suhu
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Menuang Air...");
    pulseCount = 0;
    volume = 0;

    if (suhuDipilih == 1) { // Dingin
      digitalWrite(relayAirDingin, LOW);
      while (volume < 180) {
        volume = pulseCount / calibrationFactor;
      }
      digitalWrite(relayAirDingin, HIGH);
    } 
    else if (suhuDipilih == 2) { // Hangat
      digitalWrite(relayAirPanas, LOW);
      digitalWrite(relayAirDingin, LOW);
      while (volume < 180) {
        volume = pulseCount / calibrationFactor;
      }
      digitalWrite(relayAirPanas, HIGH);
      digitalWrite(relayAirDingin, HIGH);
    } 
    else if (suhuDipilih == 3) { // Panas
      digitalWrite(relayAirPanas, LOW);
      while (volume < 180) {
        volume = pulseCount / calibrationFactor;
      }
      digitalWrite(relayAirPanas, HIGH);
    }

    // Penyajian selesai
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Jamu Siap Disajikan!");
    lcd.setCursor(0, 1); lcd.print("Total: 200 ml");
    delay(4000);

    // Reset status
    credit -= hargaPerGelas;
    jamuDipilih = 0;
    suhuDipilih = 0;
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Siap lagi...");
    delay(2000);
  }

  // Tampilan idle
  if (credit < hargaPerGelas) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Masukkan 5 koin");
    lcd.setCursor(0, 1); lcd.print("Kredit: "); lcd.print(credit);
    delay(800);
  } else if (jamuDipilih == 0) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Tekan tombol jamu");
    delay(800);
  } else if (suhuDipilih == 0) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Tekan tombol suhu");
    delay(800);
  } else if (jarak >= 9) {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Letakkan gelas...");
    delay(800);
  }

  delay(50);
}
