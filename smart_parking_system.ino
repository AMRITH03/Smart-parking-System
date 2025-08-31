#include <LiquidCrystal.h>
#include <Servo.h>
#include <stdio.h>
#include <string.h>

struct ParkingSlot {
    int parking_slot_number;
    long total_time;
};

struct Vehicle {
    int parking_slot_number;
    char vehicle_owner_name[50];
};

#define ENTRY_TRIGGER_PIN 2
#define EXIT_TRIGGER_PIN 4
#define DIST_THRESHOLD 50       // Distance threshold in cm
#define NO_MOTION_DELAY 3000    // 30 seconds in milliseconds
#define MOTIONLESS_TIME 5       // 5 seconds in milliseconds
#define FEE_PER_SECOND 0.1      // Fee per second of parking

struct ParkingSlot slots[7];   
int occupied[] = {0,0,0,0,0,0,0};         
unsigned long last_detected_time[] = {0,0,0,0,0,0,0};  
unsigned long parking_start_time[] = {0,0,0,0,0,0,0};

int SLOT_PINS[] = {0, 8, 9, 10, 7, 6, 5};  

LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);
Servo servo;
int number=0;
int total=10;
struct Vehicle *ptr=(struct Vehicle*) malloc(total*sizeof(struct Vehicle));
int *p1=&number;
int *p2=&total;
long readUltrasonicDistance(int trigEchoPin) {
    pinMode(trigEchoPin, OUTPUT);
    digitalWrite(trigEchoPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigEchoPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigEchoPin, LOW);

    pinMode(trigEchoPin, INPUT);
    long duration = pulseIn(trigEchoPin, HIGH);
    
    long distance = duration * 0.034 / 2;
    return distance;
}

/*void write_to_file(struct Vehicle *vehicles,int tot)
{
  FILE vehicle_file = SD.open("vehicles.txt",FILE_WRITE);
  
  for(int i=0;i<tot;i++)
  {
    vehicle_file.print(i);
    vehicle_file.print(")");
    vehicle_file.print(vehicles[i].parking_slot_number);
    vehicle_file.print(" , ");   
    vehicle_file.println(vehicles[i].vehicle_owner_name);
  	vehicle_file.close();
  }
}*/
void addVehicles(struct Vehicle *ptr,struct Vehicle car,int *num,int *tot){
  if (*num==*tot){
    (*tot)+=10;
    ptr=(struct Vehicle*) realloc(ptr,*tot*sizeof(struct Vehicle));
  }
  ptr[*num]=car;
  (*num)+=1;
}
float calculate_total_bill(int slot_index) {
    unsigned long current_time = millis();
    unsigned long total_time = current_time - parking_start_time[slot_index];
  
    float fee = (total_time / 1000) * FEE_PER_SECOND;
    return fee;
}

void clear_parking_slot(int slot_index) {
    occupied[slot_index] = 0;               
    parking_start_time[slot_index] = 0;     
}

void checkParkingSlotStatus(int slot_index) {
    long distance = readUltrasonicDistance(SLOT_PINS[slot_index]);

    if (distance > 0 && distance <= DIST_THRESHOLD) {
        if (last_detected_time[slot_index] == 0) {
            
            last_detected_time[slot_index] = millis();
        }
        
        
        if ((millis() - last_detected_time[slot_index]) >= MOTIONLESS_TIME * 1000 && parking_start_time[slot_index] == 0) {
            parking_start_time[slot_index] = millis(); 
            Serial.print("Parking started at slot ");
            Serial.print(slot_index);
            Serial.print(" at time: ");
            Serial.println(parking_start_time[slot_index]);
        }
    } else {
        
        last_detected_time[slot_index] = 0;
    }
}


int detectEntry() {
    long distance = readUltrasonicDistance(ENTRY_TRIGGER_PIN);
    return (distance > 0 && distance <= DIST_THRESHOLD);
}


int detectExit() {
    long distance = readUltrasonicDistance(EXIT_TRIGGER_PIN);
    return (distance > 0 && distance <= DIST_THRESHOLD);
}

int assign_parking_slot_id(int *occupied) {
    for (int i = 1; i <= 6; i++) {  
        if (occupied[i] == 0) {  
            occupied[i] = 1;      
            return i;             
        }
    }
    return -1; 
}


void openGate() {
    servo.write(90);  // Open the gate
}


void closeGate() {
    servo.write(0);   // Close the gate
}


void log_vehicle_data(struct Vehicle vehicle) {
    Serial.print("Vehicle Owner: ");
    Serial.print(vehicle.vehicle_owner_name);
    Serial.print(", Parking Slot: ");
    Serial.println(vehicle.parking_slot_number);
}


int show_bill(int slot_index) {
  	if (occupied[slot_index] == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("No car found");
        delay(1000);  
      	return 0;
    }
    float bill = calculate_total_bill(slot_index);
  	unsigned long total_time_seconds = (millis() - parking_start_time[slot_index]) / 1000;
    Serial.print("Total bill for Slot ");
    Serial.print(slots[slot_index].parking_slot_number);
    Serial.print(" is: ");
    Serial.print(bill, 2);
  	Serial.println("Rs");
  	lcd.clear();
  	lcd.setCursor(0,0);
  	lcd.print("Bill: ");
  if(parking_start_time[slot_index] == 0){
  		lcd.print(0);
  }
  else{
    lcd.print(bill);
  }
  	lcd.print(" Rs");
  	lcd.setCursor(0, 1);
    lcd.print("Time: ");
    if(parking_start_time[slot_index] == 0){
  		lcd.print(0);
  }
  else{
    lcd.print(total_time_seconds);
  }
    lcd.print(" s");
    clear_parking_slot(slot_index);  
  	return 1;
}

void setup() { 
    lcd.begin(16, 2);  
    servo.attach(3);   
    servo.write(0);    
    Serial.begin(9600);	

    lcd.setCursor(0, 0);
    lcd.print("Smart Parking");
    delay(2000);

    // Initialize parking slots with 1-based indexing
    for (int i = 1; i <= 6; i++) {
        slots[i].parking_slot_number = i;
        slots[i].total_time = 0;
    }
}

void loop() {
    lcd.clear();

    if (detectEntry()) {
        lcd.setCursor(0, 0);
        lcd.print("Car Detected");
      
        char owner_name[50];
        Serial.println("Enter vehicle owner's name: ");
        while (Serial.available() == 0) {} 
        Serial.readBytes(owner_name, 50);

        int assignedSlot = assign_parking_slot_id(occupied);

        if (assignedSlot != -1) {
            lcd.setCursor(0, 1);
            lcd.print("Your Slot: ");
            lcd.print(assignedSlot); 
            openGate();  
            while(detectEntry()){}
            delay(2000);
            closeGate(); 

            struct Vehicle vehicle;
            vehicle.parking_slot_number = assignedSlot;
            strcpy(vehicle.vehicle_owner_name, owner_name);
            log_vehicle_data(vehicle);
        } else {
            lcd.setCursor(0, 1);
            lcd.print("No Slots Free");
        }
        delay(2000);
    } else if (detectExit()) {
        lcd.setCursor(0, 0);
        lcd.print("Enter Slot Num");

        int exit_slot;
        while (Serial.available() == 0) {} 
        exit_slot = Serial.parseInt();

        if (exit_slot > 0 && exit_slot <=6) {
                      lcd.setCursor(0, 1);
            int sb = show_bill(exit_slot);
            if(sb == 1) {
                openGate();  
                while(detectExit()) {}  
                delay(2000);
                closeGate();
            }
        }
        delay(1000);
    } else {
        lcd.setCursor(0, 0);
        lcd.print("Waiting for car");
        delay(500);
    }

    
    for (int i = 1; i <= 6; i++) { 
        checkParkingSlotStatus(i);
    }
}