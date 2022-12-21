// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // Arduino: turn the LED on (HIGH)
                                     // D1 Mini: turns the LED *off*
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // Arduino: turn the LED off (LOW)
                                     // D1 Mini: turns the LED *on*
  delay(1000);                       // wait for a second
}
