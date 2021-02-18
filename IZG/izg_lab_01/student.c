/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id: $
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - ibobak@fit.vutbr.cz, orderedDithering
 */

#include "student.h"
#include "globals.h"

#include <time.h>

const int M[] = {
    0, 204, 51, 255,
    68, 136, 187, 119,
    34, 238, 17, 221,
    170, 102, 153, 85
};

const int M_SIDE = 4;

/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).
 Ukol za 0.25 bodu */
S_RGBA getPixel(int x, int y)
{
	if (x > width || x < 0 || y > height || y < 0)
		return COLOR_BLACK; //vraci barvu (0, 0, 0)
	else
    	return frame_buffer[width * y + x];
    
    
}
/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.
 Ukol za 0.25 bodu */
void putPixel(int x, int y, S_RGBA color)
{
    if (!(x > width || x < 0 || y > height || y < 0))
		frame_buffer[width * y + x] = color;
}
/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na odstiny sedi. Vyuziva funkce GetPixel a PutPixel.
 Ukol za 0.5 bodu */
void grayScale()
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			S_RGBA tmp = getPixel(j,i);
			unsigned char grey = ROUND(0.299 * tmp.red) + ROUND(0.587 * tmp.green) + ROUND(0.114 * tmp.blue);
			tmp.blue = tmp.red = tmp.green = grey;
			putPixel(j,i,tmp);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu maticoveho rozptyleni.
 Ukol za 1 bod */

void orderedDithering()
{
    grayScale();

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			S_RGBA tmp = getPixel(j,i);
			int x = j % M_SIDE;
			int y = i % M_SIDE;
			if (tmp.blue > M[M_SIDE * y + x])
				putPixel(j,i,COLOR_WHITE);
			else 
				putPixel(j,i,COLOR_BLACK);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu distribuce chyby.
 Ukol za 1 bod */

typedef enum pos {Right, Down, Diagonal} Pos;
typedef struct 
{
	int j;
	int i;
	int err;
} Condition;

Condition con[] =
{
	{1,0,3},
	{0,1,3},
	{1,1,2}
};


void distribute(Pos p, int j, int i, int error)
{
	if (con[p].j*j < width -1 && con[p].i*i < height -1)
	{
		S_RGBA tmp = getPixel(j + con[p].j, i + con[p].i);
		int test = tmp.blue + (char)ROUND(error * (con[p].err / 8.0));
		if (test > 255) test = 255;
		if (test < 0) test = 0;
		tmp.green = tmp.red = tmp.blue = (char) test;
		putPixel(j + con[p].j, i + con[p].i, tmp);
	}
}

void errorDistribution()
{
	grayScale();
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			S_RGBA tmp = getPixel(j,i);

			unsigned char threshold = 128;
			int error = 0;
			if (tmp.blue > threshold)
			{
				error = tmp.blue - 255;
				tmp.blue = COLOR_WHITE.blue;
			}
			else
			{
				error = tmp.blue;
				tmp.blue = COLOR_BLACK.blue;
			}
			tmp.green = tmp.red = tmp.blue;
			putPixel(j,i,tmp);

			distribute(Right,j,i,error);
			distribute(Down,j,i,error);
			distribute(Diagonal,j,i,error);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci metody prahovani.
 Demonstracni funkce */
void thresholding(int Threshold)
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > Threshold)
				putPixel(x, y, COLOR_WHITE);
			else
				putPixel(x, y, COLOR_BLACK);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci nahodneho rozptyleni. 
 Vyuziva funkce GetPixel, PutPixel a GrayScale.
 Demonstracni funkce. */
void randomDithering()
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Inicializace generatoru pseudonahodnych cisel */
	srand((unsigned int)time(NULL));

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);
			
			/* Porovname hodnotu cervene barevne slozky s nahodnym prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > rand()%255)
			{
				putPixel(x, y, COLOR_WHITE);
			}
			else
				putPixel(x, y, COLOR_BLACK);
		}
}
/*****************************************************************************/
/*****************************************************************************/