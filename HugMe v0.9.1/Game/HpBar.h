#ifndef HP_BAR_H
#define HP_BAR_H

#include "chai3d.h"

class HpBar
{
public:
	/**
	 * Constructor
	 * @param world The world in which the HP bar will be placed
	 * @param isLocal True is the HP bar is associated to the local user
	 */
	HpBar(cWorld* world, bool isLocal);

	/**
	 * Destructor
	 */
	~HpBar(void);

	/**
	 * Reduces the HP of the user
	 * @ param hpLost The amount of HP to be reduced
	 */
	void ReduceHP(int hpLost);

private:
	/**
	 * The current HP
	 */
	double hp;

	/**
	 * The default initial total health
	 */
	static const int totalHp = 10;

	/**
	 * The HP bar's mesh object
	 */
	cMesh* hpBarMesh;

	/**
	 * True if the HP bar is associated to the local user
	 */
	bool isLocal;
};

#endif