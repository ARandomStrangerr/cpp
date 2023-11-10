#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <cmath>

#define EPSILON 0.01

int width = 9,
	height = 9,
	offset = (width)/2;
char buffer[9*9] = {
	'-','-','-','-','-','-','-','-','-',
	'|',' ',' ',' ',' ',' ',' ',' ',' ',
	'|',' ',' ',' ',' ',' ',' ',' ',' ',
	'|',' ',' ',' ',' ',' ',' ',' ',' ',
	'|','-','-','-','-','-','-','-',' ',
	'|',' ',' ',' ',' ',' ',' ',' ',' ',
	'|',' ',' ',' ',' ',' ',' ',' ',' ',
	'|',' ',' ',' ',' ',' ',' ',' ',' ',
	'|',' ',' ',' ',' ',' ',' ',' ',' '},
	 displayBuffer[9*9],
	 backgroundCode = ' ';

void rotate(float rate){
	memset(displayBuffer, backgroundCode, width*height);
	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			if (buffer[y*width+x] == ' ') continue;
			int newX = (x - offset) * rate;
			int newY = y;
			displayBuffer[newY * width + newX + offset] = buffer[y * width + x];
		}
	}
}

void rotateY(float rate){
	memset(displayBuffer, backgroundCode, width*height);
	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++){
			int newX = x;
			int newY = (y-offset) * rate;
			displayBuffer[(newY + offset) * width + newX] = buffer [y*width+x];
		}
	}
}

int main(){
	float rate = 1.0f,
		  step;
	int i = 0;
	while (1) {
		if (fabs(rate+1) < EPSILON ) { // due to finite number representation and close substraction. refer to Number Analysis.
			step = 0.1;
		} else if (fabs(rate-1) < EPSILON) {
			step = -0.1f;
		}
		rate += step;
		rotateY(rate);
		for (int index = 0; index < width * height; index++){
			if (!(index % width)) putchar('\n');
			putchar(displayBuffer[index]);
		}
		usleep(100000);
		printf("\x1b[H"); // clear screen
		i++;
	}
}
