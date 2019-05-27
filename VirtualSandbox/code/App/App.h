#pragma once
#include <iostream>

//Include 3D-Graphic Engine
#include <irrlicht.h>

//Include WinMotion for rendering the hand models
#include "WinMotion/WinMotion.h"

//Include the World class
#include "World/World.h"

/* The App class handles all sub-modules neccessery to run the App */

class App
{
	public:

		App();

		void Run();
		bool IsOpen();

	private:

		void Update();
		void Render();

		irr::IrrlichtDevice* _device;
		irr::video::IVideoDriver* _driver;
		irr::scene::ISceneManager* _manager;
		irr::scene::ICameraSceneNode* _camera;

		//Use IrrMotion for rendering the hand models
		IrrMotion _hand;

		//Use the world class for rendering the blocks and handling the interaction with the user
		World _world;
};