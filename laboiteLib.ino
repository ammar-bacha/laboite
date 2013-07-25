void connectToServer() {
  // attempt to connect, and wait a millisecond:
  #ifdef DEBUG
  Serial.print("Connecting to ");
  Serial.print(serverName);
  Serial.println("...");
  #endif
  if (client.connect(serverName, 80)) {
    #ifdef DEBUG
    Serial.println("Making HTTP request...");
    #endif
    // make HTTP GET request to twitter:
    client.print("GET /");
    client.print(apikey);
    client.println(".json HTTP/1.1");
    client.print("Host: ");
    client.println(serverName);
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println();
  }
}   

boolean parseJSON() {
  // make sure all apps are not enabled
  resetApps();
  String content = "";
  
  while(client.available()) {
    // read incoming bytes:
    char inChar = client.read();
    wdt_reset();
    
    #ifdef DEBUG
    // debugging purposes only:
    //Serial.print(inChar);
    //Serial.println(currentLine);
    #endif
    
  
    // add incoming byte to end of line:
    currentLine += inChar;
  
    // if you get a newline, clear the line:
    if (inChar == '\n') {
      currentLine = "";
    }
    
    // fetch Time app data
    if (currentLine.endsWith("\"time\":")) {
      readingTime = true;
      content = "";
    }
  
    if (readingTime) {
      if (inChar != ',' && inChar != '}') {
        if (inChar != '"' && inChar != ':')
        content += inChar;
      }
      else {
        readingTime = false;
        hour[0] = content.charAt(0);
        hour[1] = content.charAt(1);
        hour[3] = '\0';
        
        minutes[0] = content.charAt(2);
        minutes[1] = content.charAt(3);
        minutes[3] = '\0';
        
        timeEnabled = true;
    
        #ifdef DEBUG
        Serial.print("Time: ");
        Serial.print(hour);
        Serial.print(":");
        Serial.println(minutes);
        #endif
      }
    }
    
    // fetch Bus app data
    if (currentLine.endsWith("\"bus\":")) {
      readingBus = true;
      content = "";
    }
  
    if (readingBus) {
       if (inChar != ',' && inChar != '}') {
        if (inChar != '"' && inChar != ':')
        content += inChar;
      }
      else {
        readingBus = false;
        bus[0] = content.charAt(0);
        bus[1] = content.charAt(1);
        bus[2] = '\0';
        
        busEnabled = true;
        
        #ifdef DEBUG
        Serial.print("Bus: ");
        Serial.println(bus);
        #endif
      }
    }
    
    if (currentLine.endsWith("\"bikes\":")) {
      readingBikes = true;
      content = "";
    }
  
    if (readingBikes) {
      if (inChar != ',' && inChar != '}') {
        if (inChar != '"' && inChar != ':')
        content += inChar;
      }
      else {
        readingBikes = false;
        bikes[0] = content.charAt(0);
        bikes[1] = content.charAt(1);
        bikes[2] = '\0';
        
        bikesEnabled = true;
        
        #ifdef DEBUG
        Serial.print("Bikes: ");
        Serial.println(bikes);
        #endif
      }
    }
    
    if (currentLine.endsWith("\"emails\":")) {
      readingEmails = true;
      content = "";
    }
  
    if (readingEmails) {
      if (inChar != ',' && inChar != '}') {
        if (inChar != '"' && inChar != ':')
        content += inChar;
      }
      else {
        readingEmails = false;
        emails[0] = content.charAt(0);
        emails[1] = content.charAt(1);
        emails[2] = '\0';
        
        emailsEnabled = true;
        
        #ifdef DEBUG
        Serial.print("Emails: ");
        Serial.println(emails);
        #endif
      }
    }
    
    // fetch Weather app data
    if (currentLine.endsWith("\"icon\":")) {
      readingTodayIcon = true;
      content = "";
    }
  
    if (readingTodayIcon) {
      if (inChar != ',' && inChar != '}') {
        if (inChar != '"' && inChar != ':')
        content += inChar;
      }
      else {
        readingTodayIcon = false;
        todayIcon = stringToInt(content);
      }
    }
    
    if (currentLine.endsWith("\"temperature\":")) {
      readingTemperature = true;
      content = "";
    }
  
    if (readingTemperature) {
      if (inChar != ',' && inChar != '}') {
        if (inChar != '"' && inChar != ':')
        content += inChar;
      }
      else {
        readingTemperature = false;
        temperature[0] = content.charAt(0);
        temperature[1] = content.charAt(1);
        temperature[2] = '\0';
      }
    }
    
    if (currentLine.endsWith("\"icon\":") && todayIconRead) {
      readingTomorrowIcon = true;
      content = "";
    }
  
    if (readingTomorrowIcon) {
      if (inChar != ',' && inChar != '}') {
        if (inChar != '"' && inChar != ':')
        content += inChar;
      }
      else {
        readingTomorrowIcon = false;
        tomorrowIcon = stringToInt(content);
      }
    }
    
    if (currentLine.endsWith("\"low\":")) {
      readingLow = true;
      content = "";
    }
  
    if (readingLow) {
      if (inChar != ',' && inChar != '}') {
        if (inChar != '"' && inChar != ':')
        content += inChar;
      }
      else {
        readingLow = false;
        low[0] = content.charAt(0);
        low[1] = content.charAt(1);
        low[2] = '\0';
      }
    }
    
    if (currentLine.endsWith("\"high\":")) {
      readingHigh = true;
      content = "";
    }
  
    if (readingHigh) {
      if (inChar != ',' && inChar != '}') {
        if (inChar != '"' && inChar != ':')
        content += inChar;
      }
      else {
        readingHigh = false;
        high[0] = content.charAt(0);
        high[1] = content.charAt(1);
        high[2] = '\0';
        
        weatherEnabled = true;
        
        #ifdef DEBUG
        Serial.print("Weather: ");
        Serial.print(todayIcon);
        Serial.print(", ");
        Serial.print(temperature);
        Serial.print(", ");
        Serial.print(tomorrowIcon);
        Serial.print(", ");
        Serial.print(low);
        Serial.print(", ");
        Serial.println(high);
        #endif
      }
    }
  }
}

void resetApps() {
  timeEnabled = false;
  busEnabled = false;
  bikesEnabled = false;
  emailsEnabled = false;
  weatherEnabled = false;
  coffeesEnabled = false;
}

int stringToInt(String string) {
  char buffer[8];
  string.toCharArray(buffer, string.length()+1);
  return atoi(buffer);
}

// dotmatrix functions
#ifdef HT1632C
void printTime(int x) {
  dotmatrix.putchar(x+5, 0, hour[0], GREEN);
  dotmatrix.putchar(x+10, 0, hour[1], GREEN);
  dotmatrix.putchar(x+14, 0, ':', GREEN);
  dotmatrix.putchar(x+18, 0, minutes[0], GREEN);
  dotmatrix.putchar(x+23, 0, minutes[1], GREEN);
}

void adjustBrightness() {
  // reset the watchdog timer
  wdt_reset();
  // read the analog in value:
  brightnessValue = (ldr.read() + previousBrightnessValue) / 2;
  pwm = map(brightnessValue, 0, 1023, 0, 15);
  dotmatrix.pwm(pwm);
  previousBrightnessValue = brightnessValue;
  
  // check if scrolling button has been pressed
  if(button.read())
    scrolling = !scrolling;
}

void waitAWhile() {
  for (int i = 0; i < 16; i++) {
    adjustBrightness();
    delay(50);
  }
}
void printTemperature(int x, char firstDigit, char secondDigit, byte color)
{
  if(secondDigit == '\0') {
    secondDigit = firstDigit;
    firstDigit = ' ';
  }
  dotmatrix.putchar(x, 9, firstDigit, color);
  dotmatrix.putchar(x+5, 9, secondDigit, color);
  dotmatrix.putchar(x+10, 9, '*', color);
}

void scrollFirstPanel(int x) {
  if(weatherEnabled) {
    // first panel : current weather condition 32→0
    if(x > -16) {
      dotmatrix.putchar(x+12, 9, ' ', RED);
      color = todayIcon == 0 ? color = ORANGE : color = RED;
      dotmatrix.putbitmap(x, 7, sprites[todayIcon],16,9, color);
    }
    
    if(x >= 0) {
      printTemperature(x+17, temperature[0], temperature[1], RED);
      dotmatrix.sendframe();
    }
    
    if(x == 0) {
      waitAWhile();
      printTemperature(x+17, indoorTemperatureString[0], indoorTemperatureString[1], ORANGE);
      dotmatrix.sendframe();
      waitAWhile();
    }
  }
  else
    x=-32;
}

void scrollSecondPanel(int x) {
  if(weatherEnabled) {
    // second panel : tomorrow weather condition 0→-32
    if(x <= 1 && x >= -48) {
      dotmatrix.putchar(x+44, 9, ' ', RED);
      color = tomorrowIcon == 0 ? color = ORANGE : color = RED;
      dotmatrix.putbitmap(x+32, 7, sprites[tomorrowIcon],16,9, color);
    }
    
    if(x >= -32 && x < 0) {
      printTemperature(x+17, indoorTemperatureString[0], indoorTemperatureString[1], ORANGE);
      printTemperature(x+49, low[0], low[1], RED);
      dotmatrix.sendframe();
    }
    
    if(x == -32) {
      waitAWhile();
      printTemperature(x+49, high[0], high[1], GREEN);
      dotmatrix.sendframe();
      waitAWhile();
    }
  }
}

void scrollThirdPanel(int x) {
  //third panel : bus and bikes -32→-64
  if(x >= -63 && x < -32) {
    printTemperature(x+49, high[0], high[1], GREEN);
    if(timeEnabled)
      printTime(x+32);
  }
}

void scrollFourthPanel(int x) {
  //fourth panel : coffees and energy -64→-96
  if(x <= -33) {
    if(!busEnabled && !bikesEnabled)
      x=-96;
    
    // bus app
    if(busEnabled) {
      if(bus[0] == '-')
        dotmatrix.putchar(x+68, 10, '<', GREEN);
      else
        dotmatrix.putchar(x+68, 10, bus[0], GREEN);
        
      if(bus[1] == '\0')
        dotmatrix.putchar(x+73, 10, '\'', GREEN);
      else {
        dotmatrix.putchar(x+73, 10, bus[1], GREEN);
        dotmatrix.putchar(x+78, 10, '\'', GREEN);
      }
      
      dotmatrix.putbitmap(x+68, 0, busSprite, 9, 9, ORANGE);
    }
    
    // bikes app
    if(bikesEnabled) {
      dotmatrix.putchar(x+92, 0, ' ', ORANGE);
      dotmatrix.putchar(x+92, 3, ' ', ORANGE);
      dotmatrix.putbitmap(x+77, 0, bikeSprite, 16, 9, ORANGE);
      
      if(bikes[1] != '\0')
        dotmatrix.putchar(x+87, 10, bikes[1], GREEN);
      dotmatrix.putchar(x+82, 10, bikes[0], GREEN);
    }
    
    // coffees app
    /*if(coffeesEnabled) {
      if([1] != '\0')
        dotmatrix.putchar(x+131, 2, coffees[1], GREEN);
      dotmatrix.putbitmap(x+115, 0, coffeeSprite, 16, 8, ORANGE);
      dotmatrix.putchar(x+123, 2, ' ', GREEN);
      dotmatrix.putchar(x+126, 2, coffees[0], GREEN);
    }
    */
    
    // energy app
    /*if(energyEnabled) {
      for(int i = 0; i < 7; i++) {
        drawChart(x + 111 + (i*4), kWhdHistory[i]);
      }
    }*/
    
    if(timeEnabled)
      printTime(x+97);
  }
}
            
void drawChart(byte x, byte height) {
  dotmatrix.rect(x, 16-height, x+2, 15, GREEN);
  if(height > 2)
    dotmatrix.line(x+1, 17-height, x+1, 14, BLACK);
  dotmatrix.line(x+3, 16-height, x+3, 15, BLACK);
}
#endif