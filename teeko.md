``
g++ -O3 pvp.cpp -o playAI; ./playAI
``
``
g++ -Ofast n.cpp -o create_book; ./create_book
``
https://pcarrier.com/teeko/archives/text/

the file n.cpp solves teeko and prodcues a book values for all of the postions 
change const GAME_MODE to play advanced teeko
the file pvp.cpp lets you play against ai or another player change const OPPONENTS
```c++
void play(bitboard move) {
    current_position ^= mask;
    mask |= move;
    moves++;
  }
````
TODO
- change teeko.hpp to use player mask and mask instead of yellow and black mask
- refactor
