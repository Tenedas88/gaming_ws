#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class MyExampleThing
{
	public:
		MyExampleThing():position(128,120)
		{
			color = olc::RED;
			stopped = false;		
		}

		void moveHorizontal(int shift)
		{
			if(!this->stopped)
			{	
				this->position.x += shift;
				if(this->position.x > 276) this->position.x = 128;
			}
		}

		void moveVertical(int shift)
		{
			if(!this->stopped)
			{
				this->position.y += shift;
				if(this->position.y > 260) this->position.y = 120;
			}
		}

		void changeColor(olc::Pixel color)
		{
			this->color = color;
		}

		void stopObject()
		{
			this->stopped = true;
		}

		void startObject()
		{
			this->stopped = false;
		}

		void toggleMovement()
		{
			this->stopped = !this->stopped;
		}


		olc::vi2d& GetPosition()
		{
			return position;
		}

		void updateObject(olc::PixelGameEngine* gameEngineIstance)
		{
			gameEngineIstance->DrawCircle(this->position.x,this->position.y,10,this->color);
		}

	private:
		olc::Pixel color;
		olc::vi2d  position;
		bool    stopped;
};

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
        this->frameCounter = 0;
	}

public:
    int frameCounter;
	MyExampleThing* ExampleThing = NULL;

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		ExampleThing = new MyExampleThing();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(255,255,255));
				if(GetMouse(0).bPressed)
				{
					if((GetMouseX() < (ExampleThing->GetPosition().x + 10)) && (GetMouseX() > (ExampleThing->GetPosition().x -10)))
					{
						if((GetMouseY() < (ExampleThing->GetPosition().y + 10)) && (GetMouseY() > (ExampleThing->GetPosition().y -10)))
						{
							ExampleThing->toggleMovement();
						}
					}
				}
				else
				{
					if(GetMouse(0).bHeld)
					{
						ExampleThing->moveHorizontal(GetMouseX()-ExampleThing->GetPosition().x);
						ExampleThing->moveVertical(GetMouseY()-ExampleThing->GetPosition().y);
					}
					else
					{
                		(this->frameCounter++%4 == 0) ? ExampleThing->moveHorizontal(1):ExampleThing->moveVertical(1);
						(this->frameCounter++%512 > 256) ? ExampleThing->changeColor(olc::BLUE):ExampleThing->changeColor(olc::RED);
					}
				}
				ExampleThing->updateObject((olc::PixelGameEngine*)this);
		return true;
	}
};

Example demo;

int main()
{
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}
