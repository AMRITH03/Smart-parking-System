# Smart-parking-System
An Arduino-based Smart Parking System simulation created in Tinkercad, featuring real-time parking slot detection using ultrasonic sensors,  automated gate control with servo motors, and user-friendly status display via I2C LCD. Designed to optimize parking efficiency.

## Features
- Real-time parking slot detection using ultrasonic sensors   
- Automated gate control with servo motors  
- User-friendly status display via I2C LCD  
- Bill calculation based on the parking time  

## Circuit Diagram
<img width="1329" height="750" alt="Screenshot 2025-08-31 at 10 56 35 PM" src="https://github.com/user-attachments/assets/f30bc120-ab35-41db-9e0c-6fea3e7e62fb" />


## Technologies Used
- Arduino  
- Ultrasonic Sensors   
- Servo Motors  
- I2C LCD Display  
- Tinkercad Simulation

## How It Works
- Ultrasonic sensors scan parking slots to detect availability.  
- Vehicles approach the gate.  
- Depending upon the available parking, the servo motor opens the gate.  
- The I2C LCD displays the system status and slot availability.   
- The system calculates the parking bill based on the total parking time and displays the amount on the LCD.

## C Concepts Used in the Smart Parking System

- **Structures (`struct`)**: Used to define custom data types like `ParkingSlot` and `Vehicle` for organizing related data logically.  
- **Pointers and Dynamic Memory Allocation**: Usage of pointers for managing vehicle data dynamically with functions like `malloc` and `realloc`.  
- **Arrays**: Used for storing multiple parking slots, occupancy status, sensor pins, and timing data.  
- **Macros and Constants (`#define`)**: For defining fixed values such as pin numbers, thresholds, and rates to improve code readability and maintainability.  
- **Functions**: Modularization of code into reusable functions performing specific tasks such as distance measurement, billing, gate control, and slot assignment.  
- **Control Flow Statements**: `if-else`, `for`, and `while` loops handle conditional logic, repeated checks, and delays for sensor input processing and system behavior.  
- **String Handling**: Use of character arrays and functions like `strcpy` for managing textual data such as vehicle owner names.  
- **Input/Output via Serial Communication**: Interaction with the user to get input and display debug information.  
- **Time Handling**: Use of `millis()` function to measure time for calculating parking duration and applying billing based on elapsed time.  
- **Bitwise Operations and Digital I/O**: Pin configuration and state reading/writing using digital I/O functions to interact with hardware components.  
- **Memory Management**: Explicit management of allocated memory for handling dynamic vehicle records.  
- **Use of Libraries**: Integration of Arduino and external libraries like `LiquidCrystal` and `Servo` for hardware-specific functionalities.  

## License
This project is open-source and free to use for learning and development purposes.
