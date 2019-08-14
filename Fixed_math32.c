int Norm32(uint32_t x) {
	//Returns the number of (leading zeros-1) = number of needed shifts
	int i = 0;

	if (x == 0) {
		return 0;
	}

	while (x <= (1 << 30))
	{
		x = x << 1;
		i++;
	}

	return i;
}

int32_t div32(int32_t x, int32_t y) // x - Numerator, y - Denumenator
{
	// Newton–Raphson division method

	uint32_t coef1, coef2, one, ux, uy;
	uint32_t temp1;
	uint64_t temp, res;
	uint64_t temp3;

	int32_t res_sig;
	int i = 0;

	if (y == 0) {
		printf("Error! Division by zero!\n");
		system("pause");
		return 0;
	}
	if (x == y) {
		return INT32MAX;
	}

	ux = (uint32_t)x; // N'
	uy = (uint32_t)y; // D'

	i = Norm32(uy);  
	uy = uy << i;
	ux = ux << i;

	uy = RightShift32(uy, 2);
	ux = RightShift32(ux, 2);

	coef1 = float2fixed(0.4705882352941176);  // Q31 divided by 4		32/17    
	coef2 = float2fixed(0.7058823529411765);  // Q31 divided by 4	    48/17   

	coef1 = RightShift32(coef1, 2);	
	coef2 = RightShift32(coef2, 2);
	one = RightShift32(INT32_MAX, 2);

	//Initial approximation
	temp = mul64(coef1, uy);					// Q29 * Q29 = Q58,
	temp = adds64(temp, (1 << 28)); // round
	temp = RightShift64(temp, 29); // -> Q29
	temp1 = subs32(coef2, (int32_t)temp);		// X in Q29
												// X := 48/17 - 32/17 × D'

	temp1 = LeftShift32(temp1, 2);				// divided by 4 coeffs compensation

	for (int e = 0; e < 3; e++) {
		temp3 = mul64(uy, temp1);							// D' * X			Q29 *Q29 = Q58
		temp3 = adds64(temp3, (1 << 28)); // round
		temp3 = RightShift64(temp3, 29);  // -> Q29

		temp3 = subs32(one, temp3);		                // 1 - D' * X

		temp3 = mul64(temp1, temp3);			   // X *( 1 - D' * X)
		temp3 = adds64(temp3, (1 << 28)); // round
		temp3 = RightShift64(temp3, 29);  // -> Q29

		temp3 = adds32(temp1, temp3);          // X + X *( 1 - D' * X)

		temp1 = temp3;                         // new X
	}

	res = mul64(ux, temp3);
	res = adds64(res, (1 << 26));		 // round

	res = RightShift64(res, 27);		/*res = RightShift32(res, 29);  res = LeftShift32(res, 2);*/

										//Sign Checking
	if (((x > 0) && (y < 0)) || ((x < 0) && (y > 0))) {
		res_sig = res | (1 << 31);
	}
	else {
		res_sig = res;
	}

	return res_sig;
}

int Norm16(uint16_t x) {
	//Returns the number of (leading zeros-1) = number of needed shifts
	int i = 0;

	if (x == 0) {
		return 0;
	}

	while (x <= (1 << 14))
	{
		x = x << 1;
		i++;
	}

	return i;
}

int16_t div16(int16_t x, int16_t y) // x - Numerator, y - Denumenator
{
	// Newton–Raphson division method

	uint32_t coef1, coef2, one, ux, uy;
	uint32_t temp1, temp3;
	uint64_t temp, temp2, res;

	int16_t res_sig;
	int i = 0;

	ux = (uint32_t)x; // N'
	uy = (uint32_t)y; // D'

	i = Norm16(uy);
	uy = uy << i;
	ux = ux << i;

	uy = LeftShift32(uy, 14);    //Q29
	ux = LeftShift32(ux, 14);

	coef1 = float2fixed(0.4705882352941176);  // Q31 divided by 4		32/17    
	coef2 = float2fixed(0.7058823529411765);  // Q31 divided by 4	    48/17   

	coef1 = RightShift32(coef1, 2);    //Q29
	coef2 = RightShift32(coef2, 2);
	one = RightShift32(INT32_MAX, 2);

	//Initial approximation
	temp = mul64(coef1, uy);					// Q29 * Q29 = Q58,
	temp = adds64(temp, (1 << 28)); // round
	temp = RightShift64(temp, 29); // -> Q29
	temp1 = subs32(coef2, (int32_t)temp);		// X in Q29
												// X := 48/17 ? 32/17 × D'

	temp1 = LeftShift32(temp1, 2);				// divided by 4 coeffs compensation

	for (int e = 0; e < 3; e++) {
		temp2 = mul64(uy, temp1);							// D' * X			Q29 *Q29 = Q58
		temp2 = adds64(temp2, (1 << 28)); // round
		temp2 = RightShift64(temp2, 29);  // -> Q29

		temp2 = subs32(one, temp2);		                // 1 - D' * X

		temp2 = mul64(temp1, temp2);			   // X *( 1 - D' * X)
		temp2 = adds64(temp2, (1 << 28)); // round
		temp2 = RightShift64(temp2, 29);  // -> Q29

		temp2 = adds64(temp1, temp2);          // X + X *( 1 - D' * X)

		temp1 = temp2;                         // new X
	}

	res = mul64(ux, temp2);
	res = adds64(res, (1 << 26));		 // round

	res = RightShift64(res, 43);		/*res = RightShift32(res, 29);  res = LeftShift32(res, 2);*/

										//Sign Checking
	if (((x > 0) && (y < 0)) || ((x < 0) && (y > 0))) {
		res_sig = res | (1 << 15);
	}
	else {
		res_sig = res;
	}

	return res_sig;
}
