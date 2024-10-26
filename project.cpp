#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <ctime>


const int windowWidth = 400;
const int windowHeight = 600;
const int blockSize = 40;
const int columns = windowWidth / blockSize;
const int rows = windowHeight / blockSize;


std::vector<std::vector<sf::Vector2i>> shapes = {
    { {0, 0}, {1, 0}, {2, 0}, {3, 0} }, 
    { {0, 0}, {0, 1}, {1, 0}, {1, 1} }, 
    { {0, 0}, {1, 0}, {1, 1}, {2, 1} }, 
    { {0, 1}, {1, 0}, {1, 1}, {2, 0} }, 
    { {1, 0}, {1, 1}, {1, 2}, {2, 1} }, 
    { {0, 0}, {0, 1}, {1, 1}, {2, 1} }, 
    { {0, 1}, {1, 1}, {2, 1}, {2, 0} }, 
};


std::vector<sf::Color> shapeColors = {
    sf::Color::Cyan, sf::Color::Yellow, sf::Color::Red,
    sf::Color::Green, sf::Color::Magenta, sf::Color::Blue,
    sf::Color(255, 165, 0) 
};


bool isValidPosition(std::vector<sf::Vector2i>& shape, const std::vector<std::vector<int>>& grid, int offsetX, int offsetY) {
    for (auto& block : shape) {
        int x = block.x + offsetX;
        int y = block.y + offsetY;
        if (x < 0 || x >= columns || y >= rows || (y >= 0 && grid[y][x] != 0)) {
            return false;
        }
    }
    return true;
}


void rotateShape(std::vector<sf::Vector2i>& shape) {
    for (auto& block : shape) {
        int temp = block.x;
        block.x = -block.y;
        block.y = temp;
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Tetris with Rotations and Score");

    
    std::vector<std::vector<int>> grid(rows, std::vector<int>(columns, 0));

    
    std::vector<sf::Vector2i> currentShape = shapes[std::rand() % shapes.size()];
    int currentX = columns / 2 - 2;
    int currentY = 0;
    sf::Color currentColor = shapeColors[std::rand() % shapeColors.size()];

    int score = 0;
    float fallSpeed = 0.5f;
    float fallTimer = 0.f;
    sf::Clock clock;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setCharacterSize(24);
    scoreText.setPosition(10, 10);

    bool gameOver = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (gameOver) {
            window.clear(sf::Color::White);
            scoreText.setString("Game Over! Score: " + std::to_string(score));
            window.draw(scoreText);
            window.display();

            
            sf::sleep(sf::seconds(2));
            window.close();
            continue;
        }

        float deltaTime = clock.restart().asSeconds();
        fallTimer += deltaTime;

       
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            if (isValidPosition(currentShape, grid, currentX - 1, currentY)) {
                currentX--;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            if (isValidPosition(currentShape, grid, currentX + 1, currentY)) {
                currentX++;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            auto tempShape = currentShape;
            rotateShape(tempShape);
            if (isValidPosition(tempShape, grid, currentX, currentY)) {
                currentShape = tempShape;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            fallSpeed = 0.1f; 
        } else {
            fallSpeed = 0.5f;
        }

       
        if (fallTimer >= fallSpeed) {
            fallTimer = 0.f;
            if (isValidPosition(currentShape, grid, currentX, currentY + 1)) {
                currentY++;
            } else {
               
                for (auto& block : currentShape) {
                    grid[currentY + block.y][currentX + block.x] = 1;
                }

               
                for (int y = 0; y < rows; ++y) {
                    bool fullRow = true;
                    for (int x = 0; x < columns; ++x) {
                        if (grid[y][x] == 0) {
                            fullRow = false;
                            break;
                        }
                    }
                    if (fullRow) {
                        score += 100;
                        for (int row = y; row > 0; --row) {
                            grid[row] = grid[row - 1]; 
                        }
                        grid[0] = std::vector<int>(columns, 0);
                    }
                }

                
                currentShape = shapes[std::rand() % shapes.size()];
                currentX = columns / 2 - 2;
                currentY = 0;
                currentColor = shapeColors[std::rand() % shapeColors.size()];

                
                if (!isValidPosition(currentShape, grid, currentX, currentY)) {
                    gameOver = true;  
                }
            }
        }

        
        scoreText.setString("Score: " + std::to_string(score));

        
        window.clear(sf::Color::White);

        
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < columns; ++x) {
                if (grid[y][x] != 0) {
                    sf::RectangleShape block(sf::Vector2f(blockSize, blockSize));
                    block.setPosition(x * blockSize, y * blockSize);
                    block.setFillColor(sf::Color::Blue);
                    block.setOutlineThickness(1);
                    block.setOutlineColor(sf::Color::Black);
                    window.draw(block);
                }
            }
        }

        
        for (auto& block : currentShape) {
            sf::RectangleShape shapeBlock(sf::Vector2f(blockSize, blockSize));
            shapeBlock.setPosition((currentX + block.x) * blockSize, (currentY + block.y) * blockSize);
            shapeBlock.setFillColor(currentColor);
            shapeBlock.setOutlineThickness(1);
            shapeBlock.setOutlineColor(sf::Color::Black);
            window.draw(shapeBlock);
        }

        
        window.draw(scoreText);

        
        window.display();
    }

    return 0;
}
