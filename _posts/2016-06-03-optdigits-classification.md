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

### Introducing the dataset

![optdigits sample digits](/img/posts/optdigits.png)

In the raw Optdigits data, digits are represented as 32x32 matrices where each pixel is binarised -- 1 or 0. They are also available in a pre-processed form in which 
digits have been divided into non-overlapping blocks of 4x4 pixels and the number of active pixels in each block have been counted. This process gives 8x8 matrices,
where each element is an integer in the range 0..16. Here we use these pixel totals contained in the 8x8 matrices for our experimentation.

First we load the training and the test datasets in R:

```R
trn <- read.table("optdigits.tra",sep=",")
tst <- read.table("optdigits.tes",sep=",")
```

Each row in `trn` and `tst` is an image. Viewing an image can be done as follows; here we are viewing the 31st training image, which happens to be an image of the digit 7:

```R
row.id <- 31
m <- matrix(as.numeric(trn[row.id, 1:64]), nrow=8, ncol=8)
image(m[,8:1], col=grey(seq(0, 1, length=16)))
```

![Digit 7](/img/posts/optdigits-7.png)

In order to visually see how much variation there is per digit, we could find all same digits in the training set, say all digits 7. Then we compute the average image for digit 7 and
display this:

```R
# select all training images for digit 7
trn.7 <- trn[trn[,65]==7, 1:64]

# compute the average image from all the training images of digit 7
average.7 <- matrix(as.numeric(colMeans(trn.7)), nrow=8, ncol=8)

image(average.7[,8:1], col=grey(seq(0, 1, length=16)))
```

![Average Image for Digit 7](/img/posts/optdigits-7-average.png)

If we want to visually compare and contrast two digits to each other, we can compute their average image, and subtract the two average images to find the differences. Here we illustrate this 
for digits 7 and 9:

```R
trn.9 <- trn[trn[,65]==9, 1:64]
average.9 <- matrix(as.numeric(colMeans(trn.9)), nrow=8, ncol=8)

# show the difference image of digits 7 compared to 9
image(abs(average.7 - average.9)[,8:1])
```

![Average Image for Digit 7](/img/posts/optdigits-7-average.png){: width="160px"}
![Average Image for Digit 9](/img/posts/optdigits-9-average.png){: width="160px"}
![Difference between 7 and 9](/img/posts/optdigits-7-9-diff.png)

As one can see from the above difference image, the most distinguishable differences between digits 7 and 9 lie within the lower triangular part of the image.



...WIP....


