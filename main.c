#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "string.h"
#include "unistd.h"

typedef struct RGB_t 
{
	float		red;
	float		green;
	float 		blue;
	float		*max;
	float		*min;
} RGB_t;

typedef struct HSL_t
{
	float		hue;
	float		sat;
	float		lum;
} HSL_t;

void rgb_from_string(char hex_string[], RGB_t *rgb)
{

		uint8_t		s_red[3] = {hex_string[1],hex_string[2],'\0'};
 		uint8_t		s_green[3] = {hex_string[3], hex_string[4],'\0'};
		uint8_t		s_blue[3] = {hex_string[5],hex_string[6],'\0'};

		rgb->red = strtol(s_red, NULL, 16)/255.0;
		rgb->green = strtol(s_green, NULL, 16) / 255.0;
		rgb->blue = strtol(s_blue, NULL, 16) / 255.0;
}

int main(int argc, char *argv[])
{
	uint8_t		hex_string[7];
	
	if (argv[argc-1][0] != '#' && !isatty(STDIN_FILENO))
	{
		
		scanf("%7s",hex_string);
				
	}
	else
	{
		if (argv[argc-1][0] == '#')
		{
			memcpy(hex_string, argv[argc-1], 7);
		}
	}	

	if (hex_string[0] == '#')
	{	
		RGB_t rgb = {0};
		
		rgb_from_string(hex_string, &rgb);
		
		rgb.max = (rgb.red > rgb.green) ? &rgb.red : &rgb.green;
		rgb.max = (*rgb.max > rgb.blue) ? rgb.max : &rgb.blue; 
	
		rgb.min = (rgb.red < rgb.green) ? &rgb.red : &rgb.green;
		rgb.min = (*rgb.min < rgb.blue) ? rgb.min : &rgb.blue; 
		

		HSL_t	hsl = {0};

		hsl.lum = ((*rgb.max + *rgb.min)/2);
		
		if ((int) (*rgb.max *100) == (int) (*rgb.min * 100))
		{
			hsl.sat = 0;
			hsl.hue = 0;
		}
		else
		{
			hsl.sat = (hsl.lum <= 0.5) ? (*rgb.max - *rgb.min) / (*rgb.max + *rgb.min) : (*rgb.max - *rgb.min) / (2.0 - *rgb.max - *rgb.min);
		
	
			if(rgb.max == &rgb.red)
			{	
					hsl.hue = (rgb.green - rgb.blue) / (*rgb.max - *rgb.min);
			}
			else if (rgb.max == &rgb.green)
			{
					hsl.hue = 2.0 + (rgb.blue - rgb.red) / (*rgb.max - *rgb.min);
			}	
			else
			{
					hsl.hue = 4.0 + (rgb.red - rgb.green) / (*rgb.max - *rgb.min);
			}
			
			
			hsl.hue *= 60;
			
			if (hsl.hue < -0)
			{
				hsl.hue += 361;
			}
		}
		hsl.lum *= 100;
		hsl.sat *= 100;
		
		if (argc > 1 && strcmp(argv[1],"-c") == 0)
		{
			printf("hsl(%d,%d%%,%d%%)\n", (int) hsl.hue, (int) hsl.sat, (int) hsl.lum);
		}
		else
		{
			printf("%d %d%% %d%%\n", (int) hsl.hue, (int) hsl.sat, (int) hsl.lum);	
		}
			
		
	}
	else
	{
		printf("Usage: hextohsl [-c] #rrggbb\n");
	}
	return 0;
}
