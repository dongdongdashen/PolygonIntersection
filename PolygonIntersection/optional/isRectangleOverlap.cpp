#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "PolygonIntersection.h"

using namespace std;

bool isRectangleOverlap(Polygon poly1, Polygon poly2)
{
	double max_x1 = DBL_MIN, min_x1 = DBL_MAX, max_y1 = DBL_MIN, min_y1 = DBL_MAX;
	double max_x2 = DBL_MIN, min_x2 = DBL_MAX, max_y2 = DBL_MIN, min_y2 = DBL_MAX;
	//构建多边形的外接矩形
	for (int i = 0; i < poly1.size(); i++) 
	{
		max_x1 = max(poly1[i].x, max_x1);
		min_x1 = min(poly1[i].x, min_x1);
		max_y1 = max(poly1[i].y, max_y1);
		min_y1 = min(poly1[i].y, min_y1);
	}

	for (int i = 0; i < poly2.size(); i++)
	{
		max_x2 = max(poly2[i].x, max_x2);
		min_x2 = min(poly2[i].x, min_x2);
		max_y2 = max(poly2[i].y, max_y2);
		min_y2 = min(poly2[i].y, min_y2);
	}
	//矩形重叠问题转换为x轴，y轴投影是否重叠
	bool x_overlap = !(max_x1 <= min_x2 || max_x2 <= min_x1);
	bool y_overlap = !(max_y1 <= min_y2 || max_y2 <= min_y1);

	return x_overlap && y_overlap;

}