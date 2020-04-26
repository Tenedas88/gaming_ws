#include "olcPixelGameEngine.h"

olc::vi2d JumpAnimation[20];
uint8_t backgroundLayer = 0;
uint8_t targetLayer = 0;

class MyExampleThing
{
	public:
		MyExampleThing():position(128,120),sprite("./gamesprites/police.png"),decal(&(this->sprite))
		{
			color = olc::RED;
			stopped = false;
			isJumping = false;	
			jumpingStage = 0;	
			layer = 0;
			scale = {0.2f,0.2f};
		}
		MyExampleThing(const char* spriteName,uint8_t inLayer):position((rand()%8)*32,(rand()%8)*30),sprite(spriteName),decal(&(this->sprite))
		{
			color = olc::RED;
			stopped = false;
			isJumping = false;	
			jumpingStage = 0;	
			layer = inLayer;
			scale = {0.7f,0.7f};
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
			
			gameEngineIstance->SetDrawTarget(this->layer);
			gameEngineIstance->SetPixelMode(olc::Pixel::ALPHA);
			gameEngineIstance->DrawDecal(this->position,&(this->decal),this->scale);
			gameEngineIstance->SetPixelMode(olc::Pixel::NORMAL);
		}

	void checkCollision(MyExampleThing* source)
	{
		if((abs(this->position.x - source->GetPosition().x) < 10) && (abs(this->position.y - source->GetPosition().y) < 10))
		{
			this->position.x = (rand()%8)*32;
			this->position.y = (rand()%8)*30;
		}
	}

	private:
		olc::Pixel color;
		olc::Sprite sprite;
		olc::Decal decal;
		olc::vi2d  position;
		unsigned int jumpingStage;
		uint8_t    layer;
		olc::vf2d scale;
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
	MyExampleThing* targets[20] = {NULL};
	olc::Sprite* sprite = NULL;
	olc::Decal* decal = NULL;

	bool OnUserCreate() override
	{

		sprite = new olc::Sprite("./gamesprites/spritemap.png");
		decal = new olc::Decal(sprite);
		// Called once at the start, so create things here
		Clear(olc::BLANK);

		targetLayer = CreateLayer();
		EnableLayer(targetLayer,true);
		SetDrawTarget(targetLayer);
		Clear(olc::BLANK);

		backgroundLayer = CreateLayer();
		EnableLayer(backgroundLayer,true);
		SetDrawTarget(backgroundLayer);
		Clear(olc::BLANK);

		SetDrawTarget((uint8_t)0);
		ExampleThing = new MyExampleThing();
		for(int i = 0; i < 20; i++)
		{
			targets[i] = new MyExampleThing("./gamesprites/sprite3.png",targetLayer);
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		bool userInput = false;

		SetDrawTarget(backgroundLayer);
		DrawDecal({0,0},decal,{0.5f,0.5f});
		// called once per frame
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
//				Draw(x, y, olc::BLANK));
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
				for(int i=0;i<20;i++)
				{
					targets[i]->checkCollision(ExampleThing);
					targets[i]->updateObject((olc::PixelGameEngine*)this);
				}
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
