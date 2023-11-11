# 

# 1.判断平面内任意两个多边形是否重叠

## 数据结构与函数设计：

struct Point 结构体用来表示二维平面内的点。

Node 类存放点的地址，点的类型（交点还是顶点），是否被遍历过，指向下一节点的指针。

由一系列顶点的集合 vector<Point> 表示多边形Polygon。

```
bool detectIntersect(const Point& p1, const Point& p2, const Point& p3, const Point& p4, double& line_x, double& line_y)
```

函数 detectIntersect ( ) 判断两条线段是否有交点，有就计算交点坐标。如果两条直线平行或者重合，认为不相交，交点在一条线段上，也认为不相交。p1,p2是直线一的端点坐标，
p3,p4是直线二的端点坐标，double line_x,line_y是交点。

```
bool IsPointOnSegment(const Point& p, const Point& p1, const Point& p2)
bool isPointOnPolygonEdge(const Point& pt, const Polygon& poly)
```

函数IsPointOnSegment（）和 isPointOnPolygonEdge（）分别判断点是否在线段上，点是否在多边形边上。

```
bool IsPointInPolygon(const Point& pt, Polygon& poly)
```

函数IsPointInPolygon（）判断点是否在多边形内，采用射线检测法，若与边有奇数个交点，则在多边形内，若有偶数个交点，则在多边形外。

```
bool PointCmp(Point* poly1, Point* poly2)
int ClockWise(const Polygon& p)
```

函数PointCmp ( ) 将点按从左到右，从上到下的顺序排列。函数 ClockWise（）通过计算相邻三个顶点组成的两个向量的叉积的正负来判断多边形是顺时针还是逆时针。

## 两个核心函数：

```
Node* crossWithPoly(const vector<vector<Point*>>& crossPoints, Polygon& poly1, int poly2Size, bool transpose)
```

函数 crosswithpoly（）将多边形的顶点和与另一个多边形的交点构建成一个环。

```
for (int i = 0; i < poly1.size(); ++i)
{
    vector<Point*> ptInLine;
    for (int j = 0; j < poly2Size; ++j)
    {
        Point* curCross = transpose ? crossPoints[j][i] : crossPoints[i][j];
        if (curCross != NULL)
        {
            ptInLine.push_back(curCross);
        }
    }
```

外层循环遍历多边形poly1的每条边(该边由顶点i和i+1构成)，内层循环表示poly1当前边与poly2的边是否有交点，若有则存放在ptInLine中，crossPoints[i][j]表示poly1 i与i+1顶点组成的边和poly2 j与j+1顶点组成的边的交点，该数组由另一个函数计算得到。

```
    if (crossWithPoly1 == NULL)
    {
        crossWithPoly1 = new Node(&poly1[i], vetex, false, NULL);
        pNode = crossWithPoly1;
    }
    else
    {
        pNode->next = new Node(&poly1[i], vetex, false, NULL);
        pNode = pNode->next;
    }
    for (int j = 0; j < ptInLine.size(); ++j)
    {
        pNode->next = new Node(ptInLine[j], cross, false, NULL);
        pNode = pNode->next;
    }
}
pNode->next = crossWithPoly1;
return crossWithPoly1;
```

如果链表为空，则新建链表。否则将当前顶点，以及当前顶点对应的交点依次构建链表，最后将链表首尾相连。

```
vector<Polygon> PolygonIntersection(Polygon poly1, Polygon poly2)
```

函数PolygonIntersection（）求两个多边形的交集。

```
vector<vector<Point*>> crossPoints(poly1.size(), vector<Point*>(poly2.size(), NULL));
int crossNum = 0;
for (int i = 0; i < poly1.size(); ++i)
{
    for (int j = 0; j < poly2.size(); ++j)
    {
        double x, y;
        if (detectIntersect(poly1[i], poly1[(i + 1) % poly1.size()], poly2[j], poly2[(j + 1) % poly2.size()], x, y))
        {
            crossPoints[i][j] = new Point(x, y);
            crossNum++;
        }
    }
}
```

遍历poly1,poly2的每条边，并调用detectIntersect( )函数判断是否有交点，若有结果保存在crossPoint数组中。

```
if (crossNum == 0)
{
    int inPolyNum = 0;
    for (int i = 0; i < poly1.size(); ++i)
    {
        if (IsPointInPolygon(poly1[i], poly2))
        {
            inPolyNum++;
        }
    }
    if (inPolyNum > 0)
    {
        if (inPolyNum != poly1.size())
        {
            cout << "impossible!" << endl;
            return polyInter;
        }
        polyInter.push_back(poly1);
        return polyInter;
    }
```

若无交点，则判断poly1的每个顶点是否在poly2内，若是则交集多边形是poly1本身。

对于有交点的情况，则需要分三步：

1.通过crosswithpoly构建顶点与交点的环形链表，

2.确定交集多边形的起始点，

3.根据起始点和链表构建合法的交集多边形。

```
int traver1 = 0;

while (traver1 < poly1.size() + crossNum)
{
    // 找出交集多边形的第一个点
    Node* poly1First = crossWithPoly1;

    while (true)
    {

        if (poly1First->isTraverse == false)
        {
            poly1First->isTraverse = true;
            traver1++;
            if (poly1First->ptType == cross ||
                (poly1First->ptType == vetex && IsPointInPolygon(*poly1First->pt, poly2)))
            {
                break;
            }
        }
        poly1First = poly1First->next;
        if (traver1 >= poly1.size() + crossNum)
        {
            return polyInter;
        }
    }
```

找交集多边形起始点的过程。起始点首先是要没被遍历过的，如果起始点是交点，那直接可以；如果起始点是顶点，需要判断该点是否在另一多边形内，若在，则可以。

```
    Node* cur = poly1First;
    //cout << "poly1First=" << poly1First->pt->x << "," << poly1First->pt->y << endl;
    Polygon intersection;
    bool traverseSwitch = true;
    do
    {
        intersection.push_back(*(cur->pt));
        //cout << cur->pt->x<<","<<cur->pt->y << endl;
        cur->isTraverse = true;
        if (traverseSwitch)
        {
            traver1++;
        }
        if (cur->ptType == cross && cur != poly1First)
        {
            Node* otherLinkStart = (traverseSwitch ? crossWithPoly2 : crossWithPoly1);
            while (otherLinkStart->pt != cur->pt)
            {
                otherLinkStart = otherLinkStart->next;
            }
            cur = otherLinkStart;
            traverseSwitch = !traverseSwitch;
            cur->isTraverse = true;
            if (traverseSwitch)
            {
                traver1++;
            }
        }
        cur = cur->next;
    } while (cur->pt != poly1First->pt);

    polyInter.push_back(intersection);
}
```

找到起始点后构建交集多边形的过程，根据记录的链表遍历构建多边形。如果当前遍历的节点是交点，且该交点不是起始点则需要跳转到另一链表遍历，直至形成环。点集存放在交集多边形insection中，若不只一个交集多边形，存放在多边形数组polyInter中。

main( )函数则是通过in.txt读取两个多边形顶点坐标，求交集后结果输出到out.txt中。

# 2.快速判定平面内任意两个多边形是否重叠

可以分别构建poly1，poly2的外接矩形，先判断这两个外接矩形是否重叠，若重叠，再判断多边形是否重叠。

外接矩形可以通过x轴，y轴的投影是否重叠进行判断。

```
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
```
