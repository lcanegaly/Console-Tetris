#pragma once


//Multiply Matrix A X B, return result matrix.
void matmul(int *ptr, int a_width, int direction = 1) {
	
	//Default rotation matrix
	int matB[] = { 0, 1,
				  -1, 0 };

	//if direction is less than 0, modify rotation matrix to counter clockwise
	if (direction < 0) {
		matB[2] = 1;
		matB[1] = -1;
	}
	
	int b_width = 2;

	int* temp;
	temp = new int[a_width*2];

	for (int a = 0; a < a_width*2; a++) {
		
		//write input matrix to a temporary array.
		temp[a] = ptr[a]; 
		ptr[a] = 0;

	}

	for (int y = 0; y < a_width; y++) {
		for (int x = 0; x < b_width; x++) {
			for (int i = 0; i < b_width; i++) {

				//matQ[y + x * a_width] += matA[y + i * a_width] * matB[i + x * b_width];
				ptr[y + x * a_width] += matB[i + x * b_width] * temp[y + i * a_width];
			}
		}
	}
	delete[] temp;
}