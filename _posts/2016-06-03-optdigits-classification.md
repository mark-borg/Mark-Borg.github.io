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


### Table of Contents
{:.no_toc}

* Table of Contents Placeholder
{:toc}



### Introducing the dataset

![optdigits sample digits](/img/posts/optdigits.png)

In the raw Optdigits data, digits are represented as 32x32 matrices where each pixel is binarised -- 1 or 0. They are also available in a pre-processed form in which 
digits have been divided into non-overlapping blocks of 4x4 pixels and the number of active pixels in each block have been counted. This process gives 8x8 matrices,
where each element is an integer in the range 0..16. Here we use these pixel totals contained in the 8x8 matrices for our experimentation. Thus the dataset
contains one image per row, with 64 columns representing the pixel totals of each of the 8x8 blocks, and the 65th row containg the *target label* of that image (i.e., 
the actual value of the handwritten number).

First we load the training and the test datasets in R:

```R
traindata <- read.table("optdigits.tra",sep=",")
testdata <- read.table("optdigits.tes",sep=",")
```

Each row in `traindata` and `testdata` is an image. Viewing an image can be done as follows; here we are viewing the 31st training image, which happens to be an image of the digit 7:

```R
row.id <- 31
m <- matrix(as.numeric(traindata[row.id, 1:64]), nrow=8, ncol=8)
image(m[,8:1], col=grey(seq(0, 1, length=16)))
```

![Digit 7](/img/posts/optdigits-7.png)

In order to visually see how much variation there is per digit, we could find all same digits in the training set, say all digits 7. Then we compute the average image for digit 7 and
display this:

```R
# select all training images for digit 7
train.7 <- traindata[traindata[,65]==7, 1:64]

# compute the average image from all the training images of digit 7
average.7 <- matrix(as.numeric(colMeans(train.7)), nrow=8, ncol=8)

image(average.7[,8:1], col=grey(seq(0, 1, length=16)))
```

![Average Image for Digit 7](/img/posts/optdigits-7-average.png)

If we want to visually compare and contrast two digits to each other, we can compute their average image, and subtract the two average images to find the differences. Here we illustrate this 
for digits 7 and 9:

```R
train.9 <- traindata[traindata[,65]==9, 1:64]
average.9 <- matrix(as.numeric(colMeans(train.9)), nrow=8, ncol=8)

# show the difference image of digits 7 compared to 9
image(abs(average.7 - average.9)[,8:1])
```

![Average Image for Digit 7](/img/posts/optdigits-7-average.png){: width="160px"}
![Average Image for Digit 9](/img/posts/optdigits-9-average.png){: width="160px"}
![Difference between 7 and 9](/img/posts/optdigits-7-9-diff.png)

As one can see from the above difference image, the most distinguishable differences between digits 7 and 9 lie within the lower triangular part of the image.


### $$k$$-NN classifier

One of the simplest classifiers is the [*$$k$$ Nearest Neighbour (k-NN) classifier*](https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm). In R, an implementation of the k-NN classifier is available in package `class`.

Usage of this classifier is very simple:

```R
require('class')

# try with k = 1
pred <- knn(traindata[,1:64], testdata[,1:64], traindata[,65], k=1)

# compute the accuracy of this classifier
mean(pred == testdata[,65])

# generate the confusion matrix
table(pred, testdata[,65])
```

#### With $$k$$=1

For evaluating the performance of the classifier ($$k$$ set to 1), we rely on [*2-fold cross validation*](https://www.cs.cmu.edu/~schneide/tut5/node42.html), using the train-test split created by the dataset's authors. 
The average performance of this classifier is of 97.9%. 

![k-NN results](/img/posts/optdigits-knn-results1.png)

The [*Confusion Matrix*](https://en.wikipedia.org/wiki/Confusion_matrix) allows us to look for pairwise confusion amongst the category labels (digits in this case). For example, we can see that this 
classifier confuses three instances of real 9's as 8's. There's also an image of a 4 and another of a 1 that are also confused as an 8 (i.e., the classifier thinks they are an 8, while in actual fact they are a 4 and a 1 respectively). From this confusion matrix, one can also note that digits 0 are classified correctly with no errors (no *false positives* and/or *false negatives*).


#### Finding best value of $$k$$

What's the optimal value of $$k$$ for this dataset? Let's try out with various values of $$k$$ and then we compare classifier accuracy.

```R
res = matrix(nrow=100,1)
for (k in 1:100)
{
	pred <- knn(traindata[,1:64], testdata[,1:64], traindata[,65], k)
	res[k] <- mean(pred == testdata[,65])
}

# plot the accuracy
plot(res, type='l', xlab='k', ylab='classifier accuracy', main='Accuracy vs. k for k-NN Classifier')
```

![k-NN results](/img/posts/optdigits-knn-results2.png)

From the above graph, one can see that the best performing classifier is *1-NN*, the simplest nearest neighbour classifier.



### SVM classifier

Our next experiment in classification will make use of the [*Support Vector Machine (SVM)*](https://en.wikipedia.org/wiki/Support_vector_machine). 
We will use the [`e1071` package](ftp://cran.r-project.org/pub/R/web/packages/e1071/index.html) in R. This package provides an interface to the well-known [*libsvm* library](http://www.csie.ntu.edu.tw/~cjlin/libsvm/).


```R
trn <- data.matrix(traindata)
tst <- data.matrix(testdata)

require(e1071)
```

When we try running SVM on the given optdigits training data, the following error will occur:

![SVM Error Message](/img/posts/optdigits-svm-error.png)

This error occurs because columns 1 and 40 happen to be always 0 for all the given training images, as can be confirmed by running `summary()` on the 2 columns:

![Zero columns](/img/posts/optdigits-svm-error-summary.png)

We thus need to remove these 2 columns from the data used to train the SVM model. Note also how we cast the label column (the *target* column) to type `factor`. This is required in order 
to indicate that the SVM is going to be used for *classification* purposes (Y values are categorical) and not for *regression*.

```R
ndx <- c(2:39, 41:64)	# we eliminate the all-zeros columns

# train the SVM model
svm.model <- svm(trn[,ndx], factor(trn[,65]))
```

Leaving the `svm()` call with its default parameters results in the following SVM model:

![The SVM model](/img/posts/optdigits-svm-model.png)

Now we run the SVM model on the test data in order to check the model's accuracy. 

```R
pred <- predict(svm.model, tst[,ndx])
```

This is quite straightforward, and we compute the accuracy as well as the confusion matrix:

![SVM accuracy](/img/posts/optdigits-svm-accuracy.png)



#### Tuning the SVM

We can see that the performance of the SVM classifier is worse than that of the Nearest Neighbour classifier (0.968 against 0.980). A question we can ask is the following: can we tune the SVM's parameters
in order to improve the classification accuracy? `cost` and `gamma` are the parameters of the *non-linear SVM* with a [*Gaussian radial basis function*](https://en.wikipedia.org/wiki/Radial_basis_function_kernel) kernel. 

A standard SVM seeks to find a margin that separates the different classes of objects. However, this can lead to poorly-fit models in the presence of errors and for classes that are not that easily separable. 
To account for this, the idea of a *soft margin* is used, in order to allow some of the data to be "ignored" or placed on the wrong side of the margin. 
Parameter `cost` controls this: a small cost value makes the cost of misclassification low (*soft margin*) and more points are ignored or allowed to be on the wrong side of the margin, while a large value makes the cost of misclassification high (*hard margin*) and the SVM is stricter when classifying the points which can potentially lead to *over-fitting*.

Parameter `gamma` is a free parameter of the radial basis function.  

To find the optimal values for parameters `cost` and `gamma`, we use the `tune()` method. This employs a *grid search technique* to locate the optimal parameter values and uses 10-fold cross validation in the process.
This method can be quite time-consuming. In machine learning this process of 'tuning' a classifier via finding the best parameter values is called [*hyperparameter optimisation*](https://en.wikipedia.org/wiki/Hyperparameter_optimization).

```R
tune.result <- tune(svm, train.x = rbind(trn[,1:64], tst[,1:64]), train.y = factor(c(trn[,65], tst[,65])), ranges = list(gamma = c(0.006, 0.008, 0.010, 0.012, 0.014), cost = c(3.25,3.5,3.75,4,4.25,4.5,4.75))
print(tune.result)
```

![SVM hyperparameter tuning](/img/posts/optdigits-svm-tuning.png)

```R
plot(tune.result)
```

![SVM hyperparameter plot](/img/posts/optdigits-svm-tuning-plot2.png)

We can continue re-tuning and zooming further into the hyperparameter space until we reach a good level of accuracy.

![SVM hyperparameter plot](/img/posts/optdigits-svm-tuning-plot3.png)

![SVM hyperparameter tuning](/img/posts/optdigits-svm-tuning2.png)


```R
svm.model <- svm(trn[,ndx], factor(trn[,65]), gamma=0.011, cost=4.7)
pred <- predict(model, tst[,ndx])

mean(pred == tst[,ndx])
table(pred, tst[,ndx])
```

![SVM accuracy](/img/posts/optdigits-svm-accuracy2.png)

The final accuracy of the SVM classifier is still worse than that of the nearest neighbour (0.974 against 0.980), but *hyperparameter tuning* has gone a long way in improving the results of the SVM classifier (0.974 against 0.968).


### Self-Organising Map (SOM)

The next classifier we tried is the [*Self-Organising Map (SOM)*](https://en.wikipedia.org/wiki/Self-organizing_map). This is a special type of a neural network that makes use of unsupervised learning in order to 
generate a low-dimensional (2D in our case) representation of the data. This low-dimensional representation is called a *map*. SOMs are also known as *Kohonen networks*.

Here we make use of two R packages called [`Rsomoclu`](https://cran.r-project.org/web/packages/Rsomoclu/index.html) and [`Kohonen`](https://cran.r-project.org/web/packages/kohonen/index.html). Package `Rsomoclu` provides
an R interface to the library [`Somoclu`](https://pdfs.semanticscholar.org/aa00/6437a99294f193a36d34af4e144514393043.pdf).

```R
require(Rsomoclu)
require(kohonen)

trn <- data.matrix(traindata)
tst <- data.matrix(testdata)
```

There are several SOM parameters that need to be configured before we can generate the map. For many of them, the default values can be used. The first two parameters `nSomX` and `nSomY` define the size of the 2-dimensional map.

```R
nSomX <- 100
nSomY <- 100
nEpoch <- 10
radius0 <- 0
radiusN <- 0
radiusCooling <- "linear"
scale0 <- 0
scaleN <- 0.01
scaleCooling <- "linear"
kernelType <- 0
mapType <- "planar"
gridType <- "rectangular"
compactSupport <- FALSE
codebook <- NULL
neighborhood <- "gaussian"
```

Training and generatin the SOM can then be done via the following code:

```R
som.res <- Rsomoclu.train(trn[,1:64], nEpoch, nSomX, nSomY,  radius0, radiusN,  radiusCooling, scale0, scaleN,  scaleCooling,  kernelType, mapType, gridType, compactSupport, neighborhood, codebook)

som.map = Rsomoclu.kohonen(trn[,1:64], som.res)
```

An important thing to note is that we do not pass the target labels as input to the SOM - remember that SOM uses an [*unsupervised learning*](http://machinelearningmastery.com/supervised-and-unsupervised-machine-learning-algorithms/) method.


#### SOM Visualisations

SOMs in R come with a number of plotting options to help in visualising the map as well as checking its quality.

Let's first take a look to see how the ten clusters of hadwritten digits have been organised by the SOM. We are going to use the known labels of the training images and see where they fall in the 2D map. We will
use colour coding to help in the visualisation of the clustering. The following code does this:

```R
require(RColorBrewer)

col.ind <- trn[,65] + 1
cols <- brewer.pal(10, "Paired")
cols[1] <- 'lightskyblue3'
cols[9] <- 'thistle4'

plot(som.map, type="mapping", labels=trn[,65], col=cols[col.ind], font=2, cex=1.5)
```

[![SOM Map Clusters](/img/posts/som-map-clusters-small.png)](/img/posts/som-map-clusters.png)

Note how the SOM has managed to cluster the digits quite well, keeping in mind that we have not supplied the target labels of the images; we only supplied the 64 pixel counts for the 8x8 blocks.

*Heatmaps* are perhaps the most important visualisation element of SOMs. A SOM heatmap allows the visualisation of the distribution of a single variable (single feature) across the map. For example, imagine we want to check
how much the 36th column (i.e. the pixel counts of the 36th block) helps to discriminate the various clusters we have:

```R
plot(som.map, type = "property", property = som.map$codes[,36],  main = colnames(trn)[36])
```

![SOM Map V36](/img/posts/som-map-v36.png)

When we compare the above heatmap with the clusters mapping plot, we can see that the pixel totals of the 36th column are low for roughly digits 0's and 9's, while high for digits 1's, 7's and 8's. The contours in the heatmap
do not map well with the digit clusters. But remember that this is just the heatmap of one column -- when an SOM clusters the input it takes into consideration all of the input data (all the 64 dimensions in this case).
And perhaps column 36 is not such a good discriminator on its own. Some more examples of heatmaps are shown below:

[![SOM Map V36](/img/posts/som-map-v37.png){: width="290px"}](/img/posts/som-map-v37.png)
[![SOM Map V36](/img/posts/som-map-v38.png){: width="290px"}](/img/posts/som-map-v38.png)


The SOM library in R also provides other visualisations that can help with checking the quality of the generated map. For example, a *neighbour distance plot* (also known as the *U matrix*), and a plot of how many of the input images are mapped to each node on the map. Please check the package [manual](https://cran.r-project.org/web/packages/kohonen/kohonen.pdf) for more details.


#### Performance

Using the SOM for classification purposes can be done as shown here:

```R
som.prediction <- predict(som.map, newdata=tst[,1:64], trainY=factor(trn[,65]))

mean(som.prediction$prediction == tst[,65])
```

With a 100x100 SOM, we obtained an accuracy of $$0.94769$$.


In order to find the best size of the map, we evaluated a range of map sizes using the following code. We have restricted ourselves to square-sized SOMs here:

```R
score = matrix(nrow=100,1)
for (s in seq(4,100,2))
{
   nSomX <- s
   nSomY <- s
   print(s)
 
   res <- Rsomoclu.train(trn[,1:64], nEpoch, nSomX, nSomY,  radius0, radiusN,  radiusCooling, scale0, scaleN,  scaleCooling,  kernelType, mapType, gridType, compactSupport, neighborhood, codebook)
 
   som.map = Rsomoclu.kohonen(trn[,1:64], res)
   som.prediction <- predict(som.map, newdata=tst[,1:64], trainY=factor(trn[,65]))
   score[s] <- mean(som.prediction$prediction == tst[,65])
   print(score[s])
}
```

![SOM Size plot](/img/posts/som-size-plot.png){: width="500px"}

The best accuracy of $$0.9482471$$ is obtained for a SOM size of $$70$$x$$70$$. 


We then performed another test to see what is the best value for the `epoch` parameter. Trying out with a range of epoch values varying from 10 to 150, we found out that the best result is obtained at
`epoch` = $$50$$. With this epoch value, classification accuracy reaches $$0.96272$$.

![SOM results at optimal Epoch value](/img/posts/som-results-epoch.png)

And the resulting map is given below. Please note that the map changes on each and every run. 

[![SOM Map Clusters](/img/posts/som-map-clusters-small2.png)](/img/posts/som-map-clusters2.png)


#### From a training image to its SOM location

Before we leave the topic on SOMs, you might ask: is it possible to determine where on the map a particular image falls? Yes, we can do that.

The generated som has an element called `unit.classif` (accessible via `som.map$unit.classif`) that gives the number of the SOM node to which the training image belongs to.
For example, checking the first training image (first row in `traindata`), 

```R
(unit.num <- som.map$unit.classif[1])
```

we find out that this image is mapped to SOM node 2120. We reach the 2120'th node, by starting from bottom-left corner of the map and moving in a row-order fashion. The following
code changes from node number to x,y coordinates (origin is at the bottom-left corner):

```R
coord <- c(unit.num %% nSomX, unit.num %/% nSomX)
```

That's all on SOMs.



...WIP....


