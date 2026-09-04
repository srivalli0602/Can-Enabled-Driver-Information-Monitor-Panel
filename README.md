# 🚗 CAN-Enabled Driver Information & Monitoring Panel

<p align="center">

![Embedded Systems](https://img.shields.io/badge/Domain-Embedded%20Systems-blue)
![Language](https://img.shields.io/badge/Language-Embedded%20C-green)
![Microcontroller](https://img.shields.io/badge/MCU-LPC2129-orange)
![Communication](https://img.shields.io/badge/Communication-CAN-red)
![Simulation](https://img.shields.io/badge/Simulation-Proteus-purple)
![IDE](https://img.shields.io/badge/IDE-Keil%20µVision-yellow)
![Version Control](https://img.shields.io/badge/Version%20Control-Git%20%26%20GitHub-black)

</p>

> **A CAN-based embedded vehicle monitoring system developed using LPC2129 ARM7 microcontrollers to demonstrate real-time vehicle information monitoring, distributed node communication, fuel-level monitoring, temperature sensing, and indicator control.**

---

## 📌 Table of Contents

* [🎯 Project Aim](#-project-aim)
* [💡 Project Overview](#-project-overview)
* [✨ Key Features](#-key-features)
* [🏗️ System Architecture](#️-system-architecture)
* [🔩 Hardware Components](#-hardware-components)
* [💻 Software & Tools](#-software--tools)
* [🧩 Node Architecture](#-node-architecture)
* [📦 Module Development](#-module-development)
  * [LCD Module](#1️⃣-lcd-module)
  * [Delay Module](#2️⃣-delay-module)
  * [ADC Module](#3️⃣-adc-module)
  * [CAN Module](#4️⃣-can-module)
  * [DS18B20 Module](#5️⃣-ds18b20-module)
  * [External Interrupt Module](#6️⃣-external-interrupt-module)
* [📡 CAN Communication Protocol](#-can-communication-protocol)
* [⛽ Fuel Monitoring](#-fuel-monitoring)
* [🌡️ Temperature Monitoring](#️-temperature-monitoring)
* [🔄 Indicator Control](#-indicator-control)
* [🔁 Complete Working Flow](#-complete-working-flow)
* [🧪 Proteus Simulation](#-proteus-simulation)
* [📂 Project Structure](#-project-structure)
* [🚀 How the Project Was Developed](#-how-the-project-was-developed)
* [▶️ How to Run](#️-how-to-run)
* [📊 Expected Output](#-expected-output)
* [🧠 Technical Insights](#-technical-insights)
* [🐞 Debugging & Challenges](#-debugging--challenges)
* [🔮 Future Improvements](#-future-improvements)
* [📚 References](#-references)
* [👩‍💻 Author](#-author)

---

# 🎯 Project Aim

The main aim of this project is to **design and implement a distributed vehicle information and monitoring system using CAN communication**.

The system demonstrates how multiple embedded nodes can communicate with each other over a **Controller Area Network (CAN)** to exchange vehicle-related information.

The project focuses on:

* 🚗 Vehicle information monitoring
* ⛽ Fuel-level monitoring
* 🌡️ Temperature monitoring
* 💡 Left and right indicator control
* 📡 CAN-based communication between nodes
* ⚡ External interrupt-based switch detection
* 🖥️ Real-time LCD display
* 🧪 Embedded system simulation using Proteus

---

# 💡 Project Overview

Modern vehicles contain multiple Electronic Control Units (ECUs), where each ECU performs a specific function and communicates with other ECUs.

This project follows a similar **distributed ECU architecture**.

Instead of implementing everything in one microcontroller, the functionality is divided among multiple nodes.

### The project consists of three major nodes:

| Node | Main Responsibility |
|---|---|
| 🖥️ **Main Node** | Vehicle display, temperature monitoring and indicator command generation |
| ⛽ **Fuel Node** | Reads fuel level through ADC and transmits fuel percentage |
| 💡 **Indicator Node** | Receives indicator commands and controls LEDs |

The nodes communicate using the **CAN bus**.

---

# ✨ Key Features

### 📡 CAN Communication

* CAN-based communication between multiple LPC2129 nodes.
* Message identification using CAN IDs.
* Data exchange between Main, Fuel and Indicator nodes.

### ⛽ Fuel Monitoring

* Fuel level is represented using an analog voltage.
* LPC2129's internal ADC converts the analog input into a digital value.
* The fuel node converts the reading into a percentage.
* Fuel percentage is transmitted through CAN.

### 🌡️ Temperature Monitoring

* DS18B20 digital temperature sensor is connected to the Main Node.
* Temperature is read and displayed on a 16×2 LCD.

### 💡 Indicator Control

* EINT0 is used for one indicator switch.
* EINT1 is used for the other indicator switch.
* Main Node sends the corresponding command through CAN.
* Indicator Node controls 8 LEDs according to the received command.

### 🖥️ LCD Display

The LCD displays:

* Vehicle monitoring information
* Temperature
* Fuel percentage
* Fuel-level bar representation

---

# 🏗️ System Architecture

The system follows a **distributed automotive ECU architecture**, where the Main Node, Fuel Node, and Indicator Node perform dedicated functions and communicate through the **CAN bus**.

### 📊 System Architecture Diagram

<p align="center">
  <img src="Media/Block%20Diagram.jpeg" alt="CAN-Based Driver Information and Monitoring Panel - System Architecture" width="900">
</p>

### 🖥️ Main Node

* **LPC2129** acts as the central monitoring and control node.
* Interfaces with the **DS18B20 temperature sensor**.
* Interfaces with **EINT0 and EINT1** for indicator switch inputs.
* Displays vehicle information on the **16×2 LCD**.
* Receives fuel information through the **CAN bus**.
* Sends indicator commands through the **CAN bus**.

### ⛽ Fuel Node

* **LPC2129** is used as the dedicated fuel-monitoring ECU.
* Reads the analog fuel-level signal using the internal **ADC**.
* Converts the ADC reading into **fuel percentage**.
* Transmits the fuel information to the Main Node through **CAN**.

### 💡 Indicator Node

* **LPC2129** is used as the dedicated indicator-control ECU.
* Receives indicator commands through the **CAN bus**.
* Controls the **8 indicator LEDs**.
* Produces the required left and right indicator LED sequences.

### 📡 CAN Bus Communication

```text
                         CAN BUS
                            │
          ┌─────────────────┼─────────────────┐
          │                 │                 │
          ▼                 ▼                 ▼
    ┌───────────┐     ┌───────────┐     ┌──────────────┐
    │ Main Node │     │ Fuel Node │     │ Indicator    │
    │  LPC2129  │     │  LPC2129  │     │ Node LPC2129 │
    └───────────┘     └───────────┘     └──────────────┘
          │                 │                 │
          ▼                 ▼                 ▼
      LCD +             ADC + Fuel        8 LEDs +
     DS18B20              Sensor          Indicators

# 🔩 Hardware Components

| Component                         | Purpose                      |
| --------------------------------- | ---------------------------- |
| **LPC2129 ARM7**                  | Main microcontroller         |
| **MCP2551**                       | CAN transceiver              |
| **16×2 LCD**                      | Displays vehicle information |
| **DS18B20**                       | Digital temperature sensing  |
| **Fuel gauge** | Simulates fuel-level sensor  |
| **LEDs**                          | Indicator visualization      |
| **Push Buttons / Switches**       | Indicator input              |
| **USB-UART Converter**            | Serial/debug interface       |

---

# 💻 Software & Tools

| Tool / Technology | Usage                                |
| ----------------- | ------------------------------------ |
| **Embedded C**    | Application and driver development   |
| **Keil µVision**  | Compilation and firmware development |
| **Proteus**       | Circuit design and simulation        |
| **Flash Magic**   | Firmware programming                 |
| **Git**           | Version control                      |
| **GitHub**        | Source-code management               |
| **LPC2129 ARM7**  | Target microcontroller               |

---

# 🧩 Node Architecture

## 🖥️ 1. Main Node

The Main Node acts as the central monitoring and control node.

### Responsibilities

1. Initialize LCD.
2. Initialize CAN.
3. Initialize external interrupts.
4. Read temperature from DS18B20.
5. Receive fuel percentage through CAN.
6. Display fuel percentage.
7. Display fuel bar.
8. Detect indicator switches through EINT0/EINT1.
9. Transmit indicator commands through CAN.

### Main Node Flow

```text
              START
                │
                ▼
        Initialize Peripherals
                │
       ┌────────┼─────────┐
       │        │         │
       ▼        ▼         ▼
      LCD      CAN       EINT
       │        │         │
       └────────┼─────────┘
                │
                ▼
        Read Temperature
                │
                ▼
        Display Temperature
                │
                ▼
        Check CAN Reception
                │
                ▼
        Receive Fuel Data
                │
                ▼
        Display Fuel + Bar
                │
                ▼
       Check EINT0 / EINT1
                │
                ▼
       Send Indicator Command
                │
                └──────► Repeat
```

---

# 📦 Module Development

The project was developed using a **modular driver-based approach**.

Each hardware peripheral has its own `.c` and `.h` files.

This makes the project:

* Easier to understand
* Easier to debug
* Reusable
* Easier to maintain
* Easier to extend

---

## 1️⃣ LCD Module

### Files

```text
Drivers/
└── LCD/
    ├── LCD.c
    └── LCD.h
```

### Purpose

The LCD driver provides functions for displaying:

* Characters
* Strings
* Numbers
* Commands
* Custom characters

### Development Approach

The LCD was developed by implementing the basic HD44780 communication sequence.

Typical operations include:

```text
Initialize LCD
     ↓
Send Command
     ↓
Send Data
     ↓
Display Character/String
```

### Custom Characters

Three custom characters are used:

* Left arrow
* Right arrow
* Degree symbol

The custom characters are stored in **CGRAM** and displayed using their corresponding character codes.

---

# 2️⃣ Delay Module

### Files

```text
Drivers/
└── DELAY/
    ├── delay.c
    └── delay.h
```

### Purpose

The delay driver provides timing functions required by different peripherals.

Example:

```c
delay_ms(50);
```

The delay module is used where precise timing is required, such as:

* LCD timing
* DS18B20 communication
* LED indicator sequence

---

# 3️⃣ ADC Module

### Files

```text
Drivers/
└── ADC/
    ├── ADC.c
    └── adc.h
```

### Purpose

The ADC module is used by the Fuel Node to measure the analog fuel-level signal.

The LPC2129 ADC converts the analog voltage into a digital value.

For a 10-bit ADC:

```text
ADC Range = 0 to 1023
```

The conversion process is:

```text
Analog Fuel Signal
        │
        ▼
      ADC
        │
        ▼
Digital ADC Value
        │
        ▼
Equivalent Voltage
        │
        ▼
Fuel Percentage
```

---

# 4️⃣ CAN Module

### Files

```text
Drivers/
└── CAN/
    ├── CAN.c
    └── can.h
```

### Purpose

The CAN driver handles:

* CAN initialization
* CAN transmission
* CAN reception
* CAN status checking
* CAN frame handling

The application layer does not need to directly manipulate CAN controller registers.

Instead:

```c
Init_CAN1();
CAN1_Tx(CANF);
CAN1_Rx(&CANRF);
```

This keeps the CAN hardware implementation separated from application logic.

---

# 5️⃣ DS18B20 Module

### Files

```text
Drivers/
└── DS18B20/
    ├── ds18b20.c
    └── ds18b20.h
```

### Purpose

The DS18B20 driver handles communication with the temperature sensor.

The general communication sequence is:

```text
Initialize / Reset Sensor
          ↓
Send ROM / Skip ROM Command
          ↓
Start Temperature Conversion
          ↓
Wait for Conversion
          ↓
Read Temperature Data
          ↓
Process Temperature
          ↓
Display on LCD
```

The sensor communicates using a **single-wire digital interface**.

---

# 6️⃣ External Interrupt Module

### Files

```text
Drivers/
└── EINT/
    ├── eint.c
    └── eint.h
```

### Purpose

External interrupts are used to detect indicator switch presses.

Two interrupt inputs are used:

```text
EINT0 → Left Indicator
EINT1 → Right Indicator
```

When the switch is pressed:

```text
Switch Press
     ↓
External Interrupt
     ↓
ISR Executes
     ↓
Flag is Set
     ↓
Main Program Detects Flag
     ↓
CAN Command is Transmitted
```

Using interrupts avoids continuously polling the switch.

---

# 📡 CAN Communication Protocol

The project uses CAN IDs to identify different types of messages.

## CAN ID 1 — Indicator Command

| Field  | Meaning                |
| ------ | ---------------------- |
| CAN ID | `1`                    |
| Data1  | Left indicator status  |
| Data2  | Right indicator status |

### Example

```text
Left ON:

ID    = 1
Data1 = 1
Data2 = 0
```

```text
Right ON:

ID    = 1
Data1 = 0
Data2 = 1
```

```text
Indicators OFF:

ID    = 1
Data1 = 0
Data2 = 0
```

---

## CAN ID 3 — Fuel Information

| Field  | Meaning         |
| ------ | --------------- |
| CAN ID | `3`             |
| Data1  | Fuel percentage |

Example:

```text
ID    = 3
Data1 = 75
```

means:

```text
Fuel Level = 75%
```

---

# ⛽ Fuel Monitoring

The Fuel Node receives an analog voltage representing the fuel level.

The ADC converts this voltage into a digital value.

The measured voltage is mapped between:

```text
Minimum fuel voltage = 0.45 V
Maximum fuel voltage = 2.1 V
```

The percentage is calculated using:

```text
Fuel % = ((Voltage - Vmin) / (Vmax - Vmin)) × 100
```

The calculated value is limited to:

```text
0% ≤ Fuel ≤ 100%
```

The Fuel Node then sends the result through CAN:

```text
Fuel Sensor
     ↓
ADC
     ↓
Voltage
     ↓
Fuel %
     ↓
CAN ID 3
     ↓
Main Node
     ↓
LCD
```

---

# 🌡️ Temperature Monitoring

The Main Node directly reads temperature from the DS18B20 sensor.

The temperature value is processed and displayed on the LCD.

Example display:

```text
VEHICLE MONITOR:

Temp: 25.5°C
```

The temperature monitoring is performed locally because the DS18B20 is connected to the Main Node.

---

# 💡 Indicator Control

The indicator system uses two external interrupts.

### Left Indicator

```text
EINT0
  ↓
Left indicator flag
  ↓
CAN ID = 1
Data1 = 1
Data2 = 0
  ↓
Indicator Node
  ↓
LED sequence
  ↓
Left indicator
```

### Right Indicator

```text
EINT1
  ↓
Right indicator flag
  ↓
CAN ID = 1
Data1 = 0
Data2 = 1
  ↓
Indicator Node
  ↓
LED sequence
  ↓
Right indicator
```

The Indicator Node uses 8 LEDs.

Because the LEDs are configured as **active-low**, the corresponding LED is activated by driving the required GPIO output LOW.

---

# 🔄 Complete Working Flow

```text
                    POWER ON
                       │
                       ▼
              Initialize LPC2129
                       │
          ┌────────────┼────────────┐
          │            │            │
          ▼            ▼            ▼
         LCD          CAN          EINT
          │            │            │
          │            │            │
          ▼            │            │
   Read DS18B20        │            │
          │            │            │
          ▼            │            │
 Display Temperature   │            │
                       │            │
                       │            │
        Fuel Node      │            │
           │           │            │
           ▼           │            │
          ADC          │            │
           │           │            │
           ▼           │            │
      Fuel Percentage  │            │
           │           │            │
           └──────────► CAN ────────┘
                       │
                       ▼
                   Main Node
                       │
                       ▼
                Display Fuel Bar
                       │
                       │
            ┌──────────┴──────────┐
            │                     │
          EINT0                  EINT1
            │                     │
            ▼                     ▼
       Left Command          Right Command
            │                     │
            └──────────┬──────────┘
                       ▼
                    CAN BUS
                       │
                       ▼
                Indicator Node
                       │
                       ▼
                  LED Sequence
                       │
                       ▼
                    REPEAT
```

---

# 🧪 Proteus Simulation

The complete system can be tested using **Proteus simulation** before deploying the firmware to hardware.

The simulation includes:

* LPC2129 microcontrollers
* CAN communication
* MCP2551 CAN transceivers
* LCD
* DS18B20
* Analog fuel input
* Indicator LEDs
* External interrupt switches

### Simulation Testing

The following functions were verified:

| Test                  | Expected Result                        |
| --------------------- | -------------------------------------- |
| LCD initialization    | LCD displays project information       |
| Temperature sensing   | Temperature displayed correctly        |
| ADC input variation   | Fuel percentage changes                |
| Fuel CAN transmission | Main Node receives fuel value          |
| Fuel bar              | Number of bars changes with fuel level |
| EINT0 press           | Left indicator sequence activated      |
| EINT1 press           | Right indicator sequence activated     |
| Indicator OFF         | LEDs turn OFF                          |

---

# 📂 Project Structure

```text
Can-Enabled-Driver-Information-Monitor-Panel/
│
├── README.md
│
├── Docs/
│   ├── AIM.md
│   ├── SYSTEM_ARCHITECTURE.md
│   ├── CAN_PROTOCOL.md
│   └── FLOWCHART.md
│
├── Drivers/
│   │
│   ├── ADC/
│   │   ├── ADC.c
│   │   └── adc.h
│   │
│   ├── CAN/
│   │   ├── CAN.c
│   │   └── can.h
│   │
│   ├── DELAY/
│   │   ├── delay.c
│   │   └── delay.h
│   │
│   ├── DS18B20/
│   │   ├── ds18b20.c
│   │   └── ds18b20.h
│   │
│   ├── EINT/
│   │   ├── eint.c
│   │   └── eint.h
│   │
│   └── LCD/
│       ├── LCD.c
│       └── LCD.h
│
├── Src/
│   │
│   ├── Main_Node/
│   │   └── main_node.c
│   │
│   ├── Fuel_Node/
│   │   └── fuel.c
│   │
│   └── Indicator_Node/
│       └── indicator.c
│
├── Media/
│   ├── Block_Diagram.jpeg
│   ├── Circuit.jpeg
│   └── Output_Video.mp4
│
├── Simulation/
│   └── Proteus/
│
├── firmware/
│   ├── main_node.hex
│   ├── fuel_node.hex
│   └── indicator_node.hex
│
├── References/
│   └── README.md
│
└── .gitignore
```

---

# 🚀 How the Project Was Developed

The project was developed incrementally instead of implementing the entire system at once.

### Step 1 — LCD Development

First, the LCD was interfaced with LPC2129.

Basic operations were tested:

```text
LCD Initialization
        ↓
Character Display
        ↓
String Display
        ↓
Number Display
        ↓
Custom Characters
```

---

### Step 2 — ADC Development

The LPC2129 ADC was tested using an analog input.

The ADC value was displayed and verified.

```text
Analog Input
     ↓
ADC Conversion
     ↓
Digital Value
     ↓
LCD Display
```

---

### Step 3 — DS18B20 Development

The temperature sensor was interfaced and tested independently.

The temperature reading was displayed on the LCD.

---

### Step 4 — CAN Driver Development

The CAN peripheral was initialized and tested.

The CAN driver was designed to support:

```text
CAN Initialization
CAN Transmission
CAN Reception
CAN Status Checking
```

---

### Step 5 — Multiple CAN Nodes

After verifying CAN communication, the project was divided into:

```text
Main Node
Fuel Node
Indicator Node
```

This represents a simplified distributed automotive ECU architecture.

---

### Step 6 — Fuel Node

The Fuel Node was implemented to:

```text
Read ADC
   ↓
Calculate Fuel %
   ↓
Create CAN Frame
   ↓
Transmit CAN ID 3
```

---

### Step 7 — Indicator Node

The Indicator Node was implemented to:

```text
Receive CAN ID 1
       ↓
Identify Left / Right Command
       ↓
Control LEDs
```

---

### Step 8 — External Interrupts

EINT0 and EINT1 were added to detect indicator switch presses.

```text
EINT0 → Left Indicator
EINT1 → Right Indicator
```

---

### Step 9 — Integration

Finally, all modules were integrated and tested together in Proteus.

---

# ▶️ How to Run

## 1. Clone the Repository

```bash
git clone https://github.com/srivalli0602/Can-Enabled-Driver-Information-Monitor-Panel.git
```

## 2. Open the Source Code

Open the project in **Keil µVision**.

## 3. Build the Firmware

Compile each node firmware separately:

```text
Main Node
Fuel Node
Indicator Node
```

## 4. Generate HEX Files

Build the project and generate the required `.hex` files.

## 5. Load Firmware into Proteus

Assign the corresponding HEX file to each LPC2129 node.

## 6. Run Simulation

Verify:

* Temperature
* Fuel percentage
* Fuel bar
* CAN communication
* Left indicator
* Right indicator

---

# 📊 Expected Output

### LCD

```text
VEHICLE MONITOR:

Temp: 25.5°C
Fuel: 75%[######]
```

The fuel bar dynamically changes according to the received fuel percentage.

For example:

```text
Fuel: 100%[########]

Fuel: 75% [######]

Fuel: 50% [####]

Fuel: 25% [##

Fuel: 0%  []
```

### Indicator

```text
EINT0 → LEDs sequence from one side → Left Indicator

EINT1 → LEDs sequence from opposite side → Right Indicator
```

---

# 🧠 Technical Insights

This project helped demonstrate several important embedded-system concepts.

### 🔹 Microcontroller Peripherals

Hands-on implementation of:

* GPIO
* ADC
* External Interrupts
* CAN
* Timers/delays

### 🔹 Driver Development

Hardware-specific functionality was separated into reusable drivers.

```text
Application Layer
       │
       ▼
Peripheral Driver
       │
       ▼
Microcontroller Register
```

### 🔹 Interrupt-Based Design

External interrupts allow the system to respond to switch events without continuously polling the inputs.

### 🔹 Distributed Architecture

Instead of one MCU performing every operation, different nodes perform dedicated tasks.

This resembles the architecture used in automotive embedded systems.

### 🔹 CAN Communication

The project demonstrates:

* CAN identifiers
* CAN data frames
* Message-based communication
* Multi-node communication
* CAN-based command/data exchange

### 🔹 Active-Low GPIO

The indicator LEDs use active-low logic.

Therefore:

```text
GPIO = 0 → LED ON
GPIO = 1 → LED OFF
```

---

# 🐞 Debugging & Challenges

During development, several practical embedded-system issues were addressed.

### LCD Display Issues

Incorrect LCD output can occur because of:

* Improper initialization
* Incorrect command timing
* Insufficient delays
* Incorrect cursor positioning

These were resolved by verifying LCD initialization and command/data timing.

---

### ADC Calibration

The ADC produces a raw digital value, while the application requires fuel percentage.

Therefore, the measured voltage was mapped between the defined minimum and maximum fuel voltage.

---

### CAN Communication

CAN messages were tested by verifying:

```text
CAN ID
DLC
Data fields
Transmission
Reception
```

Each node was assigned a specific responsibility to simplify debugging.

---

### Indicator Direction

The LED sequence direction depends on the GPIO-to-LED wiring.

Therefore:

```text
P0.0 → P0.7
```

and

```text
P0.7 → P0.0
```

produce opposite visual directions when the LEDs are arranged accordingly.

---

# 🔮 Future Improvements

The current project can be extended into a more complete automotive monitoring system.

Possible improvements include:

* 🚗 Vehicle speed monitoring
* 🛞 Wheel-speed sensing
* 🔋 Battery voltage monitoring
* 🛢️ Engine oil-level monitoring
* 🚨 Warning/error messages
* 📈 CAN diagnostic messages
* 🔐 CAN error handling
* 💾 Data logging
* 📊 Graphical display
* 🧩 Additional ECU nodes
* 🔄 CAN bus error recovery
* 🧪 Hardware implementation instead of simulation
* 🛠️ Unit testing for individual drivers

---

# 📚 References

The following technical documentation was used during development:

| Reference                     | Purpose                                                   |
| ----------------------------- | --------------------------------------------------------- |
| **LPC2129 User Manual**       | MCU registers, GPIO, ADC, CAN and interrupt configuration |
| **DS18B20 Datasheet**         | Temperature sensor communication and timing               |
| **MCP2551 Datasheet**         | CAN transceiver operation                                 |
| **HD44780 LCD Documentation** | LCD commands, initialization and CGRAM                    |
| **ARM7TDMI Documentation**    | ARM7 architecture and interrupt concepts                  |

> Reference documents are used for technical implementation. The repository should preferably contain links to the official documentation rather than redistributing complete copyrighted datasheets/manuals.

---

# 📁 Media

The `Media/` directory contains project demonstrations and visual documentation.

```text
Media/
├── Block_Diagram.jpeg
├── Circuit.jpeg
└── Output_Video.mp4
```

### 🔲 Block Diagram

![Block Diagram](Media/Block_Diagram.jpeg)

### 🔌 Circuit

![Circuit](Media/Circuit.jpeg)

### 🎥 Simulation Demonstration

The output video demonstrates:

* Vehicle monitoring display
* Temperature
* Fuel percentage
* Fuel bar
* Indicator operation
* CAN-based communication

---

# 📌 Key Takeaway

This project demonstrates how an embedded application can be designed using a **modular driver architecture and distributed CAN-based communication**.

The important design principle followed is:

```text
Hardware
   ↓
Peripheral Drivers
   ↓
CAN Communication
   ↓
Application Logic
   ↓
Vehicle Monitoring / Control
```

Rather than building the complete application as one large program, the system was divided into reusable modules and independent nodes.

This approach improves:

* Maintainability
* Debugging
* Reusability
* Scalability
* Understanding of automotive embedded architecture

---

# 👩‍💻 Author

### **G. V. N. Srivalli**

**Embedded Systems Engineer | Embedded C | ARM7 | CAN | Microcontrollers**

Areas of interest:

```text
Embedded Software
Embedded C
ARM Microcontrollers
CAN Communication
RTOS
Embedded Linux
Automotive Embedded Systems
```

---

<p align="center">

### ⭐ If you found this project useful, consider giving the repository a star!

**Built with Embedded C, LPC2129, CAN and Proteus**

</p>
