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

From the above list of conditions, one can see that the focus is on building a *robust* barcode recognition system.


### The many types of Barcodes

Barcodes come in many varieties and flavours. For this demo I decided to initially concentrate on 1-dimensional barcodes only, and then tackle 2-dimensional barcodes like [QR Codes](https://en.wikipedia.org/wiki/QR_code) in future
versions of the program. 



### EAN-13 and UPC

Two of the most commonly used 1-dimensional barcodes are [*EAN-13*](https://www.barcodestalk.com/ean-overview) and [*UPC*](https://www.barcodestalk.com/upc-overview). I decided to concentrate on these
due to the limited time available for developing the demo. 

Actually the software is written in such a way that the *barcode detection code* is separate from the *barcode decoding code*. All one-dimensional barcodes consist essentially of a series of lines
(bars) with alternating colours (white, black) and of varying thickness. So the barcode detection logic is the same regardless of whether the barcode is EAN-13, EAN-8, UPC-A, UPC-B, etc.
It is the barcode decoder class that is then specific to each flavour of barcode. 

The *BarcodeReader1D* class can employ one of 2 search strategies

[![BarcodeReader Class Diagram](/img/posts/barcodereader-class-diagram.png)](/img/posts/barcodereader-class-diagram.png)





