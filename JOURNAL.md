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
