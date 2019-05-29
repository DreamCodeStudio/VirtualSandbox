#pragma once
#include <iostream>

//Include 3D-Graphic Engine
#include <irrlicht.h>

/* The World class renders and updates the world. It also handles
   the interaction between the user's hand and the dynamic surface 
*/

#define WORLD_SIZE 80
#define BLOCK_SIZE 1
#define GROUND_HEIGHT 50
//#define DYNAMIC_RANGE

class World
{
	public:

		World();
		World(irr::scene::ISceneManager *manager);

		void Update(irr::core::vector3df handPosition, int sphereRadius);
		void LoadAsMesh();

	private:

		float GetDistanceXZLayer(irr::core::vector3df p1, irr::core::vector3df p2);

		irr::scene::IAnimatedMeshSceneNode* _worldBlocks[WORLD_SIZE][WORLD_SIZE];
		irr::scene::ILightSceneNode* _light;

};