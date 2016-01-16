#include <iostream>
#include <Windows.h>
#include <chrono>
#include <vector>
#include <conio.h>

#include "fsObjects.h"


using namespace std;
using namespace flightshooting;


int flightshooting::fsOverlap(const fsObject & obj1,const fsObject & obj2)
{
	if (obj1.getVisible() == 0 || obj2.getVisible() == 0) return 0;
	// 重叠判定要稍微费点脑筋
	// 判断矩形是否重叠，只需要对每个矩形，分别判断其四个顶点是否在另外一个矩形内
	const fsPoint<double> obj1ulc = obj1.getUpperLeftCorner();
	const fsPoint<double> obj1lrc = obj1.getLowerRightCorner();
	const fsPoint<double> obj1urc = fsPoint<double>(obj1lrc.x, obj1ulc.y);
	const fsPoint<double> obj1llc = fsPoint<double>(obj1ulc.x, obj1lrc.y);

	const fsPoint<double> obj2ulc = obj2.getUpperLeftCorner();
	const fsPoint<double> obj2lrc = obj2.getLowerRightCorner();
	const fsPoint<double> obj2urc = fsPoint<double>(obj2lrc.x, obj2ulc.y);
	const fsPoint<double> obj2llc = fsPoint<double>(obj2ulc.x, obj2lrc.y);

//	bool test = fsPointInRect(obj1lrc, obj2ulc, obj2lrc);
	bool obj1InObj2 = fsPointInRect(obj1ulc, obj2ulc, obj2lrc) ||
		fsPointInRect(obj1urc, obj2ulc, obj2lrc) ||
		fsPointInRect(obj1llc, obj2ulc, obj2lrc) ||
		fsPointInRect(obj1lrc, obj2ulc, obj2lrc);
	bool obj2InObj1 = fsPointInRect(obj2ulc, obj1ulc, obj1lrc) ||
		fsPointInRect(obj2urc, obj1ulc, obj1lrc) ||
		fsPointInRect(obj2llc, obj1ulc, obj1lrc) ||
		fsPointInRect(obj2lrc, obj1ulc, obj1lrc);
	return obj1InObj2 || obj2InObj1;
}

template<typename T>
bool flightshooting::fsPointInRect(fsPoint<T> point, fsPoint<T> RectUpperLeft, fsPoint<T> RectLowerRight)
{	
	return (point.x >= RectUpperLeft.x && point.x <= RectLowerRight.x)&&(point.y >= RectUpperLeft.y && point.y <= RectLowerRight.y);
}

//int flightshooting::fsInitialize()
//{
//	fsMyAircraft playerAircraft = fsMyAircraft(fsPoint<double>(0.5, 0.99),
//		fsPoint<double>(0.51, 1.00),
//		fsColor(255, 255, 255, 0));
//	fsEnemyAircraft enemy1 = fsEnemyAircraft(fsPoint<double>(0.5, 0.10),
//		fsPoint<double>(0.51, 0.11),
//		fsColor(255, 0, 0, 0));
//	cout << "aircrafts are ready!";
//	//cout << fsOverlap(playerAircraft, enemy1);
//	
//	return 0;
//}

