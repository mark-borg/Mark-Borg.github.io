---
layout: post
title:  Hello World!
tags:   blogging
image:  hello-world.png
---

&nbsp;

Welcome to my new blog!

{{ more }}

This is my first blog entry. In the spirit of programming languages, and since at the moment I am learning [R](https://www.r-project.org/), I will dedicate this short blog entry to writing a *Hello World* in R. Writing it in a fancy way and exploiting the graphing capabilities that R offers.

&nbsp;

```R
df <- data.frame(V = strsplit('HELLO WORLD', '')[[1]], X = 1:11 + runif(11), Y = runif(11, min=2, max=3.5), Z = runif(11, min=5, max=6))
require(scatterplot3d)
cols <- colorRampPalette(c("blue", "red"))(11)
s3d <- scatterplot3d(df[,-1], pch = '', color = cols, type = 'h', ylim = c(0,5), zlim = c(3,7))
s3d.coords <- s3d$xyz.convert(df[,-1])
text(s3d.coords$x, s3d.coords$y, labels=df$V, pos=3, offset=0.1, cex=2, col = cols)
```

&nbsp;

The following is the output obtained:

![Hello World in R](/img/posts/hello-world-in-R.png)

[R](https://www.r-project.org/) is an extremely powerful language that is useful for statistical computing and graphics. It is the language of choice for Data Science and can be used within many AI and machine learning frameworks and architectures, for example, the use of [R within Azure Machine Learning](https://azure.microsoft.com/en-us/documentation/articles/machine-learning-r-quickstart/).

Some good books on R that I am currently reading are the following:


[![The Art of R Programming](/img/posts/the-art-of-r-programming.png)](https://www.amazon.com/gp/search?index=books&linkCode=qs&keywords=9781593273842) | [![R Graphics Cookbook](/img/posts/r-graphics-cookbook.png)](http://shop.oreilly.com/product/0636920023135.do) | [![Data Mining with R](/img/posts/data-mining-with-r.png)](http://www.dcc.fc.up.pt/~ltorgo/DataMiningWithR/)


