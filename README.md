## Recreating Space Invader game

# Overview
This is a project I realized in my 1st year of University Bachelor of Technology. We had to choose between multiple games like Pac-Man, Snake, Pong and others.

## Setup

### Requirements
- [Linux](https://www.ubuntu-fr.org/) install the version you prefer **or** [Windows](https://www.microsoft.com/fr-fr/software-download/windows11)
- [VS code](https://code.visualstudio.com/) install it if you use Windows

### Recommended Setup
To make it work, the method on [Linux](https://www.ubuntu-fr.org/) and [Windows](https://www.microsoft.com/fr-fr/software-download/windows11) are differents :
#### Linux
On [Linux](https://www.ubuntu-fr.org/) it is important to know that you can't run a file if it is located on a USB key. So, copy the file and put it on your computer files.
- Then open a **Terminal** on the place the file is located.
- Put this code on the terminal for compiling the game : ```gcc SI_Game.c -o SI_Game -lncurses```
- Every time you want to run the game, put : ```./SI_game```
##### Windows
Before doing the manipulations, install by **Extensions** in [VS code](https://code.visualstudio.com/), C/C++ (Microsoft), C/C++ Extension Pack (optional) and WSL (Microsoft). Now you can start manipulating :
- Download _SI_Game.c_ on your computer.
- Open the file with [VS code](https://code.visualstudio.com/).
- Open the **Terminal** and type the command : ```wsl --install```.
- Apply ```y``` every time it ask you and allow every permission on *yes*.

Now, if the **Terminal** looks like a Linux terminal :
- Put this code on the terminal for compiling the game : ```gcc SI_Game.c -o SI_Game -lncurses```
- Every time you want to run the game, put : ```./SI_game```

## Usage
Play it like the first day it got released !