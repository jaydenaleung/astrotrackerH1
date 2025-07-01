---
title: "Astrotracker H1","hi"
author: "Jayden Leung (Hack Club Slack: loliipoppi)"
description: "DIY star tracker for astrophotography with a 360° azimuthal axis, adjustable tilt, and automatic North Pole finding."
created_at: "2025-06-11"
---

***IGNORE THIS FOR NOW, STILL IN PROGRESS (ESTIMATED PROJECT TIME: 60HRS)**
- **Total hours: **
- **Time spent on CAD: **
- **Time spent on PCB design: 11hr**
- **Time spent on coding: 9h**
- **Time spent on assembly: **
- **Time spent on sourcing parts, writing journal and readme, etc.: **

# June 11th: Research and Planning

Before beginning the technical work on my project, I needed to first research how a star tracker worked and what the best design for it was. I learned my lesson in previous projects to do this step first before jumping right in so I can maximize my efficiency. Bottom line, start with your MVP (minimum viable product) and build up instead of shooting for the stars and having to cut down on your project.

ChatGPT was my best friend here. I looked at a few YouTube videos, websites, forums, similar projects, etc., but ChatGPT was able to answer all my questions on simple star movements. Here are some of the resources I used: https://wiki.openastrotech.com/en/OpenAstroTracker, https://openastrotech.com/, https://agenaastro.com/sky-watcher-star-adventurer-gti-goto-mount-head-kit-s20590.html?rfsn=7382628.cf60d1, https://www.youtube.com/watch?v=RtHJ5TVkeu4

Through a series of questions, conclusions, and deductions (see my whiteboard planning below), I found out that, from Earth, stars appear to move east/counterclockwise around Polaris (the north star) at the sidereal rate (~15°/hr). This meant that all I had to do was point the axis of my rotational plate towards Polaris and turn the plate counterclockwise at 15°/hr to match the movement of the stars. It also turned out that this would work no matter the direction the camera was pointed in or even if the camera wasn't centered on the rotational axis, as long as it was physically connected to the rotational plate.

![2025-06-11 17-44](https://github.com/user-attachments/assets/c219f0d9-f97c-430a-8c27-108103f690e2)

I also sketched a few examples of what the tracker could look like in CAD. I eventually settled on the second iteration (look at the second picture). Sketching first helped me to effectively iterate and create the most efficient design without having to take a lot of time.

![2025-06-11 23-44](https://github.com/user-attachments/assets/b3ccc74e-d53e-40a7-9913-1e3c0901fccb)
![2025-06-12 00-30](https://github.com/user-attachments/assets/65bff95a-f332-4908-92cd-7ebe3311ecb5)


This was my MVP in the end (see below). There were a few assumptions and oversights that I intentionally made to simplify the design first.

Functions (Must be able to…):
- Rotate the rotating base
- Have near-270 deg turn for camera
- Align the rotating base

Parts (Must have…):
- Base that rotates the rotating base
- Rotating base - stepper motor
- Mount on rotating base for camera (use tripod mount on the camera)

Restrictions (OK to not have…):
- Belt drive/worm gear & ball bearings
- Tripod mount
- Northern/southern hemisphere switching
- Auto Correction for Polaris/rotating base calibration depending on coords
- GoTo compatibility
- Offset for through-hole polar scope OR polar scope off to the side

**Total time spent: 5h**


# June 12-13th: CADing the Rotational Plate and Hinge

The next two days after planning I spent CADing the rotational plate, swivel plate, and the hinge that connected them in Onshape. I followed the format shown in the second design (look at the previous journal entry). I decided to have the rotational gear (RA gear) be connected to a stepper motor through a belt drive system. This would provide enough torque to move the heavy camera and enable the device to rotate the plate at the slow speed of 15°/hr. I first settled for a 400T RA gear, which would give me a 1:25 gear ratio for a 16T stepper motor gear. With 1.8° steps and 1/32 microstepping (using PWM to split the steps even further on the stepper motor), that would have given me about 8.1 arcseconds/microstep, which was within the optimal range for medium to long range (zoom) astrophotography.

After discovering that a 400T gear was pretty big, I settled for a 200T gear instead but had to increase the 1/32 microstepping to 1/64 to maintain the rotational rate of 8.1 arcsecs/microstep. This would also give me enough torque to move a payload of about 500g-1kg. I ran some calculations and found that the optimal center to center distance between the RA gear and the stepper motor gear was 100mm. This would allow the gears to be close enough to mimize the size of the plate while still allowing enough space between the two gears for idler bearings that would fit the timing belt tighter. 200T gear, 16T gear, and 100mm C-C dist => 530mm GT2 belt; the GT2 belt had a pitch (tooth-to-tooth distance) of 2mm and was a bit long for the setup, but was perfect after securing it with a 20T toothless idler bearing that jutted 3.1mm into the belt.

I CADed this onto the rotational plate (RA plate), which I then connected via a hinge controlled by a worm gear to the swivel plate. The swivel plate would allow the whole mechanism to swivel along the azimuth/horizon, and the worm gear tilted the RA plate towards Polaris (the angle of elevation of the plate should equal my GPS latitude). The RA gear was generated by an Onshape script, and I CADed its additional details by hand since I couldn't find an internet model for a 200T belt drive gear. I found one for the 16T gear, though. See the picture below!

![Screenshot 2025-06-16 at 3 56 46 PM](https://github.com/user-attachments/assets/783d44ad-d46f-4438-8027-3682df74224d)

**Total time spent: 5.5h**


# June 16th: CADing the Swivel Plate and Adding Accurate Part Models

I spent today adding the functionality for the swivel plate (the plate that is connected to the RA plate by a hinge) by creating a housing for another NEMA 17 stepper motor with a 51:1 gear reduction gearbox. On top of the motor sits a thrust ball bearing, and on top of that sits the swivel plate and the entire mechanism with it. Next time, when I put the finishing touches on my CAD model, I'll make sure to add arms to the housing that curve up to support the heavy swivel plate; I didn't get to do it this time. This is a crucial part of the device since it rotates the RA plate to face Polaris on the azimuthal axis. However, I do have some reservations about allowing the entire mechanism (possibly ~500g) to sit on top of a ball bearing and a motor housing, especially when considering the extra 1-2kg load put on by the camera and its opposing counterweight. Those changes will have to be determined after printing and testing.

In case you're confused about how I spent five hours on this, I had to first research the best way to effectively rotate the swivel plate, run calculations to find the best motor gearbox that would allow for enough torque, and find the best bearing that would work for this. I also finally decided to spend some time finding online models of the motors and bearings I was using. To my great surprise, downloading the files as STEP files instead of STL files made it SO MUCH EASIER because I was working with solid, editable parts rather than meshes. Onshape was much friendlier to me with these files because of this. I was surprised because I thought I had tried this before with an earlier plane project, and they still were meshes. Meshes or not, this greatly simplified my work as it allowed me to make the right edits to these models to ensure they were accurate to the specific motor dimensions I was working with.

For reference, here are some great websites and libraries of CAD models that were helpfuL: [https://grabcad.com/library](url), [www.traceparts.com/en](url), [https://www.thingiverse.com/](url), (next two are more STLs and 3D printing files, along with Thingiverse) [https://cults3d.com/](url), [https://www.printables.com/](url).

And just as a side note, before I started working on the CAD files, I finally decided to make and update my journal. It took a few hours since I re-read all the custom project rules and had to set up my journal from scratch.

Pictures of the updated assembly!
![Screenshot 2025-06-17 at 5 20 49 AM](https://github.com/user-attachments/assets/d32584bc-ba68-4fcf-bcbe-5e21ac2d3bae)
![Screenshot 2025-06-17 at 5 21 13 AM](https://github.com/user-attachments/assets/60842a6a-b461-44aa-babe-4d40f0bbe2f5)
![Screenshot 2025-06-17 at 5 21 43 AM](https://github.com/user-attachments/assets/c7280587-61b4-449a-a857-1b97eacecad4)
![Screenshot 2025-06-17 at 5 22 08 AM](https://github.com/user-attachments/assets/0947ef17-1933-4952-9031-e7eafcece23c)

**Total time spent: 5h**


# June 18-23: Finishing the CAD (for now)

Throughout these six days (I was working quite sporadically, which is why it took so long), I finally finished adding the final touches and reference model to my assembly. It looks great!! I employed a dark grey-orange color scheme to match the color of my tripod.

Before I reveal the final result, I'll list the changes I made:
- Added real models of the NEMA 17 stepper motors
- Created and added housing for the NEMA 17 gearbox that turns the whole mechanism
- Implemented a thrust bearing to make the turning of the mechanism smoother
- Added arms onto the motor housing to better support the heavy mechanism, using ball bearings to reduce friction
- Added compatibility for the polar scope
- Added a mount for the camera and its counterweight
- Added a restraining clip to hold the gear and camera mount down
- Added a bubble/spirit level on the swivel plate for extra accuracy
- Placed ball bearing model into the assembly
- Placed polar scope model into the assembly
- Placed camera, camera mount, counterweight, and 1/4-20 screws for the camera into the assembly
- Placed tripod into the assembly
- Changed colors to dark grey-orange to reflect my tripod
![Screenshot 2025-06-23 at 8 30 47 PM](https://github.com/user-attachments/assets/4a3fc981-2f9d-4941-af63-73788ed790f3)

Here are the final results! I'll update the model again after I'm finished with the PCB design, and I'll add a housing for the PCB.
![Screenshot 2025-06-23 at 8 30 19 PM](https://github.com/user-attachments/assets/48125de6-afaf-42fb-a79b-6b6c07bfe767)
![Screenshot 2025-06-23 at 8 30 32 PM](https://github.com/user-attachments/assets/826a3012-46d4-481e-9260-dc46552d5d4b)
![Screenshot 2025-06-23 at 7 49 47 PM](https://github.com/user-attachments/assets/93af6a85-16c8-4cfe-a166-5be8a48e5616)

**Total time spent: 9h**


# June 24: PCB Part Sourcing & Schematic

As I got started on my PCB design (which, as I found being a beginner, had a steep learning curve), I quickly ran into trouble sourcing parts, symbols, footprints, and the like. I found [snapeda.com](url) (and surprisingly, no other websites) to be really helpful in this, until I encountered the "No 2D model" (symbol or footprint) label for some parts. These were mostly the parts that weren't as common, such as the QMC5883L compass module I was using, so I had to learn how to make the footprints and symbols by hand. While it was certainly frustrating and time-consuming, I think it was a good exercise, as I heard from people around the internet that that was a common area of PCB design. Here's a good resource I found: [https://www.youtube.com/watch?v=NiSkKemLK_8](url). I had to make "empirical measurements," which really involved me putting a clear, plastic ruler to my computer screen to measure my parts for my custom footprints. Fortunately, I don't think they were too inaccurate, but I'd have to check that when my PCB is delivered.

Because of the reasons mentioned above, I think that the PCB design step was the hardest (at least so far). Surprisingly, it took only half the time as to do CAD, which I was already pretty proficient in. I certainly learned a lot. For example, someone on the Hack Club Slack told me I could just use net flags instead of a ton of wires. Also, (about) seven and a half hours! It was certainly a sprint... but Undercity is a marathon! (get it, because it's a hackathon... haha...)

Here's the schematic:
![image](https://github.com/user-attachments/assets/9a216d7a-0506-446e-a1d0-45249d398259)

Here's a custom footprint I made:
![image](https://github.com/user-attachments/assets/d043d100-348c-4f76-9269-876968925a2e)

**Total time spent: 7.5hr**


# June 25: Routing & Finishing the PCB

To be honest, I thought routing the board would be a pain because of the immense number of wires I placed down in the schematic. It was actually pretty chill... except for the insane amount of vias I placed down (4+ vias on one connection??). However, it still stands now as one of my favorite parts of the engineering process. It's very satisfying to route the PCB and see your work come to life; in fact, seeing the fruits of my efforts so quickly is one of the primary reasons I do engineering in the first place.

It wasn't without some effort, however. I had to fix a few things on the schematic that the DRC picked up (pin numbers, annotations, file changes, etc.), as well as place the footprints in the right place. I spent a good amount of time trying to figure out how to put certain footprints on the back, only to find out you just have to press 'F'. I wanted to place them on the back so I could have the LCD screen and buttons in the front and all the microcontrollers/logic/drivers/etc. out of the way. It did come with the nice benefit of being able to place the footprints on top of each other to save space (given that the through-holes don't touch). After adjusting the Edge.cuts rectangle to extend 3mm from the outmost point on each side, I began routing.

Here's my finished PCB layout, before routing:
![Screenshot 2025-06-25 173033](https://github.com/user-attachments/assets/6a747a96-abc6-4dfd-9afe-a4c262a5b6fc)

And after:
![Screenshot 2025-06-25 180735](https://github.com/user-attachments/assets/f095ae40-9e24-44e3-8d89-407dbf3ab61c)

**Total time spent: 3.5hr**


# June 25-27: Coding the Firmware

After finishing my PCB on June 25th, I moved on to the software piece. I really had no idea what to expect from this phase since I've had relatively little experience programming hardware. Funny enough, although I had a bunch of components and functionalities as well as almost 500 lines of code, it took me the least amount of time compared to the other areas of this project.

I coded for the ESP32 in the Arduino IDE. I started with the stepper motor drivers, which I had to consult various website tutorials, official library code on Github, and ChatGPT. I quickly realized the learning curve, but thankfully it didn't take me long to understand the code I needed to write. After really being able to understand and engage with the software, I started to really enjoy the experience! I learned a lot about I2C serial buses, UART communications, and firmware coding. After the stepper drivers, I moved onto the more software-like piece (as opposed to firmware coding) where I programmed the logic for manually adjusting the tilt and azimuth of the mechanism using a set of four up-down-left right buttons. I also implemented another button for auto-North Pole alignment by matching the tilt of the mechanism to my GPS latitude and turning the mechanism to face north (see GPS and compass modules in the next-next sentence). Two more buttons were used for starting and stopping tracking. Then, I moved onto the NEO-8M (GPS) and QMC5883L (compass) modules, which were relatively simple to extract data from to use. Finally - and arguably the most time-consuming and problem solving-intense part - I programmed the UI through an I2C serial bus LCD interface. I had a few screens, which were:
- Home screen: "READY FOR ALIGNMENT"
- Auto-alignment: "ALIGNING + Latitude + Heading / x secs left…"
- Auto-alignment complete: "ALIGN COMPLETE (for 1s)"
- Tracking on: "TRACKING… / x hours x minutes x seconds"
- Tracking off: "STOPPED (for 1s)"
- Manual alignment: Shows custom arrows on the home screen to indicate direction
- Other custom markings/logos for azimuth, latitude, which hemisphere the device is in, etc.

In the process of doing so, I had to implement a few complex features such as tracking elapsed time, saving to non-volatile memory, etc. It was a fun excercise that increased my coding skills!

Examples of my code:
![Screenshot 2025-06-28 at 12 08 58 AM](https://github.com/user-attachments/assets/43771c23-2b89-486b-8513-90dcd8efb5ba)
![Screenshot 2025-06-28 at 12 08 26 AM](https://github.com/user-attachments/assets/2e7bc9b8-4855-415d-a494-d77490b46d26)
![Screenshot 2025-06-28 at 12 09 18 AM](https://github.com/user-attachments/assets/0df3d688-a1da-40b0-a591-714637547193)

**Total time spent: 9hr**


# June 28-30: Preparing to Propose the Project

After finishing the firmware, most of the project was already finished. The only steps left were to add a housing onto the structure for the PCB in CAD, finish sourcing parts and writing my BOM, update my journal and readme, and submit! For the housing, I figured I could just export the PCB from KiCad as a STEP file and make the holes in the case using that solid part. It made it a lot easier, except that I decided to make the housing on a sloped plane, which didn't work well when I tried to align the coordinates of the PCB with my sketch (the coordinates were in the XYZ direction). I began to finish my part sourcing during this stage because I needed the dimensions of the electronic components I was using to make the housing. This was also helpful because I realized that I needed to make two more holes for my LiPo battery supply.

Before submitting, I read over the submission guidelines again to make sure I had what I needed. Then, I went to write my journal (which I'm doing now!), and after this, I will write the readme and submit. Even though I still have a ways to go until the project is built and finished, I have to say that this project (50+ hours!) is one of the biggest projects I've ever built, one of the projects I'm most proud of, and definitely the project where I've learned the most. I first-handly experienced the learning curve at the beginning of a stage - whether it be CAD, PCB, or software - and saw how easily I could beat it if only I persevered through the beginning, explored, and put lots of time into it. In fact, my favorite outcome from this project isn't the final product but all of the skills I picked up through the way and the fun of learning so quickly. Thank you, Hack Club, for making this YSWS and community possible - I've been absolutely obsessed with Highway to Undercity for the past few weeks.

The BOM can be found in the readme. Of course, some pictures of the final design (CAD, PCB, and my magazine page design):
![Astrotracker H1 Magazine](https://github.com/user-attachments/assets/d8ef337d-7465-4fb0-a75a-600d3da2bda3)

![image](https://github.com/user-attachments/assets/0a49afde-ec49-477e-ba7d-6e80c2598333)

![Screenshot 2025-06-29 143516](https://github.com/user-attachments/assets/bb97f868-2bd2-43ed-828a-0c9c9434387f)
![Screenshot 2025-06-29 145517](https://github.com/user-attachments/assets/909fe59c-9607-4ace-a37b-e20816a2dd3b)
![Screenshot 2025-06-29 135726](https://github.com/user-attachments/assets/385da61f-6a0c-4180-8ee6-ebabd43b79aa)
![Screenshot 2025-06-29 033557](https://github.com/user-attachments/assets/ac30933d-8069-4835-ba35-f516a03119c1)
![Screenshot 2025-06-29 033515](https://github.com/user-attachments/assets/38ef93da-db89-4f8d-b3d4-9a61b6eeee45)
![Screenshot 2025-06-29 033445](https://github.com/user-attachments/assets/d27db634-4394-450f-8333-c2bfc52f1a1f)
![Screenshot 2025-06-29 033621](https://github.com/user-attachments/assets/4fe4aefd-49f4-4803-8935-9a1eec814e5b)
![Screenshot 2025-06-29 033419](https://github.com/user-attachments/assets/181997a6-9fbc-4db7-9b05-7cf1c4d1c70d)
![Screenshot 2025-06-29 033347](https://github.com/user-attachments/assets/79e439a8-8458-4a26-9b91-4121e47e76dc)
![Screenshot 2025-06-29 033332](https://github.com/user-attachments/assets/7482e0ef-e75c-4d3f-8b27-b043281cf570)
![Screenshot 2025-06-29 033256](https://github.com/user-attachments/assets/06c5fbe3-b83a-42b0-9321-9a9f1c481280)
![Screenshot 2025-06-29 033205](https://github.com/user-attachments/assets/70541154-920c-4235-940a-480b6af581e1)
![Screenshot 2025-06-29 033149](https://github.com/user-attachments/assets/8508eeb3-7329-4c74-a949-1965857d1056)
![Screenshot 2025-06-29 032955](https://github.com/user-attachments/assets/4309f75b-c9e2-40af-9019-7c1b400709ff)

You can never have too many pictures!

**Total time spent: 12hr**
