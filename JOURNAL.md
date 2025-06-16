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

Picture

I also sketched a few examples of what the tracker could look like in CAD. I eventually settled on the second iteration (look at the second picture). Sketching first helped me to effectively iterate and create the most efficient design without having to take a lot of time.

Picture

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