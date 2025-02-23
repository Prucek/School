/******************************************************************************
 * Laborator 04 Krivky - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - isvoboda@fit.vutbr.cz
 *
 */

#include "student.h"
#include "globals.h"
#include "main.h"

#include <time.h>
#include <math.h>

//Viz hlavicka vector.h
USE_VECTOR_OF(Point2d, point2d_vec)
#define point2d_vecGet(pVec, i) (*point2d_vecGetPtr((pVec), (i)))

/* Secte dva body Point2d a vysledek vrati v result (musi byt inicializovan a alokovan)*/
void	addPoint2d(const Point2d *a, const Point2d *b, Point2d *result)
{
	IZG_ASSERT(result);
	result->x = a->x+b->x;
	result->y = a->y+b->y;
}

/* Vynasobi bod Point2d skalarni hodnotou typu double a vysledek vrati v result (musi byt inicializovan a alokovan)*/
void	mullPoint2d(double val, const Point2d *p, Point2d *result)
{
	IZG_ASSERT(result);
	result->x = p->x*val;
	result->y = p->y*val;
}

/**
 * Inicializace ridicich bodu horni trajektorie ve vykreslovacim okne.
 * Pocatek souradnicove soustavy je v levem hornim rohu okna. Xova souradnice
 * roste smerem doprava, Y smerem dolu.
 * @param points vystup, kam se pridavaji ridici body
 * @param offset_x posun vsech ridicich bodu v horizontalnim smeru (aby se mohli souradnice zadavat s pocatkem [0,0])
 * @param offset_y posun vsech ridicich bodu ve vertikalnim smeru
 */
void initControlPointsUp(S_Vector **points, int offset_x, int offset_y) {
  *points = vecCreateEmpty(sizeof(Point2d));
  Point2d p;
  p.x = 0;   p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 40;  p.y = -250; point2d_vecPushBack(*points, p);
  p.x = 160; p.y = -250; point2d_vecPushBack(*points, p);
  
  p.x = 200; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 210; p.y = -180; point2d_vecPushBack(*points, p);
  p.x = 350; p.y = -180; point2d_vecPushBack(*points, p);
  
  p.x = 360; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 390; p.y = -120;  point2d_vecPushBack(*points, p);
  p.x = 430; p.y = -120;  point2d_vecPushBack(*points, p);
  
  p.x = 460; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 470; p.y = -70;  point2d_vecPushBack(*points, p);
  p.x = 525; p.y = -70;  point2d_vecPushBack(*points, p);
  
  p.x = 535; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 545; p.y = -40;  point2d_vecPushBack(*points, p);
  p.x = 575; p.y = -40;  point2d_vecPushBack(*points, p);
  
  p.x = 585; p.y = 0;    point2d_vecPushBack(*points, p);

  Point2d offset = {offset_x, offset_y, 1.0};
  for(int i = 0; i < (*points)->size; i++) {
    addPoint2d(point2d_vecGetPtr(*points, i), &offset, point2d_vecGetPtr(*points, i));
  }
}

void initControlPointsDown(S_Vector **points, int offset_x, int offset_y) {
  /* == TODO ==
   * Uloha c.2
   * Nasledujuci volanni funkce initControlPointsUp(.) zmazte a nahradte vlastnim kodem,
   * ktery inicializuje ridici body tak, aby byla trajektorie spojita (C1). Muzete skopirovat
   * kod funkce initControlPointsUp(.) a upravit primo souradnice bodu v kodu.
   */
  *points = vecCreateEmpty(sizeof(Point2d));
  Point2d p;
  p.x = 0;   p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 40;  p.y = -250; point2d_vecPushBack(*points, p);
  p.x = 160; p.y = -250; point2d_vecPushBack(*points, p);

  p.x = 200; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 230; p.y = 180;  point2d_vecPushBack(*points, p);
  p.x = 320; p.y = 180;  point2d_vecPushBack(*points, p);

  p.x = 360; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 390; p.y = -120; point2d_vecPushBack(*points, p);
  p.x = 430; p.y = -120; point2d_vecPushBack(*points, p);

  p.x = 460; p.y = 0;    point2d_vecPushBack(*points, p);
  p.x = 476; p.y = 70;   point2d_vecPushBack(*points, p);
  p.x = 518; p.y = 70;   point2d_vecPushBack(*points, p);

  p.x = 535; p.y = 0;    point2d_vecPushBack(*points, p);

  p.x = 545; p.y = -40;  point2d_vecPushBack(*points, p);
  p.x = 575; p.y = -40;  point2d_vecPushBack(*points, p);

  p.x = 585; p.y = 0;    point2d_vecPushBack(*points, p);

  Point2d offset = { offset_x, offset_y, 1.0 };
  for (int i = 0; i < (*points)->size; i++)
    addPoint2d(point2d_vecGetPtr(*points, i), &offset, point2d_vecGetPtr(*points, i));

  // initControlPointsUp(points, offset_x, offset_y);
}

/**
 * Implementace vypoctu Bezierove kubiky.
 * @param P0,P1,P2,P3 ridici body kubiky
 * @param quality pocet bodu na krivke, ktere chceme vypocitat
 * @param trajectory_points vystupni vektor bodu kubiky (nemazat, jen pridavat body)
 */
void bezierCubic(const Point2d *P0, const Point2d *P1, const Point2d *P2, const Point2d *P3, 
  const int quality, S_Vector *trajectory_points){
  
  /* == TODO ==
   * Soucast Ulohy c.1:
   * Sem pridejte kod vypoctu Bezierove kubiky. Body krivky pridavejte do trajectory_points.
   */

  for (double i = 0.0; i <= 1.0 ; i += (1.0/quality)){

    Point2d tmp = {};
    tmp.x = pow((1 - i), 3) * P0->x + 3 * i * pow((1 - i), 2) * P1->x + 3 * (1-i) * pow(i, 2)* P2->x + pow (i, 3)* P3->x;
    tmp.y = pow((1 - i), 3) * P0->y + 3 * i * pow((1 - i), 2) * P1->y + 3 * (1-i) * pow(i, 2)* P2->y + pow (i, 3)* P3->y;

    point2d_vecPushBack(trajectory_points, tmp);
  }
}

/* 
 * Implementace vypoctu trajektorie, ktera se sklada z Bezierovych kubik.
 * @param quality pocet bodu krivky, ktere mame urcit
 * @param control_points ridici body krivky 
 * @param trajectory_points vystupni body zakrivene trajektorie 
 */
void	bezierCubicsTrajectory(int quality, const S_Vector *control_points, S_Vector *trajectory_points){
  // Toto musi byt na zacatku funkce, nemazat.
  point2d_vecClean(trajectory_points);

  /* == TODO ==
   * Uloha c.1
   * Ziskejte postupne 4 ridici body a pro kazdou ctverici vypocitejte body Bezierovy kubiky.
   * 
   * Kostra:
   *  for(...; ...; ...) {
   *    Point2d *P0 = ...
   *    Point2d *P1 = ...
   *    Point2d *P2 = ...
   *    Point2d *P3 = ...
   *    bezierCubic(P0, P1, P2, P3, quality, trajectory_points);
   *  }
   * Nasledujici volani funkce getLinePoints(.) zmazte - je to jen ilustrace hranate trajektorie.
   */
  for (int i = 0; i < point2d_vecSize(control_points)- 3; i += 3){

    Point2d *P0 = point2d_vecGetPtr(control_points,i);
    Point2d *P1 = point2d_vecGetPtr(control_points,i+1);
    Point2d *P2 = point2d_vecGetPtr(control_points,i+2);
    Point2d *P3 = point2d_vecGetPtr(control_points,i+3);
    bezierCubic(P0, P1, P2, P3, quality, trajectory_points);
  }

  // getLinePoints(control_points, trajectory_points);
}
