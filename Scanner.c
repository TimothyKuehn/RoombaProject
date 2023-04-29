

/**
 * scanner.c
 * Author grantiepoo
 */

volatile int rawVal[60];
void scannerIR(void)
{
    int val0, val1;

	int i;
	for (i = 0;i < 180; i+=3)
	{
	    val1 = adc.read();
	    val2 = adc.read();
	    rawVal[i] = (val1 + val2) / 2;
	    servo_move(i);
	}
}


float scannerPING(int angle)
{
    float val0, val1, val2;
    float distance;
    servo_move(angle);
    val0 = ping_getDistance;
    val1 = ping_getDistance;
    val2 = ping_getDistance;

    distance = (val0 + val1 + val2) / 3;
    return distance;
}
