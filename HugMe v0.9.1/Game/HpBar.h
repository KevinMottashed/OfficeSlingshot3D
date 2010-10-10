#ifndef HP_BAR_H
#define HP_BAR_H

#include "chai3d.h"

class HpBar
{
private:
	double hp;
	static const int totalHp = 10;
	cMesh* hpBarMesh;

	bool isLocal;

public:
	HpBar(cWorld* world, bool isLocal);
	~HpBar(void);
	void ReduceHP(int hpLost);
};

#endif