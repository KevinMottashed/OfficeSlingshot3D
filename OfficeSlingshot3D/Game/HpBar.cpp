#include "HpBar.h"

HpBar::HpBar(cWorld* world, bool isLocal): 
	isLocal(isLocal)
{
	hp = totalHp;

	/*cLabel* titleLabel = new cLabel();
    world->addChild(titleLabel);
    titleLabel->setPos(-1.7f, 2.05f, 6.0f);
    titleLabel->m_fontColor.set(1,1,0);
    titleLabel->m_string = "Player 1";*/

	hpBarMesh = new cMesh(world);
    int vertices [4];

	if (isLocal)
	{
		vertices[0] = hpBarMesh->newVertex(-0.1, 1.9, 6);
		vertices[1] = hpBarMesh->newVertex(-0.1, 2.0, 6);
		vertices[2] = hpBarMesh->newVertex(-1.7, 2.0, 6);
		vertices[3] = hpBarMesh->newVertex(-1.7, 1.9, 6);
	}
	else
	{
		vertices[0] = hpBarMesh->newVertex(1.7, 1.9, 6);
		vertices[1] = hpBarMesh->newVertex(1.7, 2.0, 6);
		vertices[2] = hpBarMesh->newVertex(0.1, 2.0, 6);
		vertices[3] = hpBarMesh->newVertex(0.1, 1.9, 6);
	}

	hpBarMesh->newTriangle(vertices[0], vertices[1], vertices[2]);
	hpBarMesh->newTriangle(vertices[0], vertices[2], vertices[3]);

	// compute normals
    hpBarMesh->computeAllNormals();

	cMaterial mat0;
    mat0.m_ambient.set(0.0, 1.0, 0.0);
    mat0.m_diffuse.set(0.0, 1.0, 0.0);
    mat0.m_specular.set(0.0, 1.0, 0.0);
    mat0.m_emission.set(0.0, 1.0, 0.0);
    hpBarMesh->setMaterial(mat0);

	world->addChild(hpBarMesh);
}

HpBar::~HpBar(void)
{
}


void HpBar::ResetHp()
{
	hp = totalHp;
    int vertices [4];

	hpBarMesh->clear();

	if (isLocal)
	{
		vertices[0] = hpBarMesh->newVertex(-0.1, 1.9, 6);
		vertices[1] = hpBarMesh->newVertex(-0.1, 2.0, 6);
		vertices[2] = hpBarMesh->newVertex(-1.7, 2.0, 6);
		vertices[3] = hpBarMesh->newVertex(-1.7, 1.9, 6);
	}
	else
	{
		vertices[0] = hpBarMesh->newVertex(1.7, 1.9, 6);
		vertices[1] = hpBarMesh->newVertex(1.7, 2.0, 6);
		vertices[2] = hpBarMesh->newVertex(0.1, 2.0, 6);
		vertices[3] = hpBarMesh->newVertex(0.1, 1.9, 6);
	}

	hpBarMesh->newTriangle(vertices[0], vertices[1], vertices[2]);
	hpBarMesh->newTriangle(vertices[0], vertices[2], vertices[3]);
    hpBarMesh->computeAllNormals();
}

void HpBar::ReduceHP(unsigned int hpLost)
{
	hp -= hpLost;
    int vertices [4];

	hpBarMesh->clear();

	if (isLocal)
	{
		vertices[0] = hpBarMesh->newVertex((hp/totalHp)*1.6 - 1.7, 1.9, 6);
		vertices[1] = hpBarMesh->newVertex((hp/totalHp)*1.6 - 1.7, 2.0, 6);
		vertices[2] = hpBarMesh->newVertex(-1.7, 2.0, 6);
		vertices[3] = hpBarMesh->newVertex(-1.7, 1.9, 6);
	}
	else
	{
		vertices[0] = hpBarMesh->newVertex(1.7, 1.9, 6);
		vertices[1] = hpBarMesh->newVertex(1.7, 2.0, 6);
		vertices[2] = hpBarMesh->newVertex(0.1 + ((totalHp-hp)/totalHp)*1.6, 2.0, 6);
		vertices[3] = hpBarMesh->newVertex(0.1 + ((totalHp-hp)/totalHp)*1.6, 1.9, 6);
	}

	hpBarMesh->newTriangle(vertices[0], vertices[1], vertices[2]);
	hpBarMesh->newTriangle(vertices[0], vertices[2], vertices[3]);
    hpBarMesh->computeAllNormals();
}