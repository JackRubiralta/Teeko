
#include "teeko.hpp"
#include "h.hpp"
#include <fstream>
#include <windows.h>
#include <ctime>

typedef signed char int8; // range of -128 to +127 

const int8 TIE = 0;
const int8 WIN = 126; // also can be BLACK_WIN_SCORE
const int8 LOSE = -126; // also can be RED_WIN_SCORE
const int8 UNKNOWN = -127; // NOSCORE
const int8 ILLEGAL = -128; 

#define ILLEGAL (-128)

typedef unsigned long uint32;

struct {
    int8 table[75710250];
    int8& operator[](uint32 i) {
        return table[i - 1];
    }
} table;


void initializeTable() {
    // set all scores to UNKNOWN 
    // except for winning to positions which are -126
    // and illegal which are -128

    for (uint32 pos = 1; pos <= 75710250; pos = pos + 1) {
        table[pos] = TIE; // 0
        Teeko n = Teeko(numberToBITS[pos][0], numberToBITS[pos][1]);
        
        bool red_win = n.isWin();
        if (red_win) {
            table[pos] = -WIN; // would removing this chnage the result look at the if
            // it makes it range 0 to WIN, 
        }

        n.moveCounter += 1;
        bool black_win = n.isWin();
        if (black_win) {
            table[pos] = WIN;
        }

        if (black_win && red_win) {
            table[pos] = ILLEGAL;
        }
        
    }
}

int8 cms(const Teeko& node) {

    int8 succ;
    int8 incsucc;

    int8 result = UNKNOWN; // NOSCORE
    for (bitboard move : node.possibleMoves()) {
        Teeko child = node; child.makeMove(move); // neighbors  

        succ = table[child.key()];
        if (succ == UNKNOWN) { 
            succ = TIE;
        } else if (succ < LOSE || succ > WIN) { 
            break; // invalid
        
        }
        /* The next line turns a red score into a black score. */
        succ = -succ;
        
        incsucc = (succ == TIE) ? succ : (succ >= 0) ? succ - 1 : succ + 1;
    
        /* if (incsucc == 0 && succ != 0) lose("Ply overflow!");  */
        
        if (result == UNKNOWN) {
            result = incsucc;
        } else {
            if (result < incsucc) { result = incsucc; }
        }

    }
    //std::cout << "ppc() running" << "\n";

    return result;
}


#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBSTL "----------------------------------------------------------------------------------------------------"
#define PBWIDTH 100

void printProgress(double percentage) {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");

    fflush(stdout);
}



uint32 iteration = 1;
bool ppc() {
    clock_t sct = clock();

    bool change = false;
    printf("\nIteration of pcc: %d\n", iteration); fflush(stdout);
    uint32 changes = 0;
    iteration++;
    for (uint32 pos = 1; pos <= 75710250; pos = pos + 1) { // pos is g
        if (table[pos] >= TIE && table[pos] < WIN) {
            
            if ((pos) % 757102 == 50) {
                printProgress(((double)pos / (double)75710250));
            }
            
            const Teeko node = Teeko(numberToBITS[pos][0], numberToBITS[pos][1]);;
            int8 value = cms(node); // newscore
            
            if (value != table[pos] && value != UNKNOWN) {
                table[pos] = value;
                changes += 1;
                change = true;
            }
        }	
    }

    printProgress(1);
    printf("\nTotal changes: %d\n", changes); fflush(stdout);
    clock_t tr = clock() - sct;

    std::cout << "Time Taken: " << (float)tr / CLOCKS_PER_SEC << "s" << std::endl;    
    return change;
}


void solve() {
    genNumberToBITS();
    
    initializeTable();
    std::cout << "Solver Running!" << "\n";
    while (ppc()) {};
    
    
    std::ofstream book;
    book.open("book.txt");
    for (const int& p : table.table) {
        book << p << "\n";
    }
    book << std::endl;
    book.close();
    
}


int main() {
    //SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    //SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
    solve();
    return 0;
}




