#include "InfoManager.h"


InfoManager::InfoManager()
{
	fps = 0;
}


InfoManager::~InfoManager()
{
}

void InfoManager::UpdateFPS()
{
	static uint64 lastTime = timeGetTime();
	uint64 currTime = 0;
	float timeDelta = 0.0f;
	uint64 frameCnt = 0;
	float fpsTime = 0.0f;

	++frameCnt;

	currTime = timeGetTime();
	timeDelta = float(currTime - lastTime);
	lastTime = currTime;

	timeDelta *= 0.001f;
	fpsTime += timeDelta;


	if (fpsTime >= 1.0f)
	{
		fps = (uint64)(frameCnt / fpsTime);
		fpsTime = 0.0f;
		frameCnt = 0;
	}
}

