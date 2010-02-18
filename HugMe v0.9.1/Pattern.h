#pragma once

struct PatternPacket
{
	DWORD dwPacketBytes;
	DWORD dwPointsBytes;
	DWORD dwIconBytes;
	double samplingTime;
	int nPointCount;
	// points & icon bitmap
};

struct Pattern
{
	Pattern()
		: pIconBitmap(NULL)
	{
	}

	struct Point
	{
		double x;
		double y;
		int intensity;
	};

	string name;
	double samplingTime;
	vector<Point> points;
	CBitmap* pIconBitmap;
	int currentPoint;
};
