/*
//        _____                    _____                    _____                    _____          
//       /\    \                  /\    \                  /\    \                  /\    \         
//      /::\    \                /::\    \                /::\    \                /::\    \        
//      \:::\    \              /::::\    \               \:::\    \              /::::\    \       
//       \:::\    \            /::::::\    \               \:::\    \            /::::::\    \      
//        \:::\    \          /:::/\:::\    \               \:::\    \          /:::/\:::\    \     
//         \:::\    \        /:::/__\:::\    \               \:::\    \        /:::/__\:::\    \    
//         /::::\    \      /::::\   \:::\    \              /::::\    \       \:::\   \:::\    \   
//        /::::::\    \    /::::::\   \:::\    \    ____    /::::::\    \    ___\:::\   \:::\    \  
//       /:::/\:::\    \  /:::/\:::\   \:::\____\  /\   \  /:::/\:::\    \  /\   \:::\   \:::\    \ 
//      /:::/  \:::\____\/:::/  \:::\   \:::|    |/::\   \/:::/  \:::\____\/::\   \:::\   \:::\____\
//     /:::/    \::/    /\::/   |::::\  /:::|____|\:::\  /:::/    \::/    /\:::\   \:::\   \::/    /
//    /:::/    / \/____/  \/____|:::::\/:::/    /  \:::\/:::/    / \/____/  \:::\   \:::\   \/____/ 
//   /:::/    /                 |:::::::::/    /    \::::::/    /            \:::\   \:::\    \     
//  /:::/    /                  |::|\::::/    /      \::::/____/              \:::\   \:::\____\    
//  \::/    /                   |::| \::/____/        \:::\    \               \:::\  /:::/    /    
//   \/____/                    |::|  ~|               \:::\    \               \:::\/:::/    /     
//                              |::|   |                \:::\    \               \::::::/    /      
//                              \::|   |                 \:::\____\               \::::/    /       
//                               \:|   |                  \::/    /                \::/    /        
//                                \|___|                   \/____/                  \/____/         
//                                                                                                  
*/
// C++ Tris by TheSast, last edited on 2023-03-08

#include "replacement_lib.h"
#include "rssyntax_230304.h"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <ostream>
#include <stdlib.h>
#include <string>
#include <thread>
#include <time.h>
#include <vector>

using std::vector;

#define titleCardCount 6
#define titleCardSize 24
#define titleCardBreaker "⚠"

enum Turn : u8 { PLYR1, PLYR2 };
enum Cell : u8 { EMPTY__CELL, PLYR1__CELL, PLYR2__CELL };
typedef Vec<Vec<Cell>> Board;
typedef Vec<Vec<u32>> Chart;
struct Location {
    u32 x;
    u32 y;

    Location() {
        self.x = 0;
        self.y = 0;
    }

    Location(const u32 newX, const u32 newY) {
        self.x = newX;
        self.y = newY;
    }
};


struct GameBoard {
    Board content;
    struct symbols { char p1 = 'X'; char p2 = 'O';} symbols;

    GameBoard(const u32 &boardWidth, const u32 &boardHeight, const Cell &fillValue = Cell::EMPTY__CELL, Vec<char> playerSymbols = { 'X', 'Y'}) {
        self.content.assign(boardHeight, std::Vec<Cell>(boardWidth, fillValue));
        if(playerSymbols.size() != 2) { rss::panic("Board constructor necessitates Vec<Char> of exactly 2 elements, or omission of it"); }
        self.symbols.p1 = playerSymbols[0];
        self.symbols.p2 = playerSymbols[1];
    };

  fn void display() const {
    for (auto r = self.content.rbegin(); r != self.content.rend(); r += 1) {
      for (auto c = (*r).begin(); c != (*r).end(); c += 1) {
        let &cellToDisplay = *c;
        // print(" ");
        switch (cellToDisplay) {
          case Cell::PLYR1__CELL:
            print("%c", self.symbols.p1);
            break;
          case Cell::PLYR2__CELL:
            print("%c", self.symbols.p2);
            break;
          case Cell::EMPTY__CELL:
            print("%c", ' ');
            break;
        }
        if (c != (*r).end() - 1) { print(" │ "); }
      }
      if (r != self.content.rend() - 1) {
        print("\n─");
        for (auto c = (*r).begin(); c != (*r).end() - 1; c += 1) { print("─┼──"); }
        print("\n");
      }
    }
    std::cout << std::flush;
  }
};

void displayTitlecard();
void displayTurn(const Turn, const bool, const Vec<char> = {'X', 'O'});
void undisplayHistory(const u32, const bool);
Location comOppEvaluate(const Board, const Turn comTurn, const u32 trisMagnetism = 2);
void playerControl(Board&, const Turn);
void comControl(Board&, const Turn, const Location);
bool checkForTrisOrTie(const GameBoard, u32 trisSize, const Turn, const bool, const bool);

fn i32 main(i32 argc, char *argv[]) {
    // Prepare program
    srand(static_cast<u32>(time(NULL)));
    let_mut turn = Turn::PLYR1;
    let_mut gameOver = false;
    let_mut historyEnabled = DEBUG_MODE;
    let_mut comOppOnly = false;
    let_mut comOppEnabled = false;
    let_mut haveDelay = false;
    var u32 comOppStrength = 2; 
    var u32 trisLength = 3;
    var u32 bHeigth = 3;
    var u32 bWidth = 3;
    var Vec<char> playerSymbols = { 'X', 'O' };
    

    // Handle CL parameters
    for (i32 argvIndex = 1; argvIndex < argc; argvIndex += 1) {
        for (i32 argChar = 0; (argv[argvIndex][argChar] != '\0'); argChar += 1) {
            if (argv[argvIndex][argChar - 1] == '-') {
                switch (argv[argvIndex][argChar]) {
                    case 'h':
                        historyEnabled = true;
                        break;
                    case 'c':
                        comOppEnabled = true;
                        if (argv[argvIndex][argChar + 1] == 'd') {
                            haveDelay = true;
                            break;
                        }
                        if (argv[argvIndex][argChar + 1] == 'o') {
                            comOppOnly = true;
                            break;
                        }
                        if (true 
                            && argv[argvIndex][argChar + 1] >= static_cast<char>('0')
                            && argv[argvIndex][argChar + 1] <= static_cast<char>('9')
                        ) {
                            comOppStrength = (argv[argvIndex][argChar + 1] <= static_cast<char>('5')) ? static_cast<u8>(argv[argvIndex][argChar + 1]) - 48
                                                                                                    : 5;
                        }
                        break;
                    case 't':
                        trisLength = 0;
                        for (
                                i32 i = 1; 
                                true 
                                && argv[argvIndex][argChar + i] >= static_cast<char>('0')
                                && argv[argvIndex][argChar + i] <= static_cast<char>('9');
                                i += 1
                        ) {
                            trisLength *= 10;
                            trisLength += static_cast<u8>(argv[argvIndex][argChar + i]) - 48;
                        }
                        if (trisLength < 3) { rss::panic("Tris length length must be at least 3"); }
                        break;
                    case 's':
                        if (argv[argvIndex][argChar + 1] != '\0') {
                            playerSymbols[0] = argv[argvIndex][argChar + 1];
                            if (true 
                                && argv[argvIndex][argChar + 2] != '\0' 
                                && argv[argvIndex][argChar + 2] != argv[argvIndex][argChar + 1]
                            ) { playerSymbols[1] = argv[argvIndex][argChar + 2]; }
                            else if (argv[argvIndex][argChar + 1] == 'O') { playerSymbols[1] = 'X'; }
                        }
                        break;
                    case 'b':
                        bWidth = 0;
                        bHeigth = 0;
                        for (
                                i32 i = 1;
                                true 
                                && argv[argvIndex][argChar + i] >= static_cast<char>('0') 
                                && argv[argvIndex][argChar + i] <= static_cast<char>('9');
                                i += 1
                        ) {
                            bWidth *= 10;
                            bWidth += static_cast<u8>(argv[argvIndex][argChar + i]) - 48;
                            if (false
                                || (argv[argvIndex][argChar + i + 1]) == 'x' 
                                || (argv[argvIndex][argChar + i + 1]) == ':' 
                                || (argv[argvIndex][argChar + i + 1]) == '*'
                            ) {
                                for (
                                    i32 x = i + 2;
                                    true 
                                    && argv[argvIndex][argChar + x] >= static_cast<char>('0') 
                                    && argv[argvIndex][argChar + x] <= static_cast<char>('9');
                                    x += 1
                                ) {
                                    bHeigth *= 10;
                                    bHeigth += static_cast<u8>(argv[argvIndex][argChar + x]) - 48;
                                }
                            }
                        }
                        if (bWidth < 1) { bWidth = 3; }
                        if (bHeigth < 1) { bHeigth = bWidth; }
                        break;
                }
            }
        }
    }

  if (DEBUG_MODE) {
    print("DEBUG: TRIS LENGTH - %d\n", trisLength);
    print("DEBUG: COM MAGNETISM - %d\n", comOppStrength);
    print("DEBUG: BOARD WIDTH - %d\n", bWidth);
    print("DEBUG: BOARD HEIGTH - %d\n", bHeigth);
  }

  // Prepare game
    var GameBoard board(bWidth, bHeigth, Cell::EMPTY__CELL, playerSymbols);
    if (trisLength > bHeigth || trisLength > bWidth || trisLength < 3) {
             print("Tris length (%d) must not be longer than the smallest side of the board (%d)", trisLength, (bHeigth <= bWidth ? bHeigth :bWidth));
        rss::panic("            ^^^                                                        ^^^  ");
    } else {
        displayTitlecard();
        print("\n\n");
    }
    std::cout << std::flush;

  // Game
    while (!gameOver) {
        board.display();
        displayTurn(turn, (comOppEnabled && turn == Turn::PLYR2) || comOppOnly, playerSymbols);
        if((comOppEnabled && turn) || comOppOnly) { 
            comControl(board.content, turn, comOppEvaluate(board.content, turn, comOppStrength)); 
            if (haveDelay) { std::this_thread::sleep_for(std::chrono::milliseconds(500)); }
        } else { 
            playerControl(board.content, turn); 
        }
        undisplayHistory(bWidth, historyEnabled);
        gameOver = checkForTrisOrTie(board, trisLength, turn, comOppEnabled, comOppOnly);
        turn = turn == Turn::PLYR1 ? Turn::PLYR2 : Turn::PLYR1;
    }
}

fn void undisplayHistory(const u32 boardWidth, const bool historyFlag) {
    std::cout 
        << "\n\n"
        << String((boardWidth / 2) * 4 - (!(boardWidth % 2) * 2), ' ')
        << "↓\n\n\n"
        << std::flush;
    if (!historyFlag) { print("\033c"); }
}

fn void displayTitlecard() {
    var i32 roll = ((rand() % titleCardCount) + 1) * (titleCardSize + 1);
    String outstr;
    std::fstream titleCardsFile("titlecard.txt");
    titleCardsFile.seekg(std::ios::beg);
    for (i32 i = 0; i < roll; i += 1) {
        titleCardsFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    loop {
        getline(titleCardsFile, outstr);
        if (outstr == titleCardBreaker) { break; }
        DEBUG_MODE ? print("%s%d\n", outstr.c_str(), roll + 1) : print("%s\n", outstr.c_str());
        roll += 1;
    }
    titleCardsFile.close();
    std::cout << std::flush;
}

fn void displayTurn(const Turn currentTurn, const bool isComTurn, const Vec<char> playerSymbols) {
    if(playerSymbols.size() != 2) { rss::panic("'diaplyTurn()' necessitates Vec<Char> of exactly 2 elements, or omission of it"); }
    print("\n\nTurn of %s %d - %c \n", isComTurn ? "COM" : "player", currentTurn == Turn::PLYR1 ? 1 : 2, currentTurn == Turn::PLYR1 ? playerSymbols[0] : playerSymbols[1]);
}

fn void displayEvalProcess(const Chart comEvalTable, const Vec<Location> strongestVec) {
    std::cerr << "\nDEBUG - comEvalTable - DEBUG\n";
    for (auto r = comEvalTable.rbegin(); r != comEvalTable.rend(); r += 1) {
        for (auto c = (*r).begin(); c != (*r).end(); c += 1) {
            std::cerr << *c << (c != (*r).end() - 1 ?  " │ " : "");
        }
        if (r != comEvalTable.rend() - 1) {
            std::cerr << "\n─";
            for (auto c = (*r).begin(); c != (*r).end() - 1; c += 1) { std::cerr << "─┼──"; }
            std::cerr << "\n";
        }
    }
    std::cerr << "\nDEBUG - comEvalStorage - DEBUG\n[";
    for (u32 i = 0; i < strongestVec.size(); i += 1) {
        (i < strongestVec.size() - 1) ? std::cerr << strongestVec[i].x << ", " : std::cerr << strongestVec[i].x << "]";
    }
    std::cerr << "\n[";
    for (u32 i = 0; i < strongestVec.size(); i += 1) {
        (i < strongestVec.size() - 1) ? std::cerr << strongestVec[i].y << ", " : std::cerr << strongestVec[i].y << "]";
    }
    std::cerr << "\n" << std::flush;
}

fn Location comOppEvaluate(const Board boardToEvaluate, const Turn comTurn, const u32 trisMagnetism) {
    var Chart comEvalTable( static_cast<u32>(boardToEvaluate.size()), Vec<u32>(static_cast<u32>(boardToEvaluate[0].size()), 1));
    var Vec<Location> strongestVec;
    var u32 strongest = 0;

    // Evaluate the board
    for (u64 r = 0; r < boardToEvaluate.size(); r += 1) {
        for (u64 c = 0; c < boardToEvaluate[0].size(); c += 1) {
            if (boardToEvaluate[r][c] != Cell::EMPTY__CELL) { 
                comEvalTable[r][c] = 0; 
            } else {
                if (r < boardToEvaluate.size() - 1 && boardToEvaluate[r + 1][c] == (comTurn == Turn::PLYR1 ? 1 : 2)) {
                    comEvalTable[r][c] += 1;
                    if (false 
                        || (r > 0 && boardToEvaluate[r - 1][c] == (comTurn == Turn::PLYR1 ? 1 : 2)) 
                        || (r < boardToEvaluate.size() - 2 && boardToEvaluate[r + 2][c] == (comTurn == Turn::PLYR1 ? 1 : 2))
                    ) { comEvalTable[r][c] += trisMagnetism; }
                }
                if (r < boardToEvaluate.size() - 1 && c < boardToEvaluate[0].size() - 1 && boardToEvaluate[r + 1][c + 1] == (comTurn == Turn::PLYR1 ? 1 : 2)) {
                    comEvalTable[r][c] += 1;
                    if (false 
                        || (r > 0 && c > 0 && boardToEvaluate[r - 1][c - 1] == (comTurn == Turn::PLYR1 ? 1 : 2)) 
                        || (r < boardToEvaluate.size() - 2 && c < boardToEvaluate[0].size() - 2 && boardToEvaluate[r + 2][c + 2] == (comTurn == Turn::PLYR1 ? 1 : 2))
                    ) { comEvalTable[r][c] += trisMagnetism; }
                }
                if (c < boardToEvaluate[0].size() - 1 && boardToEvaluate[r][c + 1] == (comTurn == Turn::PLYR1 ? 1 : 2)) {
                    comEvalTable[r][c] += 1;
                    if (false 
                        || (c > 0 && boardToEvaluate[r][c - 1] == (comTurn == Turn::PLYR1 ? 1 : 2)) 
                        || (c < boardToEvaluate[0].size() - 2 && boardToEvaluate[r][c + 2] == (comTurn == Turn::PLYR1 ? 1 : 2))
                    ) { comEvalTable[r][c] += trisMagnetism; }
                }
                if (r > 0 && c < boardToEvaluate[0].size() - 1 && boardToEvaluate[r - 1][c + 1] == (comTurn == Turn::PLYR1 ? 1 : 2)) {
                    comEvalTable[r][c] += 1;
                    if (false 
                        || (r < boardToEvaluate.size() - 1 && c > 0 && boardToEvaluate[r + 1][c - 1] == (comTurn == Turn::PLYR1 ? 1 : 2)) 
                        || (r > 1 && c < boardToEvaluate[0].size() - 2 && boardToEvaluate[r - 2][c + 2] == (comTurn == Turn::PLYR1 ? 1 : 2))
                    ) { comEvalTable[r][c] += trisMagnetism; }
                }
                if (r > 0 && boardToEvaluate[r - 1][c] == (comTurn == Turn::PLYR1 ? 1 : 2)) {
                    comEvalTable[r][c] += 1;
                    if (false 
                        || (r < boardToEvaluate.size() - 1 && boardToEvaluate[r + 1][c] == (comTurn == Turn::PLYR1 ? 1 : 2)) 
                        || (r > 1 && boardToEvaluate[r - 2][c] == (comTurn == Turn::PLYR1 ? 1 : 2))
                    ) { comEvalTable[r][c] += trisMagnetism; }
                }
                if (r > 0 && c > 0 && boardToEvaluate[r - 1][c - 1] == (comTurn == Turn::PLYR1 ? 1 : 2)) {
                    comEvalTable[r][c] += 1;
                    if (false 
                        || (r < boardToEvaluate.size() - 1 && c < boardToEvaluate[0].size() - 1 && boardToEvaluate[r + 1][c + 1] == (comTurn == Turn::PLYR1 ? 1 : 2)) 
                        || (r > 1 && c > 1 && boardToEvaluate[r - 2][c - 2] == (comTurn == Turn::PLYR1 ? 1 : 2))
                    ) { comEvalTable[r][c] += trisMagnetism; }
                }
                if (c > 0 && boardToEvaluate[r][c - 1] == (comTurn == Turn::PLYR1 ? 1 : 2)) {
                    comEvalTable[r][c] += 1;
                    if (false 
                        || (c < boardToEvaluate[0].size() - 1 && boardToEvaluate[r][c + 1] == (comTurn == Turn::PLYR1 ? 1 : 2)) 
                        || (c > 1 && boardToEvaluate[r][c - 2] == (comTurn == Turn::PLYR1 ? 1 : 2))
                    ) { comEvalTable[r][c] += trisMagnetism; }
                }
                if (r < boardToEvaluate.size() - 1 && c > 0 && boardToEvaluate[r + 1][c - 1] == (comTurn == Turn::PLYR1 ? 1 : 2)) {
                    comEvalTable[r][c] += 1;
                    if (false 
                        || (r > 0 && c < boardToEvaluate[0].size() - 1 && boardToEvaluate[r - 1][c + 1] == (comTurn == Turn::PLYR1 ? 1 : 2)) 
                        || (r < boardToEvaluate.size() - 2 && c > 1 && boardToEvaluate[r + 2][c - 2] == (comTurn == Turn::PLYR1 ? 1 : 2))
                    ) { comEvalTable[r][c] += trisMagnetism; }
                }
            }
        }
    }

    // Find how strong is the strongest move
    for (let& r : comEvalTable) {
        for (let& c : r) {
            if (c > strongest) {
                strongest = c;
            }
        }
    }
    // Sotre all moves that are the same value as the strongest
    for (u32 r = 0; r < boardToEvaluate.size(); r += 1) {
        for (u32 c = 0; c < boardToEvaluate[0].size(); c += 1) {
            if (comEvalTable[r][c] == strongest) {
                strongestVec.push_back({ c, r });
            }
        }
    }
    let randIndex = rand() % strongestVec.size();

    if (DEBUG_MODE) {
        displayEvalProcess(comEvalTable, strongestVec);
    }

    return strongestVec[randIndex];
}

fn void playerControl(Board &board, const Turn currentTurn) {
    let x = rep::getInt("X: ", 1, static_cast<i32>(board[0].size()));
    let y = rep::getInt("Y: ", 1, static_cast<i32>(board.size()));
    if (board[y - 1][x - 1] == Cell::EMPTY__CELL) {
        board[y - 1][x - 1] = currentTurn == Turn::PLYR1 ? Cell::PLYR1__CELL : Cell::PLYR2__CELL;
        return;
    }
    std::cout << "⚠ Cannot override this tile\n" << std::flush;
    playerControl(board, currentTurn);
}

fn void comControl(Board &board, const Turn currentTurn, const Location comMove) {
    board[comMove.y][comMove.x] = currentTurn == Turn::PLYR1 ? Cell::PLYR1__CELL : Cell::PLYR2__CELL;
    print("X: %d\nY: %d\n", comMove.x + 1, comMove.y + 1);
    std::cout << std::flush;
}

fn bool checkForTrisOrTie(const GameBoard boardToCheck, u32 trisSize, const Turn currentTurn, const bool comOppEnabled, const bool comOppOnly) {
    var bool terminalCase = false;
    for (u32 r = 0; r < boardToCheck.content.size(); r += 1) {
        for (u32 c = 0; c < boardToCheck.content[0].size(); c += 1) {
            u32 trisParts = 0;
            for (u32 i = 1; i < trisSize; i += 1) {
                if (true
                    && r <= boardToCheck.content.size() - trisSize 
                    && boardToCheck.content[r + (i - 1)][c] == boardToCheck.content[r + i][c] 
                    && boardToCheck.content[r + (i - 1)][c] != EMPTY__CELL
                ) {
                    trisParts += 1;
                    if (trisParts == trisSize - 1) { terminalCase = true; }
                }
                if (DEBUG_MODE && terminalCase) {
                    std::cerr << "DEBUG: TRIS - 90°[" << r << " : " << c << "] cycles\n";
                }
            }
            trisParts = 0;
            if (terminalCase) { break; }
            for (u32 i = 1; i < trisSize; i += 1) {
                if (true 
                    && c <= boardToCheck.content[0].size() - trisSize 
                    && boardToCheck.content[r][c + (i - 1)] == boardToCheck.content[r][c + i] 
                    && boardToCheck.content[r][c + (i - 1)] != EMPTY__CELL
                ) {
                    trisParts += 1;
                    if (trisParts == trisSize - 1) { terminalCase = true; }
                }
                if (DEBUG_MODE && terminalCase) {
                    std::cerr << "DEBUG: TRIS - 0° [" << r << " : " << c << "] cycles\n";
                }
            }
            trisParts = 0;
            if (terminalCase) { break; }
            for (u32 i = 1; i < trisSize; i += 1) {
                if (true 
                    && r <= boardToCheck.content.size() - trisSize 
                    && c <= boardToCheck.content[0].size() - trisSize 
                    && boardToCheck.content[r + (i - 1)][c + (i - 1)] == boardToCheck.content[r + i][c + i] 
                    && boardToCheck.content[r + (i - 1)][c + (i - 1)] != EMPTY__CELL
                ) {
                    trisParts += 1;
                    if (trisParts == trisSize - 1) { terminalCase = true; }
                }
                if (DEBUG_MODE && terminalCase) {
                    std::cerr << "DEBUG: TRIS - 45° [" << r << " : " << c << "] cycles\n";
                }
            }
            trisParts = 0;
            if (terminalCase) { break; }
            for (u32 i = 1; i < trisSize; i += 1) {
                if (true 
                    && r <= boardToCheck.content.size() - trisSize 
                    && c >= trisSize - 1 
                    && boardToCheck.content[r + (i - 1)][c - (i - 1)] == boardToCheck.content[r + i][c - i] 
                    && boardToCheck.content[r + (i - 1)][c - (i - 1)] != EMPTY__CELL
                ) {
                    trisParts += 1;
                    if (trisParts == trisSize - 1) { terminalCase = true; }
                }
                if (DEBUG_MODE && terminalCase) {
                    std::cerr << "DEBUG: TRIS - 135° [" << r << " : " << c << "] cycles\n";
                }
            }
            trisParts = 0;
            if (terminalCase) { break; }
        }
        if (terminalCase) { break; }
    }

    // if tris is found
    if (terminalCase) {
        boardToCheck.display();
        if (comOppOnly) {
            print("\n\n[ -c ° ▥ °]-c COM %d won.\n", currentTurn == Turn::PLYR1 ? 1 : 2);
        } else {
            if (comOppEnabled && currentTurn == Turn::PLYR2) {
                print("\n\no(╥﹏╥)o COM 2 won.\n");
            } else {
                print("\n\n＼(＾O＾)／ Player %d won.\n", currentTurn == Turn::PLYR1 ? 1 : 2);
            }
        }
    } else {
        // check for tie
        for (let& r : boardToCheck.content) {
            for (let& c : r) {
                if (c == EMPTY__CELL) {
                    return false;
                }
            }
        }
        boardToCheck.display();
        print("\n\n(◞‸◟；) No one won. It's a tie.\n");
    }
    return true;
}