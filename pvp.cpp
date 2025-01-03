#include "teeko.hpp"
#include <conio.h>//For _getch().
#include <fstream>
#include <algorithm>
#include <random>
std::random_device rd;  // Obtain a random number from hardware
std::mt19937 rng(rd()); // Seed the generator
std::uniform_int_distribution<std::mt19937::result_type> dist(0,1); // Define the range

//https://stackoverflow.com/questions/24708700/c-detect-when-user-presses-arrow-key
#define KEY_UP 72       //Up arrow character
#define KEY_DOWN 80    //Down arrow character
#define KEY_LEFT 75       //Up arrow character
#define KEY_RIGHT 77    //Down arrow character
#define KEY_ENTER '\r'//Enter key charatcer

typedef signed char int8; // range of -128 to +127 
const int8 TIE = 0;
const int8 WIN = 126; // also can be BLACK_WIN_SCORE
const int8 LOSE = -126; // also can be RED_WIN_SCORE
const int8 UNKNOWN = -127; // NOSCORE
const int8 ILLEGAL = -128; 
int8 book[75710250];  

void load_book() {
    std::ifstream book_file("book.txt");

    int value;
    uint32_t key;
    key = 0;
    while (book_file >> value) {
        book[key] = value;
        key++;
    }
    book_file.close();
}
int negamax(const Teeko &node, int alpha, int beta) {
    if (node.phase() == MOVE_PHASE) {
        return book[node.key() - 1];
    }

    if (node.isWin()) {
        return -WIN;
    }
   
    int bestValue = LOSE;
    for (bitboard move : node.possibleDrops()) {
        Teeko child = node; 
        child.dropMarker(move);  

        int value = -negamax(child, -beta, -alpha);
        
        if (value > bestValue) {
            bestValue = value;
        }
        alpha = std::max(alpha, bestValue);
        if (alpha >= beta) {
            break;
        }
    }
    return bestValue;
}

int solve(Teeko node) {
    if (node.isWin()) {
        return -WIN;
    }
    if (node.phase() == MOVE_PHASE) {
        return book[node.key() - 1];
    } else {
        return negamax(node, LOSE, WIN);
    }
}

int solve2(Teeko node) {
    //if (solve(node) == 0)
    return 1;
}

bitboard bestMove(Teeko node) {
    int bestValue = LOSE;
    bitboard bestMove;
    if (node.phase() == MOVE_PHASE) {
        for (bitboard move : node.possibleMoves()) {
            Teeko child = node; 
            child.makeMove(move);  
            int value = -solve(child);
            
            if ((bestValue == value)) {
                if (rand() & 1) {
                    bestMove = move;
                }
            } else if (value > bestValue) {
                bestValue = value;
                bestMove = move;
            }
            

            

        }
        return bestMove;
    } else {
        for (bitboard move : node.possibleDrops()) {
            Teeko child = node; 
            child.dropMarker(move);  
            int value = -solve(child);
            

            
            if (value > bestValue) {
                bestValue = value;
                bestMove = move;
            } else if (value == bestValue) {
                // Randomly decide whether to update bestMove or not
                if (dist(rng) == 1) {
                    bestMove = move;
                }
            }
            
        }
        return bestMove;
    }

}


enum { PLAYER_VS_AI = false, PLAYER_VS_PLAYER = true };
const bool OPPONENTS = PLAYER_VS_AI;

void playAgainst() { 
    Teeko game = Teeko();
    while (!(game.isWin())) {
            

        unsigned int markerColumn, markerRow, destinationColumn, destinationRow;



        //int value = solve(game);
        if (OPPONENTS || game.currentPlayer() == 1) { // change false to true to change to player vs player
            bool selecting = true;

           
            system("CLS");

            game.print();
            int value = 0;
            if (OPPONENTS == PLAYER_VS_AI) {
                int value = solve(game);
            }

            if (value > 0) {
                std::cout << "Winning in: " << int((WIN - value) / 2) << " moves" << std::endl;
            } else if (value < 0) {
                std::cout << "Losing in: " << int((-(LOSE - value)) / 2) << " moves" << std::endl;
            } else {
                std::cout << "Cant force a win" << std::endl;
            }
            std::cout << (game.currentPlayer() == 0 ? "\u001b[30;1mBlack\u001b[0m" : "\u001b[31;1mRed\u001b[0m") << " player enter move: " << std::endl;


            std::cout << "\x1b[A" << "\x1b[A" << "\x1b[A" << "\x1b[A" << "\x1b[A" << "\x1b[A" << "\x1b[A" << "\x1b[A";
            std::cout << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C"  << "\x1b[C";
            //std::cout << "value: " << value << std::endl;
            bool selectingMarker = true;
            unsigned int x = 2; unsigned int y = 2; 
            while (selecting) { //As long as we are selecting...
                char c;
                switch ((c = _getch())) { //Check value of the last inputed character.
                    case KEY_UP:
                        if (y != 4) {
                            y++;
                            std::cout << "\x1b[A" << "\x1b[A"; // ESC[#A
                        }	
                        break;
                    case KEY_DOWN:
                        if (y != 0) {
                            y--;
                            std::cout << "\x1b[B" << "\x1b[B"; // ESC[#B // # is number of lines	
                        }
                        break;
                    
                    case KEY_RIGHT:
                        if (x != 4) {
                            x++;
                            std::cout << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C"; // ESC[#A	
                        }
                        break;
                    case KEY_LEFT:
                        if (x != 0) { 
                            x--;
                            std::cout << "\x1b[D" << "\x1b[D" << "\x1b[D" << "\x1b[D" << "\x1b[D" << "\x1b[D"; // ESC[#B // # is number of lines	
                        }
                        break;
                    case KEY_ENTER:
                        if (game.phase() == DROP_PHASE) {
                            bitboard combinedDrops = 0;
                            std::vector<bitboard> possibleDrops = game.possibleDrops();
                            // Loop through each possible drop and combine it using bitwise OR
                            for (const bitboard& drop : possibleDrops) {
                                combinedDrops |= drop;
                            } 
                            if (combinedDrops & ((bitboard)1 << (bitboard)(y * Teeko::LENGTH + x))) {
                                markerColumn = x;
                                markerRow = y;
                                selecting = false; 
                            }
                            break;  
                        } else {
                            if (selectingMarker) {
                                if ((game.currentPlayer() == 0 ? game.black : game.red) & ((bitboard)1 << (bitboard)(y * Teeko::LENGTH + x))) {
                                    markerColumn = x;
                                    markerRow = y;
                                    selectingMarker = false;    

                                    system("CLS");
                                    game.print(((bitboard)1 << (bitboard)(markerRow * Teeko::LENGTH + markerColumn)));                         
                                    if (value > 0) {
                                        std::cout << "Winning in: " << int((WIN - value) / 2) << " moves" << std::endl;
                                    } else if (value < 0) {
                                        std::cout << "Losing in: " << int((-(LOSE - value)) / 2) << " moves" << std::endl;
                                    } else {
                                        std::cout << "Cant force a win" << std::endl;
                                    }
                                    std::cout << (game.currentPlayer() == 0 ? "\u001b[30;1mBlack\u001b[0m" : "\u001b[31;1mRed\u001b[0m") << " player enter move: " << std::endl;

                                    std::cout << "\x1b[A" << "\x1b[A" << "\x1b[A" << "\x1b[A";
                                    std::cout << "\x1b[C" << "\x1b[C" << "\x1b[C";
                                    for (int i = 0; i != x; i++) {
                                        std::cout << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C" << "\x1b[C"; // ESC[#A	
                                    }
                                    for (int i = 0; i != y; i++) {
                                        std::cout << "\x1b[A" << "\x1b[A";                                    
                                    }
                                }
                                
                                break;  
                            } else {
                                std::vector<bitboard> possible_moves = game.possibleMoves();
                                bitboard bit_des = ((bitboard)1 << (bitboard)(y * Teeko::LENGTH + x)) | ((bitboard)1 << (bitboard)(markerRow * Teeko::LENGTH + markerColumn));
                                if (std::find(possible_moves.begin(), possible_moves.end(), bit_des) != possible_moves.end()) {
                                    destinationColumn = x;
                                    destinationRow = y;
                                    selecting = false;  
                                }
                                break;
                            }
                        }
                        

                        break;
                    default: break;
                }
            }
            if (game.phase() == DROP_PHASE) {
                unsigned int markerIndex = markerRow * Teeko::LENGTH + markerColumn; 

                game.dropMarker((bitboard)1 << (markerIndex));
            } else {

                game.makeMove(markerColumn, markerRow, destinationColumn, destinationRow);
            }
                        //return 0;

        } else {
            game.makeMove(bestMove(game));
        }
        
        

    }
    
    system("CLS");
    game.print();

    std::cout << "Player " << (game.currentPlayer() ^ 1) << " wins!" << std::endl;
    
}

int main() {
    char playAgain = 'y'; // Initialize playAgain with 'y' to enter the loop at least once

    if (OPPONENTS == PLAYER_VS_AI) {
       load_book(); // Load your game book if playing against AI, assuming this is done once at the start
    }

    do {
        playAgainst(); // Assuming playAgainst() contains the gameplay loop

        // After a game is finished, ask if the player wants to play again
        std::cout << "Play again? (y/n): ";
        std::cin >> playAgain;
        playAgain = tolower(playAgain); // Normalize input to lowercase to simplify the comparison

        if(playAgain != 'y') {
            break; // Exit the loop if the player doesn't want to play again
        }

        // Optionally clear the screen or reset game state if necessary
        // For Windows use system("CLS"), for Unix/Linux use system("clear")
    } while (true); // Continue this loop until broken explicitly

    return 0;
}