#include <Servo.h>

//#define DEBUG

Servo servo1;
float servo1PowerLevel;
Servo servo2;

Servo servo3;
Servo servo4;

#include <math.h>

#define DRIVE_TRAIN_RIGHT '0'
#define DRIVE_TRAIN_LEFT '1'

// Define what the directions are
#define FORWARD '1'
#define BACKWARD '0'

// Define what the first character of a valid message is
#define START_CHAR '!'
#define STOP_ALL 's'

// variables for storing message parts
char motor_byte;
char dir_byte;
char power_bytes[4];

// The current byte being read
char in_byte;

// What part of the message we are reading
int read_index = 0;

// Wether or not we have a complete message or not
bool message_done = false;

void setup() {

  servo1.attach(9, 1000, 2000);
  servo2.attach(10, 1000, 2000);
  servo3.attach(2, 1000, 2000);
  servo4.attach(3, 1000, 2000);

  Serial.begin(9600);

}

void loop() {

  /* what for new data to become available */

  if (Serial.available() > 0) {
    in_byte = Serial.read();
    /* if the new data was the beginning of a message then reset everything */
    /* if the new byte says stop, then stop everything and reset */

#ifdef DEBUG
    Serial.print((char)in_byte);
#endif
    
    if (in_byte == STOP_ALL)
    {
      allStop();
      read_index = 0;
      message_done = false;
    }
    else if (in_byte == START_CHAR)
    {
      read_index = 1;
      message_done = false;
    }
    /* read the next byte into motor_byte */
    else if (read_index == 1)
    {
      motor_byte = in_byte;
      read_index = 2; // start looking for dir_byte
    }
    /* read the next byte into dir_byte */
    else if (read_index == 2)
    {
      dir_byte = in_byte;
      read_index = 3; // start looking for power_bytes
    }
    /* read the next three bytes into power_bytes */
    else if (read_index == 3)
    {
      power_bytes[0] = in_byte;
      read_index = 4;
    }
    else if (read_index == 4)
    {
      power_bytes[1] = in_byte;
      read_index = 5;
    }
    else if (read_index == 5)
    {
      power_bytes[2] = in_byte;
      read_index = 0; // stop looking for new data bytes (only start byte)
      message_done = true; // let the program know that the message isready
    }
  }
  /* if we have a full message, process it */
  if (message_done)
  {
    message_done = false;
    /* convert power_bytes and dir_byte into a signed integer */
    int p_level = atoi(power_bytes);
    if (dir_byte == BACKWARD) {
      p_level = -1 * p_level;
    }

    /* set the power level of the correct motor */
    if (motor_byte == DRIVE_TRAIN_RIGHT)
    {
      servo1.write(toNewPowerFormat(p_level));
      servo3.write(toNewPowerFormat(p_level));
    }
    if (motor_byte == DRIVE_TRAIN_LEFT)
    {
      servo2.write(toNewPowerFormat(p_level));
      servo4.write(toNewPowerFormat(p_level));
    }
  }
}

int toNewPowerFormat(int old)
{

  float temp;

  temp = ((float)old) / 128;
  temp = temp * 90 + 90;

  return (int)floorf(temp);
}

void allStop()
{
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
}
