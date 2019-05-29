#include "World.h"

World::World() { }

World::World(irr::scene::ISceneManager *manager)
{
	_light = manager->addLightSceneNode(0, irr::core::vector3df(0, 500, 0), irr::video::SColor(255, 255, 255, 255), 800, -1);

	//Create the World by adding blocks which function as our ground	
	for (unsigned int x = 0; x < WORLD_SIZE; x++)
	{
		for (unsigned int y = 0; y < WORLD_SIZE; y++)
		{
			_worldBlocks[x][y] = manager->addAnimatedMeshSceneNode(manager->getMesh("Models\\WaterBlock\\WaterBlock.obj"), 0, -1,
											irr::core::vector3df(x * (BLOCK_SIZE + 0.2f) - (0.5f * WORLD_SIZE) * (BLOCK_SIZE + 0.2f), GROUND_HEIGHT, 
																 y * (BLOCK_SIZE + 0.2f) - (0.5f * WORLD_SIZE) * (BLOCK_SIZE + 0.2f)), 
											irr::core::vector3df(0, 0, 0), irr::core::vector3df(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE));
		}
	}
}

void World::Update(irr::core::vector3df handPosition, int sphereRadius)
{
	//Update the World
	if (sphereRadius < 40)
	{
		//If the user's hand is closed, he does not want to manipulate the world at the moment
		return;
	}

	//The influence range for the blocks depends on how far the user has opend his hand
	int influenceRange = 14; 
	
	//If DYNAMIC_RANGE is defined, the influence range for the blocks is depending on the left hand position of the user
	#ifdef DYNAMIC_RANGE
	influenceRange = static_cast<int>(sphereRadius * 0.25f);
	#endif 

	//Now we can create the hill
	for (unsigned int x = 0; x < WORLD_SIZE; x++)
	{
		for (unsigned int y = 0; y < WORLD_SIZE; y++)
		{
			if (GetDistanceXZLayer(_worldBlocks[x][y]->getAbsolutePosition(), handPosition) < influenceRange)
			{
				//Get the height of the player hand 
				float relativeHeight = handPosition.Y - GROUND_HEIGHT;

				//Get the distance between the mountains top and the block on the x-z layer
				float centerDistance = sqrt(pow(_worldBlocks[x][y]->getAbsolutePosition().X - handPosition.X, 2) +
											pow(_worldBlocks[x][y]->getAbsolutePosition().Z - handPosition.Z, 2));

				//Calculate how much we have to distract from the mountains top depending on how far the block is away
				float heightSub = static_cast<float>((centerDistance * (relativeHeight / sqrt(pow(influenceRange, 2) + pow(influenceRange, 2)))));
				
				//Set the new position
				_worldBlocks[x][y]->setPosition(irr::core::vector3df(_worldBlocks[x][y]->getAbsolutePosition().X,
																	 handPosition.Y - heightSub,
																	 _worldBlocks[x][y]->getAbsolutePosition().Z));
			}
		}
	}
}

/* ################## Private ##################### */

float World::GetDistanceXZLayer(irr::core::vector3df p1, irr::core::vector3df p2)
{
	return sqrt(pow(p1.X - p2.X, 2) + pow(p1.Z - p2.Z, 2));
}