---
layout: post
title:  Barcode Recognition
tags:   
 - barcodes 
 - barcode recognition
author: Mark Borg
image:  barcode-reader.png
---

&nbsp;

I worked on the *BarcodeReader* program as part of a set of *Machine Vision* demo programs that I did for some potential leads in the manufacturing industry.

{{ more }}

Now I am placing the source code in the public domain. It's available on [GitHub](https://github.com/mark-borg/BarcodeReader).

[![Barcodes](/img/posts/barcode-infographic.png)](https://www.emsbarcode.com/wp-content/uploads/2015/09/barcode-infographic.jpg){: .pull-right}

[![Source Code on Github](/img/posts/source_on_github.png)](https://github.com/mark-borg/BarcodeReader)

The main aim of this program is to look for barcodes in images acquired with a normal web camera. The program should detect barcodes that are:

- in any orientation
- viewed straight-on or obliquely
- barcodes of varying size and at varying distance from the camera
- barcodes that are captured under sub-optimal lighting conditions (shadows, specular reflections, etc.)
- barcodes that lie on round bottles and containers, thus appearing curved to the camera
- barcodes that lie on crumpled or bent surfaces, thus appearing warped in the camera's field-of-view.

From the above list of conditions, one can see that the focus is on building a *robust* barcode recognition system and without placing any constraints
(up to a limit) on how the camera should be oriented in order to view the barcode.


### The many types of Barcodes

Barcodes come in many varieties and flavours. For this demo I decided to initially concentrate on 1-dimensional barcodes only, and then tackle 2-dimensional barcodes like [QR Codes](https://en.wikipedia.org/wiki/QR_code) in future
versions of the program. 



#### EAN-13 and UPC

Two of the most commonly used 1-dimensional barcodes are [*EAN-13*](https://www.barcodestalk.com/ean-overview) and [*UPC*](https://www.barcodestalk.com/upc-overview). I decided to concentrate on these
due to the limited time available for developing the demo. 



### The Solution

Actually the software is written in such a way that the *barcode detection code* is separate from the *barcode decoding code*. All one-dimensional barcodes consist essentially of a series of lines
(bars) with alternating colours (white, black) and of varying thickness. So the barcode detection logic is the same regardless of whether the barcode is EAN-13, EAN-8, UPC-A, UPC-B, etc.
It is the *barcode decoder* class that is then specific to each flavour of barcode. 

[![BarcodeReader Class Diagram](/img/posts/barcodereader-class-diagram.png)](/img/posts/barcodereader-class-diagram.png)

The `BarcodeReader1D` class can employ one of 2 search strategies in order to locate the barcode. The first one consists of examining the image along $$N$$ equidistant horizontal 
scan-lines and looking for the telltale patterns of barcodes. The number $$N$$ of scanlines is user-configurable. The second strategy adopts a *binary search*-like approach: first 
the horizontal scanline in the middle ($$\frac{1}{2}$$) of the search area is examined, then the area is sub-divided again and scanlines located at $$\frac{1}{4}$$ 
and $$\frac{3}{4}$$ of the search area are checked, followed by scanlines located at $$\frac{1}{8}$$, $$\frac{3}{8}$$, $$\frac{5}{8}$$, $$\frac{7}{8}$$ of the search area, etc.

When checking a scanline, `BarcodeReader1D` will find the maximimum and minimum graylevel values along that scanline and then apply an [*optimal thresholding* algorithm](http://docs.opencv.org/trunk/d7/d4d/tutorial_py_thresholding.html)
to find the best thresholding value. This method provides robustness to varying lighting conditions in the image, and under-exposed or over-exposed images. Once
the image is thresholded, binary samples along the scanline are collected and passed on to the various decoders so that they can check whether the pattern along the
scanline matches their expected pattern. If $$M$$ or more valid barcode matches are found along $$M$$ scanlines, and they belong to the same barcode decoding class, then 
we report a successful barcode detection ($$M$$ is user-configurable and defaults to 3).

The `BarcodeDecoder` class works as follows. Given thresholded sample values along a scanline, it builds a histogram of the run lengths of this binary pixel scan. 
For example:

$$11111100001111111111110000000110000000000000100001111110000...$$

*Histogram analysis* is then performed in order to detect [*histogram peaks*](https://www.mathworks.com/help/signal/examples/peak-analysis.html) and map these to the bar widths allowed by that particular barcode type. For example EAN-13 employs 4
different bar widths. If a barcode is viewed from an optimal position and camera orientation, then the histogram peaks should be well-defined (4 peaks in total), but due to
oblique views, warping of the material on which a barcode is printed, detection noise, and lighting effects, the peaks will become less distinct. Using some clever histogram
analysis allows us to recover the peaks. We also employ smoothing operations as part of histogram analysis.



### EAN-13 Barcode Recognition

According to the standard, [*EAN-13*](https://en.wikipedia.org/wiki/International_Article_Number) consists of 95 *modules*, where a *module* is the width of the slimmest
bar (0.33mm according to the spec.). In the logic of class `EAN13BarcodeDecoder`, we assume that at least one pixel is able to represent an EAN-13 module; thus there is a 
lower limit of 95 pixels for the whole barcode. Any barcodes with length smaller than 95 pixels will not be recognised. 

After performing histogram analysis and peak detection, `EAN13BarcodeDecoder` takes the first histogram peak as representing the width of the slimmest module. The second
peak should then represent the next-slimmest module, and so on. The EAN specification says that the module width should be multiples of the slimmest module. Thus we use
this constraint to guide the mapping of the histogram peaks to module widths: this allows us to handle cases of missing module lengths (e.g. it happens that no digit in 
the given barcode has such a bar size), as well as allowing for some variation (up to half a module size) to cater for noise. We also add a fifth *virtual* peak to cater
for non-data elements of EAN-13, such as the *quiet zone* that surrounds a barcode.

Once we know the widths in pixels of the EAN-13 barcode modules, we can start the actual decoding process:

1. The first digit of the barcode can only be known after the next 6 digits are parsed; so we use a *placeholder* for the first digit.
2. We decode the first part of the barcode, i.e. the part before the middle separator. This consists of 6 digits plus the unknown first digit, which we will decode later.
- For the first part, either of two decoding tables (table A or table B) could be used $$-$$ we try both.
3. Then we parse the middle separator
4. Then we can determine the value of the first digit by checking which encoding table was used for the already-decoded 6 digits.
5. We then decode the second part of the barcode, i.e. the digits after the middle separator. Digits in the second part are all encoded with a single table (table C).
6. Parse the final separator.
7. Parse the quiet zone.
8. Validate the checksum.
9. Look if there is an optional 2- or 5-digit *add-on*
- If there is an *add-on*, then for the first part, either table A or table B could be used to encode it $$-$$ we try both.
- Parse the separator
- Validate the checksum of the *add-on*.



### Test Data

To check the robustness of our algorithm, we used a normal webcamera to acquire images of real barcodes taken under normal conditions (real products, no structured lighting).
Some of the test images are given below (click for a larger view):

[![barcode1](/img/posts/barcode01.jpg){: height="120px"}](/img/posts/barcode01.jpg)
[![barcode2](/img/posts/barcode02.jpg){: height="120px"}](/img/posts/barcode02.jpg)
[![barcode3](/img/posts/barcode03.jpg){: height="120px"}](/img/posts/barcode03.jpg)
[![barcode4](/img/posts/barcode04.jpg){: height="120px"}](/img/posts/barcode04.jpg)
[![barcode5](/img/posts/barcode05.jpg){: height="120px"}](/img/posts/barcode05.jpg)
[![barcode6](/img/posts/barcode06.jpg){: height="120px"}](/img/posts/barcode06.jpg)
[![barcode7](/img/posts/barcode07.jpg){: height="120px"}](/img/posts/barcode07.jpg)
[![barcode8](/img/posts/barcode08.jpg){: height="120px"}](/img/posts/barcode08.jpg)
[![barcode9](/img/posts/barcode09.jpg){: height="120px"}](/img/posts/barcode09.jpg)
[![barcode10](/img/posts/barcode10.jpg){: height="120px"}](/img/posts/barcode10.jpg)
[![barcode11](/img/posts/barcode11.jpg){: height="120px"}](/img/posts/barcode11.jpg)
[![barcode12](/img/posts/barcode12.jpg){: height="120px"}](/img/posts/barcode12.jpg)
[![barcode13](/img/posts/barcode13.jpg){: height="120px"}](/img/posts/barcode13.jpg)
[![barcode14](/img/posts/barcode14.jpg){: height="120px"}](/img/posts/barcode14.jpg)
[![barcode15](/img/posts/barcode15.jpg){: height="120px"}](/img/posts/barcode15.jpg)
[![barcode16](/img/posts/barcode16.jpg){: height="120px"}](/img/posts/barcode16.jpg)
[![barcode17](/img/posts/barcode17.jpg){: height="120px"}](/img/posts/barcode17.jpg)
[![barcode18](/img/posts/barcode18.jpg){: height="120px"}](/img/posts/barcode18.jpg)
[![barcode19](/img/posts/barcode19.jpg){: height="120px"}](/img/posts/barcode19.jpg)
[![barcode20](/img/posts/barcode20.jpg){: height="120px"}](/img/posts/barcode20.jpg)



### Results

The following are the results of barcode recognition (EAN-13 & UPC-A). Where the detection & recognition is successful, the correct barcode number is displayed in green and overlaid on the 
original image. Where barcode recognition fails, the program draws a large red cross overlaid on the image.

[![result01](/img/posts/barcodereader/result01.png){: height="172px"}](/img/posts/barcodereader/result01.png)
[![result02](/img/posts/barcodereader/result02.png){: height="172px"}](/img/posts/barcodereader/result02.png)
[![result03](/img/posts/barcodereader/result03.png){: height="172px"}](/img/posts/barcodereader/result03.png)
[![result04](/img/posts/barcodereader/result04.png){: height="172px"}](/img/posts/barcodereader/result04.png)
[![result05](/img/posts/barcodereader/result05.png){: height="172px"}](/img/posts/barcodereader/result05.png)
[![result06](/img/posts/barcodereader/result06.png){: height="172px"}](/img/posts/barcodereader/result06.png)
[![result07](/img/posts/barcodereader/result07.png){: height="172px"}](/img/posts/barcodereader/result07.png)
[![result08](/img/posts/barcodereader/result08.png){: height="172px"}](/img/posts/barcodereader/result08.png)
[![result09](/img/posts/barcodereader/result09.png){: height="172px"}](/img/posts/barcodereader/result09.png)
[![result10](/img/posts/barcodereader/result10.png){: height="172px"}](/img/posts/barcodereader/result10.png)
[![result11](/img/posts/barcodereader/result11.png){: height="172px"}](/img/posts/barcodereader/result11.png)
[![result12](/img/posts/barcodereader/result12.png){: height="172px"}](/img/posts/barcodereader/result12.png)
[![result13](/img/posts/barcodereader/result13.png){: height="172px"}](/img/posts/barcodereader/result13.png)
[![result14](/img/posts/barcodereader/result14.png){: height="172px"}](/img/posts/barcodereader/result14.png)
[![result15](/img/posts/barcodereader/result15.png){: height="172px"}](/img/posts/barcodereader/result15.png)
[![result16](/img/posts/barcodereader/result16.png){: height="172px"}](/img/posts/barcodereader/result16.png)
[![result17](/img/posts/barcodereader/result17.png){: height="172px"}](/img/posts/barcodereader/result17.png)
[![result18](/img/posts/barcodereader/result18.png){: height="172px"}](/img/posts/barcodereader/result18.png)
[![result19](/img/posts/barcodereader/result19.png){: height="172px"}](/img/posts/barcodereader/result19.png)
[![result20](/img/posts/barcodereader/result20.png){: height="172px"}](/img/posts/barcodereader/result20.png)
[![result21](/img/posts/barcodereader/result21.png){: height="172px"}](/img/posts/barcodereader/result21.png)
[![result22](/img/posts/barcodereader/result22.png){: height="172px"}](/img/posts/barcodereader/result22.png)
[![result23](/img/posts/barcodereader/result23.png){: height="172px"}](/img/posts/barcodereader/result23.png)
[![result24](/img/posts/barcodereader/result24.png){: height="172px"}](/img/posts/barcodereader/result24.png)
[![result25](/img/posts/barcodereader/result25.png){: height="172px"}](/img/posts/barcodereader/result25.png)
[![result26](/img/posts/barcodereader/result26.png){: height="172px"}](/img/posts/barcodereader/result26.png)
[![result27](/img/posts/barcodereader/result27.png){: height="172px"}](/img/posts/barcodereader/result27.png)
[![result28](/img/posts/barcodereader/result28.png){: height="172px"}](/img/posts/barcodereader/result28.png)
[![result29](/img/posts/barcodereader/result29.png){: height="172px"}](/img/posts/barcodereader/result29.png)
[![result30](/img/posts/barcodereader/result30.png){: height="172px"}](/img/posts/barcodereader/result30.png)
[![result31](/img/posts/barcodereader/result31.png){: height="172px"}](/img/posts/barcodereader/result31.png)
[![result32](/img/posts/barcodereader/result32.png){: height="172px"}](/img/posts/barcodereader/result32.png)
[![result33](/img/posts/barcodereader/result33.png){: height="172px"}](/img/posts/barcodereader/result33.png)
[![result34](/img/posts/barcodereader/result34.png){: height="172px"}](/img/posts/barcodereader/result34.png)
[![result35](/img/posts/barcodereader/result35.png){: height="172px"}](/img/posts/barcodereader/result35.png)
[![result36](/img/posts/barcodereader/result36.png){: height="172px"}](/img/posts/barcodereader/result36.png)
[![result37](/img/posts/barcodereader/result37.png){: height="172px"}](/img/posts/barcodereader/result37.png)
[![result38](/img/posts/barcodereader/result38.png){: height="172px"}](/img/posts/barcodereader/result38.png)
[![result39](/img/posts/barcodereader/result39.png){: height="172px"}](/img/posts/barcodereader/result39.png)
[![result40](/img/posts/barcodereader/result40.png){: height="172px"}](/img/posts/barcodereader/result40.png)
[![result41](/img/posts/barcodereader/result41.png){: height="172px"}](/img/posts/barcodereader/result41.png)
[![result42](/img/posts/barcodereader/result42.png){: height="172px"}](/img/posts/barcodereader/result42.png)
[![result43](/img/posts/barcodereader/result43.png){: height="172px"}](/img/posts/barcodereader/result43.png)
[![result44](/img/posts/barcodereader/result44.png){: height="172px"}](/img/posts/barcodereader/result44.png)
[![result45](/img/posts/barcodereader/result45.png){: height="172px"}](/img/posts/barcodereader/result45.png)
[![result46](/img/posts/barcodereader/result46.png){: height="172px"}](/img/posts/barcodereader/result46.png)
[![result47](/img/posts/barcodereader/result47.png){: height="172px"}](/img/posts/barcodereader/result47.png)
[![result48](/img/posts/barcodereader/result48.png){: height="172px"}](/img/posts/barcodereader/result48.png)
[![result49](/img/posts/barcodereader/result49.png){: height="172px"}](/img/posts/barcodereader/result49.png)
[![result50](/img/posts/barcodereader/result50.png){: height="172px"}](/img/posts/barcodereader/result50.png)
[![result51](/img/posts/barcodereader/result51.png){: height="172px"}](/img/posts/barcodereader/result51.png)
[![result52](/img/posts/barcodereader/result52.png){: height="172px"}](/img/posts/barcodereader/result52.png)
[![result53](/img/posts/barcodereader/result53.png){: height="172px"}](/img/posts/barcodereader/result53.png)
[![result54](/img/posts/barcodereader/result54.png){: height="172px"}](/img/posts/barcodereader/result54.png)
[![result55](/img/posts/barcodereader/result55.png){: height="172px"}](/img/posts/barcodereader/result55.png)
[![result56](/img/posts/barcodereader/result56.png){: height="172px"}](/img/posts/barcodereader/result56.png)
[![result57](/img/posts/barcodereader/result57.png){: height="172px"}](/img/posts/barcodereader/result57.png)
[![result58](/img/posts/barcodereader/result58.png){: height="172px"}](/img/posts/barcodereader/result58.png)
[![result59](/img/posts/barcodereader/result59.png){: height="172px"}](/img/posts/barcodereader/result59.png)
[![result60](/img/posts/barcodereader/result60.png){: height="172px"}](/img/posts/barcodereader/result60.png)
[![result61](/img/posts/barcodereader/result61.png){: height="172px"}](/img/posts/barcodereader/result61.png)
[![result62](/img/posts/barcodereader/result62.png){: height="172px"}](/img/posts/barcodereader/result62.png)
[![result63](/img/posts/barcodereader/result63.png){: height="172px"}](/img/posts/barcodereader/result63.png)
[![result64](/img/posts/barcodereader/result64.png){: height="172px"}](/img/posts/barcodereader/result64.png)
[![result65](/img/posts/barcodereader/result65.png){: height="172px"}](/img/posts/barcodereader/result65.png)
[![result66](/img/posts/barcodereader/result66.png){: height="172px"}](/img/posts/barcodereader/result66.png)
[![result67](/img/posts/barcodereader/result67.png){: height="172px"}](/img/posts/barcodereader/result67.png)
[![result68](/img/posts/barcodereader/result68.png){: height="172px"}](/img/posts/barcodereader/result68.png)
[![result69](/img/posts/barcodereader/result69.png){: height="172px"}](/img/posts/barcodereader/result69.png)
[![result70](/img/posts/barcodereader/result70.png){: height="172px"}](/img/posts/barcodereader/result70.png)
[![result71](/img/posts/barcodereader/result71.png){: height="172px"}](/img/posts/barcodereader/result71.png)
[![result72](/img/posts/barcodereader/result72.png){: height="172px"}](/img/posts/barcodereader/result72.png)
[![result73](/img/posts/barcodereader/result73.png){: height="172px"}](/img/posts/barcodereader/result73.png)

As one can see from the results obtained, the recognition algorithm we use is quite robust. Even cases where
the barcode is poorly lit or irregularly warped are recognised successfully.


### Vanity Barcodes

A recent and interesting trend is the use of artistic elements or modifications to traditional barcodes. This is typically done for branding purposes, advertising and/or 
marketing. Such barcodes suffer no loss of information (they carry the same data as traditional barcodes), and the artistic element is there only for human consumption.
A barcode reader should not, in theory, be affected by these modifications. These type of artistic barcodes are known as 
[*Vanity Barcodes*](http://www.barcoding.com/blog/vanity-barcodes-the-latest-trend-in-barcodes/) (or sometimes called *Designer Barcodes*). 
Examples of real vanity barcodes are given below. More examples can be found [here](http://www.vanitybarcodes.com/).

![vanity barcode example](/img/posts/vanity-barcode-1.png)
![vanity barcode example](/img/posts/vanity-barcode-2.png)

When I came across vanity barcodes, I decided to try out the program BarcodeReader on these, in part to check the robustness of the program. Will the algorithm be side-tracked
with the extra parts and fail? Below are the results of this experiment.


[![result73](/img/posts/barcodereader/vanity01a.png){: height="320px"}](/img/posts/barcodereader/vanity01a.png)
[![result73](/img/posts/barcodereader/vanity01b.png){: height="320px"}](/img/posts/barcodereader/vanity01b.png)
[![result73](/img/posts/barcodereader/vanity02a.png){: width="295px"}](/img/posts/barcodereader/vanity02a.png)
[![result73](/img/posts/barcodereader/vanity02b.png){: height="320px"}](/img/posts/barcodereader/vanity02b.png)
[![result73](/img/posts/barcodereader/vanity03a.png){: width="295px"}](/img/posts/barcodereader/vanity03a.png)
[![result73](/img/posts/barcodereader/vanity03b.png){: height="320px"}](/img/posts/barcodereader/vanity03b.png)
[![result73](/img/posts/barcodereader/vanity05a.png){: width="295px"}](/img/posts/barcodereader/vanity05a.png)
[![result73](/img/posts/barcodereader/vanity05b.png){: height="320px"}](/img/posts/barcodereader/vanity05b.png)
[![result73](/img/posts/barcodereader/vanity06a.png){: width="295px"}](/img/posts/barcodereader/vanity06a.png)
[![result73](/img/posts/barcodereader/vanity06b.png){: height="320px"}](/img/posts/barcodereader/vanity06b.png)
[![result73](/img/posts/barcodereader/vanity07a.png){: width="295px"}](/img/posts/barcodereader/vanity07a.png)
[![result73](/img/posts/barcodereader/vanity07b.png){: height="320px"}](/img/posts/barcodereader/vanity07b.png)
[![result73](/img/posts/barcodereader/vanity08a.png){: width="295px"}](/img/posts/barcodereader/vanity08a.png)
[![result73](/img/posts/barcodereader/vanity08b.png){: height="320px"}](/img/posts/barcodereader/vanity08b.png)
[![result73](/img/posts/barcodereader/vanity09a.png){: height="320px"}](/img/posts/barcodereader/vanity09a.png)
[![result73](/img/posts/barcodereader/vanity09b.png){: height="320px"}](/img/posts/barcodereader/vanity09b.png)




#### Caveat Emptor

This demo program was written over 10 years ago. Thus the GUI part of the program is now quite old and uses old technology like the now-obsolete Microsoft MFC library.
But the central part of this source code, barcode detection is still valid today and can still be considered as state-of-the-art. The barcode detection code can be 
found in folder `qvcore` and is separate from the GUI code; thus it can be easily and quickly compiled as a console application, or integrated with more recent GUI
frameworks.

The [*WiX toolset*](http://wixtoolset.org/) was used for generating the installer. *WiX* is an open source project, originally developed by Microsoft, and consists of a 
set of tools that build Windows installation packages from XML source code. Some changes might be required to be able to use the latest version of WiX.

Extensive use is made of the [*OpenCV library*](http://opencv.org/). The program was developed using OpenCV beta 4 (release in August 2004). I think that only minor
changes are needed to the core code in order to make it work with the latest version of OpenCV.

[![BarcodeReader GUI](/img/posts/barcodereader-gui.png)](/img/posts/barcodereader-gui.png)


