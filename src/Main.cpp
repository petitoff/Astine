#include "Platform/Platform.hpp"
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include <WinUser.h>
#include <Windows.h>
#include <chrono>
#include <fstream>
#include <future>
#include <thread>

class TicTacToe
{
private:
	// menu
	sf::Image background_menu_i;
	sf::Texture background_menu_t;

	// 1 vs 1 i inne
	sf::Image background_i;
	sf::Texture background_t;
	sf::Image board_i;
	sf::Texture board_t;
	sf::Image reset_i;
	sf::Texture reset_t;
	sf::Image cross_i;
	sf::Texture cross;
	sf::Image circle_i;
	sf::Texture circle;
	sf::Image blank_i;
	sf::Texture blank;
	sf::Font font;

	// server
	sf::Image server_menu_i;
	sf::Texture server_menu_t;

	// Inne
	std::string msg;
	int cur;
	bool waitForReset;

	bool checkWin(int placement);
	bool checkDraw();

public:
	sf::Sprite background_menu;
	sf::Sprite server_menu_1;

	sf::Sprite background;
	sf::Sprite board;
	sf::Sprite reset;
	sf::Sprite pieces[9];
	sf::Text text;

	char set[9];

	bool loadAssets();
	bool loadBoard(int startPlayer);

	void keyPress(sf::Vector2f pos);
	void keyPress_PC(sf::Vector2f pos);
	void keyPress_MP(sf::Vector2f pos);
	void keyPress_MP_1(sf::Vector2f pos);
	void keyPress_MP_2(int cor, sf::Vector2f pos);
	void keyPress_MP_2_1(int cor, sf::Vector2f pos);
	int check_pieces(int x, int y);
	// void server();
	// void server_receive();

	int thread_server(std::string cord)
	{
		auto f1 = std::async(&TicTacToe::server_receive, this, cord);
		auto res1 = f1.get();

		return res1;
	}

	int server_receive(std::string cord)
	{
		// sf::IpAddress ip = sf::IpAddress::getLocalAddress();
		sf::TcpSocket socket;
		// std::string text_test = "Test111";
		std::size_t received;
		// char connectionType, mode;
		char buffer[100];
		sf::TcpListener listener;
		listener.listen(2000);

		sf::Packet packet;
		socket.receive(packet);
		std::cout << packet.getDataSize() << std::endl;

		// std::cout << received << std::endl;
		listener.accept(socket);
		std::cout << received << std::endl;

		socket.send(cord.c_str(), cord.length() + 1);
		socket.receive(buffer, sizeof(buffer), received);

		// t.setString(ip);
		// window.draw(t);
		// socket.receive(buffer, sizeof(buffer), received);

		int cord_receive = 0;
		try
		{
			std::string str(buffer);
			cord_receive = std::stoi(str);
		}
		catch (std::invalid_argument)
		{
			cord_receive = 0;
		}

		return cord_receive;
	}
	int client_receive(std::string cord, sf::IpAddress ips)
	{
		sf::IpAddress ip = ips;
		sf::TcpSocket socket;
		std::size_t received;
		char buffer[100];

		sf::Packet packet;
		std::string s("0", 2048);

		packet << s;

		socket.connect(ip, 2000);

		socket.send(cord.c_str(), cord.length() + 1);

		socket.receive(buffer, sizeof(buffer), received);

		socket.send(packet);
		// std::cout << buffer << std::endl;

		// sf::TcpListener listener;
		// listener.listen(20000);
		// listener.accept(socket);
		// text_test += "server";
		// socket.send(text_test.c_str(), text_test.length() + 1);

		// t.setString(ip);
		// window.draw(t);
		// socket.receive(buffer, sizeof(buffer), received);
		// std::cout << buffer << std::endl;
		int cord_receive = 0;
		try
		{
			std::string str(buffer);
			cord_receive = std::stoi(str);
		}
		catch (std::invalid_argument)
		{
			cord_receive = 0;
		}

		return cord_receive;
	}
};

bool TicTacToe::loadAssets()
{
	// load menu
	if (!this->background_menu_i.loadFromFile("Resources\\background_menu.jpg"))
		return false;
	if (!this->background_menu_t.loadFromImage(this->background_menu_i))
		return false;
	this->background_menu.setTexture(this->background_menu_t);

	// load game
	if (!this->background_i.loadFromFile("Resources\\background.png"))
		return false;
	if (!this->background_t.loadFromImage(this->background_i))
		return false;
	this->background.setTexture(this->background_t);

	if (!this->board_i.loadFromFile("Resources\\board.png"))
		return false;
	if (!this->board_t.loadFromImage(this->board_i))
		return false;
	this->board.setTexture(this->board_t);
	this->board.setPosition(sf::Vector2f(0.0f, 100.0f));

	if (!this->reset_i.loadFromFile("Resources\\reset.png"))
		return false;
	if (!this->reset_t.loadFromImage(this->reset_i))
		return false;
	this->reset.setTexture(this->reset_t);
	this->reset.setPosition(sf::Vector2f(25.0f, 525.0f));

	if (!this->circle_i.loadFromFile("Resources\\circle.png"))
		return false;
	if (!this->circle.loadFromImage(this->circle_i))
		return false;

	if (!this->cross_i.loadFromFile("Resources\\cross.png"))
		return false;
	if (!this->cross.loadFromImage(this->cross_i))
		return false;

	if (!this->blank_i.loadFromFile("Resources\\blank.png"))
		return false;
	if (!this->blank.loadFromImage(this->blank_i))
		return false;

	if (!this->font.loadFromFile("Resources\\font.ttf"))
		return false;

	// server
	if (!this->server_menu_i.loadFromFile("Resources\\klient_serwer.png"))
		return false;
	if (!this->server_menu_t.loadFromImage(this->server_menu_i))
		this->reset.setTexture(this->reset_t);

	if (!this->server_menu_i.loadFromFile("Resources\\klient_serwer.png"))
		return false;
	if (!this->server_menu_t.loadFromImage(this->server_menu_i))
		return false;
	this->server_menu_1.setTexture(this->server_menu_t);

	this->text.setFont(this->font);
	this->text.setCharacterSize(40);
	text.setFillColor(sf::Color::Black);
	this->text.setPosition(sf::Vector2f(25.0f, 25.0f));

	return true;
}

bool TicTacToe::loadBoard(int startPlayer)
{
	memset(this->set, 0, 9);

	this->waitForReset = false;

	this->pieces[0].setPosition(sf::Vector2f(10.0f, 110.0f));
	this->pieces[1].setPosition(sf::Vector2f(150.0f, 110.0f));
	this->pieces[2].setPosition(sf::Vector2f(290.0f, 110.0f));
	this->pieces[3].setPosition(sf::Vector2f(10.0f, 250.0f));
	this->pieces[4].setPosition(sf::Vector2f(150.0f, 250.0f));
	this->pieces[5].setPosition(sf::Vector2f(290.0f, 250.0f));
	this->pieces[6].setPosition(sf::Vector2f(10.0f, 390.0f));
	this->pieces[7].setPosition(sf::Vector2f(150.0f, 390.0f));
	this->pieces[8].setPosition(sf::Vector2f(290.0f, 390.0f));
	for (int i = 0; i < 9; i++)
		this->pieces[i].setTexture(this->blank);

	this->cur = startPlayer;
	this->msg = std::string(startPlayer == 1 ? "Gracz X" : "Gracz O");
	this->text.setString(this->msg);

	return true;
}

bool TicTacToe::checkWin(int placement)
{
	int row = (placement / 3) * 3;
	int columm = placement % 3;

	if (this->set[row] == this->cur && this->set[row + 1] == this->cur && this->set[row + 2] == this->cur)
		return true;

	if (this->set[columm] == this->cur && this->set[columm + 3] == this->cur && this->set[columm + 6] == this->cur)
		return true;

	if (this->set[0] == this->cur && this->set[4] == this->cur && this->set[8] == this->cur)
		return true;

	if (this->set[2] == this->cur && this->set[4] == this->cur && this->set[6] == this->cur)
		return true;

	return false;
}

bool TicTacToe::checkDraw()
{
	for (int i = 0; i < 9; i++)
		if (this->set[i] == 0)
			return false;

	return true;
}

void TicTacToe::keyPress(sf::Vector2f pos)
{
	if (!this->waitForReset)
	{
		for (int i = 0; i < 9; i++)
		{
			if (this->pieces[i].getGlobalBounds().contains(pos))
			{
				if (this->set[i] == 0)
				{
					this->pieces[i].setTexture(this->cur == 1 ? this->cross : this->circle);

					this->set[i] = this->cur;

					if (this->checkWin(i))
					{
						this->waitForReset = true;
						this->msg = this->cur == 1 ? "Gracz X wygrywa!" : "Gracz O wygrywa!";
						this->text.setString(this->msg);
					}
					else
					{
						if (this->checkDraw())
						{
							this->waitForReset = true;
							this->msg = "Remis!";
							this->text.setString(this->msg);
						}
						else
						{
							this->cur = this->cur == 1 ? 2 : 1;
							this->msg = this->cur == 1 ? "Gracz X" : "Gracz O";
							this->text.setString(this->msg);
						}
					}
				}
				break;
			}
		}
	}

	if (this->reset.getGlobalBounds().contains(pos))
		this->loadBoard(this->cur == 1 ? 2 : 1);
}
void TicTacToe::keyPress_PC(sf::Vector2f pos)
{
	bool check = false;
	if (!this->waitForReset)
	{
		for (int i = 0; i < 9; i++)
		{
			if (this->pieces[i].getGlobalBounds().contains(pos))
			{
				if (this->set[i] == 0)
				{
					check = true;
					this->cur = 1;
					this->pieces[i].setTexture(this->cur == 1 ? this->cross : this->circle);
					this->set[i] = this->cur;

					if (this->checkWin(i))
					{
						this->waitForReset = true;
						this->msg = this->cur == 1 ? "Gracz X wygrywa!" : "Gracz O wygrywa!";
						this->text.setString(this->msg);
					}
					else
					{
						if (this->checkDraw())
						{
							this->waitForReset = true;
							this->msg = "Remis!";
							this->text.setString(this->msg);
						}
						else
						{
							this->cur = this->cur == 1 ? 2 : 1;
							this->msg = this->cur == 1 ? "Gracz X" : "Gracz O";
							this->text.setString(this->msg);
						}
					}
				}
				break;
			}
		}
	}

	if (!this->waitForReset)
	{
		if (check)
		{
			for (int i = 0; i < 9; i++)
			{
				if (this->set[i] == 0)
				{
					this->cur = 2;
					this->pieces[i].setTexture(this->cur == 1 ? this->cross : this->circle);
					this->set[i] = this->cur;
					//if (!this->set[1] == 0)

					if (this->checkWin(i))
					{
						this->waitForReset = true;
						this->msg = this->cur == 1 ? "Gracz X wygrywa!" : "Gracz O wygrywa!";
						this->text.setString(this->msg);
					}
					else
					{
						if (this->checkDraw())
						{
							this->waitForReset = true;
							this->msg = "Remis!";
							this->text.setString(this->msg);
						}
						else
						{
							this->cur = this->cur == 1 ? 2 : 1;
							this->msg = this->cur == 1 ? "Gracz X" : "Gracz O";
							this->text.setString(this->msg);
						}
					}
					break;
				}
			}
		}
	}

	if (this->reset.getGlobalBounds().contains(pos))
		this->loadBoard(this->cur == 1 ? 2 : 1);
}
void TicTacToe::keyPress_MP(sf::Vector2f pos)
{
	if (!this->waitForReset)
	{
		for (int i = 0; i < 9; i++)
		{
			if (this->pieces[i].getGlobalBounds().contains(pos))
			{
				if (this->set[i] == 0)
				{
					this->cur = 1;
					this->pieces[i].setTexture(this->cur == 1 ? this->cross : this->circle);
					this->set[i] = this->cur;

					if (this->checkWin(i))
					{
						this->waitForReset = true;
						this->msg = this->cur == 1 ? "Gracz X wygrywa!" : "Gracz O wygrywa!";
						this->text.setString(this->msg);
					}
					else
					{
						if (this->checkDraw())
						{
							this->waitForReset = true;
							this->msg = "Remis!";
							this->text.setString(this->msg);
						}
						else
						{
							this->cur = this->cur == 1 ? 2 : 1;
							this->msg = this->cur == 1 ? "Gracz X" : "Gracz O";
							this->text.setString(this->msg);
						}
					}
				}
				break;
			}
		}
	}
	if (this->reset.getGlobalBounds().contains(pos))
		this->loadBoard(this->cur == 1 ? 2 : 1);
}
void TicTacToe::keyPress_MP_1(sf::Vector2f pos)
{
	if (!this->waitForReset)
	{
		for (int i = 0; i < 9; i++)
		{
			if (this->pieces[i].getGlobalBounds().contains(pos))
			{
				if (this->set[i] == 0)
				{
					this->cur = 2;
					this->pieces[i].setTexture(this->cur == 1 ? this->cross : this->circle);
					this->set[i] = this->cur;

					if (this->checkWin(i))
					{
						this->waitForReset = true;
						this->msg = this->cur == 1 ? "Gracz X wygrywa!" : "Gracz O wygrywa!";
						this->text.setString(this->msg);
					}
					else
					{
						if (this->checkDraw())
						{
							this->waitForReset = true;
							this->msg = "Remis!";
							this->text.setString(this->msg);
						}
						else
						{
							this->cur = this->cur == 1 ? 2 : 1;
							this->msg = this->cur == 1 ? "Gracz X" : "Gracz O";
							this->text.setString(this->msg);
						}
					}
				}
				break;
			}
		}
	}
	if (this->reset.getGlobalBounds().contains(pos))
		this->loadBoard(this->cur == 1 ? 2 : 1);
}
void TicTacToe::keyPress_MP_2(int cor, sf::Vector2f pos)
{
	cor -= 1;
	if (!this->waitForReset)
	{
		for (int i = 0; i < 9; i++)
		{
			if (i == cor)
			{
				if (this->set[i] == 0)
				{
					this->cur = 1;
					this->pieces[i].setTexture(this->cur == 1 ? this->cross : this->circle);
					this->set[i] = this->cur;

					if (this->checkWin(i))
					{
						this->waitForReset = true;
						this->msg = this->cur == 1 ? "Gracz X wygrywa!" : "Gracz O wygrywa!";
						this->text.setString(this->msg);
					}
					else
					{
						if (this->checkDraw())
						{
							this->waitForReset = true;
							this->msg = "Remis!";
							this->text.setString(this->msg);
						}
						else
						{
							this->cur = this->cur == 1 ? 2 : 1;
							this->msg = this->cur == 1 ? "Gracz X" : "Gracz O";
							this->text.setString(this->msg);
						}
					}
				}
				break;
			}
		}
	}
	if (this->reset.getGlobalBounds().contains(pos))
		this->loadBoard(this->cur == 1 ? 2 : 1);
}
void TicTacToe::keyPress_MP_2_1(int cor, sf::Vector2f pos)
{
	cor -= 1;
	if (!this->waitForReset)
	{
		for (int i = 0; i < 9; i++)
		{
			if (i == cor)
			{
				if (this->set[i] == 0)
				{
					this->cur = 2;
					this->pieces[i].setTexture(this->cur == 1 ? this->cross : this->circle);
					this->set[i] = this->cur;

					if (this->checkWin(i))
					{
						this->waitForReset = true;
						this->msg = this->cur == 1 ? "Gracz X wygrywa!" : "Gracz O wygrywa!";
						this->text.setString(this->msg);
					}
					else
					{
						if (this->checkDraw())
						{
							this->waitForReset = true;
							this->msg = "Remis!";
							this->text.setString(this->msg);
						}
						else
						{
							this->cur = this->cur == 1 ? 2 : 1;
							this->msg = this->cur == 1 ? "Gracz X" : "Gracz O";
							this->text.setString(this->msg);
						}
					}
				}
				break;
			}
		}
	}
	if (this->reset.getGlobalBounds().contains(pos))
		this->loadBoard(this->cur == 1 ? 2 : 1);
}

int TicTacToe::check_pieces(int x, int y)
{
	// std::string pieces;
	if (x > 0 && x < 117 && y > 100 && y < 218)
	{
		return 1;
	}
	else if (x > 142 && x < 259 && y > 100 && y < 218)
	{
		return 2;
	}
	else if (x > 283 && x < 401 && y > 100 && y < 218)
	{
		return 3;
	}
	else if (x > 0 && x < 120 && y > 242 && y < 360)
	{
		return 4;
	}
	else if (x > 144 && x < 258 && y > 242 && y < 360)
	{
		return 5;
	}
	else if (x > 284 && x < 401 && y > 242 && y < 360)
	{
		return 6;
	}
	else if (x > 0 && x < 116 && y > 383 && y < 501)
	{
		return 7;
	}
	else if (x > 142 && x < 259 && y > 383 && y < 501)
	{
		return 8;
	}
	else if (x > 284 && x < 401 && y > 383 && y < 501)
	{
		return 9;
	}

	return 0;
}

int main()
{
	TicTacToe Game;

	if (!Game.loadAssets())
	{
		printf("Unable to load game assets\n");
		system("pause");
		return 0;
	}

	if (!Game.loadBoard(1))
	{
		printf("Unable to load game\n");
		system("pause");
		return 0;
	}

	//Hide Console
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	sf::RenderWindow window(sf::VideoMode(400, 600), "Tic Tac Toe");

	int choice { 0 };

	// menu
	while (window.isOpen())
	{
		window.clear();

		window.draw(Game.background_menu);

		window.display();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					//Game.keyPress(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
					int pixelPosX = sf::Mouse::getPosition(window).x;
					int pixelPosY = sf::Mouse::getPosition(window).y;

					if (pixelPosX > 100 && pixelPosX < 310 && pixelPosY > 225 && pixelPosY < 274)
					{
						choice = 1;
					}
					else if (pixelPosX > 100 && pixelPosX < 310 && pixelPosY > 300 && pixelPosY < 343)
					{
						choice = 2;
					}

					else if (pixelPosX > 100 && pixelPosX < 310 && pixelPosY > 369 && pixelPosY < 413)
					{
						choice = 3;
					}
					// break;

					//std::cout << "Method 2: (" << sf::Mouse::getPosition(window).x << ", " << sf::Mouse::getPosition(window).y << ")\n\n";
				}
		}
		if (choice != 0)
		{
			break;
		}
	}

	if (choice == 1)
	{
		// Wczytywanie gry 1 vs 1
		while (window.isOpen())
		{
			window.clear();

			window.draw(Game.background);
			window.draw(Game.board);
			window.draw(Game.text);
			window.draw(Game.reset);

			for (int i = 0; i < 9; i++)
				window.draw(Game.pieces[i]);

			window.display();

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Button::Left)
						Game.keyPress(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
				}
			}
		}
	}
	else if (choice == 2)
	{
		// Wczytywanie gry 1 vs PC
		while (window.isOpen())
		{
			window.clear();

			window.draw(Game.background);
			window.draw(Game.board);
			window.draw(Game.text);
			window.draw(Game.reset);

			for (int i = 0; i < 9; i++)
				window.draw(Game.pieces[i]);

			window.display();

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Button::Left)
					{
						// Game.keyPress(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
						Game.keyPress_PC(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
					}
				}
			}
		}
	}
	else if (choice == 3)
	{
		// Wczytywanie gry multiplayer

		int choice { 0 };
		sf::Font font;
		font.loadFromFile("Resources\\font.ttf");
		sf::Text t;
		t.setFillColor(sf::Color::White);
		t.setFont(font);
		std::string s;

		while (window.isOpen())
		{
			window.clear();
			window.draw(Game.server_menu_1);
			//window.draw(Game.background);

			// std::string input_text;
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}
				if (event.type == sf::Event::MouseButtonPressed)
				{
					int pixelPosX = sf::Mouse::getPosition(window).x;
					int pixelPosY = sf::Mouse::getPosition(window).y;

					if (pixelPosX > 54 && pixelPosX < 180 && pixelPosY > 263 && pixelPosY < 335)
					{
						choice = 1;
					}
					else if (pixelPosX > 227 && pixelPosX < 357 && pixelPosY > 263 && pixelPosY < 339)
					{
						choice = 2;
					}
				}
			}
			window.display();
			if (choice != 0)
			{
				break;
			}
		}
		if (choice == 1)
		{
			//klient

			sf::String Text;
			//sf::Font font;
			sf::Text text;
			sf::IpAddress ip = sf::IpAddress::getLocalAddress();

			// select the font
			text.setFont(font); // font is a sf::Font

			std::string str; // a variable of str data type

			// using the stringstream class to insert an int and
			// extract a string
			std::stringstream ss;
			ss << ip;
			ss >> str;

			std::string output_1 = "Twoje ip: " + str;
			text.setString(output_1);

			// set the character size
			text.setCharacterSize(15); // in pixels, not points!

			// set the color
			text.setFillColor(sf::Color::Red);

			// sf::FloatRect textRect = text.getLocalBounds();
			// const int SCRWIDTH = 800;
			// const int SCRHEIGHT = 600;
			// text.setOrigin(textRect.width / 2, textRect.height / 2);
			// text.setPosition(sf::Vector2f(SCRWIDTH / 2.0f, SCRHEIGHT / 2.0f));

			std::string playerInput;
			sf::Text playerText;
			playerText.setFont(font);

			// set the character size
			playerText.setCharacterSize(15);

			// set the color
			playerText.setFillColor(sf::Color::Red);
			//font.loadFromFile("Resources\\font.ttf");
			playerText.setString("Wprowadz ip: ");

			std::string cord_send;
			int user_server { 0 };
			bool break_1 = false;

			while (window.isOpen())
			{
				window.clear();
				window.draw(playerText);
				window.display();

				sf::Event event;
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
					{
						window.close();
					}
					if (event.type == sf::Event::TextEntered)
					{
						if (event.key.code == 13)
						{
							break_1 = true;
							break;
						}
						else if (event.key.code == 8 && playerInput.length() > 0)
						{
							playerInput.std::string::pop_back();
						}
						else
							playerInput += event.text.unicode;

						playerText.setString(playerInput);
					}
				}
				if (break_1)
					break;
			}

			while (window.isOpen())
			{

				window.clear();
				window.draw(Game.background);
				window.draw(Game.board);
				window.draw(Game.text);
				window.draw(Game.reset);

				for (int i = 0; i < 9; i++)
					window.draw(Game.pieces[i]);

				// window.draw(playerText);
				window.display();

				user_server = Game.client_receive(cord_send, playerInput);
				// std::cout << user_server << std::endl;
				if (user_server != 0)
				{
					Game.keyPress_MP_2(user_server, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
				}

				cord_send = "0";
				sf::Event event;
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
					{
						window.close();
					}
					if (event.type == sf::Event::MouseButtonPressed)
					{
						if (event.mouseButton.button == sf::Mouse::Button::Left)
						{
							Game.keyPress_MP_1(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
							int pixelPosX = sf::Mouse::getPosition(window).x;
							int pixelPosY = sf::Mouse::getPosition(window).y;

							int pieces = Game.check_pieces(pixelPosX, pixelPosY);
							// std::cout << pieces;
							cord_send = std::to_string(pieces);
						}
					}
				}
			}
		}
		else if (choice == 2)
		{
			sf::String Text;
			sf::Text text;
			sf::IpAddress ip = sf::IpAddress::getLocalAddress();

			// select the font
			text.setFont(font); // font is a sf::Font

			std::string str; // a variable of str data type

			// using the stringstream class to insert an int and
			// extract a string
			std::stringstream ss;
			ss << ip;
			ss >> str;

			std::string output_1 = "Twoje ip: " + str;
			text.setString(output_1);

			// set the character size
			text.setCharacterSize(15); // in pixels, not points!

			// set the color
			text.setFillColor(sf::Color::Red);
			std::string cord_send;
			int user_client { 0 };

			// std::thread th1(&TicTacToe::server_receive);
			// th1.detach();

			while (window.isOpen())
			{
				window.clear();

				window.draw(Game.background);
				window.draw(Game.board);
				window.draw(Game.text);
				window.draw(Game.reset);

				for (int i = 0; i < 9; i++)
					window.draw(Game.pieces[i]);

				window.draw(text);
				window.display();

				// auto future = std::async(&TicTacToe::server_receive, cord_send);
				// user_client = future.get();

				// user_client = Game.server_receive(cord_send);
				// std::cout << user_server << std::endl;

				user_client = Game.thread_server(cord_send);
				// std::thread worker_thread(&TicTacToe::thread_server, &Game, cord_send);

				if (user_client != 0)
				{
					Game.keyPress_MP_2_1(user_client, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
				}

				cord_send = "0";
				sf::Event event;
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
					{
						window.close();
					}
					if (event.type == sf::Event::MouseButtonPressed)
					{
						if (event.mouseButton.button == sf::Mouse::Button::Left)
						{
							Game.keyPress_MP(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
							int pixelPosX = sf::Mouse::getPosition(window).x;
							int pixelPosY = sf::Mouse::getPosition(window).y;

							int pieces = Game.check_pieces(pixelPosX, pixelPosY);
							cord_send = std::to_string(pieces);
						}
					}
				}
			}
		}
	}
	return 0;
}
