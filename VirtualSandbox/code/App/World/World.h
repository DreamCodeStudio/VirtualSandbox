#pragma once
#include <iostream>

//Include 3D-Graphic Engine
#include <irrlicht.h>

//Vector for storing blocks
#include <vector>

/* The World class renders and updates the world. It also handles
   the interaction between the user's hand and the dynamic surface 
*/

#define WORLD_SIZE 80
#define BLOCK_SIZE 1
#define GROUND_HEIGHT 50
#define INFLUENCE_RANGE 14

class World
{
	public:

		World();
		World(irr::scene::ISceneManager *manager);

		void Update(irr::core::vector3df handPosition, int sphereRadius, bool rightHand);
		void LoadAsMesh();

	private:

		void UpdateSurface(irr::core::vector3df handPosition, int sphereRadius);
		void UpdateViewpoints(irr::core::vector3df handPosition, int sphereRadius);

		float GetDistanceXZLayer(irr::core::vector3df p1, irr::core::vector3df p2);
		void UpdateBlockColor(irr::scene::IAnimatedMeshSceneNode *block);


		irr::scene::IAnimatedMeshSceneNode* _worldBlocks[WORLD_SIZE][WORLD_SIZE];

		irr::scene::IAnimatedMeshSceneNode* _markedBlock;
		std::vector<irr::scene::IAnimatedMeshSceneNode*> _savedBlocks;

		irr::scene::ILightSceneNode* _light;
		irr::scene::ISceneManager* _manager;

};