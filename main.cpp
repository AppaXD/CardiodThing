#include <SFML/Graphics.hpp>

int width = 1280, height = 720;
short pointCount = 125, radius = 300;
float pi = 3.141592;
float speed = 1.f / 10;
float multiplier = 1.f, multiplierIncrease = .04f;

std::vector<sf::RectangleShape> points;

float rotateTowards(sf::Vector2f pos1, sf::Vector2f pos2)
{
	float dx = pos1.x - pos2.x;
	float dy = pos1.y - pos2.y;

	float angle = (atan2(dy, dx)) * 180 / pi;

	return angle;
}

sf::RectangleShape createLine(int p1_index, int p2_index)
{
	sf::RectangleShape p1 = points[p1_index];
	sf::RectangleShape p2 = points[p2_index];

	sf::Vector2f p1_pos = p1.getPosition();
	sf::Vector2f p2_pos = p2.getPosition();

	float angle = rotateTowards(p1_pos, p2_pos) + 90;

	float len = sqrt(pow(p1_pos.x - p2_pos.x, 2) + pow(p1_pos.y - p2_pos.y, 2));

	sf::RectangleShape line;
	line.setFillColor(sf::Color::Black);
	line.setSize(sf::Vector2f(3, len));
	line.setPosition(p1_pos);
	line.setRotation(angle);

	return line;
}

std::vector<sf::RectangleShape> genLines()
{
	std::vector<sf::RectangleShape> lines;
	for (int i = 0; i < pointCount; i++)
	{
		int index = std::fmod(i * multiplier, pointCount);

		lines.push_back(createLine(i, index));
	}
	return lines;
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(width, height), "", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	sf::CircleShape circle;
	circle.setRadius(radius);
	circle.setOutlineThickness(4);
	circle.setOutlineColor(sf::Color::Black);
	circle.setPointCount(100);
	circle.setOrigin(radius, radius);
	circle.setPosition(width / 2, height / 2);

	sf::Vector2f origin = circle.getPosition();

	float anglePerPoint = (360.f / pointCount) * (pi / 180.f);  
	for (int i = 0; i < pointCount; i++)
	{
		sf::Vector2f pos(origin.x + std::cos(anglePerPoint * i) * radius, origin.y + std::sin(anglePerPoint * i) * radius); // calculate the equidistant points
		sf::RectangleShape point;
		point.setSize(sf::Vector2f(5, 5));
		point.setPosition(pos);
		point.setFillColor(sf::Color::Red);
		points.push_back(point);
	}

	std::vector<sf::RectangleShape> lines;
	lines = genLines();

	int linesDrawn = 0;
	bool started = false;

	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed)
			{
				int key = event.key.code;
				switch (key)
				{
				case sf::Keyboard::Space:
					started = true;
					break;
				}
			}
		}

		if (started) // added this so I could take gifs of it
		{
			if (linesDrawn < pointCount && clock.getElapsedTime().asSeconds() >= speed)
			{
				lines.clear();

				multiplier += multiplierIncrease;

				lines = genLines();

				clock.restart();
			}
		}

		window.clear(sf::Color::White);
		window.draw(circle);
		for (int i = 0; i < lines.size(); i++)
		{
			window.draw(lines[i]);
		}
		window.display();
	}
}
