#include <SPI.h>
#include <MFRC522.h>
#include <U8x8lib.h>
#include <Preferences.h>

// Define for OLED screen
#define OLED_SCK_PIN 22
#define OLED_SDA_PIN 21
#define U8LOG_WIDTH 16
#define U8LOG_HEIGHT 8

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/U8X8_PIN_NONE, OLED_SCK_PIN, OLED_SDA_PIN);
uint8_t u8log_buffer[U8LOG_WIDTH * U8LOG_HEIGHT];
U8X8LOG u8x8log; //Instance of OLED screen

//Define for RFID RC522
#define SS_PIN 5
#define RST_PIN 2
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of rfid reader
MFRC522::MIFARE_Key key;

//Define other GPIO component
#define BUZZER_PIN 4
#define LED_PIN 13
#define SETUP_LED 26
Preferences preferences;


byte currentNuidPICC[4];
byte prevNuidPICC[4];
byte prevPrevNuidPICC[4];

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

void setup()
{
  pinMode(SETUP_LED, OUTPUT);
  Serial.begin(115200);
  //setup oled
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8log.begin(u8x8, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8x8log.setRedrawMode(1);

  //Setup gpios
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as an output
  pinMode(BUZZER_PIN, OUTPUT);
  SPI.begin();     // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

  preferences.begin("rfid-reader", false);

  preferences.getBytes("prevPrev", prevPrevNuidPICC, 4);
  preferences.getBytes("prev", prevNuidPICC, 4);
  preferences.getBytes("current", currentNuidPICC, 4);

  String currentNuidString = toString(currentNuidPICC, 4);
  String prevNuidString = toString(prevNuidPICC, 4);
  String prevPrevNuidString = toString(prevPrevNuidPICC, 4);

  u8x8log.println("Three last Nuid:");
  u8x8log.print("#1: ");
  u8x8log.print(currentNuidString);
  u8x8log.print("\n");
  u8x8log.print("#2: ");
  u8x8log.print(prevNuidString);
  u8x8log.print("\n");
  u8x8log.print("#3: ");
  u8x8log.print(prevPrevNuidString);
  u8x8log.print("\n");

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
    u8x8log.println("Card read previously");
    return;

  //Get card type
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K)
  {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  if (rfid.uid.uidByte[0] != currentNuidPICC[0] || rfid.uid.uidByte[1] != currentNuidPICC[1] || rfid.uid.uidByte[2] != currentNuidPICC[2] || rfid.uid.uidByte[3] != currentNuidPICC[3])
  {
    Serial.println(F("A new card has been detected."));

    // Store NUID into currentNuidPICC, prevNuidPICC, prevPrevNuidPICC array
    for (byte i = 0; i < 4; i++)
    {
      prevPrevNuidPICC[i] = prevNuidPICC[i];
      prevNuidPICC[i] = currentNuidPICC[i];
      currentNuidPICC[i] = rfid.uid.uidByte[i];
    }

    //Loggin purpose
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));

    //Active LED, BUZZER
    digitalWrite(LED_PIN, 1);
    digitalWrite(BUZZER_PIN, HIGH);
    // Print to screen
    String current = toString(rfid.uid.uidByte, rfid.uid.size);
    String prev = toString(prevNuidPICC, 4);
    String prevPrev = toString(prevPrevNuidPICC, 4);

    preferences.putBytes("prevPrev", prevPrevNuidPICC, 4);
    preferences.putBytes("prev", prevNuidPICC, 4);
    preferences.putBytes("current", rfid.uid.uidByte, 4);

    u8x8log.println("---------");
    u8x8log.println("Current NUID: ");
    u8x8log.print(current);
    u8x8log.print("\n");
    u8x8log.println("Prev NUID: ");
    u8x8log.print(prev);
    u8x8log.print("\n");
    u8x8log.println("Pre-prev NUID: ");
    u8x8log.print(prevPrev);
    u8x8log.print("\n");
    delay(100);

    //Turn off LED & BUZZER
    digitalWrite(LED_PIN, 0);
    digitalWrite(BUZZER_PIN, LOW);
  }
  else
    u8x8log.println("Card read previously");
    Serial.println(F("Card read previously."));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
