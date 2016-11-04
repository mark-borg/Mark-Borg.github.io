---
layout: post
title:  Solving River-Crossing Puzzles with R
tags:   
image:  river-crossing-puzzles.png
---

&nbsp;

*River-Crossing Puzzles* are a popular class of puzzles in the field of AI. Many flavours of these puzzles exist. 
Here we use *R* to provide a somewhat generic framework to model and solve these type of puzzles.

{{ more }}

#### River-Crossing puzzles

*River-crossing puzzles* are a type of puzzle where the objective is to move a set of pieces (objects, animals or people) across a 
river, from one bank of the river to the opposite bank, using a boat or a bridge. What makes these puzzles interesting are the set
of rules and conditions that apply. Typically the boat is only able to carry a limited number of pieces at any one go. And normally
there are rules and constraints that forbid having certain combination of pieces on the bank river and/or the boat. 

Let's look at an example.

#### The Farmer-Wolf-Goat-Cabbage riddle

Once upon a time, there was a Farmer who had a tiny boat. The boat was so tiny that it could only take the Farmer himself and 
one additional passenger. He wanted to move a Wolf, a Goat and a Cabbage across a river with his tiny boat.

![The Farmer-Wolf-Goat-Cabbage riddle](/img/posts/farmer-wolf-goat-cabbage.png)

When the Farmer is around, everyone is safe, the Wolf will not eat the Goat, the Goat will not eat the Cabbage.

But he can’t leave the Wolf alone with the Goat because the Wolf will eat the Goat. He can’t leave the Goat alone with the Cabbage 
because the Goat will eat the Cabbage. 

And of course he can only fit one more object with him on the boat (either the Wolf, 
the Goat or the Cabbage).  

The question is: *How can he safely transport the three of them to the other side of the river?*

#### The Solution

Solving river-crossing riddles entails starting with all pieces on one side of the river (typically the left bank). This is the 
*start state*. Then one considers all possible valid moves that can be done given the start state. These possible moves create a 
set of new states. The process repeats itself with the new states until we eventually arrive at the *goal state*, i.e., having
all the pieces safe and sound on the other side of the river.

In the table below I have listed a set of moves for the Farmer-Wolf-Goat-Cabbage riddle. We are using the symbols F, W, G, and C 
to stand for the Farmer, Wolf, Goat, and Cabbage respectively.

<table width="100%" border='1px solid gray'>
<col width="50%">
<col width="*">
<col width="*">
<tr class='th'><td>move</td><td>left river bank</td><td>right river bank</td></tr>
<tr><td>Start state</td><td>F W G C</td><td>_ _ _ _</td></tr>
<tr><td>Farmer takes the Goat to the right river bank</td><td>_ W _ C</td><td>F _ G _</td></tr>
<tr><td>Farmer returns alone with the boat back</td><td>F W _ C</td><td>_ _ G _</td></tr>
<tr><td>Farmer takes the Wolf to the right river bank</td><td>_ _ _ C</td><td>F W G _</td></tr>
<tr><td>Farmer returns back with the Goat</td><td>F _ G C</td><td>_ W _ _</td></tr>
<tr><td>Farmer takes the Cabbage across the river</td><td>_ _ G _</td><td>F W _ C</td></tr>
<tr><td>Farmer returns back alone</td><td>F _ G _</td><td>_ W _ C</td></tr>
<tr><td>Farmer takes the goat across. Exit state reached!</td><td>_ _ _ _</td><td>F W G C</td></tr>
</table>

As can be seen from the above table, this puzzle can be solved in 7 steps. 

*But is this the only solution there is?*

To answer the above question we must build a graph of all possible valid moves. Thus we represent (model) the problem
in terms of *graph theory*. 

Then we can apply a *graph search algorithm* to find all possible paths from the start node to the goal node, the shortest 
path (smallest number of moves needed), etc. See the following [video](http://ed.ted.com/lessons/how-the-konigsberg-bridge-problem-changed-mathematics-dan-van-der-vieren) 
in order to apreciate the usefulness of this graph theoretic approach.

And talking of graphs, the *R* language has some great packages for solving graph related problems and performing 
graph analytics. One such package that I have used a lot is [igraph](http://igraph.org/r/). And I will be using this
package in this blog to provide a solution to the river-crossing problems.

#### Generalising the solution

But before we start working on the solution, it is worthwhile remembering that River-Crossing puzzles come in many flavours
and varieties. 

This [website](https://justpuzzles.wordpress.com/2011/04/08/river-crossings2/) lists many of these. For example, there is
the *Farmer-Fox-Chicken-Spider-Caterpillar-Lettuce* puzzle where the farmer has to transfer 5 objects, but luckily for the 
farmer the boat is a bit larger (can carry 3 pieces). There are variants where a particular piece is repeated. For example,
in the *Farmer-2 Wolves-Dog-Goat-Bag of Grain* puzzle we have 2 Wolves and they can eat both the Dog and the Goat. 

Then there is the [*Japanese Family River-Crossing puzzle*](http://www.japaneseiqtest.net/) with its extremely complex rules. Also worth noting is the popular 
[*Missionaries-and-Cannibals problem*](https://en.wikipedia.org/wiki/Missionaries_and_cannibals_problem), found in many AI text books.

Actually river-crossing puzzles are in themselves just a subset of the class of wider puzzles called 
the [*Transport Puzzles*](https://en.wikipedia.org/wiki/Transport_puzzle). But this is beyond the scope here - we will just
concentrate solely on river-crossing puzzles.

#### R (igraph) solution

Keeping the above in mind, I opted to try and write as generic a solution as much as possible. After all, the 'game'
mechanics are nearly the same for all puzzles. It's only the rules and conditions that change. We will codify the 
rules separately from the rest of the code.

##### Defining the Conflict Graph

The rules and conditions that define the incompatibilites (conflicts) between the pieces can themselves be represented
using a graph structure. For example in the Farmer-Wolf-Goat-Cabbage, the following graph encodes the rules: 

- Wolf eats (conflicts with) Goat
- Goat eats Lettuce

![The Farmer-Wolf-Goat-Cabbage conflicts](/img/posts/farmer-wolf-goat-cabbage-conflicts.png)

The following R code builds this conflict graph `gr`. 
Note that in order to simplify the puzzle solving code, we add all the
pieces, even if they do not conflict with any other piece (F for the Farmer in this case.) 

```R
# the graph showing object incompatibilities
gr <- make_empty_graph(directed = TRUE)
gr <- add.vertices(gr, 4, name = c('F', 'W', 'G', 'C'))
gr <- add.edges(gr, c('W','G', 'G','C'))
plot(gr)
```

Also note that the conflict graph 
is a *directed graph*. Wolf eats Goat, but Goat does not eat Wolf - thus we define this as a directed edge (or *directed arc* in
graph theory-speak).

##### Configuring the State Space graph

We now move on to the creation of the state space. This is the graph that will contain all valid states (states where no 
piece ends up as food and all rules of the game are observed). 

We start with some configuration for this particular puzzle, and then create the empty graph `gss` that will store the state
space. Note that we created `gss` as a directed graph - actually using an undirected graph is also valid for a state space
graph.

```R
boat.capacity <-2
farmer.symbol <- 'F'

gss <- make_empty_graph(directed=TRUE)
```

We have to define which of the pieces is the Farmer. Reason is that the code that generates the state space needs to 
know who will be rowing (handling) the boat. Only the Farmer can operate the boat.

We now create the graph node representing the *start state* as shown below and add it to the state space graph `gss`:

```R
# create the initial state
state0 <- list(bank.l = c('F', 'W', 'G', 'C'), bank.r = c(), boat.pos = 1)
state0 <- make.state.name(state0)

# add the initial state as a node in the search space
gss <- add.vertices(gss, 1, name=state0$name)
V(gss)[1]$color <- 'red'
```

We adopt the following node structure for representing a state: each node consists of a list with 3 elements, `bank.l`, `bank.r`, and `boat.pos`.
`bank.l` is a vector containing the pieces that are on the left-hand side of the river, `bank.r` is contains those pieces
that are on the right-hand side, and `boat.pos` indicates where the boat is (1 for left-hand side, 2 for right-hand side).
In the case of the starting state, all pieces are on the left bank (`bank.l`) and the right bank is empty (`bank.r` is an
empty vector).

We must make a call to the function `make.state.name` for each state we create. This function constructs a string  
that serves as a label to uniquely identify that state. For the start state, the string label is: `CFGWb|`. The pipe 
symbol (`|`) represents the river and the symbols are placed on the left-hand side or the right-hand side of the pipe symbol
according to where they are located. The lower-case character `b` indicates where the boat is.

##### Creating the State Space graph

Once we have the initial state defined, generating the full state space can be done via a simple call:

```R
gss <- solve(gss, state0)
```

Function `solve` is defined in an R source file called `solve_river_crossing_puzzles.R` that can be downloaded 
from [here](/assets/solve_river_crossing_puzzles.R). I won't go over the code contained in this source file - I think that
one can use it as it is without changes for the majority of river-crossing puzzles. Also, there are inline comments for 
those brave enough to venture in.

After generating the state space graph, we make a call to `igraph`'s `simplify()` function. This removes any duplicate links
that might be created by the state space generation code. We also change the colour of the exit node and display the graph.

```R
gss <- simplify(gss, remove.loops = FALSE, remove.multiple = TRUE)

V(gss)[startsWith(V(gss)$name, '|')]$color <- 'green'
plot(gss)
```

And here is the state space graph for the Farmer-Wolf-Goat-Cabbage puzzle:

![The Farmer-Wolf-Goat-Cabbage state space](/img/posts/farmer-wolf-goat-cabbage-state-space.png)

Note that a cursory glance at the above graph shows that there are 2 different solutions for this puzzle, both of length 7. But let's use
igraph's pathfinding functions in order to get these programmatically.

##### Finding solution paths

....to do...


....try with other puzzles...









