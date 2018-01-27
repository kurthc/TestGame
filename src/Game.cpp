#include "Game.h"

game_state::game_state(game_offscreen_buffer* Buffer)
{
	this->CurrentRound = game_round_state();
	this->CurrentRound.GameState = this;		//Not sure why it's not working to set this in the game_round_state constructor.

	this->IsGameOver = false;

	this->GameMap = new game_map(40, 30);      // Create the game map.

	Buffer->MapRegionInUse.x = Buffer->MapRegionTotal.x;
	Buffer->MapRegionInUse.y = Buffer->MapRegionTotal.y;

	// Scale the GameMap dimensions by the largest factor that will allow it to fit in MapRegionTotal.
	float MaxWidthScalingFactor = Buffer->MapRegionTotal.Width / this->GameMap->Width;
	float MaxHeightScalingFactor = Buffer->MapRegionTotal.Height / this->GameMap->Height;
	float ScalingFactor = MIN(MaxWidthScalingFactor, MaxHeightScalingFactor);
	Buffer->MapRegionInUse.Width = ScalingFactor * this->GameMap->Width;
	Buffer->MapRegionInUse.Height = ScalingFactor * this->GameMap->Height;

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
	this->Score = 0;
	
}

game_map::game_map(int Width, int Height)
{
	this->Width = Width;
	this->Height = Height;
	this->Bytes = new uint8_t[Width * Height];
	for (int i = 0; i < Width*Height; i++)
	{
		*((uint8_t *)(this->Bytes) + i) = 0;
	}
}

//void GameStateProcess(game_state *GameState, keys_down *KeysDown, game_offscreen_buffer *GameBuffer)
void game_state::GameStateProcess(keys_down *KeysDown, game_offscreen_buffer *GameBuffer)
{

	this->ProcessInput(KeysDown);

	if (this->IsGameOver)
	{
		// Something will go here.
	}
	else
	{
		this->ProcessTimers(); 
		this->ProcessSnake(this->CurrentRound.Snake);
	}
}


// Update the snake's location.
void game_state::ProcessSnake(snake *Snake)
{
	//std::list<snake_segment> *Segments = &(Snake->Segments);
	std::list<snake_segment>& Segments = Snake->Segments;

	// The timer counts up to 1. When it gets there, move the snake.
	Snake->Timer += Snake->Speed / 30.0f;   // FPS?
	if (Snake->Timer >= 1)
	{
		vec2 LastLocation = {0, 0};
		for (std::list<snake_segment>::iterator it = Segments.begin(); it != Segments.end(); ++it)
		{
			// Update the location and direction of each segment.
			// Set the new direction from the
			it->Location += it->Direction;    //TODO: Throw in a round-to-integer here.
			if (it == Segments.begin())
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


	//snake_segment *SnakeHead = Snake->Head;
	snake_segment& SnakeHead = Snake->Segments.front();
	vec2 HeadLocation = SnakeHead.RealLocation();

	// Check if the snake ran into the game border.
	if (HeadLocation.X < 0 || HeadLocation.Y < 0 || HeadLocation.X > this->GameMap->Width-1 || HeadLocation.Y > this->GameMap->Height-1)
	{
		this->IsGameOver = true;
	}

	// Check if the snake ran into one of its segments.
	for (std::list<snake_segment>::iterator it = Segments.begin(); it != Segments.end(); ++it)
	{
		if (it != Segments.begin() && it->IntangibleTimer == 0)
		{
			//if (DoRectanglesIntersect(SnakeHead->HitRectangle(), it->HitRectangle(), .00))
			//if (DoRectanglesIntersect(SnakeHead->HitRectangle(), it->HitRectangle(), .01))
			if (abs(HeadLocation.X - it->Location.X) < .99 && abs(HeadLocation.Y - it->Location.Y) < .99)
			{
				this->IsGameOver = true;
			}
		}
	}
	
	// Check if the snake hit a pellet.
	game_round_state& CurrentRound = this->CurrentRound;
	std::list<pellet>::iterator PelletIt = CurrentRound.Pellets.begin();
	while (PelletIt != this->CurrentRound.Pellets.end())
	{
		if (DoRectanglesIntersect(PelletIt->HitRectangle(), SnakeHead.HitRectangle(), .00))
		{
			// Snake head is on a pellet. Clear the pellet. (This works because the parameters are
			// evaluated before the function call.)
			CurrentRound.Pellets.erase(PelletIt++);
			Snake->AddSegments(SEGMENTS_TO_ADD_ON_EAT_PELLET);
			std::cout << "Ate a pellet!" << std::endl;
			CurrentRound.Score += 1;
			Snake->Speed += .15;
			//Snake->Speed *= 1.1;
		}
		else
		{
			PelletIt++;
		}
	}
}


rectangle pellet::HitRectangle()
{
	return {this->Location.X, this->Location.Y, 1.0f, 1.0f};
}

void game_state::ProcessInput(keys_down *KeysDown)
{
	if (!this->IsGameOver)
	{
		vec2 NewDirection;
		bool DirectionChanged = false;

		snake& Snake = *(this->CurrentRound.Snake);

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

		// Don't allow the direction to change if you're pressing the same direction the snake is already going
		// or in the opposite direction.
		if (DirectionChanged && (abs(Snake.Direction * NewDirection) <= 0.0005f))
		{
			// TODO: Bug: While going up, press right and the down quickly and you can do a 180.

			this->CurrentRound.Snake->SetDirection(NewDirection);
		}



	}
	else  // If in game over state
	{
		if (KeysDown->Space)
		{
			this->CurrentRound = game_round_state();
			this->CurrentRound.GameState = this;
			this->IsGameOver = false;
		}

	}
}

void game_state::ProcessTimers()
{
	this->CurrentRound.NewPelletTimer -= 1.0f / 30.0f;
	if (this->CurrentRound.NewPelletTimer <= 0)
	{
		this->CurrentRound.AddPellet();
		this->CurrentRound.NewPelletTimer = 10.0f;
	}

	snake* Snake = this->CurrentRound.Snake;
	std::list<snake_segment>& Segments = this->CurrentRound.Snake->Segments;
	for (std::list<snake_segment>::iterator it = Segments.begin(); it != Segments.end(); ++it)
	{
		if (it->IntangibleTimer > 0)     // The segment doesn't get hit-checked if it is intangible.
		{
			it->IntangibleTimer -= Snake->Speed / 30.0f;
			if (it->IntangibleTimer < 0.0f)
				it->IntangibleTimer = 0.0f;
		}
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

