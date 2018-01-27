void setup() {
  Serial.begin(9600); //Baud

  //specify analog output pins
  int pinUsed[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
  int index;
  for (index=0; index<12; index+=1){
    pinMode(pinUsed[index], OUTPUT);
  }

  Serial.println("Enter a pin number from 2 to 13 and an output value.");
}

String input;

void loop() {
  if(Serial.available())
  {
    char digit = serial.read();

    if (digit == "\n")
    {
      parsing(input);
      input = "";
    }
    else
    {
      input += digit;
    }
  }
}

void parsing(String in){
  //initialize variables internal to function
  String pinNum; 
  String pinOutput;

  //divide input string into its two values of pin number and output
  pinNum = in.substring(0, in.indexOf(" "));
  pinOutput = in.substring(in.indexOf(" ")+1);

  //change strings to numerical value
  int setPin = pinNum.toInt();
  float floatOutput = pinOutput.toFloat();

  //scale to write function input range and change to int
  setOutput *= 51; 
  int setOutput = (int)floatOutput;

  //write to analog pin
  analogWrite(setPin, setOutput);
}

