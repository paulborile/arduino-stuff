/*
 * Paul Stephen Borile - 23/05/2013
 *
 * Max Lamp Controller
 *
 */

#define  RED_LED_PIN  9
#define  GREEN_LED_PIN  10
#define  BLUE_LED_PIN  11           // the pin that the LED is attached to


// Analog input pin Accelerometer in connected to

#define  ANALOG_IN_PIN_X  A0
#define  ANALOG_IN_PIN_Y  A1
#define  ANALOG_IN_PIN_Z  A2

// global values read from accelerometer

int  sensorValueX = 0;
int  sensorValueY = 0;
int  sensorValueZ = 0;

// Color arrays
int black[3]  = { 0, 0, 0 };
int white[3]  = { 255, 255, 255 };
int red[3]    = { 255, 0, 0 };
int green[3]  = { 0, 255, 0 };
int blue[3]   = { 0, 0, 255 };
int yellow[3] = { 51, 120, 0 };
int dimWhite[3] = { 75, 75, 75 };
// etc.

// accelerometer actions

#define  SWITCH_ON   1  
#define  SWITCH_OFF  2
#define  CHANGE_TO_RED   5
#define  CHANGE_TO_WHITE  6
#define  CHANGE_TO_GREEN  7
#define  CHANGE_TO_BLUE  8
#define  CHANGE_TO_YELLOW  9


void setup() {

  
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  
  Serial.begin(9600);
  
  crossFade(black);
}

#define  DIM_CYCLE_STEP 32

#define  STATE_ON  1
#define  STATE_OFF  0
int  current_state = STATE_OFF;
int  action = 0;
int  last_action = 0;

void loop() {


  
  // test function prints out values
  
  // test_accelerometer();

  // tets led dimmer
  
  //crossFade(green);
  //delay(1000);
  
  //dim_cycle(10, green);
  //delay(300);
    
  // test led fader
    
  // crossFade(red);
  
  //Serial.println("red");
  //delay(2000);
  //crossFade(green);
  //Serial.println("green");
  //delay(2000);
  //crossFade(blue);
  //Serial.println("blue");
  //delay(2000);
  
  // loop reading accelerometer and control led
  
  action = read_accelerometer();
  
  if ( action != 0 ) {
    Serial.print("last_action = ");
    Serial.print(last_action, DEC);
    Serial.print(" action = ");
    Serial.println(action, DEC);
  }
  switch (action) {

    case  SWITCH_ON :
        
        Serial.println("loop() - SWITCH_ON");
        crossFade(white);
        current_state = STATE_ON;
        delay(500);
        
        break;
  
    case  SWITCH_OFF :
        
        Serial.println("loop() - SWITCH_OFF");
        crossFade(black);
        current_state = STATE_OFF;
        delay(500);
                
        break;
  
    case  CHANGE_TO_RED :
    
      if ( current_state == STATE_ON ) {
        if ( last_action == CHANGE_TO_RED ) {
            dim_cycle(DIM_CYCLE_STEP, red);
        }
        else {
          crossFade(red);
        }
      }     
      break;

    case  CHANGE_TO_WHITE :
  
      if ( current_state == STATE_ON )
        crossFade(white);
      
      break;

    case  CHANGE_TO_GREEN : 
      
      if ( current_state == STATE_ON ) {
        if ( last_action == CHANGE_TO_GREEN ) {
            dim_cycle(DIM_CYCLE_STEP, green);
        }
        else {
          crossFade(green);
        }
      }
      
      break;

    case  CHANGE_TO_BLUE :
      
      if ( current_state == STATE_ON ) {
        if ( last_action == CHANGE_TO_BLUE ) {
            dim_cycle(DIM_CYCLE_STEP, blue);
        }
        else {
          crossFade(blue);
        }
      }
      
      break;

    case  CHANGE_TO_YELLOW :  
      
      if ( current_state == STATE_ON ) {
        if ( last_action == CHANGE_TO_YELLOW ) {
            dim_cycle(DIM_CYCLE_STEP, yellow);
        }
        else {
          crossFade(yellow);
        }
      }      
      
      break;  
   
   default :
    // do nothing
    ;
  }
  // record only valid actions
  if ( action != 0 ) {
    last_action = action;
  }
}

unsigned long  msec_since_last_read = 0;
#define  MIN_TIME_BETWEEN_ACCEL_READ  500

int  read_accelerometer() {

  unsigned long  now;
 
  
  // read values
  
  now = millis();
  
  if ((now - msec_since_last_read) < MIN_TIME_BETWEEN_ACCEL_READ) {
    return 0;
  }  
    
  sensorValueX = analogRead(ANALOG_IN_PIN_X);
  sensorValueY = analogRead(ANALOG_IN_PIN_Y);
  sensorValueZ = analogRead(ANALOG_IN_PIN_Z);
  
  msec_since_last_read = now;
  
   
  /*
  Serial.print("raw values x: ");
  Serial.print(sensorValueX);
  Serial.print(" | y: ");
  Serial.print(sensorValueY);
  Serial.print(" | z: ");
  Serial.println(sensorValueZ);
  */
  
  // check position 1 : red
  
  // if ((sensorValueX > 347 && sensorValueX < 355) && (sensorValueY > 320 && sensorValueY < 335) && (sensorValueZ > 410 && sensorValueZ < 423)) {
  
  // clock position 9 arduino board side
  
  if (sensorValueX > 345) {
    Serial.print("clock pos 9 - sensorValueX : ");
    Serial.println(sensorValueX, DEC);
    if (current_state == STATE_OFF) {
      return SWITCH_ON;
    }
    else {
      return SWITCH_OFF;
    }
  }
  
  // Clock position 6 - led driver side 
  
  if (sensorValueY > 340 ) {
    Serial.print("clock pos 6 : sensorValueY : ");
    Serial.println(sensorValueY, DEC);

    return CHANGE_TO_GREEN;
  }

  // clock position 12 - led sockets side

  if (sensorValueY < 310 ) {
    Serial.print("clock pos 12 : sensorValueY : ");
    Serial.println(sensorValueY, DEC);
    
    return CHANGE_TO_BLUE;
  }
  
  // clock position 3 - red led side
  
  if ( sensorValueX < 315 ) {
    Serial.print("clock pos 3 : sensorValueX : ");
    Serial.println(sensorValueX, DEC);

    return CHANGE_TO_RED;
    
  }
 
  return 0;
}

/* 
 * use to test values read from accelerometer
 */
 
void test_accelerometer(){

  /*
   * The minimum and maximum values that came from
   * the accelerometer while standing still
   * You very well may need to change these
   */
   
  int minVal = 265;
  int maxVal = 402;
  
  double x, y, z;
  
  //read the analog values from the accelerometer
  sensorValueX = analogRead(ANALOG_IN_PIN_X);
  sensorValueY = analogRead(ANALOG_IN_PIN_Y);
  sensorValueZ = analogRead(ANALOG_IN_PIN_Z);
  
  //Output the caculations
  
  Serial.print("raw values x: ");
  Serial.print(sensorValueX, DEC);
  Serial.print(" | y: ");
  Serial.print(sensorValueY, DEC);
  Serial.print(" | z: ");
  Serial.println(sensorValueZ, DEC);

  //convert read values to degrees -90 to 90 - Needed for atan2
  int xAng = map(sensorValueX, minVal, maxVal, -90, 90);
  int yAng = map(sensorValueY, minVal, maxVal, -90, 90);
  int zAng = map(sensorValueZ, minVal, maxVal, -90, 90);

  //Caculate 360deg values like so: atan2(-yAng, -zAng)
  //atan2 outputs the value of -p to p (radians)
  //We are then converting the radians to degrees
  
  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  //Output the caculations
  /*
  Serial.print("360deg x: ");
  Serial.print(x);
  Serial.print(" | y: ");
  Serial.print(y);
  Serial.print(" | z: ");
  Serial.println(z);
  */
  delay(1000);//just here to slow down the serial output - Easier to read
}


/*
 * cross fader
 */
 
/*
 * BELOW THIS LINE IS THE MATH -- YOU SHOULDN'T NEED TO CHANGE THIS FOR THE BASICS
 * 
 * The program works like this:
 * Imagine a crossfade that moves the red LED from 0-10, 
 *   the green from 0-5, and the blue from 10 to 7, in
 *   ten steps.
 *   We'd want to count the 10 steps and increase or 
 *   decrease color values in evenly stepped increments.
 *   Imagine a + indicates raising a value by 1, and a -
 *   equals lowering it. Our 10 step fade would look like:
 * 
 *   1 2 3 4 5 6 7 8 9 10
 * R + + + + + + + + + +
 * G   +   +   +   +   +
 * B     -     -     -
 * 
 * The red rises from 0 to 10 in ten steps, the green from 
 * 0-5 in 5 steps, and the blue falls from 10 to 7 in three steps.
 * 
 * In the real program, the color percentages are converted to 
 * 0-255 values, and there are 1020 steps (255*4).
 * 
 * To figure out how big a step there should be between one up- or
 * down-tick of one of the LED values, we call calculateStep(), 
 * which calculates the absolute gap between the start and end values, 
 * and then divides that gap by 1020 to determine the size of the step  
 * between adjustments in the value.
 */



// Set initial color
int redVal = black[0];
int grnVal = black[1]; 
int bluVal = black[2];

int wait = 1;      // 10ms internal crossFade delay; increase for slower fades
int  int_step = 2;    // intesity step - increse for faster fades 
int hold = 0;       // Optional hold when a color is complete, before the next crossFade
int DEBUG = 1;      // DEBUG counter; if set to 1, will write values back via serial
int loopCount = 60; // How often should DEBUG report?
int repeat = 3;     // How many times should we loop before stopping? (0 for no stop)
int j = 0;          // Loop counter for repeat

// Initialize color variables
int prevR = redVal;
int prevG = grnVal;
int prevB = bluVal;


int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero, 
    step = 1020/step;              //   divide by 1020
  } 
  return step;
}

/* The next function is calculateVal. When the loop value, i,
*  reaches the step size appropriate for one of the
*  colors, it increases or decreases the value of that color by 1. 
*  (R, G, and B are each calculated separately.)
*/

int calculateVal(int step, int val, int i) {

  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += int_step;           
    } 
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= int_step;
    } 
  }
  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {
    val = 255;
  } 
  else if (val < 0) {
    val = 0;
  }
  return val;
}

/* crossFade() converts the percentage colors to a 
*  0-255 range, then loops 1020 times, checking to see if  
*  the value needs to be updated each time, then writing
*  the color values to the correct pins.
*/

void crossFade(int color[3]) {
  // Convert to 0-255
  //int R = (color[0] * 255) / 100;
  //int G = (color[1] * 255) / 100;
  //int B = (color[2] * 255) / 100;

  // use 0-255 valures
  int R = color[0];
  int G = color[1];
  int B = color[2];


  int stepR = calculateStep(prevR, R);
  int stepG = calculateStep(prevG, G); 
  int stepB = calculateStep(prevB, B);

  for (int i = 0; i <= 1020; i++) {
    redVal = calculateVal(stepR, redVal, i);
    grnVal = calculateVal(stepG, grnVal, i);
    bluVal = calculateVal(stepB, bluVal, i);

    analogWrite(RED_LED_PIN, redVal);   // Write current values to LED pins
    analogWrite(GREEN_LED_PIN, grnVal);      
    analogWrite(BLUE_LED_PIN, bluVal); 

    delay(wait); // Pause for 'wait' milliseconds before resuming the loop

    if (DEBUG) { // If we want serial output, print it at the 
      if (i == 0 or i % loopCount == 0) { // beginning, and every loopCount times
        Serial.print("Loop/RGB: #");
        Serial.print(i);
        Serial.print(" | ");
        Serial.print(redVal);
        Serial.print(" / ");
        Serial.print(grnVal);
        Serial.print(" / ");  
        Serial.println(bluVal); 
      } 
      DEBUG += 1;
    }
  }
  // Update current values for next loop
  prevR = redVal; 
  prevG = grnVal; 
  prevB = bluVal;
  delay(hold); // Pause for optional 'wait' milliseconds before resuming the loop
}


int verse = 0;
  
void dim_cycle(int dim_step, int color[3]) {

  int newcolor[3];
  int adj_color[3];
  
  adj_color[0] = color[0];
  adj_color[1] = color[1];
  adj_color[2] = color[2];
  
  
  Serial.print(" dim cycle - verse : ");
  Serial.print(verse, DEC);
  
  Serial.print(" redVal : ");
  Serial.print(redVal, DEC);
  
  Serial.print(" grnVal. : ");
  Serial.print(grnVal, DEC);
  
  Serial.print(" bluVal : ");
  Serial.println(bluVal, DEC);

  if ( verse == 0) {
    newcolor[0] = redVal - dim_step;
    newcolor[1] = grnVal - dim_step;
    newcolor[2] = bluVal - dim_step;
  }
  else {
    newcolor[0] = redVal + dim_step;
    newcolor[1] = grnVal + dim_step;
    newcolor[2] = bluVal + dim_step;
  }
    
  if ( newcolor[0] > adj_color[0] ) newcolor[0] = adj_color[0];
  if ( newcolor[1] > adj_color[1] ) newcolor[1] = adj_color[1];
  if ( newcolor[2] > adj_color[2] ) newcolor[2] = adj_color[2];
  
  
  if ( newcolor[0] < 0 ) newcolor[0] = 0;
  if ( newcolor[1] < 0 ) newcolor[1] = 0;
  if ( newcolor[2] < 0 ) newcolor[2] = 0;
  
  if ( (newcolor[0] == 0) && (newcolor[1] == 0 ) && (newcolor[2] == 0))
    verse = 1; // up now
  
  if ( (newcolor[0] == adj_color[0]) && (newcolor[1] == adj_color[1] ) && (newcolor[2] == adj_color[2]))
    verse = 0; // down now
  
    
  Serial.print("New color R = ");
  Serial.print(redVal, DEC);
  Serial.print(" G = ");
  Serial.print(grnVal, DEC);
  Serial.print(" B = ");
  Serial.println(bluVal, DEC);
  
  crossFade(newcolor);
  
  //analogWrite(RED_LED_PIN, redVal);   // Write current values to LED pins
  //analogWrite(GREEN_LED_PIN, grnVal);      
  //analogWrite(BLUE_LED_PIN, bluVal);
  
  return;
}

