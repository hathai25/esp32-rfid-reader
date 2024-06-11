#include <SPI.h>
#include <MFRC522.h>
#include <U8x8lib.h>
#include <Preferences.h>

#define OLED_SCK_PIN 22
#define OLED_SDA_PIN 21

Preferences preferences;

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/U8X8_PIN_NONE, OLED_SCK_PIN, OLED_SDA_PIN);

#define U8LOG_WIDTH 16
#define U8LOG_HEIGHT 8
uint8_t u8log_buffer[U8LOG_WIDTH * U8LOG_HEIGHT];
U8X8LOG u8x8log;

#define SS_PIN 5
#define RST_PIN 2
#define PIN_BUZZER 4

const int ledPin = 13; // GPIO pin connected to the LED
const int LED1 = 26;

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];
byte preNuidPICC[4];
byte pre2NuidPICC[4];
uint32_t counter;
/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void printHex(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? "0" : "");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize)
{
  String result = "";
  for (byte i = 0; i < bufferSize; i++)
  {
    result += (buffer[i] < 0x10 ? "0" : "") + String(buffer[i], DEC);
  }
  Serial.print(result);
}

String toString(byte *buffer, byte bufferSize)
{
  String result = "";
  for (byte i = 0; i < bufferSize; i++)
  {
    result += (buffer[i] < 0x10 ? "0" : "") + String(buffer[i], DEC);
  }
  return result;
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void setup()
{

  pinMode(LED1, OUTPUT);
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8log.begin(u8x8, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8x8log.setRedrawMode(1);

  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); // Set the LED pin as an output
  pinMode(PIN_BUZZER, OUTPUT);
  SPI.begin();     // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

  preferences.begin("my-app", false);

  preferences.getBytes("prev2", pre2NuidPICC, 4);
  preferences.getBytes("prev", preNuidPICC, 4);
  preferences.getBytes("now", nuidPICC, 4);
  counter = preferences.getUInt("counter", 0);

  String now = toString(nuidPICC, 4);
  String prev = toString(preNuidPICC, 4);
  String prev2 = toString(pre2NuidPICC, 4);
  if (counter == 0)
  {
    u8x8log.print("There is no card");
  }
  else if (counter == 1)
  {
    u8x8log.print("#1: ");
    u8x8log.print(now);
    u8x8log.print("\n");
  }
  else if (counter == 2)
  {
    u8x8log.print("#1: ");
    u8x8log.print(prev);
    u8x8log.print("\n");

    u8x8log.print("#2: ");
    u8x8log.print(now);
    u8x8log.print("\n");
  }
  else
  {
    u8x8log.print("#");
    u8x8log.print(counter - 2);
    u8x8log.print(": ");
    u8x8log.print(prev2);
    u8x8log.print("\n");
    u8x8log.print("#");
    u8x8log.print(counter - 1);
    u8x8log.print(": ");
    u8x8log.print(prev);
    u8x8log.print("\n");
    u8x8log.print("#");
    u8x8log.print(counter);
    u8x8log.print(": ");
    u8x8log.print(now);
    u8x8log.print("\n");
  }
  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}

void loop()
{

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K)
  {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || rfid.uid.uidByte[1] != nuidPICC[1] || rfid.uid.uidByte[2] != nuidPICC[2] || rfid.uid.uidByte[3] != nuidPICC[3])
  {
    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++)
    {
      pre2NuidPICC[i] = preNuidPICC[i];
      preNuidPICC[i] = nuidPICC[i];
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    digitalWrite(ledPin, 1);
    digitalWrite(PIN_BUZZER, HIGH);
    printDec(rfid.uid.uidByte, rfid.uid.size);
    // Print to screen
    String now = toString(rfid.uid.uidByte, rfid.uid.size);
    String prev = toString(preNuidPICC, 4);
    String prev2 = toString(pre2NuidPICC, 4);

    preferences.putBytes("prev2", pre2NuidPICC, 4);
    preferences.putBytes("prev", preNuidPICC, 4);
    preferences.putBytes("now", rfid.uid.uidByte, 4);
    counter++;
    preferences.putUInt("counter", counter);
    u8x8log.print("#");
    u8x8log.print(counter);
    u8x8log.print(": ");
    u8x8log.print(now);
    u8x8log.print("\n");

    delay(100);
    digitalWrite(ledPin, 0);
    digitalWrite(PIN_BUZZER, LOW);
  }
  else
    Serial.println(F("Card read previously."));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
