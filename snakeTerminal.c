#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>

struct Node {
    int x, y;
    struct Node *next;
};
struct Node *head = NULL, *tail = NULL;

int board[30][30];
int highestScore = 0;
int score = 0;
int foodx = 5, foody = 5;
char dir = 'l';

void createFood();
void createHead(int x, int y);
void display();
void printScore();
void increment(char dir);
int bombardment();
void eatFood();

void gameOn() {
    struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI }; 
    char key;
    if( poll(&mypoll, 1, 3000) )    { 
        scanf("%c", &key);  
        if (key == 'A' || key == 'a') dir = 'r';
        else if (key == 'W' || key == 'w') dir = 'u';
        else if (key == 'S' || key == 's') dir = 'd';
        else if (key == 'D' || key == 'd') dir = 'l';
    }
    
    int out = bombardment();
    if (out == -1) return;
    increment(dir);
    display();
    gameOn();
}


int main() {
    while (1) {
        for(int i = 0; i < 30; i++) {
            for (int j = 0; j < 30; j++) {
                board[i][j] = 0;
            }
        }
        createHead(1, 1);
        createHead(1, 2);
        createHead(1, 3);
        createFood();
        gameOn();
        char inp;
        scanf("%c", &inp);
        scanf("%c", &inp);
        if (inp == 's') {
            continue;
        }
        else {
            break;
        }
    }
    return 0;
}

int bombardment() {
    if (foodx == head->x && foody == head->y) {
        board[foodx][foody] = 0;
        createFood();
        eatFood();
        score++;
    }
    struct Node *temp = head->next;
    while (temp != NULL) {
        if (head->x == temp->x && head->y == temp->y) {
            printScore();
            return -1;
        }
        temp = temp->next;
    }
    return 0;
}

void eatFood() {
    int x = head->x, y = head->y;
    if (dir == 'l') y = (y + 1 + 30)%30;
    else if (dir == 'r') y = (y - 1 + 30)%30;
    else if (dir == 'u') x = (x - 1 + 30)%30;
    else if (dir == 'd') x = (x + 1 + 30)%30;

    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node *));
    board[x][y] = 1;
    new_node->x = x;
    new_node->y = y;
    new_node->next = head;
    head = new_node;

}

void createFood() {
    int x = rand() % 30;
    int y = rand() % 30;
    printf("Food Created");

    if (board[x][y] == 0) {
        foodx = x;
        foody = y;
        board[x][y] = 2;
    }
    else {
        createFood();
    }
}

void display() {
    printf("\e[1;1H\e[2J");
    for (int i = 0 ; i < 30; i++) {
        printf(" -");
    }
    printf("\n");

    for (int i = 0 ; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            if (j == 0) printf(" |");
            else if (j == 29) printf(" |");
            else {
                if (board[i][j] == 0) printf("  ");
                else if (board[i][j] == 1) printf("+ ");
                else if (board[i][j] == 2) printf("@ ");
            }
        }
        printf("\n");
    }
    for (int i = 0 ; i < 30; i++) {
        printf(" -");
    }
}

void printScore() {
    if (score > highestScore) highestScore = score;
    printf("\nYour Score: %d\nHighest Score: %d\n", score, highestScore);
    return;
}

void createHead(int x, int y) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node *));
    board[x][y] = 1;
    new_node->x = x;
    new_node->y = y;
    if (head == NULL) {
        head = new_node;
        tail = head;
        return;
    }
    new_node->next = head;
    head = new_node;
}

void increment(char dir) {
    int x = head->x, y = head->y;
    if (dir == 'l') y = (y + 1 + 30)%30;
    else if (dir == 'r') y = (y - 1 + 30)%30;
    else if (dir == 'u') x = (x - 1 + 30)%30;
    else if (dir == 'd') x = (x + 1 + 30)%30;
    
    struct Node *temp = head;
    while (temp != NULL) {
        int exch = temp->x;
        temp->x = x;
        x = exch;
        exch = temp->y;
        temp->y = y;
        y = exch;
        board[x][y] = 0;
        board[temp->x][temp->y] = 1;
        temp = temp->next;
    }
}