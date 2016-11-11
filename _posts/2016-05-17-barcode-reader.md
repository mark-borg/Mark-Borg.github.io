---
layout: post
title:  Barcode Recognition
tags:   blogging
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


....WIP....



### Results



....WIP....



#### Caveat Emptor

This demo program was written over 10 years ago. Thus the GUI part of the program is now quite old and uses old technology like the now-obsolete Microsoft MFC library.
But the central part of this source code, barcode detection is still valid today and can still be considered as state-of-the-art. The barcode detection code can be 
found in folder `qvcore` and is separate from the GUI code; thus it can be easily and quickly compiled as a console application, or integrated with more recent GUI
frameworks.

The [*WiX toolset*](http://wixtoolset.org/) was used for generating the installer. *WiX* is an open source project, originally developed by Microsoft, and consists of a 
set of tools that build Windows installation packages from XML source code. Some changes might be required to be able to use the latest version of WiX.

Extensive use is made of the [*OpenCV library*](http://opencv.org/). The program was developed using OpenCV beta 4 (release in August 2004). I think that only minor
changes are needed to the core code in order to make it work with the latest version of OpenCV.



