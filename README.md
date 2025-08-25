# Finless Thrust Vector Controlled Rocket
This page details the progress I have made in developing an autonomous Thrust Vector Controlled Model Rocket. This project was inspired by Joe Barnard at BPS Space, with his launching and landing of model rockets, as well as my own interest in Aerospace & Controls Engineering. It is a continuation of a project that began last summer (2024), where I designed the first prototype of a gimbal mount and ran a couple of tests while firing a motor. [Link](https://github.com/Hunter-Rohovit/TVC-Gimbal-Mount). All the work shown here was done over this summer (2025).

## Sections of this page:
1. [Overview](#overview)  
2. [CAD](#cad)  
3. [Avionics](#avionics)  
4. [Gimbal Mount](#gimbal-mount)  
5. [Servo Characterization](#servo-characterization)  
6. [Experimental Tests](#experimental-tests)  
7. [Computational Fluid Dynamics](#computational-fluid-dynamics)  
8. [6DoF Simulation in MATLAB](#6dof-simulation-in-matlab)  
9. [Control Algorithm](#control-algorithm)  
10. [Parachute Ejection & Recovery System](#parachute-ejection--recovery-system)  
11. [Launch Pad](#custom-launch-pad)


![IMG_6904 (1)](https://github.com/user-attachments/assets/9193fb69-d851-4873-8680-e29c87584456)



---

## Overview
The goal of this project was to design, simulate, and build a finless rocket capable of stabilizing itself through thrust vectoring. This project integrates mechanical design, electronics, control theory, simulation, and experimental testing into a single system. Unfortunately, as the summer came to a close, I fell short of a first launch attempt by a couple of weeks. However, I made significant progress towards the eventual goal of launch and stable flight. Here's an overview of key accomplishments from this project so far: 
- Finalized design of gimbal mount and decreased thrust vector step size down to 0.2 degrees compared with the first prototype's step size of 0.75 degrees.
- Designed and assembled a custom flight computer Printed Circuit Board (PCB) consisting of a micro controller, Inertial Measurement Unit, Barometer Module, Data Storage, LoRa telemetry, and a MOSFET pyro channel.
- Used Solidworks CAD to design the rocket from scratch, integrating 3D printed mounts and other subsystems on the rocket.
- Developed a piston ejection system using 4Fg Black Powder for reliable deployment of parachutes.
- Designed and built a parachute to safely recover the vehicle during descent.
- Designed a custom Launch Pad with servo-driven quick-release Clamps, ignition circuit, and a custom Launch Pad PCB capable of controlling systems and transceiving radio commands.
- Computationally estimated and experimentally measured the rocket's Center of Gravity and Moment of Inertia about its lateral axes.
- Experimentally measured and analyzed servo motor hysteresis, latency, and response lag to accurately model gimbal mount dynamics.
- Formulated a non linear state-space representation of the rocket's rotational and translational dynamics in MATLAB.
- Built a full 6DoF state-space model of the rocket in MATLAB, integrating aerodynamic forces & moments, thrust vectoring, and environmental factors.
- Used Ansys Fluent's Computational Fluid Dynamics to determine lift and drag coefficients, aerodynamic moments, and center of pressure at various angles of attack to be used in 6DoF Simulation.
- Developed the structure of a Simulink Model integrating the 6DoF MATLAB function, a cascading PID Loop, and servo dynamics for accurate simulation and tuning of controller.
- Performed preliminary tuning and adjustment of PID gains of both inner and outer loops.
- Used C++ to program fundamental systems and tests of the Flight Computer, Launch Pad Computer, and Ground Station.

## CAD
Everything that was physically built was designed in Solidworks CAD beforehand. This includes the rocket, launch pad, and test stand. 
### Rocket
[Link To Onshape CAD Model of Rocket](https://cad.onshape.com/documents/63a8c4b73cd8738e99a4bd5d/w/619c465f67d6cd9f3f21c386/e/602e70b6c7c6c566dec7f707)
<img width="1904" height="271" alt="Image" src="https://github.com/user-attachments/assets/1b5335cf-3550-4e9d-b786-d5acf6c53edf" />
<img width="1903" height="290" alt="Image" src="https://github.com/user-attachments/assets/e051e71e-0873-4158-8883-095db382eaac" />
### Launch Pad
[Link To Onshape CAD Model of Launch Pad](https://cad.onshape.com/documents/538db86218341a015942f826/w/e10a91c609211941c8562602/e/c57fd2841c9af4f969e365a8?renderMode=0&uiState=68a87673f35ea043092abfba)
<img width="958" height="749" alt="Image" src="https://github.com/user-attachments/assets/22541fb5-6d22-4c90-824a-935fbbebc4d0" />

## Avionics
There were two custom Printed Circuit Boards built for this project. The first was for the flight computer and the second was for the launch pad computer. 
### Flight Computer
The flight computer consisted of a Teensy 4.0 Microcontroller, 5V and 3.3V Pololu Buck Converters, BNO085 Inertial Measurement Unit, BMP388 Barometer, SPI Flash SD, RFM95W Radio Transceiver, LED, Buzzer, external micro SD breakout, screw terminals for servo outputs, and a MOSFET circuit for a pyrotechnic ignition channel. I2C communication was primarily used for simplicity, though SPI communication was used for some devices.
#### Block Diagram
<p align="center">
  <img src="https://github.com/user-attachments/assets/32219732-c2da-4af3-a368-9d3f18d37975" width="1000">
</p>

#### Altium PCB Designer & Assembled PCB
<p align="center">
  <img src="https://github.com/user-attachments/assets/c5dc85ba-91ab-4ab4-9b9d-74dc3a3f61e7" width="400">
  <img src="https://github.com/user-attachments/assets/e71b7816-1b24-4344-b288-0c0137317bf2" width="400">
</p>




### Launch Pad Computer
The Launch Pad Computer consisted of a Teensy 4.0 Microcontroller, 5V Pololu Buck Converter, RFM95W Radio Transceiver, LEDs, Buzzer, 6 screw terminals for servo control (extra 2 for redundancy), 3 MOSFET circuits connected to Igniter Channels (2 extra channels for redundancy as well). 
#### Block Diagram
<p align="center">
  <img src="https://github.com/user-attachments/assets/854b1910-6c50-45d3-8732-c93f60410b72" width="1000">
</p>

#### Altium PCB Designer & Assembled PCB
<p align="center">
  <img src="https://github.com/user-attachments/assets/0f89f336-5ad1-49de-993b-66890e80bf5b" width="400">
  <img src="https://github.com/user-attachments/assets/3836f269-08bb-4d7a-b6cc-e502c21291a0" width="400">
</p>

## Gimbal Mount
Having a robust gimbal mount that can effectively vector thrust is key to the success of this project, which is why I spent an entire summer dedicated just to it. The gimbal mount is controlled by two servo motors each connected to linkage mechanisms. There are two "loops". The inner loop is connected to the rocket motor case at a pivot, allowing the inner servo motor to rotate the motor case back in forth. The entire inner loop is connected to the outer loop by a pivot. The servo motor on the outer loop moves the inner loop back and forth in a direction perpendicular to the inner servo to allow vectoring in two directions. This mount has maximum and minimum vector angles of 4.5 degrees in both the pitch and yaw directions. 

While the final design of the gimbal mount is the same as the first prototypes in terms of how it vectors the thrust, these were some key changes made to improve the overall design:
- Shortened distance between control horn pivot and linkage connection.
- Increased distance between linkage connection and motor case/inner loop pivot.
- Increased support on inner and outer loops for more rigidity.
  
These three changes increased servo resolution and decreased flex in the mount. For more info, look at "Old TVC Mount Issues" folder.
[Link to Onshape CAD for Gimbal Mount](https://cad.onshape.com/documents/97e67a90e16df294bea9c07c/w/7f59de1ae6b53eca2492785b/e/3da8b51d2cceb306dbc743ae?renderMode=0&uiState=68a87699922c3160367ccdfa)
<p align="center">
  <img src="https://github.com/user-attachments/assets/9d134729-5a47-4b03-a3ea-30e0f80a2cc5" width="400">
  <img src="https://github.com/user-attachments/assets/dfcfa19e-c8a7-4a01-93bd-2042fdaed939" width="400">
  <img src="https://github.com/user-attachments/assets/8fa94acd-9812-4f31-a717-380aacafddd5" width="400">
  <img src="https://github.com/user-attachments/assets/50c88e81-0574-4ec2-a2dc-e23cbb6f6bc8" width="400">
</p>


## Servo Characterization
In order to accurately represent the dynamics of the gimbal mount I needed an in depth understanding of how it moves when I command it to certain angles. This was one part of the project I hadn't anticipated needing to do, but as I began developing the Simulink Model, I realized it was very important. There are four characteristics of the servo I needed to understand it's dynamics:
1. Hysteresis (servos behave differently when commanded in different directions)
2. Latency (how much time it takes for the servo to begin moving once the command is called)
3. Response Lag (How long it takes for the servo to move to it's final position once the command is called)

### Servo Hysteresis
To accurately model this, I ran multiple tests increasing the servo angle from -22 to +22 degrees (max range of gimbal mount, corresponds to ~4.5 degrees actual) and decreasing from +22 to -22 degrees. 
I recorded the commanded angles and true angles and plotted them on graphs using MATLAB. I repeated this several times for both pitch and yaw axes. I then fitted a polynomial to the data. 
<p align="center">
  <img src="https://github.com/user-attachments/assets/f08536d8-6a70-4b9d-86fc-33d31fc3c7d4" width="400">
  <img src="https://github.com/user-attachments/assets/d3ee188e-6fef-49d9-acc1-33a51c50a729" width="400">
  <img src="https://github.com/user-attachments/assets/9e3eb0d4-4c3c-468e-9d82-c1036e41459c" width="400">
  <img src="https://github.com/user-attachments/assets/89dbb897-0473-4b5c-857a-b22f80e7dc47" width="400">
</p>


### Servo Latency & Response Lag
To find these two values I used a camera to capture the motion of the gimbal mount. When I sent the signal to the servo motor, I simultaneously turned on the LED on the Flight Computer, so I could visually record the time stamp when command sent. From there I recorded the time stamp where the gimbal mount began moving (Latency) and recorded the time stamp where the gimbal mount stopped it's movement (Response Lag). I counted the number of frames between these three time stamps and divided by the frame rate to get the actual time. I repeated this for multiple tests to get several values at different angles.
<p align="center">
  <img src="https://github.com/user-attachments/assets/105b2449-df60-4300-9cc4-40ead1440aea" width="1000">
  <img width="859" height="607" alt="image" src="https://github.com/user-attachments/assets/4ea49847-5e71-46e7-a5a5-692bbe75ddd2" />
</p>



## Experimental Tests
To further understand the system, two important values needed to be found: the Center of Gravity and the Moment of Inertia about the lateral axes. I had estimated these values using Solidworks CG locator by overwriting the material density, finding the CG of each individual part, and finding their distance from the nose tip. However, the physical tests account for additional fasteners, glue, and wires that I didn't account for in the calculation, making it more accurate. 
### Center of Gravity
Finding the center of gravity was relatively simple. I hung a string and created a loop for the rocket to be placed. I shifted the rocket around until it was able to balance by itself and labeled that point the CG. 
<img width="2915" height="1918" alt="image" src="https://github.com/user-attachments/assets/8f57155d-5e51-415d-a33e-ef8ac5d9b83a" />


### Moment of Inertia
To find the Moment of Inertia I used the Bifilar Pendulum Test. I hung two lines and attached them to the rocket 40 inches apart and centered about the CG, and recorded those measurements. I then rotated the rocket and started a timer as soon as I let go. After 10 oscillations, I recorded the time, to get the period of oscillation, and repeated this for multiple tests. From there, I could derive the Moment of Inertia from the mass of hte rocket, the length of the string, the distance between the two points, and the period time. I assumed the rocket to be symmetric about the longitudinal axis, so I only found one moment of inertia and used the same values for both Iy and Iz (pitch & yaw axes). I didn't find the roll Moment of Inertia as I could only control Pitcha and Yaw with Thrust Vectoring, and there wouldn't be any significant torques acting to roll the rocket. I still estimated it through a calculation, to be used in the 6DoF simulation.  
![IMG_4121 (1)](https://github.com/user-attachments/assets/98bfb930-e8ab-49be-a2cf-0a49f3796ac4)

## Computational Fluid Dynamics
Used Ansys Fluent CFD to deteremine coefficients of forces, moments, and the location of center of pressure at various angles of attack. Generated lift, drag, and moment coefficient curves across 0-90 degrees angle of attack. This data along with estimations for the aerodynamic damping derivative (derived from CFD results) was implemented into the MATLAB 6DoF function. For Lift and Drag Coefficients, Ansys was used to find the magnitude of these forces at 40m/s, which could then be used to solve for Cd x ReferenceArea and Cl x ReferenceArea rather than needing to calculate both Cd/Cl and reference area at different angles of attack. The static moment was normalized for velocity and CP location relative to CG. The center of pressure was defined from the nose tip and was calculated through dividing the static moment by the normal force to get the distance from CG. 

### 2D Simulation Velocity Contour
<img width="1233" height="613" alt="Ansys_Contour_Screenshot" src="https://github.com/user-attachments/assets/15e3b50d-fd90-44bf-a6e0-c634562168ea" />

### 3D Simulation Static Pressure Contour
<img width="970" height="357" alt="Static Pressure Contour CFD" src="https://github.com/user-attachments/assets/10c542b9-383d-44dc-a47a-132530589289" />

### CFD Results
<p align="center">
  <img src="https://github.com/user-attachments/assets/e6d4c5dc-1561-4d82-8417-38297ff5b903" width="400">
  <img src="https://github.com/user-attachments/assets/2ffe8c59-2080-4e9c-8dd3-f37bfd60376d" width="400">
  <img src="https://github.com/user-attachments/assets/d5a20113-4854-497a-86ac-559bbeff1e1a" width="400">
  <img src="https://github.com/user-attachments/assets/bbfe2d37-130d-4d66-af93-df1f56fe5d07" width="400">
</p>

## 6DoF Simulation in MATLAB
Developed the equations for state space estimation and built a six-degree-of-freedom function in MATLAB to compute the time derivative of the rocket's 6DoF state. The state equation was formulated using quaternions and angular velocity for rotational motion, as well as inertial position and velocity to represent the rocket's translational motion. The choice of using quaternions over traditional euler angles was to prevent gimbal lock in the simulation. This function uses control inputs (vector angles), thrust data, aerodynamic data, and environmental conditions to estimate the translational and rotational acceleration. Aerodynamic forces and moments are computed in the body reference frame, then transformed into the inertial frame to integrate the full equations of motion. This simulation is fundamental to the development of the control algorithm and generation of flight profiles. 

### Visualization of Forces, Moments, and Axes 
<img width="1001" height="860" alt="image" src="https://github.com/user-attachments/assets/3b0c4e9c-0466-4acd-8bb9-d95b7f78bb8d" />

## Control Algorithm
To develop the control algorithm I used Simulink to create a closed-loop model that integrates the 6DoF simulation from above and servo dynamics. The controller is structured as a cascaded PI-PID controller. The outer PI loop computes error based on attitude, and then outputs a desired angular rate. The inner loop then uses that output and the measured actual angular rate to calculate the error and generates commanded pitch and yaw outputs. The decision to use a PI rather than PID controller for the outer loop was based off the fact that the derivative of attitude error is angular rate, which is already being regulated in the inner loop. These commands then pass through a section that encapsulates the experimentally derived servo dynamics to feed the true thrust vector angles back into the process section. These were the Simulink Blocks used to model servo dynamics:
- Dead Zone: Very small changes to servo angle will not result in any change in servo position.
- Saturation: Maximum and minimum gimbal angles.
- Rate Limiter: Rising and falling slew rate of servo motor (maximum angular rate of motor).
- Transport Delay: Modeled servo latency
- Transfer Function: Used for servo response lag (the time to move from initial to final position)
- True Gimbal Function: Determines whether servo is moving up or down, and uses servo hysteresis data to ouput actual gimbal
- Zero-Order Hold: Models refresh rate of Inertial Measurement Unit (400hz)

Controller Tuning Strategy:
I have already completed preliminary tuning of this control algorithm, but some refinement is still needed. I started with the inner loop and then moved onto the outer loop, once the angular velocity showed convergence. The general strategy I used to tune both loops was to start with low Proportional gain and increase until I saw adequate response. If there were large-amplitude oscillations I increased the derivative gain, until it was less severe. If there was any steady-state offset I increased the integral gain. For both loops, I tuned one axis at a time, independently of one another. After, I modified the gains slightly when incorporating pitch and yaw together. In the future, I plan to add derivative filtering to the inner PID loop to prevent sharp spikes in commanded angles, and reduce the noise in the servo outputs.

### Simulink Model
<img width="1132" height="872" alt="image" src="https://github.com/user-attachments/assets/e12be683-ee75-4d46-9754-0e7bc859818e" />

### Scope Output
<img width="3831" height="2018" alt="Screenshot 2025-08-01 220636" src="https://github.com/user-attachments/assets/9c21da99-b5b3-4de0-8d97-36cea43cc9d5" />
Example output including angular velocity, quaternions, desired pitch rate, commanded pitch rate. These scopes are what allowed visual tuning of the gains. There's significant noise in the commanded pitch angle showing the need for derivative filtering. 

## Parachute Ejection & Recovery System
The parachute ejection system on the TVC model rocket is a custom built piston ejection device. At the bottom of this system is the piston canister bulkhead. It is what stores the FFFFg black powder and the e-matches used to ignite it. The high pressure created by the ignition gases pushes out the piston cap, which in turn also pushes out the shock cord, parachute, and nosecone. This ejection method is significantly more reliable and easier to implement than a mechanical deployment device. It's also a lot lighter, helping with total mass reduction. The electronics are very simple, only requiring a single MOSFET pyro channel. For recovery, the parachute was designed for a 1kg rocket (actual mass is ~850g) to land at a velocity of 4m/s. It's made of ripstop nylon fabric, which is very strong and ligthweight. Attached to the shroud lines is a swivel to reduce the chances of lines tangling. Here's a video of the parachute ejection test: [Ejection Test](https://github.com/Hunter-Rohovit/TVC-Rocket-Project/blob/main/Pictures_Videos/Rocket%20Systems%20Tests/Parachute_Ejection_Test.mov)      
### Recovery System
![IMG_4104](https://github.com/user-attachments/assets/9fbf0814-50a6-491c-906d-a575c6429cf9)
### Assembly
<p align="center">
  <img src="https://github.com/user-attachments/assets/f5b5bda0-0041-48a1-adb8-4aad38e67415" width="400">
  <img src="https://github.com/user-attachments/assets/875111f8-384a-4d7b-a96b-8cff231a27e6" width="400">
  <img src="https://github.com/user-attachments/assets/664e3cb9-bc86-4682-8366-88f54067281c" width="400">
  <img src="https://github.com/user-attachments/assets/e65e5683-b5cf-4c4c-81ae-797b1cecc3b3" width="400">
</p>


## Custom Launch Pad
The custom designed launch pad is composed of a spring loaded clamp release mechanism, a custom launch pad computer, and ignition circuit. In the secured position, the four clamps lock down onto the rocket's launch pad mount. Shortly after rocket motor ignition, the servo motors rotate and hit the release arms out from underneath the clamps, allowing the springs to snap downward. This design ensures the clamps retract quickly and remain out of the rocket's path. Here's a video releasing the launch clamps: [Launch Clamp Test](https://github.com/Hunter-Rohovit/TVC-Rocket-Project/blob/main/Pictures_Videos/Rocket%20Systems%20Tests/LaunchPadTest.mov)

### Clamped Position
<img width="3453" height="2130" alt="image" src="https://github.com/user-attachments/assets/b6ca4bf0-9b9f-4c58-a372-ef1b366c90ec" />

### Open Position
![IMG_4189](https://github.com/user-attachments/assets/e555f3f1-7844-4933-a082-b6355cd4bedd)



