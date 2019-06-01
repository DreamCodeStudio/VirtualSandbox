#include "World.h"

World::World() { }

World::World(irr::scene::ISceneManager *manager)
{
	//Save pointer to scene manager
	_manager = manager;

	//Add light to our world
	_light = manager->addLightSceneNode(0, irr::core::vector3df(0, 500, 0), irr::video::SColor(255, 255, 255, 255), 250, -1);

	//Create the World by adding blocks which function as our ground	
	for (unsigned int x = 0; x < WORLD_SIZE; x++)
	{
		for (unsigned int y = 0; y < WORLD_SIZE; y++)
		{
			_worldBlocks[x][y] = manager->addAnimatedMeshSceneNode(manager->getMesh("Models\\GrassBlock\\GrassBlock.obj"), 0, -1,
											irr::core::vector3df(x * (BLOCK_SIZE + 0.2f) - (0.5f * WORLD_SIZE) * (BLOCK_SIZE + 0.2f), GROUND_HEIGHT, 
																 y * (BLOCK_SIZE + 0.2f) - (0.5f * WORLD_SIZE) * (BLOCK_SIZE + 0.2f)), 
											irr::core::vector3df(0, 0, 0), irr::core::vector3df(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE));
		}
	}

	//Set start values
	_markedBlock = nullptr;
}

void World::Update(irr::core::vector3df handPosition, int sphereRadius, bool rightHand)
{
	if (rightHand == true)
	{
		//If the user uses his right hand, the surface should get manipulated
		this->UpdateSurface(handPosition, sphereRadius);
	}
	else
	{
		//If the user uses his left hand, he wants to mark a location
		this->HandleViewpointInput(handPosition, sphereRadius);
	}

	this->UpdateViewpointConnections();
	_light->setPosition(_manager->getActiveCamera()->getAbsolutePosition() * 1.5f);
}

/* ################## Private ##################### */

void World::UpdateSurface(irr::core::vector3df handPosition, int sphereRadius)
{
	//Update the World
	if (sphereRadius < 40)
	{
		//If the user's hand is closed, he does not want to manipulate the world at the moment
		return;
	}

	//Now we can create the hill
	for (unsigned int x = 0; x < WORLD_SIZE; x++)
	{
		for (unsigned int y = 0; y < WORLD_SIZE; y++)
		{
			if (GetDistanceXZLayer(_worldBlocks[x][y]->getAbsolutePosition(), handPosition) < INFLUENCE_RANGE)
			{
				//Get the height of the player hand 
				float relativeHeight = handPosition.Y - GROUND_HEIGHT;

				//Get the distance between the mountains top and the block on the x-z layer
				float centerDistance = sqrt(pow(_worldBlocks[x][y]->getAbsolutePosition().X - handPosition.X, 2) +
					pow(_worldBlocks[x][y]->getAbsolutePosition().Z - handPosition.Z, 2));

				//Calculate how much we have to distract from the mountains top depending on how far the block is away
				float heightSub = static_cast<float>((centerDistance * (relativeHeight / sqrt(pow(INFLUENCE_RANGE, 2) + pow(INFLUENCE_RANGE, 2)))));

				//Check if the user would accidently lower the height of the block when he truly wants to rise it
				if ((handPosition.Y > _worldBlocks[x][y]->getAbsolutePosition().Y &&
					handPosition.Y - heightSub > _worldBlocks[x][y]->getAbsolutePosition().Y + (0.5f * _worldBlocks[x][y]->getScale().Y)) ||
					(handPosition.Y < _worldBlocks[x][y]->getAbsolutePosition().Y &&
					handPosition.Y - heightSub < _worldBlocks[x][y]->getAbsolutePosition().Y))
				{
					//Set the new position
					_worldBlocks[x][y]->setPosition(irr::core::vector3df(_worldBlocks[x][y]->getAbsolutePosition().X,
																		 handPosition.Y - heightSub,
																		 _worldBlocks[x][y]->getAbsolutePosition().Z));

					//Now go through check if we have to scale the block to create a solid surface
					_worldBlocks[x][y]->updateAbsolutePosition();
					float heightFromGround = _worldBlocks[x][y]->getAbsolutePosition().Y - GROUND_HEIGHT;
					
					//If the heightFromGround is negative, the block is part of a lake, so we do not want to scale it in any way
					if (heightFromGround < 0)
					{
						_worldBlocks[x][y]->setScale(irr::core::vector3df(1, 1, 1));
					}
					else
					{
						//Otherwise it is part of a hill and we want to scale the block, so it goes from its highest point down to the ground
						_worldBlocks[x][y]->setScale(irr::core::vector3df(1, heightFromGround, 1));
						_worldBlocks[x][y]->setPosition(irr::core::vector3df(_worldBlocks[x][y]->getAbsolutePosition().X,
							_worldBlocks[x][y]->getAbsolutePosition().Y - (0.5f * heightFromGround),
							_worldBlocks[x][y]->getAbsolutePosition().Z));
					}

					this->UpdateBlockColor(_worldBlocks[x][y]);
				}
			}
		}
	}
}

void World::HandleViewpointInput(irr::core::vector3df handPosition, int sphereRadius)
{
	//Mark the location under the user's hand
	bool marked = false;

	//First delete previously marked location
	if (_markedBlock != nullptr)
	{
		_markedBlock->remove();
	}
	
	for (unsigned int x = 0; x < WORLD_SIZE; x++)
	{
		for (unsigned int y = 0; y < WORLD_SIZE; y++)
		{
			//If the distance on the x-z layer is under 1, we are below the user's hand
			if (this->GetDistanceXZLayer(_worldBlocks[x][y]->getAbsolutePosition(), handPosition) <= 1 && !marked)
			{
				//If the block is under the user's hand, mark it red
				_markedBlock = _manager->addAnimatedMeshSceneNode(_manager->getMesh("Models\\LocMarkBlock\\LocMarkBlock.obj"),
					0, -1, irr::core::vector3df(_worldBlocks[x][y]->getAbsolutePosition().X,
												_worldBlocks[x][y]->getAbsolutePosition().Y + BLOCK_SIZE,
												_worldBlocks[x][y]->getAbsolutePosition().Z));

				marked = true;
			}
			else
			{
				//Otherwise turn it back to its original color
				this->UpdateBlockColor(_worldBlocks[x][y]);
			}
		}
	}

	if (sphereRadius < 30)
	{
		//If the new position is near a previously saved one, the user planted it accidently
		for (unsigned int i = 0; i < _savedBlocks.size(); i++)
		{
			//If the distaance is less than 3 the user probably planted is accidently
			if (GetDistanceXZLayer(_savedBlocks[i]->getAbsolutePosition(),
				_markedBlock->getAbsolutePosition()) < 3)
			{
				return;
			}
		}

		//If not, store the block 
		_savedBlocks.push_back(_markedBlock);
		_markedBlock = nullptr;
	}
}

void World::UpdateViewpointConnections()
{
	//Clear all previously created lines
	for (unsigned int i = 0; i < _lines.size(); i++)
	{
		_lines[i]->remove();
	}
	_lines.clear();

	//Update the connection between the different Viewpoints
	for (unsigned int i = 0; i < _savedBlocks.size(); i++)
	{
		for (unsigned int k = 0; k < _savedBlocks.size(); k++)
		{
			if (k == i)
			{
				break;
			}

			irr::core::vector3df connection(_savedBlocks[i]->getAbsolutePosition() - _savedBlocks[k]->getAbsolutePosition());
			irr::core::line3df line(_savedBlocks[k]->getAbsolutePosition(), _savedBlocks[k]->getAbsolutePosition() + connection);

			//Now check if the line intersects with a world block
			bool intersect = false;
			for (unsigned int x = 0; x < WORLD_SIZE; x++)
			{
				for (unsigned int y = 0; y < WORLD_SIZE; y++)
				{
					if (_worldBlocks[x][y]->getTransformedBoundingBox().intersectsWithLine(line))
					{
						intersect = true;
					}
				}
			}

			//If they don't create a line between the viewpoints
			if (!intersect)
			{
				//Calculate radiant on X- Y- and Z-Axis
				irr::core::vector3df connectionCopy = irr::core::vector3df(connection.X, 0, connection.Z);

				if ((connectionCopy.X < 0 && connectionCopy.Z > 0))
				{
					connectionCopy *= -1;
				}
				if (connectionCopy.X > 0 && connectionCopy.Z > 0)
				{
					connectionCopy *= -1;
				}

				//Calculate the degree on X-Z-Layer
				float radiantY = acos((connectionCopy.X) / (connectionCopy.getLength()));
				float degreeY = (360 / (2 * 3.14f)) * radiantY;

				//Create a line between the two viewpoints
				_lines.push_back(_manager->addCubeSceneNode(1, 0, -1, line.getMiddle(),
					irr::core::vector3df(0, degreeY, 0),
					irr::core::vector3df(connection.getLength(), 1, 1)));

			}
		}
	}
}

float World::GetDistanceXZLayer(irr::core::vector3df p1, irr::core::vector3df p2)
{
	return sqrt(pow(p1.X - p2.X, 2) + pow(p1.Z - p2.Z, 2));
}

void World::UpdateBlockColor(irr::scene::IAnimatedMeshSceneNode *block)
{
	//If the block is below height 40 -> it should be water
	if (block->getAbsolutePosition().Y <= 40)
	{
		block->setMesh(_manager->getMesh("Models\\WaterBlock\\WaterBlock.obj"));
	}
	//If the block is between height 40 and 70 it should be grass
	else if (block->getAbsolutePosition().Y >= 40 && block->getAbsolutePosition().Y <= 70)
	{
		block->setMesh(_manager->getMesh("Models\\GrassBlock\\GrassBlock.obj"));
	}
	//If the block is over the height of 70 it should be stone
	else if (block->getAbsolutePosition().Y >= 70)
	{
		block->setMesh(_manager->getMesh("Models\\StoneBlock\\StoneBlock.obj"));
	}
}

void World::LoadAsMesh()
{
	
}