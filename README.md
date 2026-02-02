# arduino-graphical-multimeter
A multimeter design that is able to measure ACV DCV and current and display it on an OLED screen as a graph.
## What it does
This multimeter design uses a rotary switch to switch between measurement modes to measure AC/DC current and voltage. Measurement values are then scaled and displayed on the OLED screen.
## Power System
The system is powered by a 3.7 Li-ion battery that is connected to TP4056 charging module to protect the system against reverse polarity and overcharge as well as providing a way to recharge the battery. The output of TP4056 is connected to the MT3608 step-up module which increases the voltage to 5V. The output is then connected to Arduino Nano’s 5V pin through a switch to power the system.
## Rotary Switch System
This system is responsible for switching active circuits to change measurement modes. The rotary switch has, in use,  2 poles which have 3 throws each. The A pole changes the physical circuit by connecting the positive probe to throw 1, 2 or 3 which correspond to DCV, Current and ACV measurement respectively. The B pole shifts the HIGH logic output from D6 simultaneously with the A pole. This logic signal is redirected to Arduino’s digital inputs D8 and D4 which correspond to DCV and Current measurement through throws 4 and 5. If neither is active the Arduino selects the ACV measurement mode.
## DCV Measurement
The system is connected to the T1 of the rotary switch. The system design consists of a voltage divider and a voltage buffer. The voltage is divided using R5(380kΩ) and R6(20kΩ). This way a maximum of 100V voltage is divided to 5V which is then fed into the Arduino Nano A1 pin through a voltage buffer (voltage follower) op-amp circuit.
## ACV Measurement
This circuit includes a small addition to the DCV measurement circuit to allow measuring the negative swings of the AC voltage. The systems includes a voltage divider, a coupling capacitor, a DC bias and a voltage buffer. Since Arduino’s inputs can only measure 0-5V, the voltage needs to be divided and an offset is required to prevent the voltage from going negative. This setup allows the signal to swing around 2.5V instead of 0V which is then captured by the Arduino Nano’s A7 pin. (R1= 190kΩ , R2= 10kΩ, R3=R4= 100kΩ, C1 = 1 uF) 
## Current Measurement
This system utilizes an INA219 module and connects to Arduino using I2C connection. The module is powered by Arduino Nano’s 5V pin. SDA and SCL pins are connected to A4 and A5 respectively. Vout pin is connected to GND and Vin pin is connected to the probe through T2 of the rotary switch.
## Microcontroller Setup
The system is controlled by an Arduino Nano. The microcontroller’s purpose is to detect which system is active and use the input from that system to calculate the actual measurement by scaling the input. This value is then displayed on the OLED screen and added to the graph.\
D6 pin is set to HIGH logic level and connected to the B pole of the rotary switch through a 1kΩ resistor. D8 and D4 pins are digital inputs and are connected to throws 4 and 5 on the rotary switch. D8 and D4 are wired to the GND through a 10kΩ resistor in order to avoid floating values when the connection is open. A1 and A7 are wired to the outputs of the DCV and ACV measurement circuits. A4 and A5 are I2C pins and are connected to the SDA and SCL pins of the INA219. A4 and A5 pins are also connected to the SDA and SCL pins of the GME128128-01-IIC 1.5” OLED screen through a level shifter. Arduino Nano is powered through the 5V pin by the power system of the circuit.
## Display System
The display system consists of a GME128128-01-IIC 1.5’’ OLED display and a logic level shifter. The display’s SDA and SCL connected to the I2C pins (A4 and A5) through a logic level shifter to avoid harming the display. The screen is powered by the 3.3V pin on the Arduino Nano.
### Status: WIP / Working prototype
#### Author: Mert SIĞINMIŞ (GitHub: mertsgnms).
