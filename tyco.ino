#include <SoftwareSerial.h>


int l1=6, l2=9, r1=5, r2=3;

SoftwareSerial mySerial(11,10);

char buffer[125];

void set_speed(int joy_x, int joy_y) {

	int left = joy_y + joy_x;
	if(left >= 0) {
		analogWrite(l1, left);
		analogWrite(l2, 0);
	} else {
		analogWrite(l2, -left);
		analogWrite(l1, 0);
	}
	

	int right = joy_y - joy_x;
	if(right >= 0) {
		analogWrite(r1, right);
		analogWrite(r2, 0);
	} else {
		analogWrite(r2, -right);
		analogWrite(r1, 0);
	}

}

// the setup routine runs once when you press reset:
void setup() {                
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  
  mySerial.begin(9600);
  Serial.begin(9600);
}

typedef struct {
	char startbit;

	char xprefix;
	char xpos[2];

	char yprefix;
	char ypos[2];

	char endbit;
} frame;


char buf_xpos[3], buf_ypos[3];

// the loop routine runs over and over again forever:
void loop() {
  
	/*
	if(mySerial.available()) {
		Serial.print(mySerial.read());
	}
	return;
	*/
  
  if(mySerial.available()) {
		//Serial.println("Something available");
    int nreadBytes = mySerial.readBytesUntil(3, buffer, 125);
    buffer[nreadBytes] = 0;
    if(buffer[0] == 2) {

			frame *fr = (frame *) buffer;

			strncpy(buf_xpos, fr->xpos, 2);
			buf_xpos[2] = 0;
			int xpos = atoi(buf_xpos);
			xpos = (fr->xprefix=='2') ? xpos : -(100-xpos);

			strncpy(buf_ypos, fr->ypos, 2);
			buf_ypos[2] = 0;
			int ypos = atoi(buf_ypos);
			ypos = (fr->yprefix=='2') ? ypos : -(100-ypos);

			if(xpos!=0 && ypos!=0 && (xpos!=61&&ypos!=75)) {
				Serial.println(nreadBytes);
				Serial.println(buffer);

				Serial.print("X: ");
				Serial.println(xpos);

				Serial.print("Y: ");
				Serial.println(ypos);

				Serial.println();

				set_speed(xpos, ypos);
			}
    }
    else //normal frames start with byte 0x02 transmitted
      Serial.println("ERROR");
  }
  
	return;
  if(mySerial.available())
    mySerial.readBytes(buffer, 125);
  else
    return;

}
