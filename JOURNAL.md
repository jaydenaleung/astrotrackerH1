---
title: "Astrotracker H1"
author: "Jayden Leung" (Hack Club Slack: loliipoppi)
description: "DIY star tracker for astrophotography with a 360° azimuthal axis, adjustable tilt, and automatic North Pole finding."
created_at: "2025-06-11"
---

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

I CADed this onto the rotational plate (RA plate), which I then connected via a hinge controlled by a worm gear to the swivel plate. The swivel plate would allow the whole mechanism to swivel along the azimuth/horizon, and the worm gear tilted the RA plate towards Polaris (the angle of elevation of the plate should equal my GPS latitude). See the picture below!

Picture

**Total time spent: 5.5h**