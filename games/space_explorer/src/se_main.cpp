#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

olc::vi2d JumpAnimation[20];

class MyExampleThing
{
	public:
		MyExampleThing():position(128,120),sprite("./gamesprites/sprite.png")
		{
			color = olc::RED;
			stopped = false;
			isJumping = false;	
			jumpingStage = 0;	
		}

		void moveHorizontal(int shift)
		{
			if(!this->stopped && !this->isJumping)
			{	
				this->position.x += shift;
				if(this->position.x > 276) this->position.x = 0;
				if(this->position.x < 0) this->position.x = 276;
			}
		}

		void moveVertical(int shift)
		{
			if(!this->stopped && !this->isJumping)
			{
				this->position.y += shift;
				if(this->position.y > 260) this->position.y = 0;
				if(this->position.y < 0) this->position.y = 260;
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

		void Jump(bool leftPressed, bool rightPressed)
		{
			this->isJumping = true;
			this->isLeftJump = leftPressed;
			this->isRightJump = rightPressed;
		}


		olc::vi2d& GetPosition()
		{
			return position;
		}

		void updateObject(olc::PixelGameEngine* gameEngineIstance)
		{
			if(!this->isJumping)
			{
				
			}
			else
			{
				this->position.y += JumpAnimation[this->jumpingStage].y; 
				if(isRightJump)
				{
					this->position.x += JumpAnimation[this->jumpingStage].x; 
				}
				if(isLeftJump)
				{
					this->position.x -= JumpAnimation[this->jumpingStage].x; 
				}
				this->jumpingStage++;
				
				if(jumpingStage == 20)
				{
					this->jumpingStage = 0;
					this->isJumping = false;
				}
			}

			gameEngineIstance->DrawSprite(this->position,&(this->sprite));
		}

	private:
		olc::Pixel color;
		olc::Sprite sprite;
		olc::vi2d  position;
		unsigned int jumpingStage;
		bool       isJumping;
		bool 	   isRightJump;
		bool       isLeftJump;
		bool       stopped;
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
	olc::Sprite* sprite;

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		ExampleThing = new MyExampleThing();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		bool userInput = false;
		// called once per frame
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(255,255,255));
				if(GetKey(olc::A).bHeld)
				{
					ExampleThing->moveHorizontal(-2);
					userInput = true;
				}
				if(GetKey(olc::D).bHeld)
				{
					ExampleThing->moveHorizontal(2);
					userInput = true;
				}
				if(GetKey(olc::W).bHeld)
				{
					ExampleThing->moveVertical(-2);
					userInput = true;
				}
				if(GetKey(olc::S).bHeld)
				{
					ExampleThing->moveVertical(2);
					userInput = true;
				}
				if(GetKey(olc::SPACE).bPressed)
				{
					ExampleThing->Jump(GetKey(olc::A).bHeld, GetKey(olc::D).bHeld);
					userInput = true;
				}
				if(GetMouse(0).bPressed)
				{
					if((GetMouseX() < (ExampleThing->GetPosition().x + 10)) && (GetMouseX() > (ExampleThing->GetPosition().x -10)))
					{
						if((GetMouseY() < (ExampleThing->GetPosition().y + 10)) && (GetMouseY() > (ExampleThing->GetPosition().y -10)))
						{
							ExampleThing->toggleMovement();
							userInput = true;
						}
					}
				}
				else
				{
					if(GetMouse(0).bHeld)
					{
						ExampleThing->moveHorizontal(GetMouseX()-ExampleThing->GetPosition().x);
						ExampleThing->moveVertical(GetMouseY()-ExampleThing->GetPosition().y);
						userInput = true;
					}
				}
				if(!userInput)
				{
                	//(this->frameCounter++%4 == 0) ? ExampleThing->moveHorizontal(1):ExampleThing->moveVertical(1);
					(this->frameCounter++%512 > 256) ? ExampleThing->changeColor(olc::BLUE):ExampleThing->changeColor(olc::RED);
				}
				ExampleThing->updateObject((olc::PixelGameEngine*)this);
		return true;
	}
};

Example demo;

int main()
{
	//init jumping animation
	for(int i= 0; i < 10; i++)
	{
		JumpAnimation[i].x = 3;
		JumpAnimation[i].y = -3;
	}
	for(int i= 10; i < 20; i++)
	{
		JumpAnimation[i].x = 3;
		JumpAnimation[i].y = 3;
	}
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}
