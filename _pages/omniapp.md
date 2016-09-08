---
permalink: /projects/omniapp
layout: default
title:  OmniApp - Omnidirectional Visual Surveillance
tags:   
image:  omnidirectional.png
---

# OmniApp - Omnidirectional Visual Surveillance
------------------

This is my MSc project, titled *"Omnidirectional Visual Tracking"*, that was submitted 
in 2003 for my *MSc Engineering & Information Science* degree at the University of Reading.

The source code, which is available on github [here](https://github.com/mark-borg/OmniApp) is in C++. 

The dataset used for training and testing the algorithms is the PETS 2001 dataset 
that is available for download [here](http://www.cvg.reading.ac.uk/slides/pets.html). The version 
acquired using the catadioptric sensor is used.

{{ more }}

A catadioptric sensor is a camera made up of a mirror (catoptrics) and lenses (dioptrics). By simplifying things
a lot, one can think of a catadioptric sensor as consisting of a normal camera viewing the world reflected in a 
parabolic-shaped mirror. To be useful, the mirror must be perfectly aligned with and placed at a precise distace 
from the camera. The main advantage of a catadioptric sensor is its panoramic view of the world, being able to
view a full hemisphere (360 degrees by 90 degrees. Some example images of catadioptric sensors and the output 
that they give can be found [here](http://www.cis.upenn.edu/~kostas/omni.html) and [here](https://sites.google.com/site/piropodatabase/sample-videos).

Below is the reproduced abstract of my thesis.

> #### ABSTRACT
>
> Omnidirectional vision is the ability to see in all directions at the same time. Sensors 
> that are able to achieve omnidirectional vision offer several advantages to many areas 
> of computer vision, such as that of tracking and surveillance. This area benefits from 
> the unobstructed views of the surroundings acquired by these sensors and allows 
> objects to be tracked simultaneously in different parts of the field-of-view without 
> requiring any camera motion. 
> 
> This thesis describes a system that uses an omnidirectional camera system for the 
> purpose of detecting and tracking moving objects. We describe the methods used for 
> detecting objects, by means of a motion detection technique, and investigate two 
> different methods for tracking objects – a method based on tracking groups of moving 
> pixels, commonly referred to as blob tracking methods, and a statistical colour-based 
> method that uses a mixture model for representing the object’s colours. We evaluate 
> these methods and show the robustness of the latter method for occlusion and other 
> problems that are normally encountered in tracking applications. 
> 
> For this thesis, we make use of a catadioptric omnidirectional camera based on a 
> paraboloidal mirror, because of its single viewpoint and its flexibility of calibration 
> and use. We also describe the methods used to generate virtual perspective views 
> from the non-linear omnidirectional images acquired by the catadioptric camera. This 
> is used in combination with the tracking results to create virtual cameras that produce 
> perspective video streams while automatically tracking objects as they move within 
> the camera’s field-of-view.
> 
> Finally, this thesis demonstrates the advantage that an omnidirectional visual tracking 
> system provides over limited field-of-view systems. Objects have the potential of 
> being tracked for as long as they remain in the scene, and are not lost because they 
> exit the field-of-view as happens for the latter systems. This should ultimately lead to 
> a better awareness of the surrounding world – of the objects present in the scene and 
> their behaviour.


&nbsp;


The MSc thesis report can be found below:

- [Front matter](/assets/omniapp-frontmatter.pdf)
- [Chapter 1 - Introduction](/assets/omniapp-chapter1.pdf)
- [Chapter 2 - Omnidirectional Vision](/assets/omniapp-chapter2.pdf)
- [Chapter 3 - Catadioptric Systems](/assets/omniapp-chapter3.pdf)
- [Chapter 4 - Application Scenarios](/assets/omniapp-chapter4.pdf)
- [Chapter 5 - Implementation Aspects](/assets/omniapp-chapter5.pdf)
- [Chapter 6 - Catadioptric Geometry and Virtual View Generation](/assets/omniapp-chapter6.pdf)
- [Chapter 7 - Catadioptric Camera Calibration](/assets/omniapp-chapter7.pdf)
- [Chapter 8 - Moving Object Detection](/assets/omniapp-chapter8.pdf)
- [Chapter 9 - Object Tracking - an Overview](/assets/omniapp-chapter9.pdf)
- [Chapter 10 - Blob Tracking](/assets/omniapp-chapter10.pdf)
- [Chapter 11 - Colour-based Tracking](/assets/omniapp-chapter11.pdf)
- [Chapter 12 - 3D Localisation and High-Level Processing](/assets/omniapp-chapter12.pdf)
- [Chapter 13 - Conclusions and Future Work](/assets/omniapp-chapter13.pdf)
- [Bibliography](/assets/omniapp-biblio.pdf)
- [Appendices](/assets/omniapp-appendices.pdf)




