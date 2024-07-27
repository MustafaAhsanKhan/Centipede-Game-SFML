#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;

// Speeds of various objects

const int player_speed = 400;
const int centipede_speed = 3;
const int bullet_speed = 800;


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void end_winning_screen(RenderWindow &window, int &score)  // Displaying winning screen
{
	sleep(seconds(2));

	window.clear();

	Texture backgroundTexture;
	Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/winning_screen.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(Color(255, 255, 255, 255 * 0.7)); // Reduces Opacity to 70%

	Font font;
	font.loadFromFile("fonts/KarmaFuture.ttf");
	Text winning_text;
	winning_text.setFont(font);
	winning_text.setCharacterSize(50);
	winning_text.setPosition(390,420);
	winning_text.setString("YOU WIN");
	winning_text.setFillColor(Color::White);

	Text scoreT;
	scoreT.setFont(font);
	scoreT.setCharacterSize(35);
	scoreT.setPosition(390,480);
	scoreT.setString("Score = " + to_string(score));
	scoreT.setFillColor(Color::White);


	window.draw(backgroundSprite);
	window.draw(scoreT);
	window.draw(winning_text);
	window.display();

	sleep(seconds(8));

	window.close();
}

void end_losing_screen(RenderWindow &window, int &score)  // Displaying losing screen
{
	sleep(seconds(2));

	window.clear();

	Texture backgroundTexture;
	Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/winning_screen.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(Color(255, 255, 255, 255 * 0.7)); // Reduces Opacity to 70%

	Font font;
	font.loadFromFile("fonts/KarmaFuture.ttf");
	Text losing_text;
	losing_text.setFont(font);
	losing_text.setCharacterSize(50);
	losing_text.setPosition(380,420);
	losing_text.setString("YOU LOSE");
	losing_text.setFillColor(Color::White);

	Text scoreT;
	scoreT.setFont(font);
	scoreT.setCharacterSize(35);
	scoreT.setPosition(380,480);
	scoreT.setString("Score = " + to_string(score));
	scoreT.setFillColor(Color::White);


	window.draw(backgroundSprite);
	window.draw(scoreT);
	window.draw(losing_text);
	window.display();

	sleep(seconds(8));

	window.close();
}

void check_game_end(RenderWindow &window, int &score, float Centipede[12][5][12], float mini_Centipede[20][5])  // Checking if all segments have been destroyed
{
	for(int i = 0; i < 12; ++i)  // Main Centipede
	{
		if(Centipede[i][exists][0] == true)
		{
			return;
		}
	}

	for(int j = 0; j < 20; ++j)  // Mini head centipedes
	{
		if(mini_Centipede[j][exists] == true)
		{
			return;
		}
	}

	end_winning_screen(window, score);
}

void drawPlayer(RenderWindow& window, float player[], Sprite& playerSprite)  // Displaying the player
{
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}

void drawMushroom(RenderWindow &window, int gameGrid[30][30], Sprite &MushroomSprite, Sprite &half_MushroomSprite, Sprite PoisonMushroomSprite, Sprite half_PoisonMushroomSprite)  // Displaying Mushrooms respective of their type
{
	for(int i = 0 ; i < 30; ++i)
	{
		for(int j = 0; j < 30; ++j)
		{
			if(gameGrid[i][j] == 5)  // If it is a mushroom(full health)
			{
				MushroomSprite.setPosition(i * 32, j * 32);
				window.draw(MushroomSprite);
			}
			else if(gameGrid[i][j] == 4)  // If it is a mushroom(half health)
			{
				half_MushroomSprite.setPosition(i * 32, j * 32);
				window.draw(half_MushroomSprite);
			}
			else if(gameGrid[i][j] == 3)  // If it is a poisinous mushroom(full health)
			{
				PoisonMushroomSprite.setPosition(i * 32, j * 32);
				window.draw(PoisonMushroomSprite);
			}
			else if(gameGrid[i][j] == 2)  // If it is a poisinous mushroom(half health)
			{
				half_PoisonMushroomSprite.setPosition(i * 32, j * 32);
				window.draw(half_PoisonMushroomSprite);
			}
		}
	}
}

void updateMushroom(int gameGrid[30][30], int bulletGridX, int bulletGridY, int &score)  // Updating a mushrooms state when it is hit by a bullet
{
	if(gameGrid[bulletGridX][bulletGridY] == 5)
	{
		gameGrid[bulletGridX][bulletGridY] = 4; // Mushroom taking a hit making it half health
	}
	else if(gameGrid[bulletGridX][bulletGridY] == 4)
	{
		gameGrid[bulletGridX][bulletGridY] = 0; // Removing the mushroom
		score = score + 1;
	}

	if((gameGrid)[bulletGridX][bulletGridY] == 3)
	{
		gameGrid[bulletGridX][bulletGridY] = 2; // Poisinous Mushroom taking a hit making it half health
	}
	else if((gameGrid)[bulletGridX][bulletGridY] == 2)
	{
		gameGrid[bulletGridX][bulletGridY] = 0; // Removing the poisinous mushroom
		score = score + 1;
	}
}

void drawCentipede(RenderWindow &window, float Centipede[12][5][12], float mini_Centipede[12][5], Sprite &centipedeSprite, Sprite &centipedeHeadSprite)  // Displaying the centipede and its segments  // Displaying the mini head centipedes
{
	for(int i = 0; i < 12; ++i)
	{
		if(Centipede[i][exists][0] == true)  // The centipede exists
		{
			for(int j = 0; j < 12; ++j)
			{
				if(Centipede[i][exists][j] == true)  // That segment of the centipede exists
				{
					if(j == 0)
					{
						centipedeHeadSprite.setPosition(Centipede[i][x][0], Centipede[i][y][0]);
						window.draw(centipedeHeadSprite);
						continue;
					}
					centipedeSprite.setPosition(Centipede[i][x][j], Centipede[i][y][j]);
					window.draw(centipedeSprite);
				}
			}
		}
	}

	for(int i = 0; i < 20; ++i)
	{
		if(mini_Centipede[i][exists] == true)  // The head exists
		{
			centipedeHeadSprite.setPosition(mini_Centipede[i][x], mini_Centipede[i][y]);
			window.draw(centipedeHeadSprite);
		}
	}
}

void moveCentipede(RenderWindow &window, float Centipede[12][5][12], float player[2], int gameGrid[30][30], float deltaTime, int &score)  // Movement of the centipede segments
{
	int centipedeGridX;
	int centipedeGridY;
	const int move_left = 3;
	const int move_down = 4;

	int playerGridX;
	int playerGridY;

	playerGridX = (player[x] + 16) / 32;
	playerGridY = (player[y] + 16) / 32;

	for(int i = 0; i < 12; ++i)  // Centipede number
	{
		if(Centipede[i][exists][0] == true)  // Head is intact  // This will tell us if that centipede exists
		{
			for(int j = 0 ; j < 12; ++j)  // Segment number
			{
				if(Centipede[i][exists][j] == true)
				{
					centipedeGridX = (Centipede[i][x][j] + 16) / 32;
					centipedeGridY = (Centipede[i][y][j] + 16) / 32;

					if(playerGridX == centipedeGridX && playerGridY == centipedeGridY)
					{
						end_losing_screen(window, score);
						return;
					}

					if(Centipede[i][move_left][j] == 1 && Centipede[i][move_down][j] == 1)
					{
						Centipede[i][x][j] = Centipede[i][x][j] - (centipede_speed);
						if(Centipede[i][x][j] < 0)  // Reached left side of the screen
						{
							Centipede[i][x][j] = 0;
							Centipede[i][y][j] = Centipede[i][y][j] + 32;
							Centipede[i][move_left][j] = 0;
						}
						if(Centipede[i][y][j] >= 900)  // Reached bottom of the screen
						{
							Centipede[i][move_down][j] = 0;
						}
					}
					else if(Centipede[i][move_left][j] == 0 && Centipede[i][move_down][j] == 1)
					{
						Centipede[i][x][j] = Centipede[i][x][j] + (centipede_speed);
						if(Centipede[i][x][j] > 930)  // Reached right side of the screen
						{
							Centipede[i][x][j] = 930;
							Centipede[i][y][j] = Centipede[i][y][j] + 32;
							Centipede[i][move_left][j] = 1;
						}
						if(Centipede[i][y][j] >= 900)  // Reached bottom of the screen
						{
							Centipede[i][move_down][j] = 0;
						}
					}
					else if(Centipede[i][move_left][j] == 1 && Centipede[i][move_down][j] == 0)
					{
						Centipede[i][x][j] = Centipede[i][x][j] - (centipede_speed);
						if(Centipede[i][x][j] < 0)  // Reached the left side of the screen
						{
							Centipede[i][x][j] = 0;
							Centipede[i][y][j] = Centipede[i][y][j] - 32;
							Centipede[i][move_left][j] = 0;
						}
						if(Centipede[i][y][j] <= 800)  // Reached the 5th last row of the screen
						{
							Centipede[i][move_down][j] = 1;
						}
					}
					else if(Centipede[i][move_left][j] == 0 && Centipede[i][move_down][j] == 0)
					{
						Centipede[i][x][j] = Centipede[i][x][j] + (centipede_speed);
						if(Centipede[i][x][j] > 930)  // Reached the right side of the screen
						{
							Centipede[i][x][j] = 930;
							Centipede[i][y][j] = Centipede[i][y][j] - 32;
							Centipede[i][move_left][j] = 1;
						}
						if(Centipede[i][y][j] <= 800)  // Reached the 5th last row of the screen
						{
							Centipede[i][move_down][j] = 1;
						}
					}
					if(gameGrid[centipedeGridX][centipedeGridY] == 5 || gameGrid[centipedeGridX][centipedeGridY] == 4 || gameGrid[centipedeGridX][centipedeGridY] == 3 || gameGrid[centipedeGridX][centipedeGridY] == 2)  // Collision with a mushroom
					{
						if(Centipede[i][move_left][j] == 1 && Centipede[i][move_down][j] == 1)
						{
							Centipede[i][y][j] = Centipede[i][y][j] + 32;
							//gameGrid[centipedeGridX][centipedeGridY] = 0;
							Centipede[i][move_left][j] = 0;
						}
						else if(Centipede[i][move_left][j] == 0 && Centipede[i][move_down][j] == 1)
						{
							Centipede[i][y][j] = Centipede[i][y][j] + 32;
							//gameGrid[centipedeGridX][centipedeGridY] = 0;
							Centipede[i][move_left][j] = 1;
						}
						else if(Centipede[i][move_left][j] == 1 && Centipede[i][move_down][j] == 0)
						{
							Centipede[i][y][j] = Centipede[i][y][j] - 32;
							//gameGrid[centipedeGridX][centipedeGridY] = 0;
							Centipede[i][move_left][j] = 0;
						}
						else if(Centipede[i][move_left][j] == 0 && Centipede[i][move_down][j] == 0)
						{
							Centipede[i][y][j] = Centipede[i][y][j] - 32;
							//gameGrid[centipedeGridX][centipedeGridY] = 0;
							Centipede[i][move_left][j] = 1;
						}
					}
				}
			}
		}
	}
}

void moveMiniCentipede(RenderWindow &window, float mini_Centipede[20][5], float player[2], int &score)  // Movement of the mini head centipedes
{
	int centipedeGridX;
	int centipedeGridY;
	const int move_left = 3;
	const int move_down = 4;

	int playerGridX;
	int playerGridY;

	playerGridX = (player[x] + 16) / 32;
	playerGridY = (player[y] + 16) / 32;

	for(int i = 0; i < 20; ++i)  // Head number
	{
		if(mini_Centipede[i][exists] == true)  // This will tell us if that centipede exists
		{
			centipedeGridX = (mini_Centipede[i][x] + 16) / 32;
			centipedeGridY = (mini_Centipede[i][y] + 16) / 32;

			if(playerGridX == centipedeGridX && playerGridY == centipedeGridY)
			{
				end_losing_screen(window, score);
				return;
			}

			if(mini_Centipede[i][move_left] == 1 && mini_Centipede[i][move_down] == 1)
			{
				mini_Centipede[i][x] = mini_Centipede[i][x] - (centipede_speed);
				if(mini_Centipede[i][x] < 0)  // Reached left side of the screen
				{
					mini_Centipede[i][x] = 0;
					mini_Centipede[i][y] = mini_Centipede[i][y] + 32;
					mini_Centipede[i][move_left] = 0;
				}
				if(mini_Centipede[i][y] >= 900)  // Reached bottom of the screen
				{
					mini_Centipede[i][move_down] = 0;
				}
			}
			else if(mini_Centipede[i][move_left] == 0 && mini_Centipede[i][move_down] == 1)
			{
				mini_Centipede[i][x] = mini_Centipede[i][x] + (centipede_speed);
				if(mini_Centipede[i][x] > 930)  // Reached right side of the screen
				{
					mini_Centipede[i][x] = 930;
					mini_Centipede[i][y] = mini_Centipede[i][y] + 32;
					mini_Centipede[i][move_left] = 1;
				}
				if(mini_Centipede[i][y] >= 900)  // Reached bottom of the screen
				{
					mini_Centipede[i][move_down] = 0;
				}
			}
			else if(mini_Centipede[i][move_left] == 1 && mini_Centipede[i][move_down] == 0)
			{
				mini_Centipede[i][x] = mini_Centipede[i][x] - (centipede_speed);
				if(mini_Centipede[i][x] < 0)  // Reached the left side of the screen
				{
					mini_Centipede[i][x] = 0;
					mini_Centipede[i][y] = mini_Centipede[i][y] - 32;
					mini_Centipede[i][move_left] = 0;
				}
				if(mini_Centipede[i][y] <= 800)  // Reached the 5th last row of the screen
				{
					mini_Centipede[i][move_down] = 1;
				}
			}
			else if(mini_Centipede[i][move_left] == 0 && mini_Centipede[i][move_down] == 0)
			{
				mini_Centipede[i][x] = mini_Centipede[i][x] + (centipede_speed);
				if(mini_Centipede[i][x] > 930)  // Reached the right side of the screen
				{
					mini_Centipede[i][x] = 930;
					mini_Centipede[i][y] = mini_Centipede[i][y] - 32;
					mini_Centipede[i][move_left] = 1;
				}
				if(mini_Centipede[i][y] <= 800)  // Reached the 5th last row of the screen
				{
					mini_Centipede[i][move_down] = 1;
				}
			}
			if(gameGrid[centipedeGridX][centipedeGridY] == 5 || gameGrid[centipedeGridX][centipedeGridY] == 4 || gameGrid[centipedeGridX][centipedeGridY] == 3 || gameGrid[centipedeGridX][centipedeGridY] == 2)  // Collision with a mushroom
			{
				if(mini_Centipede[i][move_left] == 1 && mini_Centipede[i][move_down] == 1)
				{
					mini_Centipede[i][y] = mini_Centipede[i][y] + 32;
					//gameGrid[centipedeGridX][centipedeGridY] = 0;
					mini_Centipede[i][move_left] = 0;
				}
				else if(mini_Centipede[i][move_left] == 0 && mini_Centipede[i][move_down] == 1)
				{
					mini_Centipede[i][y] = mini_Centipede[i][y] + 32;
					//gameGrid[centipedeGridX][centipedeGridY] = 0;
					mini_Centipede[i][move_left] = 1;
				}
				else if(mini_Centipede[i][move_left] == 1 && mini_Centipede[i][move_down] == 0)
				{
					mini_Centipede[i][y] = mini_Centipede[i][y] - 32;
					//gameGrid[centipedeGridX][centipedeGridY] = 0;
					mini_Centipede[i][move_left] = 0;
				}
				else if(mini_Centipede[i][move_left] == 0 && mini_Centipede[i][move_down] == 0)
				{
					mini_Centipede[i][y] = mini_Centipede[i][y] - 32;
					//gameGrid[centipedeGridX][centipedeGridY] = 0;
					mini_Centipede[i][move_left] = 1;
				}
			}
		}
	}
}

void moveBullet(RenderWindow &window, float bullet[], int gameGrid[30][30], float Centipede[12][5][12], float mini_Centipede[20][5], float deltaTime, int &score)  // Moving the bullet and checking for collisions
{
	int bulletGridX;
	int bulletGridY;
	bulletGridX = (bullet[x] + 16) / 32;
	bulletGridY = (bullet[y] + 16) / 32;

	const int move_left = 3;
	const int move_down = 4;

	int centipedeGridX;
	int centipedeGridY;

	if(gameGrid[bulletGridX][bulletGridY] == 5 || gameGrid[bulletGridX][bulletGridY] == 4 || gameGrid[bulletGridX][bulletGridY] == 3 || gameGrid[bulletGridX][bulletGridY] == 2)  // Collision with a mushroom
	{
		bullet[exists] = false;
		updateMushroom(gameGrid, bulletGridX, bulletGridY, score);
		return;
	}

	for(int i = 0; i < 12; ++i)  // Checking for collisions on the main centipede
	{
		if(Centipede[i][exists][0] == true)  // Head is intact  // This will tell us if that centipede exists
		{
			for(int j = 0 ; j < 12; ++j)
			{
				if(Centipede[i][exists][j] == true)
				{
					centipedeGridX = (Centipede[i][x][j] + 16) / 32;
					centipedeGridY = (Centipede[i][y][j] + 16) / 32;

					if(bulletGridX == centipedeGridX && bulletGridY == centipedeGridY)  // Collision with the Centipede
					{
						bullet[exists] = false;

						if(gameGrid[centipedeGridX][centipedeGridY] == 0 && centipedeGridY >= 25)
						{
							gameGrid[centipedeGridX][centipedeGridY] = 3;  // Adding a poisisnous mushroom
							Centipede[i][exists][j] = 0;
						}
						if(gameGrid[centipedeGridX][centipedeGridY] == 0 && centipedeGridY < 25)
						{
							gameGrid[centipedeGridX][centipedeGridY] = 5;  // Adding a normal mushroom
							Centipede[i][exists][j] = 0;
						}

						if(j == 0)  // Bullet collided with a head
						{
							score = score + 20;
							check_game_end(window, score, Centipede, mini_Centipede);
							return;  //  This line kills the whole centipede as it will not move or be rendered  // Remove to have a head created at the adjacent index
						}
						else
						{
							score = score + 10;
						}

						++j;

						for(int temp_i = 0; temp_i < 12; ++temp_i)
						{
							if(Centipede[temp_i][exists][0] == 0)  // If a new centipede can be created
							{
								for(int temp_j = 0; temp_j < 12; ++temp_j)
								{
									if(Centipede[i][exists][j] == 0 || j == 12)
									{
										break;
									}
									// Copying all indexes into a new centipede
									Centipede[temp_i][exists][temp_j] = true;
									Centipede[temp_i][x][temp_j] = Centipede[i][x][j];
									Centipede[temp_i][y][temp_j] = Centipede[i][y][j];
									Centipede[temp_i][move_left][temp_j] = Centipede[i][move_left][j];
									Centipede[temp_i][move_down][temp_j] = Centipede[i][move_down][j];
									Centipede[i][exists][j] = 0;
									++j;
								}
								break;
							}
						}

						return;
					}
				}
			}
		}
	}

	for(int i = 0; i < 20; ++i)  // Head number
	{
		if(mini_Centipede[i][exists] == true)  // This will tell us if that centipede exists
		{
			centipedeGridX = (mini_Centipede[i][x] + 16) / 32;
			centipedeGridY = (mini_Centipede[i][y] + 16) / 32;

			if(bulletGridX == centipedeGridX && bulletGridY == centipedeGridY)  // Collision with the Centipede
			{
				bullet[exists] = false;

				if(gameGrid[centipedeGridX][centipedeGridY] == 0)
				{
					gameGrid[centipedeGridX][centipedeGridY] = 3;  // Adding a poisisnous mushroom
					mini_Centipede[i][exists] = 0;
				}

				score = score + 20;
				check_game_end(window, score, Centipede, mini_Centipede);
			}
		}
	}

	bullet[y] = bullet[y] - (bullet_speed * deltaTime);
	if (bullet[y] < -32)
	{
		bullet[exists] = false;
		return;
	}
}

void create_mini_centipede(float mini_Centipede[20][5])
{
	for(int i = 0; i < 20; ++i)
	{
		if(mini_Centipede[i][exists] == 0)
		{
			mini_Centipede[i][exists] = true;
			break;  // Creates only 1 centipede
		}
	}
}

void drawBullet(RenderWindow& window, float bullet[], Sprite& bulletSprite)  // Displaying the bullet
{
	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
}

void movePlayerLeft(float player[], Sprite &playerSprite, float deltaTime)
{
	player[x] = player[x] - (player_speed * deltaTime);
}

void movePlayerRight(float player[], Sprite &playerSprite, float deltaTime)
{
	player[x] = player[x] + (player_speed * deltaTime);
}

void movePlayerUp(float player[], Sprite &playerSprite, float deltaTime)
{
	player[y] = player[y] - (player_speed * deltaTime);
}

void movePlayerDown(float player[], Sprite &playerSprite, float deltaTime)
{
	player[y] = player[y] + (player_speed * deltaTime);
}

int main()
{
	srand(time(0));

	// Declaring RenderWindow.
	RenderWindow window(VideoMode(resolutionX, resolutionY), "Centipede", Style::Close | Style::Titlebar);
	window.setFramerateLimit(120);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	//window.setSize(Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	//window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(Vector2i(900, 200));

	// Initializing Background Music.
	Music bgMusic;
	bgMusic.openFromFile("Music/NGGUU.ogg");
	bgMusic.play();
	bgMusic.setVolume(10);

	// Initializing Bullet sound

	SoundBuffer buffer;
	Sound bullet_sound;
	buffer.loadFromFile("Sound_Effects/death.wav");
	bullet_sound.setBuffer(buffer);
	bullet_sound.setVolume(3);

	// Initializing Background.
	Texture backgroundTexture;
	Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/space_background.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(Color(255, 255, 255, 255 * 0.7));  // Reduces Opacity to 70%

	// Initializing score text
	Font font;
	font.loadFromFile("fonts/KarmaFuture.ttf");
	Text scoreT;
	scoreT.setFont(font);
	scoreT.setCharacterSize(35);
	scoreT.setPosition(15,10);

	int score = 0;

	// Initializing Player and Player Sprites.
	float player[2] = {};
	player[x] = (gameColumns / 2) * boxPixelsX;
	player[y] = (gameColumns * 9 / 10) * boxPixelsY;
	Texture playerTexture;
	Sprite playerSprite;
	playerTexture.loadFromFile("Textures/Ship_3.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

	int playerGridX;
	int playerGridY;

	// Initializing Bullet and Bullet Sprites.
	float bullet[3] = {};
	bullet[x] = player[x];
	bullet[y] = player[y] - boxPixelsY;
	bullet[exists] = false;
	float deltaTime;
	Clock delta_Clock;
	
	Texture bulletTexture;
	Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet_3.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Centipede and Centipede Sprites

	float Centipede[12][5][12] = {};  // Centipede Number, x/y/exists/move_left/move_down, Segment Number
	int starting_posX, starting_posY;
	starting_posX = (rand() % 13) + 2;
	starting_posY = (rand() % 15);
	//starting_posY = 23;
	const int move_left = 3;
	const int move_down = 4;

	for(int i = 0; i < 12; ++i)
	{
		Centipede[0][x][i] = ((gameColumns / starting_posX) + i) * (boxPixelsX - 5);
		Centipede[0][y][i] = starting_posY * boxPixelsY;
		Centipede[0][exists][i] = true;
		Centipede[0][move_left][i] = 1;
		Centipede[0][move_down][i] = 1;
	}

	// Initializing the small head Centipede array

	float mini_Centipede[20][5] = {};  // Head number, x/y/exists/move_left/move_down
	Clock miniCentipede_clock;  // Used to spawn the head at intervals

	for(int i = 0; i < 20; ++i)  // A maximum of 20 mini head centipedes
	{
		mini_Centipede[i][exists] = 0;
		mini_Centipede[i][move_left] = 1;
		mini_Centipede[i][move_down] = 1;
		mini_Centipede[i][x] = 930;
		mini_Centipede[i][y] = 800;
	}

	int texture_temp;  // This will be used to control the animation
	texture_temp = 0;
	
	Texture centipedeTexture;
	Sprite centipedeSprite;
	centipedeTexture.loadFromFile("Textures/c_body_left_walk_2.png");
	centipedeSprite.setTexture(centipedeTexture);
	centipedeSprite.setTextureRect(IntRect(texture_temp, 0, boxPixelsX, boxPixelsY));

	Texture centipedeHeadTexture;
	Sprite centipedeHeadSprite;
	centipedeHeadTexture.loadFromFile("Textures/c_head_left_walk_2.png");
	centipedeHeadSprite.setTexture(centipedeHeadTexture);
	centipedeHeadSprite.setTextureRect(IntRect(texture_temp, 0, boxPixelsX, boxPixelsY));

	Clock animation_clock;

	// Initialising Mushrooms and Mushroom sprites

	Texture MushroomTexture;
	Sprite MushroomSprite;
	MushroomTexture.loadFromFile("Textures/mushroom.png");
	MushroomSprite.setTexture(MushroomTexture);
	MushroomSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY - 1));

	Texture half_MushroomTexture;
	Sprite half_MushroomSprite;
	half_MushroomTexture.loadFromFile("Textures/mushroom.png");
	half_MushroomSprite.setTexture(half_MushroomTexture);
	half_MushroomSprite.setTextureRect(IntRect(64, 0, boxPixelsX, boxPixelsY - 1));

	Texture PoisonMushroomTexture;
	Sprite PoisonMushroomSprite;
	PoisonMushroomTexture.loadFromFile("Textures/mushroom.png");
	PoisonMushroomSprite.setTexture(PoisonMushroomTexture);
	PoisonMushroomSprite.setTextureRect(IntRect(0, 32, boxPixelsX, boxPixelsY - 1));

	Texture half_PoisonMushroomTexture;
	Sprite half_PoisonMushroomSprite;
	half_PoisonMushroomTexture.loadFromFile("Textures/mushroom.png");
	half_PoisonMushroomSprite.setTexture(half_PoisonMushroomTexture);
	half_PoisonMushroomSprite.setTextureRect(IntRect(64, 32, boxPixelsX, boxPixelsY - 1));

    int mushroom_count = 0;
	int rand_x, rand_y;

	while(mushroom_count < (rand() % 10) + 20)  // Number of Mushrooms
	{
		rand_x = rand() % 30;
		rand_y = rand() % 25;
		if(gameGrid[rand_x][rand_y] == 0 && rand_y != starting_posY)  // That space is empty and it is not the starting row of the centipede
		{
			gameGrid[rand_x][rand_y] = 5;  // 5 is a full health mushroom  // 4 is a half health mushroom
			mushroom_count = mushroom_count + 1;
		}
	}


	while(window.isOpen())
	{
		deltaTime = delta_Clock.restart().asSeconds();

		///////////////////////////////////////////////////////////////
		//                                                           //
		// Call Your Functions Here. Some have been written for you. //
		// Be vary of the order you call them, SFML draws in order.  //
		//                                                           //
		///////////////////////////////////////////////////////////////

		scoreT.setString("Score = " + to_string(score));

		window.draw(backgroundSprite);
		drawPlayer(window, player, playerSprite);
		drawMushroom(window, gameGrid, MushroomSprite, half_MushroomSprite, PoisonMushroomSprite, half_PoisonMushroomSprite);
		drawCentipede(window, Centipede, mini_Centipede, centipedeSprite, centipedeHeadSprite);
		window.draw(scoreT);

		moveCentipede(window, Centipede, player, gameGrid, deltaTime, score);

		if(animation_clock.getElapsedTime().asMilliseconds() > 100)  // Centipede animation
		{
			if(texture_temp == 196)
			{
				texture_temp = 0;
			}
			else
			{
				texture_temp = texture_temp + 28;
			}
			centipedeSprite.setTextureRect(IntRect(texture_temp, 0, boxPixelsX - 5, boxPixelsY));
			centipedeHeadSprite.setTextureRect(IntRect(texture_temp, 0, boxPixelsX - 5, boxPixelsY));
			animation_clock.restart().asMilliseconds();
		}

		if(Centipede[0][y][0] >= 800)  // Spawning Mini head centipedes in player area
		{
			if(miniCentipede_clock.getElapsedTime().asSeconds() >= 10 && Centipede[0][exists][0] == true)
			{
				miniCentipede_clock.restart().asSeconds();
				create_mini_centipede(mini_Centipede);
			}
			moveMiniCentipede(window, mini_Centipede, player, score);
		}

		if(bullet[exists] == true)
		{
			moveBullet(window, bullet, gameGrid, Centipede, mini_Centipede, deltaTime, score);
			drawBullet(window, bullet, bulletSprite);
		}

		playerGridX = (player[x] + 16) / 32;
		playerGridY = (player[y] + 16) / 32;

		if(gameGrid[playerGridX][playerGridY] == 3 || gameGrid[playerGridX][playerGridY] == 2)  // Checking for collisions between player and poisinous mushrooms
		{
			end_losing_screen(window, score);
			return 0;
		}

		// Player movement
		if(Keyboard::isKeyPressed(Keyboard::W))
		{
			if(player[y] > 800)
			{
				movePlayerUp(player, playerSprite, deltaTime);
			}
		}

		if(Keyboard::isKeyPressed(Keyboard::A))
		{
			if(player[x] > 0)
			{
				movePlayerLeft(player, playerSprite, deltaTime);
			}
			if(player[x] < 0)
			{
				player[x] = 0;
			}
		}

		if(Keyboard::isKeyPressed(Keyboard::S))
		{
			if(player[y] < 929)
			{
				movePlayerDown(player, playerSprite, deltaTime);
			}
			if(player[y] > 930)
			{
				player[y] = 928;
			}
		}

		if(Keyboard::isKeyPressed(Keyboard::D))
		{
			if(player[x] < 929)
			{
				movePlayerRight(player, playerSprite, deltaTime);
			}
			if(player[x] > 929)
			{
				player[x] = 929;
			}
		}
		

		if(Keyboard::isKeyPressed(Keyboard::Space))
		{
			if(bullet[exists] == false)
			{
				bullet[exists] = true;
				bullet[x] = player[x];
				bullet[y] = player[y];
			}
			bullet_sound.play();
		}

		Event e;
		while(window.pollEvent(e)) 
		{
			if (e.type == Event::Closed) 
			{
				return 0;
			}
		}
		window.display();
		window.clear();
	}
}