#pragma once
#include <iostream>

//Include 3D-Graphic Engine
#include <irrlicht.h>

/* The World class renders and updates the world. It also handles
   the interaction between the user's hand and the dynamic surface 
*/

#define WORLD_SIZE 20

class World
{
	public:

		World();
		World(irr::scene::ISceneManager *manager);

		void Update();
		void Render();

	private:

		irr::scene::ISceneNode* _worldBlocks[WORLD_SIZE][WORLD_SIZE];

};