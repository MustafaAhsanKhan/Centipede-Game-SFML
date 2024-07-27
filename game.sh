#!/bin/bash  
g++ -c Centipede.cpp
g++ Centipede.o -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
./sfml-app