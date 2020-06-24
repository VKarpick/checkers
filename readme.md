# Checkers

A Checkers game that can be played either with graphics and mouse input or by printing to the console. 
Written with C++17 and SFML 2.5.1.

## Gameplay

<img src="/checkers/data/start-menu.png"
	width="280"/>
<img src="/checkers/data/gameplay.png"
	width="280"/>

#### AI

The AI is a little slow, using a minimax algorithm to look ahead four moves with the value of states 
being determined by the following features:
- the current player
- what piece occupies each square of the board
- how many pieces each player has
  * non-kings
  * kings
  * total
- number of trapped kings (kings that either can't moved or will be captured if they do)
- number of runaway pieces (pieces with an unimpeded path to becoming kings)

Though the runaway pieces allow for some false positives in favor of running quicker.

#### Reinforcement Learning

I tried to abstract the reinforcement learning aspects of the project as much as I could to allow for 
use in future projects.  The AI was trained using TD Leaf (or, at least, my understanding of TD Leaf). 
I'm not entirely sure how good the AI is/can be and I may make occasional pushes when weights receive 
further training.


## Background

This is my first C++ project.  I've been trying to learn some reinforcement learning on my own and decided 
I needed a project to test things out on.  Checkers seemed like a reasonable place to start.  I orginally 
intended to write it in Python - in fact, I had written pretty much all of it in Python - but when I started 
out, I didn't know what algorithm I was going to be using.  I read about TD Leaf being used for Chinook 
and decided to try to implement it but found Python to be to too slow to reach any reasonable depth with 
minimax.  So I decided to dive in to C++.  I have no doubt there are improvements that can be made, but it 
seems to run, essentially, as intended.

#### Previous Implementations

I orginally only wrote it to print to the console - as I said, this was intended as an RL project, not a 
fully functioning game.  After I got most of the work done, I decided to try to make it easier/more enjoyable 
to actually use.  This is why there are two different classes for Checkers (checkers and checkers_sfml).  One 
is the original, the other an inherited class making use of SFML for graphics.

I also started with fewer features.  I wanted to see how good it could get with minimal features (only using 
the current configuration of pieces on the board) and a minimax depth of six moves.  But I later changed my 
mind and added further features which necessitated decreasing the max depth (it's now four moves).

# Hat Tips

[David Silver's YouTube lectures and slides](https://www.davidsilver.uk/teaching/)  
Reinforcement Learning:  An Introduction - Sutton and Barto
