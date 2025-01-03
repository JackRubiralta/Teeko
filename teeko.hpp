#include <bitset>
#include <iostream>
#include <string>
#include <vector>
#include <climits> // Include this for CHAR_BIT
#include <cstdint> // Ensure this is included for uint32_t and similar types


using bitboard = uint32_t; 

#define clz(x) __builtin_clz(x)
bitboard log2(bitboard x) {
    return sizeof(bitboard) * CHAR_BIT - clz(x) - 1;
}
const bool MOVE_PHASE = true;
const bool DROP_PHASE = false;

struct Teeko {
    public:
        static constexpr unsigned int LENGTH = 5;
        static constexpr unsigned int MAX_MOVES = 100;
        
        bitboard red;
        bitboard black;
        unsigned int moveCounter = 0;
        
        // black then red
        Teeko(const bitboard b, const bitboard r) {
            red = r;
            black = b;   
        }

        Teeko() {
            red = 0;
            black = 0;
        };

        Teeko(const Teeko& other) { 
            red = other.red;
            black = other.black;
            moveCounter = other.moveCounter;
        }

        Teeko(bool x) {
            red = 0;
            black = 0;
            moveCounter = 0;
           
            unsigned int redIndexes[4] = {14, 18, 19, 24};
            unsigned int blackIndexes[4] = {13, 15, 17, 23};
            for (unsigned int i = 0; i != 4; i++) {
                black |= (bitboard)1 << (bitboard)(blackIndexes[i] - 1); 
                red |= (bitboard)1 << (bitboard)(redIndexes[i] - 1);
            }
            std::cout << key() << std::endl;
        }

        // the move consist of a 2 bits one of the piece to be moved and the other of the destination
        void makeMove(bitboard move) {
            // black is the player who plays first
            if (currentPlayer() == 0) {
                black ^= move;
            } else {
                red ^= move;
            }
            moveCounter++;
        }

        void dropMarker(bitboard move) {
            if (currentPlayer() == 0) {
                black ^= move;
            } else {
                red ^= move;
            }
            
            moveCounter++;
        }

        bool phase() const {
            bitboard n = currentPlayer() == 0 ? black : red; 
            n = n & (n - 1);
            n = n & (n - 1);
            n = n & (n - 1);

            if (n != 0) { // still has one bit set
                return MOVE_PHASE;
            } else {
                return DROP_PHASE;
            }
        }

        

        void makeMove(unsigned int markerIndex, unsigned int destinationIndex) {
            makeMove((bitboard)1 << (markerIndex) | (bitboard)1 << (destinationIndex) );
        }

        void makeMove(unsigned int markerColumn, unsigned int markerRow, unsigned int destinationColumn, unsigned int destinationRow) {
            unsigned int markerIndex = markerRow * LENGTH + markerColumn; 
            unsigned int destinationIndex = destinationRow * LENGTH + destinationColumn; 
            makeMove(markerIndex, destinationIndex);
        }

        bitboard mask() const {
            return black | red;
        }

        unsigned int currentPlayer() const {
            return moveCounter & 1;
        }

        enum { ADVANCED = true, NORMAL = false };
        const bool GAME_MODE = ADVANCED;
        bool isWin() const {
            bitboard position;
            if (currentPlayer() == 0) { position = red; } else { position = black; }

            bitboard m;

            // horizontal
            m = position & (position >> 1) & (position >> 2) & (position >> 3);
            if ((m & 0b0001100011000110001100011) != 0) {
                return true;
            }

            // vertical
            m = position & (position >> 5) & (position >> 10) & (position >> 15);
            if ((m & 0b0000000000000001111111111) != 0) {
                return true;
            }

            // diagonal 1
            m = position & (position >> 6) & (position >> 12) & (position >> 18);
            if ((m & 0b0001100011000110001100011) != 0) {
                return true;
            }

            // diagonal 2
            m = position & (position >> 4) & (position >> 8) & (position >> 12);
            if ((m & 0b1100011000110001100011000) != 0) {
                return true;
            }
            
            // 2x2 square
            m = position & (position >> 1) & (position >> 5) & (position >> 6);
            if ((m & 0b01111011110111101111011110111101111011110111101111) != 0) {
                return true;
            }

            // 3x3 need wor
            
            if (GAME_MODE) {
                m = position & (position >> 2) & (position >> 10) & (position >> 12);
                if ((m & 0b00111001110011100111001110011100111001110011100111) != 0) {
                    return true;
                }

                // 4x4 need work
                m = position & (position >> 3) & (position >> 15) & (position >> 18);
                if ((m & 0b00011000110001100011000110001100011000110001100011) != 0) {
                    return true;
                }
            
                // 5x5 square corners - This is the whole board so we can just check the corners directly
                if ((position & 17825809) == 17825809) {
                    return true;
                }
            }

            return false;
        };


        bool isOldWin() const { // normal teeko
            bitboard position;
            if (currentPlayer() == 0) { position = red; } else { position = black; }

            bitboard m;
            m = position & (position >> 1) & (position >> 2) & (position >> 3);
            if ((m & 0b0001100011000110001100011) != 0) {
                return true;
            }

            // vertical
            m = position & (position >> 5) & (position >> 10) & (position >> 15);
            if ((m & 0b0000000000000001111111111) != 0) {
                return true;
            }

            // diagonal 1
            m = position & (position >> 6) & (position >> 12) & (position >> 18);
            if ((m & 0b0001100011000110001100011) != 0) {
                return true;
            }

            m = position & (position >> 4) & (position >> 8) & (position >> 12);
            if ((m & 0b1100011000110001100011000) != 0) {
                return true;
            }
            
            // square
            m = position & (position >> 1) & (position >> 5) & (position >> 6);
            if ((m & 0b01111011110111101111011110111101111011110111101111) != 0) {
                return true;
            }

            return false;
        };

        bool isDraw() const {
            return moveCounter >= MAX_MOVES;
        }

        bool isGameOver() const {
            return isWin() || isDraw();
        }

        std::vector<bitboard> possibleMoves() const {
            std::vector<bitboard> moves = std::vector<bitboard>();

            bitboard markers = currentPlayer() == 0 ? black : red; 
            const bitboard unoccupied = ~mask();

           for (unsigned int i = 0; i <= 7; i++) {
                bitboard marker = markers ^ (markers & (markers - 1));
                markers ^= marker;

                bitboard move;    
                move = ((marker << 1) & 0b1111011110111101111011110) & unoccupied;
                if ((move & boardMask) != 0) { moves.push_back(move | marker); };
                move = ((marker << 6) & 0b1111011110111101111011110) & unoccupied;
                if ((move & boardMask) != 0) { moves.push_back(move | marker); };
                move = ((marker >> 4) & 0b1111011110111101111011110) & unoccupied;
                if ((move & boardMask) != 0) { moves.push_back(move | marker); };
                move = ((marker >> 1) & 0b0111101111011110111101111) & unoccupied;
                if ((move & boardMask) != 0) { moves.push_back(move | marker); };
                move = ((marker >> 6) & 0b0111101111011110111101111) & unoccupied;
                if ((move & boardMask) != 0) { moves.push_back(move | marker); };
                move = ((marker << 4) & 0b0111101111011110111101111) & unoccupied;
                if ((move & boardMask) != 0) { moves.push_back(move | marker); };
                move = (marker >> 5) & unoccupied; // south
                if ((move & boardMask) != 0) { moves.push_back(move | marker); };
                move = (marker << 5) & unoccupied; // north 
                if ((move & boardMask) != 0) { moves.push_back(move | marker); };
            }      
            return moves;      
        }

                
        std::vector<bitboard> possibleDrops() const {
    const bitboard board_mask = 0b1111111111111111111111111; // Mask for all positions on the board
    bitboard empty_positions = mask() ^ board_mask; // Calculate positions that are not occupied

    // If it's the first move, exclude the center position from possible drops
    if (GAME_MODE) {
    if (moveCounter == 0) {
        empty_positions &= ~bitboard(4096); // Remove center position by clearing the bit at position 12
    }
    }

    std::vector<bitboard> possible_drops;
    while (empty_positions != 0) {
        bitboard current_position = empty_positions & (-empty_positions); // Get the lowest bit set
        possible_drops.push_back(current_position);
        empty_positions &= (empty_positions - 1); // Clear the lowest bit set
    }
    return possible_drops;
}



        uint64_t key() const {
            // https://stackoverflow.com/questions/31393100/how-to-get-position-of-right-most-set-bit-in-c
            
            uint64_t pos[8];

            bitboard markers = currentPlayer() == 0 ? black : red; 
            for (unsigned int i = 0; i <= 3; i++) {
                bitboard marker = markers ^ (markers & (markers - 1));
                markers ^= marker;
                pos[i] = log2(marker) + 1;
                //std::cout << "pos" << i << " " << (pos[i]) << "\n";
            }

            markers = currentPlayer() == 1 ? black : red; 
            for (unsigned int i = 4; i <= 7; i++) {
                bitboard marker = markers ^ (markers & (markers - 1));
                markers ^= marker;
                pos[i] = log2(marker) + 1;
                //std::cout << "pos" << i << " " << (pos[i]) << "\n";
            }
            
            
            uint64_t t = 25;
            uint64_t k1 = (((t - 3) + 0) * ((t - 3) + 1) * ((t - 3) + 2) * ((t - 3) + 3)) / (4 * 3 * 2 * 1) - ((((t - 2) - pos[0]) + 0) * (((t - 2) - pos[0]) + 1) * (((t - 2) - pos[0]) + 2) * (((t - 2) - pos[0]) + 3)) / (4 * 3 * 2 * 1);
            uint64_t k2 = ((((t - 2) - pos[0]) + 0) * (((t - 2) - pos[0]) + 1) * (((t - 2) - pos[0]) + 2)) / (3 * 2 * 1) - (((((t - 1) - pos[0]) - (pos[1] - pos[0])) + 0) * ((((t - 1) - pos[0]) - (pos[1] - pos[0])) + 1) * ((((t - 1) - pos[0]) - (pos[1] - pos[0])) + 2)) / (3 * 2 * 1);
            uint64_t k3 = ((((t - 1) - pos[1]) + 0) * (((t - 1) - pos[1]) + 1)) / (2 * 1) - (((((t) - pos[1]) - (pos[2] - pos[1])) + 0) * ((((t) - pos[1]) - (pos[2] - pos[1])) + 1)) / (2 * 1);
            uint64_t k4 = (pos[3] - pos[2]);

            uint64_t m1 = pos[4];
            uint64_t m2 = pos[5];
            uint64_t m3 = pos[6];
            uint64_t m4 = pos[7];
            if (pos[4] >= pos[0]) { m1--; } if ( pos[4] >= pos[1] ) {m1--; } if ( pos[4] >= pos[2] )  {m1--; } if ( pos[4] >= pos[3] )  {m1--; } 
            if (pos[5] >= pos[0]) { m2--; } if ( pos[5] >= pos[1] ) {m2--; } if ( pos[5] >= pos[2] )  {m2--; } if ( pos[5] >= pos[3] )  {m2--; } 
            if (pos[6] >= pos[0]) { m3--; } if ( pos[6] >= pos[1] ) {m3--; } if ( pos[6] >= pos[2] )  {m3--; } if ( pos[6] >= pos[3] )  {m3--; } 
            if (pos[7] >= pos[0]) { m4--; } if ( pos[7] >= pos[1] ) {m4--; } if ( pos[7] >= pos[2] )  {m4--; } if ( pos[7] >= pos[3] )  {m4--; } 
            // other color
            //std::cout << "m1: " << m1 << "  m2: " << m2 << "  m3: " << m3 << "  m4: " << m4 << "\n";
            
            uint64_t s = 21;
            uint64_t j1 = (((s - 3) + 0) * ((s - 3) + 1) * ((s - 3) + 2) * ((s - 3) + 3)) / (4 * 3 * 2 * 1) - ((((s - 2) - m1) + 0) * (((s - 2) - m1) + 1) * (((s - 2) - m1) + 2) * (((s - 2) - m1) + 3)) / (4 * 3 * 2 * 1);
            uint64_t j2 = ((((s - 2) - m1) + 0) * (((s - 2) - m1) + 1) * (((s - 2) - m1) + 2)) / (3 * 2 * 1) - (((((s - 1) - m1) - (m2 - m1)) + 0) * ((((s - 1) - m1) - (m2 - m1)) + 1) * ((((s - 1) - m1) - (m2 - m1)) + 2)) / (3 * 2 * 1);
            uint64_t j3 = ((((s - 1) - m2) + 0) * (((s - 1) - m2) + 1)) / (2 * 1) - (((((s) - m2) - (m3 - m2)) + 0) * ((((s) - m2) - (m3 - m2)) + 1)) / (2 * 1);
            uint64_t j4 = (m4 - m3);


            
            uint64_t kpos = k1 + k2 + k3 + k4;
            uint64_t jpos = j1 + j2 + j3 + j4;
            //std::cout << "kpos" << kpos << std::endl;
            //std::cout << "jpos" << jpos << std::endl;
            
                                            
            return ((kpos - 1) * 5985) + jpos;
        }

        void print() const {
            const std::string verticalSeperator = "\u001b[36m|\u001b[0m";
                                                                                              
            const std::string horizontalSeperator = "\u001b[36m-------------------------------\u001b[0m";
            const std::string redMarker = "\u001b[31;1m R \u001b[0m";
            const std::string blackMarker = "\u001b[30;1m B \u001b[0m";
            std::string arrayboard[25] = {" - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - "};
            

            for (int i = 0; i < 25; i++) {
                if ((((bitboard)1 << i) & black) != 0) {
                    arrayboard[i] = blackMarker;
                }
                if ((((bitboard)1 << i) & red) != 0) {
                    arrayboard[i] = redMarker;
                }
                for (const bitboard move : possibleMoves()) {
                    if ((((bitboard)1 << i) & (move & ~mask())) != 0) {
                        //arrayboard[i] = "\u001b[35;1m + \u001b[0m";
                    }
                } 
            }
            
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << arrayboard[20] << " " << verticalSeperator << " " << arrayboard[21] << " " << verticalSeperator << " " << arrayboard[22] << " " << verticalSeperator << " " << arrayboard[23] << " " << verticalSeperator << " " << arrayboard[24] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << arrayboard[15] << " " << verticalSeperator << " " << arrayboard[16] << " " << verticalSeperator << " " << arrayboard[17] << " " << verticalSeperator << " " << arrayboard[18] << " " << verticalSeperator << " " << arrayboard[19] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << arrayboard[10] << " " << verticalSeperator << " " << arrayboard[11] << " " << verticalSeperator << " " << arrayboard[12] << " " << verticalSeperator << " " << arrayboard[13] << " " << verticalSeperator << " " << arrayboard[14] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << arrayboard[5] << " " << verticalSeperator << " " << arrayboard[6] << " " << verticalSeperator << " " << arrayboard[7] << " " << verticalSeperator << " " << arrayboard[8] << " " << verticalSeperator << " " << arrayboard[9] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << arrayboard[0] << " " << verticalSeperator << " " << arrayboard[1] << " " << verticalSeperator << " " << arrayboard[2] << " " << verticalSeperator << " " << arrayboard[3] << " " << verticalSeperator << " " << arrayboard[4] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            //std::cout << verticalSeperator << " " << " - " << " " << verticalSeperator << " " << "\u001b[34m 0 \u001b[0m" << " " << verticalSeperator << " " << "\u001b[34m 1 \u001b[0m" << " " << verticalSeperator << " " << "\u001b[34m 2 \u001b[0m" << " " << verticalSeperator << " " << "\u001b[34m 3 \u001b[0m" << " " << verticalSeperator << " " << "\u001b[34m 4 \u001b[0m" << " " << verticalSeperator << std::endl;
            //std::cout << horizontalSeperator << std::endl;
        };

        void print(bitboard marker) const {
            const std::string verticalSeperator = "\u001b[36m|\u001b[0m";
            //const std::string horizontalSeperator = "\u001b[36m-------------------------------------\u001b[0m";
            const std::string horizontalSeperator = "\u001b[36m-------------------------------\u001b[0m";
            const std::string redMarker = "\u001b[31;1m R \u001b[0m";
            const std::string blackMarker = "\u001b[30;1m B \u001b[0m";
            std::string arrayboard[25] = {" - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - ", " - "};
            

            for (int i = 0; i < 25; i++) {
                if ((((bitboard)1 << i) & black) != 0) {
                    arrayboard[i] = blackMarker;
                }
                if ((((bitboard)1 << i) & red) != 0) {
                    arrayboard[i] = redMarker;
                }
                if ((((bitboard)1 << i) & (marker)) != 0) {
                    arrayboard[i] = "\u001b[46m" + arrayboard[i]; 
                    
                }
                for (const bitboard move : possibleMoves()) {
                        if (((((bitboard)1 << i) & ((move) & ~mask())) != 0)) {
                            if (((((bitboard)1 << i) ^ move) & marker) != 0) {
                                arrayboard[i] = "\u001b[35;1m + \u001b[0m";
                            }
                        }
                    } 

                
            }
            
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << arrayboard[20] << " " << verticalSeperator << " " << arrayboard[21] << " " << verticalSeperator << " " << arrayboard[22] << " " << verticalSeperator << " " << arrayboard[23] << " " << verticalSeperator << " " << arrayboard[24] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << arrayboard[15] << " " << verticalSeperator << " " << arrayboard[16] << " " << verticalSeperator << " " << arrayboard[17] << " " << verticalSeperator << " " << arrayboard[18] << " " << verticalSeperator << " " << arrayboard[19] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << arrayboard[10] << " " << verticalSeperator << " " << arrayboard[11] << " " << verticalSeperator << " " << arrayboard[12] << " " << verticalSeperator << " " << arrayboard[13] << " " << verticalSeperator << " " << arrayboard[14] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << arrayboard[5] << " " << verticalSeperator << " " << arrayboard[6] << " " << verticalSeperator << " " << arrayboard[7] << " " << verticalSeperator << " " << arrayboard[8] << " " << verticalSeperator << " " << arrayboard[9] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << arrayboard[0] << " " << verticalSeperator << " " << arrayboard[1] << " " << verticalSeperator << " " << arrayboard[2] << " " << verticalSeperator << " " << arrayboard[3] << " " << verticalSeperator << " " << arrayboard[4] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            /*
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << "\u001b[34m 4 \u001b[0m" << " " << verticalSeperator << " " << arrayboard[20] << " " << verticalSeperator << " " << arrayboard[21] << " " << verticalSeperator << " " << arrayboard[22] << " " << verticalSeperator << " " << arrayboard[23] << " " << verticalSeperator << " " << arrayboard[24] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << "\u001b[34m 3 \u001b[0m" << " " << verticalSeperator << " " << arrayboard[15] << " " << verticalSeperator << " " << arrayboard[16] << " " << verticalSeperator << " " << arrayboard[17] << " " << verticalSeperator << " " << arrayboard[18] << " " << verticalSeperator << " " << arrayboard[19] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << "\u001b[34m 2 \u001b[0m" << " " << verticalSeperator << " " << arrayboard[10] << " " << verticalSeperator << " " << arrayboard[11] << " " << verticalSeperator << " " << arrayboard[12] << " " << verticalSeperator << " " << arrayboard[13] << " " << verticalSeperator << " " << arrayboard[14] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << "\u001b[34m 1 \u001b[0m" << " " << verticalSeperator << " " << arrayboard[5] << " " << verticalSeperator << " " << arrayboard[6] << " " << verticalSeperator << " " << arrayboard[7] << " " << verticalSeperator << " " << arrayboard[8] << " " << verticalSeperator << " " << arrayboard[9] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << "\u001b[34m 0 \u001b[0m" << " " << verticalSeperator << " " << arrayboard[0] << " " << verticalSeperator << " " << arrayboard[1] << " " << verticalSeperator << " " << arrayboard[2] << " " << verticalSeperator << " " << arrayboard[3] << " " << verticalSeperator << " " << arrayboard[4] << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            std::cout << verticalSeperator << " " << " - " << " " << verticalSeperator << " " << "\u001b[34m 0 \u001b[0m" << " " << verticalSeperator << " " << "\u001b[34m 1 \u001b[0m" << " " << verticalSeperator << " " << "\u001b[34m 2 \u001b[0m" << " " << verticalSeperator << " " << "\u001b[34m 3 \u001b[0m" << " " << verticalSeperator << " " << "\u001b[34m 4 \u001b[0m" << " " << verticalSeperator << std::endl;
            std::cout << horizontalSeperator << std::endl;
            */

        
          
        };

        static constexpr bitboard indexMask(unsigned int index) {
            return (bitboard)1 << index;
        };

        static constexpr bitboard boardMask = 0b1111111111111111111111111;
};
