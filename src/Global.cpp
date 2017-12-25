#include "Global.h"

int RGB(float Red, float Green, float Blue)
{
	int iRed = (int)(MAX(MIN(Red, 1), 0) * 255);
	int iGreen = (int)(MAX(MIN(Green, 1), 0) * 255);
	int iBlue = (int)(MAX(MIN(Blue, 1), 0) * 255);

	int Output = (int)iBlue * 255 + (iGreen << 8) + (iRed << 16);
	return Output;
}