#include <iostream>
using namespace std;
bool gameOver;
const int width = 20;
const int height = 20;
int x,y,fruitX, fruitY,score;
enum eDirection{ STOP =0,LEFT,RIGHT,UP,DOWN};
eDirection dir;
void Setup(){
  gameOver = false;
  dir =STOP;
  x = width/2;
  y =height/2;
  fruitX = rand() % width;
  fruitY = rand() % height;
  score =0;
}
void Draw(){
    system("cls");
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "#";
            if (i == y && j == x)
                cout << "O"; // Rắn
            else if (i == fruitY && j == fruitX)
                cout << "*"; // Trái cây
            else
                cout << " ";
            if (j == width - 1) cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
    cout << "Score: " << score << endl;
}
void Input(){

}
void Logic(){

}
int main(){
    Setup();
    while(!gameOver){
        Draw();
        Input();
        Logic();
    }
    return 0;
}
