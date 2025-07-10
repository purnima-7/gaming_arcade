#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "mancala.h"
#include "ai.h"

// Game states
enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

// Button class for menu interface
class Button {
public:
    Button(float x, float y, float width, float height, const std::string& text, const sf::Font& font) {
        shape.setPosition(x, y);
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color(200, 200, 255));
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::Black);
        
        this->text.setFont(font);
        this->text.setString(text);
        this->text.setCharacterSize(20);
        this->text.setFillColor(sf::Color::Black);
        
        // Center text in button
        sf::FloatRect textBounds = this->text.getLocalBounds();
        this->text.setPosition(
            x + (width - textBounds.width) / 2,
            y + (height - textBounds.height) / 2 - 5 // Small adjustment for visual alignment
        );
    }
    
    bool isMouseOver(sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        
        return shape.getGlobalBounds().contains(mousePosF);
    }
    
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
    
    void setHighlight(bool highlight) {
        if (highlight) {
            shape.setFillColor(sf::Color(150, 150, 255));
        } else {
            shape.setFillColor(sf::Color(200, 200, 255));
        }
    }
    
private:
    sf::RectangleShape shape;
    sf::Text text;
};

int main() {
    // Create the game window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mancala Game");
    window.setFramerateLimit(60);
    
    // Load font
    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cout << "Error loading font. Using system default." << std::endl;
        // Continue without a font - SFML will use a default font
    }
    
    // Game objects
    MancalaGame game;
    MancalaAI ai(3); // Medium difficulty AI
    
    // Menu buttons
    Button easyButton(300, 200, 200, 50, "Easy AI", font);
    Button mediumButton(300, 275, 200, 50, "Medium AI", font);
    Button hardButton(300, 350, 200, 50, "Hard AI", font);
    Button quitButton(300, 425, 200, 50, "Quit", font);
    
    // Game state
    GameState state = GameState::MENU;
    int aiDifficulty = 3; // Default medium
    
    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Handle mouse clicks
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (state == GameState::MENU) {
                    // Handle menu button clicks
                    if (easyButton.isMouseOver(window)) {
                        aiDifficulty = 1;
                        ai.setDifficulty(aiDifficulty);
                        game = MancalaGame(); // Reset game
                        state = GameState::PLAYING;
                    }
                    else if (mediumButton.isMouseOver(window)) {
                        aiDifficulty = 3;
                        ai.setDifficulty(aiDifficulty);
                        game = MancalaGame(); // Reset game
                        state = GameState::PLAYING;
                    }
                    else if (hardButton.isMouseOver(window)) {
                        aiDifficulty = 5;
                        ai.setDifficulty(aiDifficulty);
                        game = MancalaGame(); // Reset game
                        state = GameState::PLAYING;
                    }
                    else if (quitButton.isMouseOver(window)) {
                        window.close();
                    }
                }
                else if (state == GameState::PLAYING) {
                    // Handle game board clicks
                    if (game.isPlayer1Turn()) { // Human's turn
                        int mouseX = event.mouseButton.x;
                        int mouseY = event.mouseButton.y;
                        int selectedPit = game.getPitFromMousePosition(mouseX, mouseY);
                        
                        if (selectedPit >= 0 && game.isValidMove(selectedPit)) {
                            bool extraTurn = game.makeMove(selectedPit);
                            
                            // Check if game is over after human move
                            if (game.isGameOver()) {
                                state = GameState::GAME_OVER;
                            }
                            else if (!extraTurn && !game.isPlayer1Turn()) {
                                // AI's turn after human move
                                sf::sleep(sf::milliseconds(500)); // Small delay before AI move
                                processAIMove(game, ai);
                                
                                // Check if game is over after AI move
                                if (game.isGameOver()) {
                                    state = GameState::GAME_OVER;
                                }
                            }
                        }
                    }
                }
                else if (state == GameState::GAME_OVER) {
                    // Click anywhere to return to menu
                    state = GameState::MENU;
                }
            }
        }
        
        // Render
        window.clear(sf::Color(240, 240, 240));
        
        if (state == GameState::MENU) {
            // Draw menu title
            sf::Text titleText;
            titleText.setFont(font);
            titleText.setString("Mancala Game");
            titleText.setCharacterSize(40);
            titleText.setFillColor(sf::Color::Black);
            titleText.setPosition(270, 100);
            window.draw(titleText);
            
            // Draw subtitle
            sf::Text subtitleText;
            subtitleText.setFont(font);
            subtitleText.setString("Select Difficulty:");
            subtitleText.setCharacterSize(24);
            subtitleText.setFillColor(sf::Color::Black);
            subtitleText.setPosition(310, 160);
            window.draw(subtitleText);
            
            // Update button highlights
            easyButton.setHighlight(easyButton.isMouseOver(window));
            mediumButton.setHighlight(mediumButton.isMouseOver(window));
            hardButton.setHighlight(hardButton.isMouseOver(window));
            quitButton.setHighlight(quitButton.isMouseOver(window));
            
            // Draw buttons
            easyButton.draw(window);
            mediumButton.draw(window);
            hardButton.draw(window);
            quitButton.draw(window);
        }
        else if (state == GameState::PLAYING || state == GameState::GAME_OVER) {
            // Draw the game board
            game.displayBoard(window, font);
            
            if (state == GameState::GAME_OVER) {
                // Draw "click to continue" message
                sf::Text continueText;
                continueText.setFont(font);
                continueText.setString("Click anywhere to return to the menu");
                continueText.setCharacterSize(20);
                continueText.setFillColor(sf::Color::Blue);
                continueText.setPosition(250, 500);
                window.draw(continueText);
            }
        }
        
        window.display();
        
        // If it's AI's turn in playing state, process AI move
        if (state == GameState::PLAYING && !game.isPlayer1Turn()) {
            processAIMove(game, ai);
            
            // Check if game is over after AI move
            if (game.isGameOver()) {
                state = GameState::GAME_OVER;
            }
        }
    }
    
    return 0;
}

// Helper function to process AI moves
void processAIMove(MancalaGame& game, MancalaAI& ai) {
    // Find and execute the AI's best move
    int aiMove = ai.findBestMove(game);
    
    if (aiMove >= 0) {
        // Add a small delay to make the AI move visible
        sf::sleep(sf::milliseconds(500));
        bool extraTurn = game.makeMove(aiMove);
        
        // If AI gets an extra turn, it will continue on the next loop iteration
    }
}