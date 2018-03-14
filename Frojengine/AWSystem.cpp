#include "AWSystem.h"

AWSystem::AWSystem()
{

}

AWSystem::~AWSystem()
{

}

void AWSystem::Initialize()
{

}

void AWSystem::Update()
{
	///*
#define GRID_SIZE 50.0f
#define GRID_WIDTH 1.0f
	for (int i = 0; i <= GRID_SIZE; i++)
	{
		// x축 그리드
		Debug::DrawLine(VECTOR3(0, -(GRID_SIZE * GRID_WIDTH / 2), -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), VECTOR3(0, GRID_SIZE * GRID_WIDTH / 2, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), COLOR(0.3f, 0.3f, 0.3f, 1));
		Debug::DrawLine(VECTOR3(0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), -(GRID_SIZE * GRID_WIDTH / 2)), VECTOR3(0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), (GRID_SIZE * GRID_WIDTH / 2)), COLOR(0.3f, 0.3f, 0.3f, 1));

		// y축 그리드
		Debug::DrawLine(VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2), 0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), VECTOR3(GRID_SIZE * GRID_WIDTH / 2, 0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), COLOR(0.3f, 0.3f, 0.3f, 1));
		Debug::DrawLine(VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0, -(GRID_SIZE * GRID_WIDTH / 2)), VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0, (GRID_SIZE * GRID_WIDTH / 2)), COLOR(0.3f, 0.3f, 0.3f, 1));

		// z축 그리드
		Debug::DrawLine(VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2), -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0), VECTOR3(GRID_SIZE * GRID_WIDTH / 2, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0), COLOR(0.3f, 0.3f, 0.3f, 1));
		Debug::DrawLine(VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), -(GRID_SIZE * GRID_WIDTH / 2), 0), VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), (GRID_SIZE * GRID_WIDTH / 2), 0), COLOR(0.3f, 0.3f, 0.3f, 1));
	}
	//*/
}