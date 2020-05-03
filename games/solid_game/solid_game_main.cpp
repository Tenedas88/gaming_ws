#include "olcPixelGameEngine.h"
#include "solidObjEngine.h"

olc::vi2d 			   surface[3];//TODO: add support for surface shape calculation

class MySolidObject : public SolidSquareObject
{
	public:
		MySolidObject(SolidObjGameEngine* solidEngine,olc::vi2d& onCreatePosition,unsigned int onCreateRadius):
			SolidSquareObject(solidEngine,onCreatePosition,onCreateRadius),
			lastPosition(onCreatePosition)
		{
			reset = false;
		}

	private:
		virtual olc::vi2d objectMovementFunction(void* Args) override
		{
			olc::vi2d destination = this->getPosition();
			if(Args != NULL)
			{
				olc::vi2d* inputDestination = (olc::vi2d*)Args;
				if((abs(inputDestination->y-lastPosition.y) < this->getRadius())
				   && (abs(inputDestination->x-lastPosition.x) < this->getRadius()) 
				   && reset)
				{
					destination.y = 175+rand()%25;
					destination.x = 128;
					reset = false;
				}
				else
				{
					if((inputDestination->y - destination.y) != 0)
					{
						((inputDestination->y - destination.y) < 0) ? destination.y--:destination.y++;
						destination.y = destination.y%240;
						lastPosition.y = destination.y;
					}
					if((inputDestination->x - destination.x) != 0)
					{
						((inputDestination->x - destination.x) < 0) ? destination.x--:destination.x++;
						//destination.x = destination.x%256;
						lastPosition.x = destination.x;
					}
					reset = true;
				}
			}

			return destination;
		}

		olc::vi2d lastPosition;
		bool      reset;
};

class Example : public olc::PixelGameEngine
{
public:
	SolidObjGameEngine*    SolidEngine = NULL;
	olc::vi2d 			   stoppedObjStart;
	olc::vi2d 			   stoppedObjLast;
	MySolidObject*         stoppedObj  = NULL;
	olc::vi2d			   movingObjStart;
	olc::vi2d			   movingObjLast;
	MySolidObject*         movingObj   = NULL;

	olc::vi2d 			   destination;
	CollisionSpaceHandle_t objSpace    = INVALID_COLLISION_HANDLE;

	Example():stoppedObjStart(123,128),movingObjStart(128,118),destination(120,240)
	{
		this->SolidEngine = new SolidObjGameEngine();
		this->stoppedObj  = new MySolidObject(SolidEngine,stoppedObjStart,6);
		this->movingObj   = new MySolidObject(SolidEngine,movingObjStart,3);

		objSpace = SolidEngine->createCollisionSpace(surface, 3);
		SolidEngine->registerSolidObject(objSpace,stoppedObj);
		SolidEngine->registerSolidObject(objSpace,movingObj);
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		// called once per frame
		for (int x = 0; x < ScreenWidth(); x++)
		 	for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::BLANK);	

		if(GetKey(olc::A).bHeld)
		{
			destination.x-=2;
		}
		if(GetKey(olc::D).bHeld)
		{
			destination.x+=2;
		}
		if(GetKey(olc::W).bHeld)
		{
			destination.y-=2;
		}
		if(GetKey(olc::S).bHeld)
		{
			destination.y+=2;
		}

		for(int i = 0; i < 3; i++)
		{
			DrawLine(surface[i],surface[(i+1)%3],olc::GREEN);
		}
		
		DrawCircle(destination,movingObj->getRadius()+1,olc::WHITE);
		DrawCircle(movingObj->updatePosition(destination,(void*)&destination),movingObj->getRadius(),olc::RED);
		DrawCircle(stoppedObj->getPosition(),stoppedObj->getRadius(),olc::WHITE);

		return true;
	}
};


int main()
{
	surface[0] = {128,100};
	surface[1] = {148,200};
	surface[2] = {108,200};
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}
