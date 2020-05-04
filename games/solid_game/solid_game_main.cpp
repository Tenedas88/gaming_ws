#include "olcPixelGameEngine.h"
#include "solidObjEngine.h"

olc::vi2d 			      surface[3];//TODO: add support for surface shape calculation
CollisionSurfaceCorners_t customShape;

class MySolidObject : public SolidRoundObject
{
	public:
		MySolidObject(SolidObjGameEngine* solidEngine,olc::vi2d& onCreatePosition,unsigned int onCreateRadius):
			SolidRoundObject(solidEngine,onCreatePosition,onCreateRadius),
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

	//obstacles
	olc::vi2d 			   stoppedObjStart;
	olc::vi2d 			   stoppedObjLast;
	SolidRoundObject*      stoppedObj  = NULL;

	olc::vi2d 			   stoppedTriangleObjStart;
	SolidTriangleObject*   stoppedTriangleObj  = NULL;

	olc::vi2d 			   stoppedSquareObjStart;
	SolidSquareObject*     stoppedSquareObj  = NULL;

	olc::vi2d 			   stoppedPolygonObjStart;
	SolidPolygonObject*    stoppedPolygonObj = NULL;

	olc::vi2d 			   stoppedLineObjStart;
	olc::vi2d 			   stoppedLineObjDirection;
	SolidLineObject*       stoppedLineObj = NULL;

	//enemy
	olc::vi2d			   movingObjStart;
	olc::vi2d			   movingObjLast;
	MySolidObject*         movingObj   = NULL;

	//cursor
	olc::vi2d 			   destination;

	//collision space
	CollisionSpaceHandle_t objSpace    = INVALID_COLLISION_HANDLE;

	Example():
		stoppedObjStart(123,128),
		stoppedTriangleObjStart(115,180),
		stoppedSquareObjStart(140,180),
		stoppedPolygonObjStart(128,160),
		stoppedLineObjStart(118,190),
		stoppedLineObjDirection(1,-1),
		movingObjStart(128,118),
		destination(120,240)
	{
		this->SolidEngine 		  = new SolidObjGameEngine();

		this->stoppedObj  		  = new SolidRoundObject   (SolidEngine,stoppedObjStart,6);
		this->stoppedTriangleObj  = new SolidTriangleObject(SolidEngine,stoppedTriangleObjStart,6);
		this->stoppedSquareObj    = new SolidSquareObject  (SolidEngine,stoppedSquareObjStart,6);
		this->stoppedPolygonObj   = new SolidPolygonObject (SolidEngine,stoppedPolygonObjStart,6,customShape);
		this->stoppedLineObj      = new SolidLineObject    (SolidEngine,stoppedLineObjStart,stoppedLineObjDirection,6);

		this->movingObj   		  = new MySolidObject      (SolidEngine,movingObjStart,3);

		objSpace = SolidEngine->createCollisionSpace(surface, 3);
		SolidEngine->registerSolidObject(objSpace,stoppedObj);
		SolidEngine->registerSolidObject(objSpace,stoppedTriangleObj);
		SolidEngine->registerSolidObject(objSpace,stoppedSquareObj);
		SolidEngine->registerSolidObject(objSpace,stoppedPolygonObj);
		SolidEngine->registerSolidObject(objSpace,stoppedLineObj);
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
		
		//draw obstacles
		DrawRect(stoppedSquareObjStart-olc::vi2d(6,6),olc::vi2d(12,12),olc::WHITE);
		DrawCircle(stoppedObj->getPosition(),stoppedObj->getRadius(),olc::WHITE);
		DrawTriangle(stoppedTriangleObjStart+olc::vi2d(0,-6),
					 stoppedTriangleObjStart+olc::vi2d((6/2)*2,6/3),
					 stoppedTriangleObjStart+olc::vi2d(-(6/2)*2,6/3),
					 olc::WHITE);

		for(int i = 0; i < 5; i++)
		{
			DrawLine(customShape[i]+stoppedPolygonObjStart,customShape[(i+1)%5]+stoppedPolygonObjStart,olc::WHITE);
		}

		DrawLine(stoppedLineObjStart+(stoppedLineObjDirection*(6/2)),stoppedLineObjStart-(stoppedLineObjDirection*(6/2)),olc::WHITE);

		//draw cursor
		DrawCircle(destination,movingObj->getRadius()+1,olc::CYAN);

		//draw enemy
		DrawCircle(movingObj->updatePosition(destination,(void*)&destination),movingObj->getRadius(),olc::RED);

		return true;
	}
};


int main()
{
	surface[0] = {128,100};
	surface[1] = {148,200};
	surface[2] = {108,200};

	customShape.push_back(olc::vi2d(-5,-5));
	customShape.push_back(olc::vi2d(-5,5));
	customShape.push_back(olc::vi2d(5,5));
	customShape.push_back(olc::vi2d(7,7));
	customShape.push_back(olc::vi2d(-6,-5));

	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}
