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


...WIP....


