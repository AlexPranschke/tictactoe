#include <iostream>


using namespace std;

typedef int** Map;
const unsigned int MapSize = 3;

enum PosStatus {
    EMPTY = 0,
    PLAYER1 = 1,
    PLAYER2 = 2,
};

struct Pkt {
    int x, y;
    Pkt(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

void InitMap(Map map) {
    for (int i = 0; i < MapSize; i++) {
        for (int j = 0; j < MapSize; j++) {
            map[i][j] = EMPTY;
        }
    }
}

PosStatus posStatus(Map map, Pkt pkt) {
    return static_cast<PosStatus>(map[pkt.x][pkt.y]);
}

void DrawMap(Map map) {
    cout << "  ";
    for (int i = 0; i < MapSize; i++) {
        cout << i << " " ;;
    }
    cout << endl;
    for (int i = 0; i < MapSize; i++) {
        cout << i << " "; 
        for (int j = 0; j < MapSize; j++) {
            
            cout << (map[i][j] == PLAYER1 ? "O" : (map[i][j] == PLAYER2 ? "X" : "_")) << " ";
        }
        cout << endl;
    }
}

Pkt PlayerInput() {
    int x, y;
    while (true) {
        cout << "Enter row and column (0-" << MapSize - 1 << "): ";
        cin >> x >> y;
        if (x >= 0 && x < MapSize && y >= 0 && y < MapSize) {
            return Pkt(x, y);
        }
        else {
            cout << "Invalid input" << endl;
        }
    }
}

void Move(Map map, Pkt pkt, bool isPlayer1) {
    if (posStatus(map, pkt) == EMPTY) {
        map[pkt.x][pkt.y] = isPlayer1 ? PLAYER1 : PLAYER2;
    }
}



int CheckState(Map map, int tab[], int & end) {
    
    bool temp=false;
    
    for (int i = 0; i < MapSize; i++) {
        for (int j = 0; j < MapSize; j++) {
            if (map[i][j] == EMPTY) {
                temp = true;
                break;
            }
        }
    }

    if (temp == false) {
            end = 3;
            cout << "draw";
            return 0;
        }
    

    
    for (int i = 0; i < 2 * MapSize + 2; i++) {
        tab[i] = 0;
    }

    // Count marks for rows
    for (int i = 0; i < MapSize; i++) {
        for (int j = 0; j < MapSize; j++) {
            if (map[i][j] == PLAYER1) {
                tab[i]++;  // Count for rows
                if (tab[i] == MapSize)
                {
                    end = 1;
                }
            }
            else if (map[i][j] == PLAYER2) {
                tab[i] = 0;  // Reset if AI is present
                break;
            }
        }
    }

    // Count marks for columns
    for (int i = 0; i < MapSize; i++) {
        for (int j = 0; j < MapSize; j++) {
            if (map[j][i] == PLAYER1) {
                tab[MapSize + i]++;
                if (tab[MapSize + i] == MapSize)
                {
                    end = 1;
                }
            }
            else if (map[j][i] == PLAYER2) {
                tab[MapSize + i] = 0;  // Reset if AI is present
                break;
            }
        }
    }

    // Count marks for main diagonal
    for (int i = 0; i < MapSize; i++) {
        if (map[i][i] == PLAYER2) {
            tab[2 * MapSize] = 0;  // Reset if AI is present
            break;
        }
        else if (map[i][i] == PLAYER1) {
            tab[2 * MapSize]++;
            if (tab[2 * MapSize] == MapSize)
            {end = 1;}
        }
    }

    // Count marks for anti-diagonal
    for (int i = 0; i < MapSize; i++) {
        if (map[i][MapSize - i - 1] == PLAYER2) {
            tab[2 * MapSize + 1] = 0;  // Reset if AI is present
            break;
        }
        else if (map[i][MapSize - i - 1] == PLAYER1) {
            tab[2 * MapSize + 1]++;
            if (tab[2 * MapSize+1] == MapSize)
            {
                end = 1;
            }
        }
    }
    return 0;
}

Pkt AiInput(Map map, int* dangerTab, int & end) {
    Pkt aiMove;
    int counter=0;
    

    for (int i = 0; i < MapSize; i++) {
        counter = 0;
        for (int j = 0; j < MapSize; j++) {
            if (map[i][j] == PLAYER1) {
                counter = 0;
                break;
            }
            if (map[i][j] == PLAYER2) {
                counter++;
                if (counter == MapSize-1) {
                    for (int k = 0; k < MapSize; k++) {
                        if (map[i][k] == EMPTY) {
                            aiMove = Pkt(i, k);
                            map[i][k] = PLAYER2;
                            end = 2;
                            cout << "Row win" << endl;
                            return aiMove;
                        }
                    }
                }
            }
        }
    }

    
    for (int j = 0; j < MapSize; j++) {
        counter = 0;
        for (int i = 0; i < MapSize; i++) {
            if (map[i][j] == PLAYER1) {
                counter = 0;
                break;
            }
            if (map[i][j] == PLAYER2) {
                counter++;
                if (counter == MapSize - 1) {
                    for (int k = 0; k < MapSize; k++) {
                        if (map[k][j] == EMPTY) {
                            aiMove = Pkt(k, j);
                            map[k][j] = PLAYER2;
                            end = 2;
                            cout << "Column win" << endl;
                            return aiMove;
                        }
                    }
                }
            }
        }
    }

    // Diagonal 1 check
    counter = 0;
    for (int i = 0; i < MapSize; i++) {
        if (map[i][i] == PLAYER1) {
            counter = 0;
            break;
        }
        if (map[i][i] == PLAYER2) {
            counter++;
            if (counter == MapSize - 1) {
                for (int j = 0; j < MapSize; j++) {
                    if (map[j][j] == EMPTY) {
                        aiMove = Pkt(j, j);
                        map[j][j] = PLAYER2;
                        end = 2;
                        cout << "Diagonal 1 win" << endl;
                        return aiMove;
                    }
                }
            }
        }
    }

    // Anti-diagonal check
    counter = 0;
    for (int i = 0; i < MapSize; i++) {
        if (map[i][MapSize - i - 1] == PLAYER1) {
            counter = 0;
            break;
        }
        if (map[i][MapSize - i - 1] == PLAYER2) {
            counter++;
            if (counter == MapSize - 1) {
                for (int j = 0; j < MapSize; j++) {
                    if (map[j][MapSize - j - 1] == EMPTY) {
                        aiMove = Pkt(j, MapSize - j - 1);
                        map[j][MapSize - j - 1] = PLAYER2;
                        end = 2;
                        cout << "Diagonal 2 win" << endl;
                        return aiMove;
                    }
                }
            }
        }
    }

  
    if (dangerTab[2 * MapSize] == MapSize - 1) {
        for (int i = 0; i < MapSize; i++) {
            if (map[i][i] == EMPTY) {
                aiMove = Pkt(i, i);
                map[i][i] = PLAYER2;  // Make the move
                return aiMove;
            }
        }
    }
    if (dangerTab[2 * MapSize + 1] == MapSize - 1) {
        for (int i = 0; i < MapSize; i++) {
            if (map[i][MapSize - i - 1] == EMPTY) {
                aiMove = Pkt(i, MapSize - i - 1);
                map[i][MapSize - i - 1] = PLAYER2;  // Make the move
                return aiMove;
            }
        }
    }

    
    for (int i = 0; i < MapSize; i++) {
        // Check rows for blocking move
        if (dangerTab[i] == MapSize - 1) {
            for (int j = 0; j < MapSize; j++) {
                if (map[i][j] == EMPTY) {
                    aiMove = Pkt(i, j);
                    map[i][j] = PLAYER2;  
                    return aiMove;
                }
            }
        }
        // Check columns for blocking move
        if (dangerTab[MapSize + i] == MapSize - 1) {
            for (int j = 0; j < MapSize; j++) {
                if (map[j][i] == EMPTY) {
                    aiMove = Pkt(j, i);
                    map[j][i] = PLAYER2;  
                    return aiMove;
                }
            }
        }
    }

    // Check diagonals for blocking move
    if (dangerTab[2 * MapSize] == MapSize - 1) {
        for (int i = 0; i < MapSize; i++) {
            if (map[i][i] == EMPTY) {
                aiMove = Pkt(i, i);
                map[i][i] = PLAYER2;  // Make the block
                return aiMove;
            }
        }
    }
    if (dangerTab[2 * MapSize + 1] == MapSize - 1) {
        for (int i = 0; i < MapSize; i++) {
            if (map[i][MapSize - i - 1] == EMPTY) {
                aiMove = Pkt(i, MapSize - i - 1);
                map[i][MapSize - i - 1] = PLAYER2;  // Make the block
                return aiMove;
            }
        }
    }

    // Random move 
    do {
        aiMove.x = rand() % MapSize;
        aiMove.y = rand() % MapSize;
    } while (map[aiMove.x][aiMove.y] != EMPTY);
    map[aiMove.x][aiMove.y] = PLAYER2;

    return aiMove;
}

int main() {
    Map map = new int* [MapSize];
    for (int i = 0; i < MapSize; i++) {
        map[i] = new int[MapSize];
    }
    InitMap(map);
    int end = 0;
    bool firstMove = false;
    int dangerTab[2 * MapSize + 2] = { 0 };
    cout << " win =" << end << " " << end  ;
    while (true) {
        system("CLS");
        DrawMap(map);
        
        
        firstMove = !firstMove;
       
        if (firstMove)
        {
            Pkt playerMove = PlayerInput();
            Move(map, playerMove, firstMove);
        }
        else {
            Pkt aiMove = AiInput(map, dangerTab, end);
        }
        
        CheckState(map, dangerTab, end);
        if (end != 0) {
            DrawMap(map);
            cout << "game over" << endl;
            break;
        }
        
    }

   
    for (int i = 0; i < MapSize; i++) {
        delete[] map[i];
    }
    delete[] map;

    return 0;
}
