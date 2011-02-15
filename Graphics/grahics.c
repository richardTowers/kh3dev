#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
	FILE* file;
	file=fopen("test.svg", "w+");
	
	//Get width and height
	int width=1640, height=400;
	
	//Print header
	fprintf(file, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"no\"?>\n"
	"<svg xmlns=\"http://www.w3.org/2000/svg\" height=\"%d\" width=\"%d\" version=\"1.1\" "
	"y=\"0\" x=\"0\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"  >\n\n", height, width);
	
	//Print background rectangle
	fprintf(file, "<rect x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" style=\"fill:white; fill-opacity:1;\"/>\n\n", width, height);
	
	//Print boundaries
	fprintf(file, "<line style=\"fill:none; stroke:red; stroke-width:2\" "
	"x1=\"0\" y1=\"0\" x2=\"%d\" y2=\"%d\" />\n\n", width, height);
	
	fprintf(file, "<polyline style=\"fill:none; stroke:black; stroke-width:1\" "
	"points=\"\n"
	"500.00,250.00\n"
	"540.00,250.00\n"
	"560.00,284.64\n"
	"600.00,284.64\n"
	"620.00,250.00\n"
	"640.00,215.36\n"
	"680.00,215.36\n"
	"700.00,250.00\n"
	"740.00,250.00\n"
	"760.00,284.64\n"
	"740.00,319.28\n"
	"760.00,353.92\n"
	"800.00,353.92\n"
	"840.00,353.92\n"
	"800.00,353.92\n"
	"780.00,319.28\n"
	"800.00,284.64\n"
	"780.00,250.00\n"
	"800.00,215.36\n"
	"840.00,215.36\n"
	"860.00,180.72\n"
	"840.00,146.08\n"
	"820.00,111.44\n"
	"840.00,76.79\n"
	"880.00,76.79\n"
	"900.00,42.15\n"
	"940.00,42.15\n"
	"960.00,76.79\n"
	"1000.00,76.79\n"
	"1020.00,42.15\n"
	"1040.00,7.51\n"
	"1080.00,7.51\n"
	"1100.00,42.15\n"
	"1140.00,42.15\"\n"
	"/>\n\n"
	"</svg>");
	
	fclose(file);
}
