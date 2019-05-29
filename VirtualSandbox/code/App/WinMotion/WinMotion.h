#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>

//Include LeapMotion and irrlicht
#include <Leap.h>
#include <irrlicht.h>

/* This class handles the input of the LeapMotion sensor and */
/* renders a visualization of the user's hand to the screen.  */

#define INTERPOLATED_BLOCK_AMOUNT 10 //How many blocks get interpolated between every bone in the hand

//#define DYNAMIC_RANGE

class IrrMotion
{
	public:

		IrrMotion();
		IrrMotion(irr::scene::ISceneManager *manager);

		void SetSceneManager(irr::scene::ISceneManager *manager);

		void Update();

		irr::core::vector3df GetPalmPosition();
		int GetSphereRadius();

	private:    //Methods

		void SetupHandBlocks();
		void UpdateHandBlocks();
		void InterpolateHandBlocks();

	private:    //Attributes

		Leap::Controller _leapController;                   //The LeapMotion controller to get the data 
		std::vector<irr::scene::ISceneNode*> _handBlocks;   //The SceneNode objects, which will form the hand
		std::vector<irr::scene::ISceneNode*> _interpolatedHandBlocks;

		irr::scene::ISceneManager *_manager;

};
#pragma once
