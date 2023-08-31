#ifndef primitiveUtils
#define primitiveUtils

#include <time.h>
#include <vector>
#include <string>

float clamp(float number, float min, float max){

	if (number < min) return min;
	
	if (number > max) return max;

	return number;

}

int random(int lower, int upper){

	return rand() % (upper - lower + 1) + lower;

}

int alignGrid(int number, int gridSize){

	
	return ((number / gridSize) * gridSize);
				

}

#endif
