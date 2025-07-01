# Astrotracker H1
The Astrotracker H1 is a DIY star tracker camera mount for astrophotography, made to open access to astrophotography without needing to purchase a more expensive star tracker. Includes a multi-axis rotational gimbal, automatic North Pole alignment, and possible upcoming GoTo integration.

<img src="https://github.com/user-attachments/assets/fdabbf7d-fc66-46d1-a8b9-78f6e98e323c" height="450">
<img src="https://github.com/user-attachments/assets/1529174f-2fba-47f1-aa35-8de9ad0aa9d6" height="450">


# Figures

**Total Minimum Cost:** $220 USD

**Total hours spent:** hrs

*See JOURNAL.md and the BOM below for an in-depth explanation of these numbers.*


# How It Works

The Three-Part Process
-

There are always three parts to an engineering project like this, at least for the Highway YSWS program. One is computer-aided design (CAD) modeling; for large projects, this often takes the most time, as it did with this project. Another is printed circuit board (PCB) design, which is the electrical component of the project. Finally, the brain of the build is programmed in the software part, where firmware is written onto a microcontroller that controls the hardware. See the pictures below for how I went about this!

CAD:

<img src="https://github.com/user-attachments/assets/ef3d5406-489f-4ed6-808c-16b9482de664" height="450">

PCB:

<img src="https://github.com/user-attachments/assets/7ddf3a52-341b-4e1a-b705-9e8467ffd60f" height="450">

Firmware:

<img src="https://github.com/user-attachments/assets/97df2937-c1de-4e24-96b7-290230785419" height="450">

Star Trackers
-

Since astrophotography is done in very dark environments, a long exposure ranging from a second to the whole night is needed. Due to Earth's rotation, keeping the camera in the same place for a long exposure causes the image to display star trails instead of clear, detailed stars. Star trackers fix this problem by rotating the camera on an equatorial mount aligned with the path of the stars, eliminating star trails. See this picture for a diagrammed explanation:

<img src="https://github.com/user-attachments/assets/8fb4716c-f4d1-4220-9ddf-2e6f4bcd9941" height="450">

Astrotracker H1
-

This device attempts to solve these issues by upgrading the traditional equatorial mount to include a polar scope for precise alignment, a counterweight to balance the camera's weight, automatic computerized alignment using azimuthal and latitude (GPS) measurements, and a standard 1/4" tripod mount.

The mechanism is based on two plates connected by a hinge and a rotating gear. The bottom plate is the swivel plate, which swivels 360° and adjusts the azimuthal heading of the device. The other plate is the rotational axis (RA) plate, which tilts to match is angle of elevation to the device's GPS latitude and is driven by a precise worm gear. Matching the device's rotation to face exactly 0° north and its tilt to its GPS latitude allows the RA, the axis perpendicular to and in the middle of the rotating gear and the RA plate, to point exactly to the North Pole (next to the star Polaris). When the RA is pointed toward the North Pole and the rotating gear turns counterclockwise, the tracking will be able to accurately and precisely follow the path of the stars and prevent capturing star trails. Microstepping enhances these features by increasing the precision and resolution of the stepper motor by up to 64 times.

<img src="https://github.com/user-attachments/assets/742cd0ea-88f7-4ca4-8f89-0a78f77b9cf3" height="450">

Additional Features
-

Apart from the bare-bones mechanism that drives the star tracker, these upgrades elevate the project to create a truly effective star tracker:
- Automatic, computerized alignment using GPS and compass data
- 1/16 and 1/64 microstepping for stepper motors
- Polar scope for precise adjustment
- Counterweight for balance
- Bubble level for precision
- Thrust and radial ball bearings for smooth rotation
- 12.5:1 reduction ratio for belt-driven rotating gear
- 60:1 reduction ratio for worm gear-driven hinge
- 51:1 reduction ratio for the gearbox on the stepper motor turning the swivel plate
- Organized PCB connecting everything on one board
- Standard 1/4" camera and tripod mount screws
- Ball mount to point camera the right way
- Center control console with up-down-left-right buttons for fine adjustment, start-stop tracking buttons, auto-adjustment capability, and a smart LCD UI interface

<img src="https://github.com/user-attachments/assets/6636d2c8-e260-416a-bd52-7c95160e74e9" height="280">
<img src="https://github.com/user-attachments/assets/82e41896-1b35-4667-9c97-da2e2985ecbb" height="280">
<img src="https://github.com/user-attachments/assets/578ddc86-da92-454e-837b-f4daa4648be4" height="280">


# The 'Why' Behind the Project
As an avid photographer and engineer myself, I have always wanted to get more involved with astrophotography. Unfortunately, living in a city didn't play out well for me because of the massive amount of light pollution around me, blocking my view of the stars. I knew I wanted to combine my passions, and the opportunity to do so came through Hack Club's Highway to Undercity program. They fund almost any project you propose to them through their You Ship We Ship (YSWS) initiative.

After conducting some research, I found out that a star tracker was one of the most necessary tools to take better pictures of the night sky. Unfortunately, they were all priced around $500 USD - a hefty investment. Building a star tracker would allow me to combine my passions and achieve a more cost-effective solution at the same time.


# Why This Is Worth Ten Points
This star tracker isn't just any star tracker or equatorial mount - it adds many more complex features that empower young photographers on a budget, like myself, to pursue astrophotography. Fundamentally, the project is already as mechanically complex as a six-point project without all of the additional features. The time spent building intricate features into the mechanical model, custom footprints into the PCB, and special features into the firmware truly takes this project above and beyond the guidelines for a six-point project. 

Not only did I pay attention to the detail and precision needed for such a device with the bubble level, high reduction ratios, and microstepping, but I also considered what would take this above its minimum viable product by adding complex logic for auto-alignment, a balancing counterweight to increase control, and an intuitive LCD user interface controlled with standard pushbuttons. Being able to connect all the electrical components on a single, thought-out PCB and the firmware logic for tracking and auto-alignment indeed also makes this project electrically and computationally complex. Furthermore, I was able to achieve all of these features for under $300 USD when most market star trackers cost about $500 USD. Finally, I spent a ton of time perfecting every detail and feature, putting in almost sixty hours *before* building it (while the recommended minimum time spent for a ten-point project is fifty hours).


# **Bill of Materials (BOM):**

*Note: I am using Aliexpress for some and Amazon for others. Prices listed below are in USD, per part, and rounded. e.g. 2x Nema 17 ($14) = $28 total. Also see the .csv file for the official BOM.*

| Quantity | Name | Price Per Part | Link |
| ----- | ------------ | ------ | -------------------------------------------- |
| 2x | Nema 17 stepper motor (regular) | $14 | [https://www.amazon.com/STEPPERONLINE-Stepper-Bipolar-Connector-compatible/dp/B00PNEQKC0](url) |
| 1x | Nema 17 stepper motor (w/gearbox) | $42 | [https://www.amazon.com/51-Planetary-Gearbox-Stepper-Precision/dp/B00QEVLDVO](url) |
| 1x | F8-22M thrust ball bearing | $1 | [https://www.aliexpress.us/item/3256807051413217.html](url) |
| 1x | 10-pack radial ball bearing, 3mm bore | $2 | [https://www.aliexpress.us/item/3256807605344885.html](url) |
| 1x | Worm gear set (worm + gear) | $12 | [https://www.amazon.com/uxcell-Worm-Gear-Set-Reduction/dp/B0DFCVKXLX](url) |
| 1x | PCB board (through JLCPCB & Hack Club $20 Coupon) | $0, covered by coupon | See gerbers.zip in 'pcb' folder |
| 1x | 20-pack 6x6x14 pushbutton | $1 | [https://www.aliexpress.us/item/2255800980685095.html](url) |
| 1x | I2C 16x2 LCD with I2C backpack, blue color | $1 | [https://www.aliexpress.us/item/3256808568663888.html](url) |
| 1x | ESP32 DEVKIT WROOM-32 38-Pin Development Board Type-C | $1 | [https://www.aliexpress.us/item/3256808295104972.html](url) |
| 1x | Resistor pack | $5 | [https://www.amazon.com/Resistor-Resistors-Assortment-Breadboard-Electronics/dp/B0F4P352BB](url) |
| 1x | NEO-8M GPS Module | $1 | [https://www.aliexpress.us/item/3256806859439052.html](url) |
| 1x | QMC5883L Compass Module | $2 | [https://www.aliexpress.us/item/3256807913730129.html](url) |
| 3x | TMC2209 stepper motor driver | $1 | [https://www.aliexpress.us/item/3256808046478114.html](url) |
| 1x | 16T 5mm bore gear | $7 | [https://www.amazon.com/WINSINN-Aluminum-Synchronous-Timing-Printer/dp/B077GMR328](url) |
| 1x | 16T toothless idler bearing for 6mm width GT2 belt, 3mm bore | $9 | [https://www.amazon.com/WINSINN-Aluminum-Timing-Toothless-Printer/dp/B07BPHPYTN?th=1](url) |
| 1x | 2-pack GT2 6mm width belt, 530mm length | $7 | [https://www.aliexpress.us/item/3256808983126633.html?gatewayAdapt=glo2usa4itemAdapt](url) |
| 1x | Polar scope | $27 | [https://www.amazon.com/Tydeux-Polar-Telescope-Finderscope-EQ-5/dp/B0D2L11C4G/](url) |
| 1x | 3-pack 1/4" camera screw | $1 | [https://www.aliexpress.us/item/3256806761503223.html](url) |
| 1x | Camera ball joint mount, red color | $1 | [https://www.aliexpress.us/item/3256802805830814.html](url) |
| 1x | 5-pack bubble level, 18x9mm | $1 | [https://www.aliexpress.us/item/3256808611329580.html](url) |
| 4x | 200g counterweight | $1 | [https://www.aliexpress.us/item/3256807217165073.html](url) |
| 1x | 6S LiPo battery, 22.2V 1000mAh 100C, XT60 connector | $15 | [https://www.amazon.com/Ovonic-Battery-1000mAh-Connector-CINEWHOOP/dp/B07JPK99D9](url) |
| 1x | XT60 male and female plug pack wires, 14 AWG, 15cm length | $1 | [https://www.aliexpress.us/item/3256808104814371.html](url) |
| 1x | LiPo battery charger | $30 | [https://www.amazon.com/B6-Battery-Charger-Discharger-Connectors/dp/B0F2H3XR6S](url) |
| 1x | M4 nuts and bolts, 270 pcs | $9 | [https://www.amazon.com/Kadrick-Assortment-Metric-Washers-Upgrade/dp/B0BZ6XG8PT?th=1](url) |
| 4x | M3 50pcs bolts, 6mm, 12mm, 16mm, 35mm versions | $1 | [https://www.aliexpress.us/item/2255800794906149.html](url) |
| 1x | M3 50 pcs hex nuts | $1 | [https://www.aliexpress.us/item/3256807407546447.html](url) |
| 1x | M2.5x30mm 50pcs bolts | $1 | [https://www.aliexpress.us/item/2255799955713291.html](url) |
| 1x | M2.5 50 pcs nuts | $1 | [https://www.aliexpress.us/item/3256807407546447.html](url) |
| 1x | Allen key set | $1 | [https://www.aliexpress.us/item/3256807283413035.html](url) |
| 1x | Long-nosed plier | $1 | [https://www.aliexpress.us/item/3256805792649837.html](url) |

| **Total minimum cost** | $220 |
| ---------------------- | ----- |
| **Total including 'in-case' of price changes, spare parts, etc.** | $235 |
| **Total including in-cases, sales tax (assumed ~6%), import and customs fees, etc.** | $270 |
