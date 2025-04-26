#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

const int blockSize = 20;
const int width = 40;   // 40 * 20 = 800 pixels
const int height = 30;  // 30 * 20 = 600 pixels
Vector2i food;
vector<Vector2i> snake = { {20, 15} };
Vector2i direction = {1, 0};
float timer = 0, delay = 0.1;
bool gameOver = false;

void spawnFood() {
    food.x = rand() % width;
    food.y = rand() % height;
}

void moveSnake() {
    for (int i = snake.size() - 1; i > 0; --i)
        snake[i] = snake[i-1];

    snake[0].x += direction.x;
    snake[0].y += direction.y;

    if (snake[0].x == food.x && snake[0].y == food.y) {
        snake.push_back(snake.back());
        spawnFood();
    }

    if (snake[0].x < 0 || snake[0].y < 0 || snake[0].x >= width || snake[0].y >= height)
        gameOver = true;

    for (int i = 1; i < snake.size(); ++i)
        if (snake[0] == snake[i])
            gameOver = true;
}

int main() {
    srand(time(0));
    spawnFood();

    RenderWindow window(VideoMode(width * blockSize, height * blockSize), "Snake Game!");
    window.setFramerateLimit(60);

    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }
    Text gameOverText("Game Over!", font, 50);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(width * blockSize / 3, height * blockSize / 3);

    Clock clock;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Up) && direction.y != 1) direction = {0, -1};
        if (Keyboard::isKeyPressed(Keyboard::Down) && direction.y != -1) direction = {0, 1};
        if (Keyboard::isKeyPressed(Keyboard::Left) && direction.x != 1) direction = {-1, 0};
        if (Keyboard::isKeyPressed(Keyboard::Right) && direction.x != -1) direction = {1, 0};

        if (timer > delay && !gameOver) {
            timer = 0;
            moveSnake();
        }

        window.clear(Color::Black);

        RectangleShape rect(Vector2f(blockSize - 1, blockSize - 1));

        rect.setFillColor(Color::Green);
        for (auto s : snake) {
            rect.setPosition(s.x * blockSize, s.y * blockSize);
            window.draw(rect);
        }

        rect.setFillColor(Color::Red);
        rect.setPosition(food.x * blockSize, food.y * blockSize);
        window.draw(rect);

        if (gameOver)
            window.draw(gameOverText);

        window.display();
    }

    return 0;
}