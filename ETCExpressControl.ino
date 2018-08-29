// Written by Michael Moreau, IATSE 50
// There is an app I published to the Google app store called ETC Express Contol.
// I made a 3D printable case thats availible on thingiverse.
// The lighting console needs to be set to MIDI Channel 2.
// I used the TX2 and RX2 pins on an ESP32(DoIt DEV Kit V1 or a good clone, I can't tell)
// Pin 13 in an LED indicator for messages sent to console.
// Any questions can be sent to allaxisaudio@gmail.com


#include <HardwareSerial.h>
#include <SPI.h>
#include <WiFi.h>


const char* ssid     = "LightTech";  // Set your BSSID here, I recomend not broadcasting
const char* password = "LightGod";   // Set your router password here
  
WiFiServer server(80);

HardwareSerial MySerial(1);
#define MySerial_RXPIN 16  //defines RX2
#define MySerial_TXPIN 17  //defines TX2

int Comch = 193; // is 11000001 in binary for change program on ch 2
int CChange1 = 177; // is 10110001 in binary for program change on ch 2
int CChan = 70; // is 1000110 in binary for control number
int NoteOn = 145; // is 10010001 in binary for note on command
int NoteOff = 129; // is 10000001 in binary for note off command
int Note = 59; // is the start note for C5

void setup()
{

    // Start Serials
    Serial.begin(115200);
    MySerial.begin(31250, SERIAL_8N1, 16, 17); // TX2 and RX2 on an ESP32(May vary)
                 //115200 baud for testing

    WiFi.mode(WIFI_STA); // Sets wifi to station with no onboard AP
    
    delay(10);

   

    // Connect to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();
}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming queries
pinMode(13, OUTPUT);
  if (client) {                             // if adress queried
   
// Setup
   
    String req = client.readStringUntil('\r');
    String Cat = req.substring(5,9); // Designate Command Type
    String Next = req.substring(10,12); // 2 digit for go command
    String Test = req.substring(10,13); // 3 digit for cue commands
    String numbs =  req.substring(10,13); //make 3 mumbers
    Serial.println (Test);
    Serial.println (Cat);

// A/B Fader Pair Operation
   
   if (Cat == "midi") {       // If command type is Cue Control
   if (Next == "go"){        // If cue control command is next cue
    
    client.flush();    
    digitalWrite(13, HIGH);
    MySerial.write (Comch); // Send program change
    MySerial.write (int(0)); //Send intiger
    Serial.println (int(0)); // Debug the get funtion
    delay (500);  // delay to keep LED lit long enough to see it
    digitalWrite(13, LOW);   }

    else {
    
   if ((Test.toInt()) > 0 ) {      // If cue command is valid and not go
   if ((Test.toInt()) < 128 ) {    //If cue command is the first set
    
   
    client.flush();    
    digitalWrite(13, HIGH);
    MySerial.write (Comch); // Send program change
    MySerial.write (numbs.toInt()); //Send integer
    Serial.println (numbs.toInt()); // Debug the get funtion
    delay (500);  // delay to keep LED lit long enough to see it
    digitalWrite(13, LOW);
    }
    else {
    if ((Test.toInt()) < 999 ) {    //If cue command is the second set
    String numbs =  req.substring(10,13); //make 3 mumbers
   
    client.flush();    
    digitalWrite(13, HIGH);
    MySerial.write (CChange1); // Send controller change
        if ((Test.toInt()) < 256) { //Determins the program change
        MySerial.write (CChan);
        MySerial.write ((numbs.toInt()-128)); //Send integer
        Serial.println (CChan);
        Serial.println ((numbs.toInt()-128));
        }
        else { if ((Test.toInt()) < 384) { //Determins the program change
        MySerial.write ((CChan)+ 1);
        MySerial.write ((numbs.toInt()-256)); //Send integer
        Serial.println ((CChan)+ 1);
        Serial.println ((numbs.toInt()-256));
        }
        else { if ((Test.toInt()) < 512) { //Determins the program change
        MySerial.write ((CChan)+ 2);
        MySerial.write (numbs.toInt()-384); //Send integer
        }
        else { if ((Test.toInt()) < 640) { //Determins the program change
        MySerial.write ((CChan)+ 3);
        MySerial.write (numbs.toInt()-512); //Send integer
        }
        else { if ((Test.toInt()) < 768) { //Determins the program change
        MySerial.write ((CChan)+ 4);
        MySerial.write (numbs.toInt()-640); //Send integer
        }
        else { if ((Test.toInt()) < 896) { //Determins the program change
        MySerial.write ((CChan)+ 5);
        MySerial.write (numbs.toInt()-768); //Send integer
        }
        else { if ((Test.toInt()) < 999) { //Determins the program change
        MySerial.write ((CChan)+ 6);
        MySerial.write (numbs.toInt()-896); //Send integer
        }}}}}}}
      
    delay (500);  // delay to keep LED lit long enough to see it
    digitalWrite(13, LOW); }
    }
   }}}

         // End A/B Faders
      
// Begin C/D Fader Pair Operation
      if (Cat == ("cdpr")){       // If command type is Cue Control
        Serial.println ("CD Fader Pair"); 
        digitalWrite(13, HIGH);      
        if (Next == "go"){        // If cue control command is next cue
    
        client.flush();    
        
        MySerial.write (CChange1); // Send controller change
        MySerial.write ((CChan)+ 7); // Send controller change
        MySerial.write (0); //Send parameter 0
        Serial.println (CChan);
        Serial.println (0);
        delay (500);
        digitalWrite(13, LOW);
         }

    else {
    
   if ((Test.toInt()) > 0 ) {      // If cue command is valid and not go
   if ((Test.toInt()) < 128 ) {    //If cue command is the first set
   
    client.flush();
    digitalWrite(13, HIGH);    
   
        MySerial.write (CChange1); // Send controller change
        MySerial.write ((CChan)+ 7);
        MySerial.write (numbs.toInt()); //Send integer
        Serial.println (CChan);
        Serial.println (numbs.toInt());
        delay (500);
        digitalWrite(13, HIGH);
    
    }
    else {
    if ((Test.toInt()) < 999 ) {    //If cue command is the second set
   
    client.flush();
    digitalWrite(13, HIGH);    
    
    MySerial.write (CChange1); // Send controller change
        if ((Test.toInt()) < 256) { //Determins the program change
        MySerial.write ((CChan)+ 8);
        MySerial.write ((numbs.toInt()-128)); //Send integer
        Serial.println ((CChan)+ 8);
        Serial.println ((numbs.toInt()-128));
        }
        else { if ((Test.toInt()) < 384) { //Determins the program change
        MySerial.write ((CChan)+ 9);
        MySerial.write ((numbs.toInt()-256)); //Send integer
        Serial.println ((CChan)+ 9);
        Serial.println ((numbs.toInt()-256));
        }
        else { if ((Test.toInt()) < 512) { //Determins the program change
        MySerial.write ((CChan)+ 10);
        MySerial.write (numbs.toInt()-384); //Send integer
        }
        else { if ((Test.toInt()) < 640) { //Determins the program change
        MySerial.write ((CChan)+ 11);
        MySerial.write (numbs.toInt()-512); //Send integer
        }
        else { if ((Test.toInt()) < 768) { //Determins the program change
        MySerial.write ((CChan)+ 12);
        MySerial.write (numbs.toInt()-640); //Send integer
        }
        else { if ((Test.toInt()) < 896) { //Determins the program change
        MySerial.write ((CChan)+ 13);
        MySerial.write (numbs.toInt()-768); //Send integer
        }
        else { if ((Test.toInt()) < 999) { //Determins the program change
        MySerial.write ((CChan)+ 14);
        MySerial.write (numbs.toInt()-896); //Send integer
        }}}}}}}
        delay (500);
        digitalWrite(13, LOW);
        }}}}
          // End C/D Fader
        
            }

// Begin Macro Operation   
   
      if (Cat == ("mcro")){ 
        digitalWrite(13, HIGH);
        Serial.println ("Macro"); 
      if ((Test.toInt()) > 0 ) {      // If cue command is valid and not go

      MySerial.write (CChange1); // Send controller change
      
      if ((Test.toInt()) < 128 ) {    //If cue command is the first set
        MySerial.write ((CChan)+ 15);
        MySerial.write (numbs.toInt());}

      else { 
        if ((Test.toInt()) < 256 ) {
        MySerial.write ((CChan)+ 16);
        MySerial.write ((numbs.toInt()-128)); } 
      else {
        if ((Test.toInt()) < 384) {
        MySerial.write ((CChan)+ 17);
        MySerial.write ((numbs.toInt()-256)); }
      else {
        if ((Test.toInt()) < 512) {
        MySerial.write ((CChan)+ 18);
        MySerial.write ((numbs.toInt()-384)); }
      else {
        if ((Test.toInt()) < 640) {
        MySerial.write ((CChan)+19);
        MySerial.write ((numbs.toInt()-512)); }
      else {
        if ((Test.toInt()) < 768) {
        MySerial.write ((CChan)+20);
        MySerial.write ((numbs.toInt()-640)); }
      else {
        if ((Test.toInt()) < 896) {
        MySerial.write ((CChan)+21);
        MySerial.write ((numbs.toInt()-768)); }
      else {
        if ((Test.toInt()) < 999) {
        MySerial.write ((CChan)+22);
        MySerial.write ((numbs.toInt()-896)); }
      }}}}}}}}
      delay (500);
      digitalWrite(13, LOW); }
    
        // End of Macros   
      
  else {


// Begin Sub Bumps
        // Note on commands
    
      if (Cat == ("subs")) {
        Serial.println ("Sub");

      if ((Test.toInt()) < 27) {
        digitalWrite(13, HIGH);
        MySerial.write (NoteOn); // Sends note on command
        MySerial.write ((Note)+ (numbs.toInt()));  // Writes note
        Serial.println ((Note)+ (numbs.toInt()));  // Writes note
        MySerial.write (int(100));    // Sends velocity at 100
        delay (500);
        digitalWrite(13, LOW);
      }}
        // Note off commands
        
      if (Cat == ("subx")) {
        Serial.println ("Subs Off");

      if ((Test.toInt()) < 27) {  
        digitalWrite(13, HIGH);
        MySerial.write (NoteOff);  // Sends note off command
        MySerial.write ((Note) + (numbs.toInt())); // Writes note
        MySerial.write (int(100));   // Sends Velocity at 100
        delay (500);
        digitalWrite(13, LOW);
      }}
      
        // Clear Subs Command
      if (Cat == ("sbno")) {
        digitalWrite(13, HIGH);
               
        for (int j=60; j<=83; j=j+1) {
          MySerial.write (NoteOff);  // Sends note off command
          MySerial.write (j);
          MySerial.write (int(100));
          delay (100);

        }
        delay (500);
        digitalWrite(13, LOW);
      
      }
      }
        
           
    // Close Subs

  
      } }
      
      
  

