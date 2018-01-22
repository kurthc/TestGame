#include "Game.h"

game_state::game_state(game_offscreen_buffer* Buffer)
{
	this->CurrentRound = game_round_state();
	this->CurrentRound.GameState = this;		//Not sure why it's not working to set this in the game_round_state constructor.

	this->IsGameOver = false;

	//this->KeysDown = KeysDown;
	
	this->GameMap = new game_map(20, 15);      // Create the game map.


	Buffer->MapRegionInUse.x = Buffer->MapRegionTotal.x;
	Buffer->MapRegionInUse.y = Buffer->MapRegionTotal.y;
	Buffer->MapRegionInUse.Height = Buffer->MapRegionTotal.Height;
	Buffer->MapRegionInUse.Width = (int)((float)Buffer->MapRegionTotal.Height / this->GameMap->Height * this->GameMap->Width);

}

game_round_state::game_round_state(game_state* GameState)
{
	game_round_state::game_round_state();
	this->GameState = GameState;
}

game_round_state::game_round_state()
{
	// Create the snake
	vec2 InitialPosition = { 2, 2 };
	vec2 InitialDirection = { 1, 0 };
	this->Snake = new snake(5, InitialPosition, InitialDirection);

	this->NewPelletTimer = 3;
	
}

game_map::game_map(int Width, int Height)
{
	//game_map *GameMap = new game_map();
	this->Width = Width;
	this->Height = Height;
	this->Bytes = new uint8_t[Width * Height];
	for (int i = 0; i < Width*Height; i++)
	{
		*((uint8_t *)(this->Bytes) + i) = 0;
	}
}




void GameStateProcess(game_state *GameState, keys_down *KeysDown, game_offscreen_buffer *GameBuffer)
{

	GameState->ProcessInput(KeysDown);

	if (GameState->IsGameOver)
	{
		// Something will go here.
	}
	else
	{
		//ProcessInput(GameState, KeysDown);
		ProcessTimers(GameState);   // Currently just adds pellets
		ProcessSnake(GameState, GameState->CurrentRound.Snake);    // Move the snake, check for collisions.
	}
}



// Update the snake's location.
void ProcessSnake(game_state* GameState, snake *Snake)
{
	std::list<snake_segment> *Segments = &(Snake->Segments);

	// The timer counts up to 1. When it gets there, move the snake.
	Snake->Timer += Snake->Speed / 30.0f;   // FPS?
	if (Snake->Timer >= 1)
	{
		vec2 LastLocation = {0, 0};
		for (std::list<snake_segment>::iterator it = Segments->begin(); it != Segments->end(); it++)
		{
			it->Location = it->Location + it->Direction;    //TODO: Throw in a round-to-integer here.
			if (it->IntangibleTimer > 0)
			{
				it->IntangibleTimer -= Snake->Speed / 30.0f;
				if (it->IntangibleTimer < 0.0f)
					it->IntangibleTimer = 0.0f;
			}
			if (it == Segments->begin())
			{
				it->Direction = Snake->Direction;
			}
			else
			{
				it->Direction = LastLocation - it->Location;
			}
			LastLocation = it->Location;
		}
		Snake->Timer = 0;
	}


	snake_segment *SnakeHead = Snake->Head;
	vec2 HeadLocation = SnakeHead->RealLocation();

	if (HeadLocation.X < 0 || HeadLocation.Y < 0 || HeadLocation.X > GameState->GameMap->Width-1 || HeadLocation.Y > GameState->GameMap->Height-1)
	{
		GameState->IsGameOver = true;
	}


	for (std::list<snake_segment>::iterator it = Segments->begin(); it != Segments->end(); it++)
	{
		int c = 0;
		++c;

		//if (c > 1 && it != Segments->begin() && it->IntangibleTimer == 0)
		if (it != Segments->begin() && it->IntangibleTimer == 0)
		{
			//if (DoRectanglesIntersect(SnakeHead->HitRectangle(), it->HitRectangle(), .00))
			//if (DoRectanglesIntersect(SnakeHead->HitRectangle(), it->HitRectangle(), .01))
			if (abs(HeadLocation.X - it->Location.X) < .9 && abs(HeadLocation.Y - it->Location.Y) < .9)
			{
				GameState->IsGameOver = true;
			}
		}
	}
	
	// Check if the snake hit a pellet.
	std::list<pellet>::iterator it = GameState->CurrentRound.Pellets.begin();
	while (it != GameState->CurrentRound.Pellets.end())
	{
		if (DoRectanglesIntersect(it->HitRectangle(), GameState->CurrentRound.Snake->Head->HitRectangle(), .00))
		{
			// Snake head is on a pellet. Clear the pellet. (This works because the parameters are
			// evaluated before the function call.)
			GameState->CurrentRound.Pellets.erase(it++);
			GameState->CurrentRound.Snake->AddSegments(3);
			std::cout << "Ate a pellet!" << std::endl;
		}
		else
		{
			it++;
		}
	}
}


rectangle pellet::HitRectangle()
{
	return {(float)this->Location.X, (float)this->Location.Y, 1.0f, 1.0f};
}

//void ProcessInput(game_state *GameState, keys_down *KeysDown)
void game_state::ProcessInput(keys_down *KeysDown)
{
	if (!this->IsGameOver)
	{
		vec2 NewDirection;
		bool DirectionChanged = false;

		if (KeysDown->Left)
		{
			NewDirection = -UnitVectorX;
			DirectionChanged = true;
		}
		if (KeysDown->Right)
		{
			NewDirection = UnitVectorX;
			DirectionChanged = true;
		}
		if (KeysDown->Up)
		{
			NewDirection = -UnitVectorY;
			DirectionChanged = true;
		}
		if (KeysDown->Down)
		{
			NewDirection = UnitVectorY;
			DirectionChanged = true;
		}
		if (KeysDown->Space)
		{
			bool Dummy = false;
		}

		if (DirectionChanged)
		{
			this->CurrentRound.Snake->SetDirection(NewDirection);
		}
	}
	else
	{
		if (KeysDown->Space)
		{
			//game_round_state NewRound = game_round_state(this);
			this->CurrentRound = game_round_state();
			this->CurrentRound.GameState = this;
			this->IsGameOver = false;
		}

	}
}

void ProcessTimers(game_state *GameState)
{
	GameState->CurrentRound.NewPelletTimer -= 1.0f / 30.0f;
	if (GameState->CurrentRound.NewPelletTimer <= 0)
	{
		GameState->CurrentRound.AddPellet();
		GameState->CurrentRound.NewPelletTimer = 10.0f;
	}
}


void game_round_state::AddPellet()
{
	int x = (rand() % this->GameState->GameMap->Width);
	int y = (rand() % this->GameState->GameMap->Height);
	pellet *Pellet = new pellet();		//TODO: Destroy these when done.
	Pellet->Location.SetXY((float)x, (float)y);
	Pellet->Color = RGB(1.0, 1.0, 1.0);
	this->Pellets.push_back(*Pellet);
}

