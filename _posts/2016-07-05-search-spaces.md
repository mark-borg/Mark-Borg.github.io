---
layout: post
title:  Introductory Video on Search Spaces
tags:   
image:  search-space-video.png
---

&nbsp;

An important concept in the field of Artificial Intelligence is that of *Search Spaces* or *State Space Search*.


{{ more }}

This concept can be helpful in gaining a deep insight into this field of research by being able to interpret the various AI tools 
as different ways of how to go about exploring and searching a state space. And any real-world problem can be mapped as a state space
search problem.

#### State Space as a Threshold Concept

While teaching an undergrad class in AI, I observed that students can experience difficulties in grasping this concept. They find it 
difficult to make the jump from an every-day real-world problem to being able to represent it on a higher abstract level, the 
*search space*. 

But once (and if) students are able to gain a deep understanding of this concept, then it opens new doors for their 
understanding of AI. And then things start to fall into place - diverse tools and topics suddenly appear to make sense and their 
inter-connectedness becomes obvious. I observed many a student who passed through this Eureka! moment. Sadly, others didn't make it.
In the field of pedagogy, this is called a [*Threshold Concept*](http://www.ee.ucl.ac.uk/~mflanaga/thresholds.html).

#### Towards mastering State Space Search

To help students better understand the concept of search spaces, I started working on a number of tools, visualisations and videos. 
This blog entry is about one such experimental video. This work also forms part of my ongoing studies for a *Post-Graduate certificate
in Higher Education* at *Middlesex University*.

#### Video - Representing how we go about our day-to-day life as a State Space

As an initial experiment, I opted to choose a scenario that is grounded in the real world (representing a person's daily activity)
and which in my opinion is a non-trivial example. 

Traditionally AI under-grad courses tend to tackle state space search using toy-like simplified examples.
Examples include the [*8-Queens Puzzle*](https://en.wikipedia.org/wiki/Eight_queens_puzzle) and various flavours from the class of
[River Crossing puzzles](https://en.wikipedia.org/wiki/River_crossing_puzzle). While these are valid examples, in my opinion, they 
fail to show how to cross the gap from the problem grounded in the physical world to an abstract representation of the problem.

In this introductory [video](https://youtu.be/f-l4ZXo7_aM) I am trying to convey the following messages:

- any complex problem can be modelled or represented as a *state space*
- an *evaluation function* helps us to compare the 'goodness' of points (solutions) within the state space
- different AI algorithms employ different 'tricks' (*heuristics*) of how to traverse/explore/search a state space


{% include youtubePlayer.html id="f-l4ZXo7_aM" %}


&nbsp;

The video is essentially a recording of a powerpoint presentation, with voice narration done via the [FromTextToSpeech](http://www.fromtexttospeech.com/)
website. Editing of the video was performed in Adobe Premiere and then uploaded to YouTube.

#### Future Work

I plan to extend this work further. Apart from video material, interactive visualisations can help a student to 'experiment' with 
this concept. I might use R for the visualisation part.

<!-- Another option is to leverage non-traditional user interfaces, like [*Tangible User Interfaces (TUIs)*](https://en.wikipedia.org/wiki/Tangible_user_interface). 
Some great work on TUIs is currently ongoing at Middlesex University by other researchers and colleagues of mine. -->

Look out for more on this in future blogs!


