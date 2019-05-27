#include "World.h"

World::World() { }

World::World(irr::scene::ISceneManager *manager)
{
	for (unsigned int x = 0; x < WORLD_SIZE; x++)
	{
		for (unsigned int y = 0; y < WORLD_SIZE; y++)
		{
			_worldBlocks[x][y] = manager->addCubeSceneNode(5, 0, -1, irr::core::vector3df(x * 6 - (0.5f * WORLD_SIZE) * 5, 35, y * 6 - (0.5f * WORLD_SIZE) * 5));
			_worldBlocks[x][y]->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		}
	}
}