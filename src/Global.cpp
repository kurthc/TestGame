#include "Global.h"

int MyRGB(float Red, float Green, float Blue)
{
	int iRed = (int)(MAX(MIN(Red, 1.0f), 0.0f) * 255);
	int iGreen = (int)(MAX(MIN(Green, 1.0f), 0.0f) * 255);
	int iBlue = (int)(MAX(MIN(Blue, 1.0f), 0.0f) * 255);

	int Output = (int)iBlue + (iGreen << 8) + (iRed << 16);
	return Output;
}

rectangle::rectangle(float x, float y, float Width, float Height)
{
	this->x = x;
	this->y = y;
	this->Width = Width;
	this->Height = Height;
}

// TODO: Fix this method. It doesn't work.
bool DoRectanglesIntersect(rectangle R1, rectangle R2, float Precision)
{
	float R1x1 = R1.x;
	float R1y1 = R1.y;
	float R1x2 = R1.x + R1.Width;
	float R1y2 = R1.y + R1.Height;

	float R2x1 = R2.x;
	float R2y1 = R2.y;
	float R2x2 = R2.x + R2.Width;
	float R2y2 = R2.y + R2.Height;

	return (
		((R1x1 + Precision <= R2x1 && R2x1 < R1x2 - Precision) || (R1x1 + Precision < R2x2 && R2x2 <= R1x2 - Precision))
		&&
		((R1y1 + Precision <= R2y1 && R2y1 < R1y2 - Precision) || (R1y1 + Precision < R2y2 && R2y2 <= R1y2 - Precision))
	);

}