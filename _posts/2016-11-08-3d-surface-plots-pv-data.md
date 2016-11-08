---
layout: post
title:  Visualising PV output with 3D Surface Plots
tags:   
image:  pv-output.png
---

&nbsp;

In order to experiment with the 3D plotting functionality of R, I used nearly a year's worth of power output data from my solar panel
installation at home. 

{{ more }}

### The photovoltaic data 

At home I have a 6-panel photovoltaic (PV) system that can generate a peak power of 290W per panel, i.e., a system total of 1.74 kWp (kilowatt peak).
The inverter that comes with this system has a network connectivity card that allows for remote monitoring of the system from the internal network using a specific
application called *SunnyExplorer*. The inverter's network card has also the capability of uploading daily power production data and diagnostic information automatically 
to a cloud-based service [www.sunnyportal.com](https://www.sunnyportal.com/Templates/Start.aspx?ReturnUrl=%2f).

![SunnyExplorer screenshot](/img/posts/pv-screenshot.png)

For experimenting with R's 3D plotting functionality, I am using daily power production files downloaded from the system's inverter. These files are CSV with instantaneous power
output sampled every 5 minutes throughout the daylight portion of the day.

The data contained in the file is very straightforward: it consists of a timestamp, the kWh cumulative total power yield, and the kW output being generated at that moment in time (probably
smoothed by the inverter's electronics/software logic over the current 5-minute sampling window). 

![CSV file sample](/img/posts/csv-file-sample.png)

### 3D Plotting

Once upon a time...  (WIP) .....


<!--
	(*) Mention possible data analysis that can be done...
 
	(*) Google Project Sunroof
	
	(*) mention some other stuff on how to measure yield
	
-->
