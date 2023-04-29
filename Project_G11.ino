#include <Keypad.h>
#include <RTClib.h>
#include <Wire.h>
#include <IRremote.h>
#include <SevSeg.h>
#include <LiquidCrystal_I2C.h>
RTC_DS3231 rtc;
SevSeg sevseg;
LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int LEFT_THRESHOLD = 400;
const int RIGHT_THRESHOLD = 800;
const int UP_THRESHOLD = 400;
const int DOWN_THRESHOLD = 800;

#define buzzer 14
#define axisX A0
#define axisY A1
#define greenled 15
#define redled 16
#define blueled 17
#define x A14 // WHAT IS X STAND FOR?

// IR pin
#define IRpin 11

// Dot Matrix LED pins
#define COL_1 9
#define COL_2 2
#define COL_3 3
#define COL_4 4
#define COL_5 5
#define COL_6 6
#define COL_7 7
#define COL_8 8

#define ROW_1 29
#define ROW_2 28
#define ROW_3 27
#define ROW_4 26
#define ROW_5 25
#define ROW_6 24
#define ROW_7 23
#define ROW_8 22

// Map the buttons to an array for the Keymap instance
char hexaKeys[4][4] = {
  { 'A', 'B', 'C', 'D' },
  { 'E', 'F', 'G', 'H' },
  { 'I', 'J', 'K', 'L' },
  { 'M', 'N', 'O', 'P' }
};


char KeyChar[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'};
byte colKeyPadPins[4] = { 37, 36, 35, 34 };  // Pins used for the rows of the keypad
byte rowKeyPadPins[4] = { 38, 39, 40, 41 };  // Pins used for the columns of the keypad
// Initialize the Keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowKeyPadPins, colKeyPadPins, 4, 4);

const byte rows[] = { ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8 };
const byte col[] = { COL_8, COL_7, COL_6, COL_5, COL_4, COL_3, COL_2, COL_1 };
//mapForGame2
byte map3[] = { B00000000,B10111000,B11101100,B00000111,B01110001,B01011111,B01000000,B11000000};
byte map2[] = { B00000000,B11111000,B00001000,B00001000,B00001000,B00001000,B00001000,B00001111};
byte map1[]={ B00100000,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};

// Emoji pattern
byte happy[] = { B00000000, B01100110, B01100110, B00000000, B00000000, B01000010, B01111110, B00000000 };
byte sad[] = { B00000000, B01100110, B01100110, B00000000, B00000000, B01111110, B01000010, B0000000 };
byte heart[] = { B00000000, B01100110, B11111111, B11111111, B01111110, B00111100, B00011000, B00000000 };
int TIMEE;
// IRremote receive data in DEC
IRRawDataType  game1 = 4077715200;      // Among us
IRRawDataType  game2 = 3877175040;      // Walk following
IRRawDataType  resetGame = 3125149440;  // Leave the game

// Set timer
bool gameState = false;
int timeUse = 0;
DateTime start;
//int timeUse = 0;
bool resetTimer = true;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  pinMode(greenled,OUTPUT);
  pinMode(redled,OUTPUT);
  pinMode(blueled,OUTPUT);
  pinMode(axisX, INPUT);
  pinMode(axisY, INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(x,INPUT);

  // Set 4 digits display
  byte numDigits = 4;

  // Defines common pins while using multi-digit display from left to right digits.
  byte digitPins[] = {52, 53, 12, 10};

  // Defines arduino pin connections in order: A, B, C, D, E, F, G, DP
  byte segmentPins[] = {44, 45, 46, 47, 48, 49, 50, 51};
  bool resistorsOnSegments = true;

  //Initialize sevseg object. Uncomment second line if you use common cathode 7 segment
  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  //sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);

  // Initialize digital columns pins.
  for (int i = 2; i <= 9; i++) {pinMode(i, OUTPUT); digitalWrite(i, LOW);}
  // Initialize digital rows pins.
  for (int i = 22; i <= 29; i++) {pinMode(i, OUTPUT); digitalWrite(i, LOW);}

  // Start the receiver
  IrReceiver.begin(IRpin, ENABLE_LED_FEEDBACK);  
  IrReceiver.resume();

  // Show HEART emoji until the remote is pushed
  while (!IrReceiver.decode()) {
    drawScreen(heart);
  }
  randomSeed(analogRead(x));
}


void loop() {
  // If ...
  int counter=0;
  if (IrReceiver.decode()) {   //เช็คว่ามีสัญญาณจาก remote เข้ามาที่ ir sensor หรือไม่
    bool gameResult1 = false;
    bool gameResult2 = false;
    bool gameOver1 = false;
    bool gameOver2 = false;
    int gameChoose = int(3);
    // Game 1 section is Among Us game

    if (IrReceiver.decodedIRData.decodedRawData == game1) {
      //Serial.println(IrReceiver.decodedIRData.decodedRawData);
      Serial.println("Play Game 1");
      gameResult1 = game_1_hit(&counter);
      gameOver1 = true;
    }
    
    //Game 2 section
    else if (IrReceiver.decodedIRData.decodedRawData == game2){
       Serial.println("Play Game 2");
         TIMEE=game_2_memorize_route();
         if(TIMEE != 0){gameResult2=true;}
         else{gameResult2=false;}
         gameOver2 = true;
    }
  
    // reset to receive the next data received
    IrReceiver.resume();
    if(gameResult2==1)
    {     lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("You win !!!");
          delay(1000);
          lcd.clear();
          Serial.print("Time:");
          Serial.println(TIMEE);
          int score = (500 - TIMEE);
          String scoreText = "Your score is... ";
          lcd.print(scoreText);
          lcd.setCursor(0,1);
          lcd.print(String(score));
      }
    else {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("You lose !!!");
    }
    // Hold until the the IR is received the data.
    while ((gameOver1 || gameOver2) && !(IrReceiver.decode() && IrReceiver.decodedIRData.decodedRawData == resetGame)) {
      IrReceiver.resume();
      // if game 1 a show this
      if (gameOver1){
        drawScreen(sad);
        sevseg.setNumber(counter);
        sevseg.refreshDisplay();
      }
      else if (gameOver2){
        if (gameResult2==1){
          sevseg.setNumber(TIMEE);
          sevseg.refreshDisplay();
          drawScreen(happy);
        }
        else {
          drawScreen(sad);
        }
      }
    }
    //reset_all_output
    digitalWrite(redled,LOW);
    digitalWrite(blueled,LOW);
    digitalWrite(greenled,LOW);
    bool resetTime = true;
    lcd.clear();
  }
}


bool game_1_hit(int* addressCounter) {
  // Make the pattern of the position
  byte sq1[] = { B11000000, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 };
  byte sq2[] = { B00110000, B00110000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 };
  byte sq3[] = { B00001100, B00001100, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 };
  byte sq4[] = { B00000011, B00000011, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 };
  byte sq5[] = { B00000000, B00000000, B11000000, B11000000, B00000000, B00000000, B00000000, B00000000 };
  byte sq6[] = { B00000000, B00000000, B00110000, B00110000, B00000000, B00000000, B00000000, B00000000 };
  byte sq7[] = { B00000000, B00000000, B00001100, B00001100, B00000000, B00000000, B00000000, B00000000 };
  byte sq8[] = { B00000000, B00000000, B00000011, B00000011, B00000000, B00000000, B00000000, B00000000 };
  byte sq9[] = { B00000000, B00000000, B00000000, B00000000, B11000000, B11000000, B00000000, B00000000 };
  byte sq10[] = { B00000000, B00000000, B00000000, B00000000, B00110000, B00110000, B00000000, B00000000 };
  byte sq11[] = { B00000000, B00000000, B00000000, B00000000, B00001100, B00001100, B00000000, B00000000 };
  byte sq12[] = { B00000000, B00000000, B00000000, B00000000, B00000011, B00000011, B00000000, B00000000 };
  byte sq13[] = { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11000000, B11000000 };
  byte sq14[] = { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00110000, B00110000 };
  byte sq15[] = { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00001100, B00001100 };
  byte sq16[] = { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000011, B00000011 };
  
  // Make sequence game
  byte all_sq[16][8];
  combineArrays(sq1, sq2, sq3, sq4, sq5, sq6, sq7, sq8, sq9, sq10, sq11, sq12, sq13, sq14, sq15, sq16,all_sq);
  // set the number of sequence
  int size = 8;

  // Random Map
  int rand1 = random(0, 15);
  int rand2 = random(0, 15);
  int rand3 = random(0, 15);
  int rand4 = random(0, 15);
  int rand5 = random(0, 15);
  int rand6 = random(0, 15);
  int rand7 = random(0, 15);
  int rand8 = random(0, 15);
  byte map[8][8];
  combineArrays(all_sq[rand1], all_sq[rand2], all_sq[rand3], all_sq[rand4], all_sq[rand5], all_sq[rand6], all_sq[rand7], all_sq[rand8], map);


  bool isLose = false;
  bool resetTime = false;

  // Game loop
  int isCorrect = 0;
  while (!isCorrect) {
    // random number for correct position
    int rand = random(0, 15);
    byte map[8];
    // Write seven segment 
    sevseg.setNumber(*addressCounter);
    sevseg.refreshDisplay();
    // stored the random position to map
    memcpy(map, all_sq[rand], sizeof(all_sq[rand]));
    byte correctPosition[8];
    memcpy(correctPosition, map, sizeof(map));
    // Draw the correct position 1 sec for user to remember and choose the correct position
    drawByTime_counter(correctPosition, 1, addressCounter);

    // Convert the key received to square pattern.
    char key = customKeypad.getKey();
    while (key == NO_KEY) {
      key = customKeypad.getKey();
      // Write seven segment 
      sevseg.setNumber(*addressCounter);
      sevseg.refreshDisplay();
    }

    // Assign the pattern of choosePosition
    byte choosePosition[8];
    // Convert key receive to postion.
    for (int i=0; i<16;i++){
      // Write seven segment 
      sevseg.setNumber(*addressCounter);
      sevseg.refreshDisplay();
      if (key == KeyChar[i]){
        memcpy(choosePosition, all_sq[i], 8);
        break;
      }
    }
    
    // Check whether the choose position is true or not.
    for (int i = 0; i < 8; i++) {      
      sevseg.setNumber(*addressCounter);
      sevseg.refreshDisplay();
      // if the choose postion is the same as correct position then XOR opertion between them = 0
      isCorrect += (choosePosition[i]) ^ (correctPosition[i]);
    }

    // if isCorrect = 0 mean choose posion = correct position.
    if (isCorrect != 0) {
      // SOUND BEEP
      isLose = true;
      break;
    }
    else {*addressCounter = *addressCounter + 1;}
  }
  Serial.println("Game Over");
  Serial.println("Lose: " + String(isLose));
  return isLose;
}



int drawCountTime(boolean resetTimer) {
  static DateTime start_time = rtc.now();
  // Reset timer
  if (resetTimer){start_time = rtc.now();}
  
  // find difference in time.
  DateTime now = rtc.now();
  timeUse = now.unixtime()-start_time.unixtime() + 1;
  
  //Display numbers one by one with 2 seconds delay
  sevseg.setNumber(timeUse);
  sevseg.refreshDisplay();
  return timeUse;
};
//Function to draw picture in a predetermined time
void drawScreenByTime(byte buffer[], int timer) {
  DateTime start_time = rtc.now();
  // Reset timer
  while (rtc.now().unixtime() - start_time.unixtime() < timer){
    drawScreen(buffer);
    sevseg.setNumber(timeUse);
    sevseg.refreshDisplay();
   //Serial.println(rtc.now().unixtime() - start_time.unixtime());
  }
};

void drawScreen(byte buffer[]) {
  // Turn on each row in series
  for (byte i = 0; i < 8; i++)  // count next row
  {
    //Reset whole columns to be unactivate.
    for (byte j = 0; j < 8; j++) {
      digitalWrite(col[j], LOW);
    }

    // Reset whole rows to be activate
    digitalWrite(rows[i], LOW);
    // Activate each columns follow the buffer
    for (byte j = 0; j < 8; j++)  // count next row
    {

      digitalWrite(col[j], (buffer[i] >> j) & 0x01);
      //delayMicroseconds(250);
      //digitalWrite(col[j], LOW);
    }
    // Reset whole rows to be unactivate.
    digitalWrite(rows[i], HIGH);
  }
};

void drawByTime_counter(byte buffer[], int timer, int* addressCounter) {
  DateTime start_time = rtc.now();
  // Reset timer
  while (rtc.now().unixtime() - start_time.unixtime() < timer) {
    drawScreen(buffer);
    sevseg.setNumber(*addressCounter);
    sevseg.refreshDisplay();
  };
};


// Function to combine two one-dimensional arrays into a two-dimensional array
void combineArrays(byte array1[], byte array2[], byte array3[], byte array4[], byte array5[], byte array6[], byte array7[], byte array8[], byte result[][8]) {
  // Copy the contents of array1 and array2 into the result array
  for (int i = 0; i < 8; i++) {
    result[0][i] = array1[i];
    result[1][i] = array2[i];
    result[2][i] = array3[i];
    result[3][i] = array4[i];
    result[4][i] = array5[i];
    result[5][i] = array6[i];
    result[6][i] = array7[i];
    result[7][i] = array8[i];
  }
};

void combineArrays(byte array1[], byte array2[], byte array3[], byte array4[], byte array5[], byte array6[], byte array7[], byte array8[],
                    byte array9[], byte array10[], byte array11[], byte array12[], byte array13[], byte array14[], byte array15[], byte array16[],
                  byte result[][8]) {
  // Copy the contents of array1 and array2 into the result array
  for (int i = 0; i < 8; i++) {
    result[0][i] = array1[i];
    result[1][i] = array2[i];
    result[2][i] = array3[i];
    result[3][i] = array4[i];
    result[4][i] = array5[i];
    result[5][i] = array6[i];
    result[6][i] = array7[i];
    result[7][i] = array8[i];
    result[8][i] = array9[i];
    result[9][i] = array10[i];
    result[10][i] = array11[i];
    result[11][i] = array12[i];
    result[12][i] = array13[i];
    result[13][i] = array14[i];
    result[14][i] = array15[i];
    result[15][i] = array16[i];
  }
};
//Function to change form analog output form joystick to direction
char checkdirection(int x, int y) {
  if (x < LEFT_THRESHOLD) {
    return 'D'; //left เเต่ return เป็น down เนื่องจากการวางทิศทางของ joystick บน  board
  } else if (x > RIGHT_THRESHOLD) {
    return 'U'; //right  เเต่ return เป็น up เนื่องจากการวางทิศทางของ joystick บน  board
  } else if (y < UP_THRESHOLD) {
    return 'L'; //up เเต่ return เป็น left เนื่องจากการวางทิศทางของ joystick บน  board
  } else if (y > DOWN_THRESHOLD) {
    return 'R'; //down  เเต่ return เป็น right เนื่องจากการวางทิศทางของ joystick บน  board
  } else {
    return 'N'; //no direction ยังไม่มีการขยับ joystick
  }
}
//Function to convert matrix to byte
void matrixToByte(byte matrix[][8], byte byteArray[]) {
  for (int i = 0; i < 8; i++) {
    byteArray[i] = 0;
    for (int j = 0; j < 8; j++) {
      byteArray[i] |= (matrix[i][j] << (7-j));
    }
    byteArray[i] = (byteArray[i]); // Use the "B" macro to add the prefix
  }
}
//Function to byte to matrix
void byteToMatrix(byte byteArray[], byte matrix[][8]) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matrix[i][j] = bitRead(byteArray[i], 7-j);
    }
  }
}
int game_2_memorize_route()
  { 
    start = rtc.now();
    byte reset[]= {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000}; //use this byte to reset an dot matrix
    byte mapp[8];
    byte matrix[8][8];
    byte convertedByteArray[8];
    byte matrix_player[8][8]={{0,0,0,0,0,0,0},{1,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}}; //position of player
      int r = random(1,4);
      if(r==1){memcpy(mapp, map1, sizeof(map1));}//ถ้า 1 เลือก map1 
      else if(r==2){memcpy(mapp, map2, sizeof(map2));}//ถ้า 2 เลือก map2
      else if(r==3){memcpy(mapp, map3, sizeof(map3));}//ถ้า 3 เลือก map3
    drawScreenByTime(mapp,2); // วาด map ที่เราเลือก เวลา 2 วิ
    drawScreenByTime(reset,2);// reset หน้าเปล่า 2 วิ
    digitalWrite(blueled,HIGH);//ไฟ led ติด
    int row=1;//set จุดเเรกของผู้เล่นอยู่เเกน x=1
    int column=0;//setจุดเเรกของผู้เล่นอยู่เเกน y=0
    int rown=row; // rown สร้างเพื่อใช้ check ว่าเดินอยู่ใน path หรือป่าว
    int columnn=column;  // coloumnn สร้างเพื่อใช้ check ว่าเดินอยู่ใน path หรือป่าว
    lcd.print("Game start");
    delay(1000);
    while(1)
    {
      lcd.setCursor(0,0);
      lcd.print("Playing .....");
      //delay(1000);
      int x = analogRead(axisX); //รับ input เเกน x มาจาก joystick
      int y = analogRead(axisY); //รับ input เเกน y มาจาก joystick
      char direction = checkdirection(x, y); 
      timeUse = drawCountTime(resetTimer); //เขียนเวลาลง 7 digit
      resetTimer = false;
      while(direction=='N') // ถ้ายังไม่มีการขยับ joystick ให้อยู่ใน loop นี้
      {
          matrixToByte(matrix_player, convertedByteArray);
          drawScreenByTime(convertedByteArray, 1);  
          x = analogRead(axisX);
          y = analogRead(axisY);
          timeUse = drawCountTime(resetTimer);
          direction = checkdirection(x, y);
       }
    if(direction=='R'&&(column+1<=7)) //ถ้าขยับขวาเเละไม่เกินขอบให้ + coloumn ไป 1
      {
        columnn=column+1;
        timeUse = drawCountTime(resetTimer);
      }
    else if(direction== 'L'&&(column-1>=0)) //ถ้าขยับซ้ายเละไม่เกินขอบให้ + coloumn ไป -1
      {
        columnn=column-1;
        timeUse = drawCountTime(resetTimer);
      }
    else if(direction=='U'&&(row-1>=0)) //ถ้าขยับบนเเละไม่เกินขอบให้ + row ไป -1
      {
        rown=row-1;
        timeUse = drawCountTime(resetTimer);
      }
    else if(direction=='D'&&(row+1<=7))  //ถ้าขยับล่างเเละไม่เกินขอบให้ + row ไป 1
      {
        rown=row+1;
        timeUse = drawCountTime(resetTimer);
      }
    byteToMatrix(mapp,matrix); // เเปลง mapp ซึ่งเป็น byte ให้เป็น matrix เพื่อนำไปเปลี่ยน check ค่าได้
    if(int(matrix[rown][columnn])==1)  //ถ้าเดินถูกทาง จะเข้ามาเปลี่ยนจุด Player
    { 
      byteToMatrix(convertedByteArray,matrix_player); //เปลี่ยน จุดของ player จาก byte เป็น matrix เพื่อให้นำไปเปลี่ยนข้อมูลภายในได้
      matrix_player[row][column]=0; //ให้จุดเก่าเป็น 0 (ไฟดับ) 
      matrix_player[rown][columnn]=1;  //ให้จุดใหม่เป็น 1 (ไฟติด)
      matrixToByte(matrix_player,convertedByteArray); //เปลี่ยน matrix เป็น byte เพื่อให้ใช้คำสั่ง drawScreenByTime ได้
      drawScreenByTime(convertedByteArray, 1); //วาดจุด player ใหม่
      row=rown;
      timeUse = drawCountTime(resetTimer);
      column=columnn;

      if(rown==0 and columnn==2 and r==1) //ถ้าถึงทางออกของ map 1
      {
      digitalWrite(blueled,LOW); 
      digitalWrite(greenled,HIGH);
      gameState = false;
      timeUse = drawCountTime(resetTimer);
      int timeUseForCal = timeUse;
      resetTimer = true;
      byteToMatrix(convertedByteArray,matrix_player);
      matrix_player[row][column]=0;
      matrix_player[1][0]=1;
      matrixToByte(matrix_player,convertedByteArray);
      return timeUseForCal;
      }

    else if(r==2 and rown==7 and column==7) //ถ้าถึงทางออก map 2
      {
      digitalWrite(blueled,LOW);
      digitalWrite(greenled,HIGH);
      gameState = false;
      timeUse = drawCountTime(resetTimer);
      int timeUseForCal = timeUse;
      resetTimer = true;
      byteToMatrix(convertedByteArray,matrix_player);
      matrix_player[row][column]=0;
      matrix_player[1][0]=1;
      matrixToByte(matrix_player,convertedByteArray);
      return timeUseForCal;
      }
    else if(r==3 and rown==7 and column==0) //ถ้าถึงทางออก map 3
      {
      digitalWrite(blueled,LOW);
      digitalWrite(greenled,HIGH);
      gameState = false;
      timeUse = drawCountTime(resetTimer);
      int timeUseForCal = timeUse;
      resetTimer = true;
      byteToMatrix(convertedByteArray,matrix_player);
      matrix_player[row][column]=0;
      matrix_player[1][0]=1;
      matrixToByte(matrix_player,convertedByteArray);
        return timeUseForCal;
      }
    }
    else //ถ้าเดินผิดทาง
    {
      digitalWrite(blueled,LOW);
      digitalWrite(redled,HIGH);
      gameState = false;
      int timeUseForCal = timeUse;
      resetTimer = true;
      timeUse = drawCountTime(resetTimer);
      digitalWrite(buzzer,HIGH);
      delay(1000);
      digitalWrite(buzzer,LOW);
      byteToMatrix(convertedByteArray,matrix_player);
      matrix_player[row][column]=0;
      matrix_player[1][0]=1;
      matrixToByte(matrix_player,convertedByteArray);
      digitalWrite(buzzer,HIGH);
      delay(1000);
      digitalWrite(buzzer,LOW);
      timeUseForCal=0;
      return timeUseForCal;    
    }
  }   
}      