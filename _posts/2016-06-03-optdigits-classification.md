---
layout: post
title:  Experiments in Optdigits Classification
tags:   
image:  optdigits-classification.png
---

&nbsp;

[*Optdigits*](https://archive.ics.uci.edu/ml/datasets/Optical+Recognition+of+Handwritten+Digits) is a well-known dataset consisting of a collection of hand-written 
digits available at the [*UCI Machine Learning Repository*](https://archive.ics.uci.edu/ml/index.html). In this blog I report classification results obtained using 
various machine learning classification techniques.

{{ more }}

![optdigits sample digits](/img/posts/optdigits.png)

In the raw Optdigits data, digits are represented as 32x32 matrices where each pixel is binarised -- 1 or 0. They are also available in a pre-processed form in which 
digits have been divided into non-overlapping blocks of 4x4 pixels and the number of active pixels in each block have been counted. This process gives 8x8 matrices,
where each element is an integer in the range 0..16. Here we use these pixel totals contained in the 8x8 matrices for our experimentation.



...WIP....





