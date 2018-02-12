---
layout: post
title:  Image Noise Modeling
tags:   
  - image noise
  - statistics
  - modeling
author: Mark Borg
image:  image-noise-modeling.png
---

&nbsp;

A common assumption underlying many computer vision algorithms is that image noise is normally distributed. 
But is this a valid assumption? 

{{ more }}

Recently I was doing some investigations into error quantification and uncertainty estimation of computer vision algorithms.
Many algorithms must rely on particular assumptions in order to be able to generalise well. 
[Domingos (2012)](#references) does a good job at discussing this point and highlights that assumptions are an essential element of **inductive learning** and is what leads to the well-known **bias** effect.

A common assumption taken by many computer vision algorithms is that image noise is uniformly distributed, i.e., it follows a Gaussian probability distribution.
And many times, these same algorithms are used without first ascertaining whether this assumption really holds or not. Personally I have done this a lot of times: blindly assuming that noise follows a Gaussian distribution.

![The assumptions underlying computer vision algorithms is like a house of playing cards. If the assumptions do not hold, the cards come tumbling down.](/img/posts/house_of_playing_cards.png){: width=200 .pull-right}


But if such an assumption does not hold, then it can have repercussions on the accuracy of the results provided by algorithms. In the worst case an algorithm might "fail" outright because its workings are based on flawed foundations. In the best case, an additional error is introduced in the results giving rise to more uncertainty.
As mentioned by [Thacker (2003,2008)](#references), when it comes to determining the uncertainty of the results of an algorithm, one has to consider the input data and its assumptions. 




Here are some of the results obtained from my investigation on the assumption of image noise normality.





### Checking for Normality

Lorem ipsum...




&nbsp;

#### References

P. Domingos, “A Few Useful Things to Know About Machine Learning,” *Commun. ACM*, vol. 55, no. 10, pp. 78–87, 2012.

N. A. Thacker et al., “Performance characterization in computer vision: A guide to best practices,” *Computer Vision and Image Understanding*, vol. 109, no. 3, pp. 305–334, 2008.

N. A. Thacker, “Using quantitative statistics for the construction of machine vision systems,” *Proc. SPIE*, vol. 4877, 2003.
