#include "Game.h"

game_state::game_state(game_offscreen_buffer* Buffer)
{
	this->CurrentRound = game_round_state();

	this->IsGameOver = false;

	// Create the game map.
	//GameState->GameMap = new game_map(30, 20);
	this->GameMap = new game_map(20, 15);

	// Create the snake
	vec2 InitialPosition = {2, 2};
	vec2 InitialDirection = {1, 0};
	this->Snake = new snake(5, InitialPosition, InitialDirection);

	Buffer->MapRegionInUse.x = Buffer->MapRegionTotal.x;
	Buffer->MapRegionInUse.y = Buffer->MapRegionTotal.y;
	Buffer->MapRegionInUse.Height = Buffer->MapRegionTotal.Height;
	Buffer->MapRegionInUse.Width = (float)Buffer->MapRegionTotal.Height / this->GameMap->Height * this->GameMap->Width;

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

	if (GameState->IsGameOver)
	{

	}
	else
	{
		ProcessInput(GameState, KeysDown);
		ProcessTimers(GameState);   // Currently just adds pellets
		ProcessSnake(GameState, GameState->Snake);    // Move the snake, check for collisions.
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


	//snake_segment *SnakeHead = &(GameState->Snake->Segments.front());
	snake_segment *SnakeHead = GameState->Snake->Head;
	vec2 HeadLocation = SnakeHead->RealLocation();

	if (HeadLocation.X < 0 || HeadLocation.Y < 0 || HeadLocation.X > GameState->GameMap->Width-1 || HeadLocation.Y > GameState->GameMap->Height-1)
	{
		GameState->IsGameOver = true;
	}


	//for (std::list<snake_segment>::iterator it = std::next(Segments->begin(), 2); it != Segments->end(); it++)
	for (std::list<snake_segment>::iterator it = Segments->begin(); it != Segments->end(); it++)
	{
		if (it != Segments->begin() && it->IntangibleTimer == 0)
		{
			if (abs(HeadLocation.X - it->Location.X) < .9 && abs(HeadLocation.Y - it->Location.Y) < .9)
			{
				GameState->IsGameOver = true;
			}
		}
	}
	
	// Check if the snake hit a pellet.
	std::list<pellet>::iterator it = GameState->Pellets.begin();
	while (it != GameState->Pellets.end())
	{
		//if (it->Location.X == SnakeHead->Location.X && it->Location.Y == SnakeHead->Location.Y)
		if (abs(it->Location.X - SnakeHead->Location.X) < .1 && abs(it->Location.Y - SnakeHead->Location.Y) < .1)
		{
			// Snake head is on a pellet. Clear the pellet. (This works because the parameters are
			// evaluated before the function call.)
			GameState->Pellets.erase(it++);
			GameState->Snake->AddSegments(3);
			std::cout << "Ate a pellet!" << std::endl;
			//GameState->IsGameOver = true;
		}
		else
		{
			it++;
		}
	}



}





void ProcessInput(game_state *GameState, keys_down *KeysDown)
{
	vec2 NewDirection;
	bool DirectionChanged = false;

	if (KeysDown->Left)
	{
		NewDirection = -UnitVectorX;
		DirectionChanged = true;
		//GameState->Snake->SetDirection(-UnitVectorX);
	}
	if (KeysDown->Right)
	{
		NewDirection = UnitVectorX;
		DirectionChanged = true;
		//GameState->Snake->SetDirection(UnitVectorX);
	}
	if (KeysDown->Up)
	{
		NewDirection = -UnitVectorY;
		DirectionChanged = true;
		//GameState->Snake->SetDirection(-UnitVectorY);
	}
	if (KeysDown->Down)
	{
		NewDirection = UnitVectorY;
		DirectionChanged = true;
		//GameState->Snake->SetDirection(UnitVectorY);
	}
	if (KeysDown->Space)
	{
		bool Dummy = false;
		//GameState->Snake.AddSegments(1);
	}

 	if (DirectionChanged)
	{
		GameState->Snake->SetDirection(NewDirection);
	}
}

void ProcessTimers(game_state *GameState)
{
	GameState->NewPelletTimer -= 1.0f / 30.0f;
	if (GameState->NewPelletTimer <= 0)
	{
		AddPellet(GameState);
		GameState->NewPelletTimer = 10.0f;
	}
}


//Map Coordinates go from:      0 to GameState->GameMap->Width
//                    and:      0 to GameState->GameMap->Height
//Display Coordinates go from:  Buffer->MapRegionInUse.x to Buffer->MapRegionInUse.x + Buffer->MapRegionInUse.Width
//                        and:  Buffer->MapRegionInUse.y to Buffer->MapRegionInUse.y + Buffer->MapRegionInUse.Height


vec2 MapToDisplayCoordinates(float x, float y, game_state *GameState, game_offscreen_buffer *Buffer)
{
	float NewX = x / GameState->GameMap->Width * Buffer->MapRegionInUse.Width + Buffer->MapRegionInUse.x;
	float NewY = y / GameState->GameMap->Height * Buffer->MapRegionInUse.Height + Buffer->MapRegionInUse.y;
	return vec2(NewX, NewY);
}

//vec2 DisplayToMapCoordinates(float x, float y, game_state *GameState, game_offscreen_buffer *Buffer)
//{
//	//TODO: Write if needed.
//}

//rectangle MapToDisplayRectangle(rectangle r, game_state *GameState, game_offscreen_buffer *Buffer)
//{
//	
//}

rectangle MapToDisplayRectangle(float x, float y, float Width, float Height, game_state *GameState, game_offscreen_buffer *Buffer)
{
	float Right = x + Width;
	float Bottom = y + Height;
	vec2 NewV1 = MapToDisplayCoordinates(x, y, GameState, Buffer);
	vec2 NewV2 = MapToDisplayCoordinates(Right, Bottom, GameState, Buffer);
	rectangle output = { NewV1.X, NewV1.Y, NewV2.X-NewV1.X, NewV2.Y - NewV1.Y };
	return output;
}

void DrawMap(game_state *GameState, game_offscreen_buffer *Buffer)
{
	game_map *GameMap = GameState->GameMap;
	for (int y = 0; y < GameMap->Height; y++)
	{
		for (int x = 0; x < GameMap->Width; x++)
		{
			vec2 UpperLeftCorner = MapToDisplayCoordinates(x, y, GameState, Buffer);
			Buffer->DrawRectangle(UpperLeftCorner.X, UpperLeftCorner.Y, 2.0, 2.0, RGB(.5, .5, .5));
		}
	}
}

void DrawBorder(game_state *GameState, game_offscreen_buffer *Buffer)
{
	//rectangle Inner = GameState->GameboardDisplayRegion;
	intrectangle Inner = Buffer->MapRegionInUse;
	intrectangle Outer = {};
	int BorderWidth = Buffer->MapBorderThickness;
	int Color = Buffer->MapBorderColor;
	Outer.x = Inner.x - BorderWidth;
	Outer.y = Inner.y - BorderWidth;
	Outer.Width = Inner.Width + 2 * BorderWidth;
	Outer.Height = Inner.Height + 2 * BorderWidth;

	Buffer->DrawRectangle(Outer, Color);
	Buffer->DrawRectangle(Inner, RGB(0.0, 0.0, 0.0));
}

void DrawSnake(game_state *GameState, game_offscreen_buffer *Buffer)
{
	snake *Snake = GameState->Snake;
	game_map *GameMap = GameState->GameMap;
	std::list<snake_segment> Segments = Snake->Segments;

	// Iterate through the segmeents in reverse order
	for (std::list<snake_segment>::reverse_iterator it = Segments.rbegin(); it != Segments.rend(); it++)
	{
		vec2 DrawLocation = it->Location + it->Direction * Snake->Timer;
		
		rectangle Rec = MapToDisplayRectangle(DrawLocation.X, DrawLocation.Y, 1, 1, GameState, Buffer);
		Buffer->DrawRectangle(Rec.x, Rec.y, Rec.Width, Rec.Height, it->Color);
	}

}

void RenderBuffer(game_state *GameState, game_offscreen_buffer *Buffer)
{
	Buffer->ClearBuffer();
	//ClearBuffer(Buffer);
	DrawBorder(GameState, Buffer);
	DrawMap(GameState, Buffer);

	// Draw the pellets.
	for (std::list<pellet>::iterator it = GameState->Pellets.begin(); it != GameState->Pellets.end(); it++)
	{
		rectangle Rec = MapToDisplayRectangle(it->Location.X, it->Location.Y, 1.0, 1.0, GameState, Buffer);
		Buffer->DrawRectangle(Rec.x, Rec.y, Rec.Width, Rec.Height, it->Color);
	}

	DrawSnake(GameState, Buffer);
}


void AddPellet(game_state *GameState)
{
	int x = (rand() % GameState->GameMap->Width);
	int y = (rand() % GameState->GameMap->Height);
	pellet *Pellet = new pellet();
	Pellet->Location.SetXY(x, y);
	Pellet->Color = RGB(1.0, 1.0, 1.0);
	GameState->Pellets.push_back(*Pellet);
}

