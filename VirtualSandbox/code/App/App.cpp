#include "App.h"

App::App()
{
	//Create graphical window
	_device = irr::createDevice(irr::video::EDT_DIRECT3D9, irr::core::dimension2d<irr::u32>(1920, 1080), 16U, false, true, true, 0);
	
	//Get the video driver
	_driver = _device->getVideoDriver();

	//Get scene manager
	_manager = _device->getSceneManager();


	//Add camera to the scene
	_camera = _manager->addCameraSceneNodeMaya(0, -1500, 200, 1500, -1, 70);
	_camera->setTarget(irr::core::vector3df(0, 35, 0));

	//Create the hand
	_hand = IrrMotion(_manager);

	//Create world
	_world = World(_manager);
}

void App::Run()
{
	//Call the Render and Update methods
	this->Update();
	this->Render();
}

bool App::IsOpen()
{
	return _device->run();
}

/* ################### Private ##################### */

void App::Update()
{
	//Update everything here
	_hand.Update();
	_world.Update(_hand.GetPalmPosition(), _hand.GetSphereRadius(), _hand.IsRightHand());
}

void App::Render()
{
	//Render everything here
	if (_device->run())
	{
		_driver->beginScene(true, true, irr::video::SColor(180, 180, 180, 180));

		_manager->drawAll();

		_driver->endScene();
	}

}