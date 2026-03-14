# 2048 Game

## Table of Contents
- [About](#about)
- [Installation & setup](#installation)
- [Gameplay](#Gameplay)
- [License](#license)

## About
This is a simple 2048 game application coded in the C programming language

## Installation & setup
Download all files and run the following commands:

gcc -c workWithFile.c -o workWithFile.o
gcc -c scores.c -o scores.o
gcc -c 2048.c -o 2048.o
gcc -o program scores.o workWithFile.o 2048.o

Finally run the *program.exe*

## Gameplay
When the game is launched, the gameplay controls are always displayed at the bottom of the game board. Simply press the action button, and the move will be initiated. All records are saved in a 
.dat file to keep track of the current best high score, the biggest won tile, and the current state of the board. This also helps in the prevention of game data loss as the progress is saved automatically. 
Additionally, there is a .txt file in which the duration of each game is saved.

## License
This project currently has no license assigned. All rights reserved until a license is chosen.
