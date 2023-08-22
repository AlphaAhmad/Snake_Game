
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include<MMSystem.h>

using namespace std;

#define Height 86
#define Width 110

//______________________________

void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}
void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int clr)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}


//_______________________________


enum Direction { UP, DOWN, RIGHT, LEFT };

struct position
{
    int pos_row, pos_col;
};

struct Snake
{
    position* pos;
    string P_name;
    int size;
    int score;
    int clr;
    int UP_KEY, DOWN_KEY, RIGHT_KEY, LEFT_KEY;
    Direction Dir;
    bool alive;
};

struct stage
{
    position** wall;
    int* wall_lnth;
    int walls_num = 0;
    bool boundry;
    string name;

};

struct food
{
    position fp;
    bool isALive;
    int score;
    char f_sym;

};
struct time_food
{
    position fp;
    bool isALive;
    int score;
    char tf_sym;
    int timer;
};

//_______________ food section ______________
bool food_position_legal(food f, Snake* No_of_Snakes, int NOP, stage s)
{
    
    
                     // simple food
        //______________ 1st checking boundries ________________
        
        

        if (f.fp.pos_row <= 2 || f.fp.pos_row >= Height - 2 || f.fp.pos_col <= 2 || f.fp.pos_col >= Width - 2)
        {
            return false;
        }
        
        //____________________ Walls _________

        for (int i = 0; i < s.walls_num; i++)
        {
            for (int j = 0; j < s.wall_lnth[i]; j++)
            {
                if (f.fp.pos_row == s.wall[i][j].pos_row && f.fp.pos_col == s.wall[i][j].pos_col)
                {
                    return false;
                }
            }
        }
        //__________ now checking snakes ____________________
        for (int i = 0; i < NOP; i++)
        {
            for (int j = 0; j < No_of_Snakes[i].size; j++)
            {
                if (No_of_Snakes[i].pos[j].pos_row == f.fp.pos_row && No_of_Snakes[i].pos[j].pos_col == f.fp.pos_col)
                {
                    return false;
                }
            }
        }
        return true;
    
   
}
bool tfood_position_legal(time_food tf, food f, Snake* No_of_Snakes, int NOP, stage s)
{
     
     
                   // time  food
        //______________ 1st checking boundries ________________
        
        

        if (tf.fp.pos_row <=1  || tf.fp.pos_row >= Height - 2 || tf.fp.pos_col <= 1 || tf.fp.pos_col >= Width - 2)
        {
            return false;
        }
        
        //____________________ Walls _________

        for (int i = 0; i < s.walls_num; i++)
        {
            for (int j = 0; j < s.wall_lnth[i]; j++)
            {
                if (tf.fp.pos_row == s.wall[i][j].pos_row && tf.fp.pos_col == s.wall[i][j].pos_col)
                {
                    return false;
                }
            }
        }
        //__________ now checking snakes ____________________
        for (int i = 0; i < NOP; i++)
        {
            for (int j = 0; j < No_of_Snakes[i].size; j++)
            {
                if (No_of_Snakes[i].pos[j].pos_row == tf.fp.pos_row && No_of_Snakes[i].pos[j].pos_col == tf.fp.pos_col)
                {
                    return false;
                }
            }
        }
        //_____________________checking simple food coodinates _________________
        if (f.fp.pos_row == tf.fp.pos_row && f.fp.pos_col == tf.fp.pos_col)
        {
            return false;
        }
        return true;
    
}
  // these all functions will work for simple food if x is false and for time food if x is true
             // for time food and simple food
void all_food_generator(food &f, time_food &tf,Snake* No_of_Snakes, int NOP, stage s, bool x)
{
    if (x == false)   // for simple food
    {
        do {
            f.fp.pos_row = rand() % Height;
            f.fp.pos_col = rand() % Width;

        } while (!food_position_legal(f, No_of_Snakes, NOP, s));
        f.isALive = true;
        f.score = 1;
        f.f_sym = '*';
    }
    else
    {
        do {
            tf.fp.pos_row = (rand()+34) % Height;
            tf.fp.pos_col = (rand()+34) % Width;

        } while (!tfood_position_legal(tf,f ,No_of_Snakes, NOP, s));
        tf.isALive = true;
        tf.score = 3;
        tf.timer = 7 * 18;
        tf.tf_sym = char(-37);
    }
    
}
void all_food_displayer(food f, time_food tf, bool x)
{
    if (x == false) // for simple food
    {
        gotoRowCol(f.fp.pos_row, f.fp.pos_col);
        SetClr(6);
        cout << f.f_sym;
        SetClr(15);
    }
    else
    {
        gotoRowCol(tf.fp.pos_row, tf.fp.pos_col);
        SetClr(13);
        cout << tf.tf_sym;
        SetClr(15);
    }


}
void all_food_eraser(food f, time_food tf, bool x)
{
    if (x == false) // for simple food
    {
        gotoRowCol(f.fp.pos_row, f.fp.pos_col);
        
        cout << " ";
        
    }
    else
    {
        gotoRowCol(tf.fp.pos_row, tf.fp.pos_col);
       
        cout << " ";
       
    }


}
bool food_eaten(food f,Snake * No_of_Snakes ,int &which_snake, int NOP)
{
    for (int i = 0; i < NOP; i++)
    {
        if (No_of_Snakes[i].pos[0].pos_row == f.fp.pos_row && No_of_Snakes[i].pos[0].pos_col == f.fp.pos_col)
        {
            which_snake = i;
            return true;
        }
    }
    return false;
}
bool tfood_eaten(time_food tf, Snake* No_of_Snakes, int& which_snake, int NOP)
{
    for (int i = 0; i < NOP; i++)
    {
        if (No_of_Snakes[i].pos[0].pos_row == tf.fp.pos_row && No_of_Snakes[i].pos[0].pos_col == tf.fp.pos_col)
        {
            which_snake = i;
            return true;
        }
    }
    return false;
}
//_______________________________
void stage_maker(stage &s, int which_stage)
{
    //  Medium stage
    if (which_stage == 2)
    {
        s.name = "MEDIUM";
        s.boundry = false;
        s.walls_num = 3;
        s.wall = new position * [s.walls_num] {};
        s.wall_lnth = new int[s.walls_num] {};
        for (int i = 0; i < s.walls_num; i++)
        {
            s.wall[i] = new position[100];
            
            if (i == 0)
            {
                for (int j = 15; j <= 37; j++)
                {
                    s.wall[i][s.wall_lnth[i]].pos_row = j;
                    s.wall[i][s.wall_lnth[i]].pos_col = Width / 2;
                    s.wall_lnth[i]++;
                }
            }
            if (i == 1)
            {
                for (int j = 42; j <= 66; j++)
                {
                    s.wall[i][s.wall_lnth[i]].pos_row = j;
                    s.wall[i][s.wall_lnth[i]].pos_col = (Width / 4)+3;
                    s.wall_lnth[i]++;


                }
            }
            if (i == 2)
            {
                for (int j = 42; j <= 66; j++)
                {
                    s.wall[i][s.wall_lnth[i]].pos_row = j;
                    s.wall[i][s.wall_lnth[i]].pos_col = (3*Width / 4) - 3;
                    s.wall_lnth[i]++;


                }
            }
        }

    }
    // Hard stage
    if (which_stage == 3)
    {
        s.name = "HARD";
        s.boundry = true;
        s.walls_num = 2;
        s.wall = new position * [s.walls_num] {};
        s.wall_lnth = new int[s.walls_num] {};
        for (int i = 0; i < s.walls_num; i++)
        {
            s.wall[i] = new position[100];
            if (i == 0)
            {
                for (int k = 13, j = Width / 2; k < 45; k++, j++)
                {
                    s.wall[i][s.wall_lnth[i]].pos_row = k;
                    s.wall[i][s.wall_lnth[i]].pos_col = j;
                    s.wall_lnth[i]++;
                }
            }

            if (i == 1)
            {
                for (int k = 45, j = (Width / 4) + 4; k < 75; k++, j++)
                {
                    s.wall[i][s.wall_lnth[i]].pos_row = k;
                    s.wall[i][s.wall_lnth[i]].pos_col = j;
                    s.wall_lnth[i]++;
                }
            }
        }
    }
}

void stage_displayer(stage s,int which_stage)
{
    if(which_stage>1)
    {
        //_____________________ boundries ______________________
        if (s.boundry == true)
        {
            for (int i = 0; i < Height; i++)
            {
                for (int j = 0; j < Width; j++)
                {
                    if (i == 0 || i == Height - 1 || j == 0 || j == Width - 1)
                    {
                        SetClr(4);
                        gotoRowCol(i, j);
                        cout << char(-37);

                    }
                }
            }
            SetClr(15);
        }
        // ___________________walls___________________________
        for (int i = 0; i < s.walls_num; i++)
        {
            for (int j = 0; j < s.wall_lnth[i]; j++)
            {
                SetClr(11);
                gotoRowCol(s.wall[i][j].pos_row, s.wall[i][j].pos_col);
                cout << char(-79);
            }
        }
        SetClr(15);
    }
}

int Colour_Determiner(string x)
{
    // RED =4 , White = 15, Green= 10, Blue =11.
    if (x == "R")
    {
        return 4;
    }
    else if (x == "W")
    {
        return 15;
    }
    else if (x == "G")
    {
        return 10;
    }
    else
    {
        return 11;
    }
}
void Move_Keys_setter(int i, Snake*& No_of_Snakes)
{
    if (i == 0)
    {
        No_of_Snakes[i].UP_KEY = 72;
        No_of_Snakes[i].DOWN_KEY = 80;
        No_of_Snakes[i].LEFT_KEY = 75;
        No_of_Snakes[i].RIGHT_KEY = 77;

    }
    else if (i == 1)
    {
        No_of_Snakes[i].UP_KEY = 119;
        No_of_Snakes[i].DOWN_KEY = 115;
        No_of_Snakes[i].LEFT_KEY = 97;
        No_of_Snakes[i].RIGHT_KEY = 100;
    }
    else if (i == 2)
    {
        No_of_Snakes[i].UP_KEY = 121;
        No_of_Snakes[i].DOWN_KEY = 104;
        No_of_Snakes[i].LEFT_KEY = 103;
        No_of_Snakes[i].RIGHT_KEY = 106;

    }
    else 
    {
        No_of_Snakes[i].UP_KEY = 112;
        No_of_Snakes[i].DOWN_KEY = 59;
        No_of_Snakes[i].LEFT_KEY = 108;
        No_of_Snakes[i].RIGHT_KEY = 39;
    }
    // upkey= 72, downkey= 80, leftkey= 75, rightkey= 77
    // w = 119, a=97, s=115, d=100
    // y= 121, g=103, h=104, j= 106
   // p=112 , l= 108, ;= 59, '= 39
}
void Snake_Initializer(Snake *& No_of_Snakes, int i)
{
     //  1st snake will spawn on bottom right and with direction left
    if (i == 0)
    {
        No_of_Snakes[i].pos[0].pos_col = (3 * Width) / 4;
        No_of_Snakes[i].pos[0].pos_row = ((3 * Height) / 4)+5;


        No_of_Snakes[i].pos[1].pos_col = ((3 * Width) / 4)+1;
        No_of_Snakes[0].pos[1].pos_row = ((3 * Height) / 4)+5;


        No_of_Snakes[i].pos[2].pos_col = ((3 * Width) / 4)+2;
        No_of_Snakes[i].pos[2].pos_row = ((3 * Height) / 4)+5;

        No_of_Snakes[i].Dir = LEFT;
    }
        // 2nd snake on will spawn on bottom left with direction upward
    if (i == 1)
    {
        No_of_Snakes[i].pos[0].pos_col = (Width) / 4;
        No_of_Snakes[i].pos[0].pos_row = (3 * Height) / 4;


        No_of_Snakes[i].pos[1].pos_col = ((Width) / 4);
        No_of_Snakes[i].pos[1].pos_row = ((3 * Height) / 4) + 1;


        No_of_Snakes[i].pos[2].pos_col = (Width) / 4;
        No_of_Snakes[i].pos[2].pos_row = ((3 * Height) / 4) + 2;

        No_of_Snakes[i].Dir = UP;
    }
    // 3rd sanke will spawn on Top Left with direction right
    if (i == 2)
    {
        No_of_Snakes[i].pos[0].pos_col = (Width) / 4;
        No_of_Snakes[i].pos[0].pos_row = (Height) / 4;


        No_of_Snakes[i].pos[1].pos_col = ((Width) / 4)-1;
        No_of_Snakes[i].pos[1].pos_row = ((Height) / 4);


        No_of_Snakes[i].pos[2].pos_col = ((Width) / 4)-2;
        No_of_Snakes[i].pos[2].pos_row = ((Height) / 4);

        No_of_Snakes[i].Dir = RIGHT;
    }
    // 4th Snake will spawn at Top Right with direction DownWard
    if (i == 3)
    {
        No_of_Snakes[i].pos[0].pos_col = (3*Width) / 4;
        No_of_Snakes[i].pos[0].pos_row = (Height) / 4;


        No_of_Snakes[i].pos[1].pos_col = ((3*Width) / 4)-1;
        No_of_Snakes[i].pos[1].pos_row = ((Height) / 4);


        No_of_Snakes[i].pos[2].pos_col = (3*Width) / 4-1;
        No_of_Snakes[i].pos[2].pos_row = ((Height) / 4);

        No_of_Snakes[i].Dir = DOWN;
    }
}
void box(int row, int col, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            gotoRowCol(row + i, col + j);
            if (i == 0 || j == 0 || i == size - 1 || j == size - 1)
            {
                SetClr(4);
                cout << char(-37);
                SetClr(15);
            }
            else
            {
                SetClr(6);
                cout << char(-37);
                SetClr(15);
            }
        }
    }
}
void InIt(Snake* &No_of_Snakes, int &NOP, stage &s, int &which_stage, food &f, time_food &tf, int& time_limit, int &Score_limit)
{
    gotoRowCol(31, Width / 4);
    cout << "How many players are playing (Minimum can be 1 and Maximum can be 4): ";
    for (int i = 0; i < 4; i++)
    {
        box(34, (Width/4)+(15*i), 10);
        gotoRowCol(34 + 5, ((Width / 4) + (15 * i)) + 5);
        SetClr(97);
        cout << i + 1;
        SetClr(15);
    }
    int row = 0, col = 0;
    do {
        

        getRowColbyLeftClick(row, col);
        if (row >= 34 && row <= 34 + 10 && col >= Width / 4 && col <= (Width / 4) + 10)
        {
            NOP = 1;
        }
        else if (row >= 34 && row <= 34 + 10 && col >= ((Width / 4) + 15) && col <= ((Width / 4) + 15) + 10)
        {
            NOP = 2;
        }
        else if (row >= 34 && row <= 34 + 10 && col >= ((Width / 4) + 30) && col <= ((Width / 4) + 30) + 10)
        {
            NOP = 3;
        }
        else if (row >= 34 && row <= 34 + 10 && col >= ((Width / 4) + 45) && col <= ((Width / 4) + 45) + 10)
        {
            NOP = 4;
        }
        else
            NOP = -1;
    } while (NOP > 4 || NOP < 1);
    
    system("cls");
    No_of_Snakes = new Snake[NOP];
    for (int i = 0; i < NOP; i++)
    {    
        
        // Name of player
        gotoRowCol(25, Width / 4);
        cout << "What is the name of Player " << i+1<<endl;
        box(27,Width/4,15);
        gotoRowCol(33, (Width / 4)+2);
        SetClr(97);
        cin >> No_of_Snakes[i].P_name;
        SetClr(15);
        // Chosing colour of snake
        gotoRowCol(47, Width / 4);
        cout << "Which colour you want for your snake from ( Red (R) , Green (G) , BLUE (B), WHite (W) )    NOTE:These are the only choices" << endl;
        string x;
        box(49, Width / 4,5);
        
        do {
            gotoRowCol(51, (Width / 4) + 2);
            SetClr(97);
            cin >> x;
        } while (!(x=="R" || x == "W" || x == "G"||x == "B"));
        SetClr(15);
        No_of_Snakes[i].clr = Colour_Determiner(x);
        
        // Determining score which in start = 0
        No_of_Snakes[i].score = 0;

        // Determinig Movement keys.
        Move_Keys_setter(i,No_of_Snakes);

        {                 // Making Snakes
            
            // Creating position array of snake
            No_of_Snakes[i].size = 3;   // Default size at the start
            No_of_Snakes[i].pos = new position[No_of_Snakes[i].size];

           
           
            // Initializing snake
            Snake_Initializer(No_of_Snakes,i);
            
        }
        No_of_Snakes[i].alive = true;


    }
    system("cls");
    char x;
    gotoRowCol(31, Width / 4);
    cout << "Which stage you want to play ( EASY (E), MEDIUM (M), HARD (H) )" << endl;
    for (int i = 0; i < 3; i++)
    {
        box(34, (Width / 4) + (15 * i), 10);
        gotoRowCol(34 + 5, ((Width / 4) + (15 * i)) + 3);
        SetClr(97);
        if (i == 0)
        {
            cout << "EASY";
        }
        else if (i == 1)
        {
            cout << "MEDIUM";
        }
        else if (i == 2)
        {
            cout << "HARD";
        }

        SetClr(15);
    }
    do
    {
        getRowColbyLeftClick(row, col);
        if (row >= 34 && row <= 34 + 10 && col >= Width / 4 && col <= (Width / 4) + 10)
        {
            x = 'E';
            which_stage = 1;

        }
        else if (row >= 34 && row <= 34 + 10 && col >= ((Width / 4) + 15) && col <= ((Width / 4) + 15) + 10)
        {
            x = 'M';
            which_stage = 2;
        }
        else if (row >= 34 && row <= 34 + 10 && col >= ((Width / 4) + 30) && col <= ((Width / 4) + 30) + 10)
        {
            x = 'H';
            which_stage = 3;
        }
        else
            x = 'F';
    }while (!(x=='M' || x =='E' || x == 'H'));
    SetClr(15);
    system("cls");
    gotoRowCol(31, Width / 4);
    cout << "What score limit you want (atleast should be 15):  ";
    box(33, Width / 4,7);
    do 
    {
        SetClr(97);
        gotoRowCol(33 + 3, (Width / 4) + 3);
        cin >> Score_limit;
    } while (Score_limit<15);
    SetClr(15);
    system("cls");
    gotoRowCol(31, Width / 4);
    cout << "What should be the time limit (atleast should be 40):  ";
    box(33, Width / 4, 7);
    do
    {
        SetClr(97);
        gotoRowCol(33 + 3, (Width / 4) + 3);
        cin >> time_limit;
    } while (time_limit < 40);
    stage_maker(s,which_stage);
    SetClr(15);
    system("cls");
    all_food_generator(f,tf,No_of_Snakes,NOP,s,false);
    
}


void Snake_Mover( Snake* &No_of_Snakes, int NOP)
{                // This is Tracking Movement piece by piece
    for (int i = 0; i < NOP; i++)
    {
        for (int j = No_of_Snakes[i].size-1; j >= 1; j--)
        {
            // We are leaving the head part because it's going to be calculated according 
            // to the direction of snake
            No_of_Snakes[i].pos[j] = No_of_Snakes[i].pos[j - 1];

        }
        if (No_of_Snakes[i].Dir == UP)
        {
            No_of_Snakes[i].pos[0].pos_row = No_of_Snakes[i].pos[0].pos_row - 1;
            if (No_of_Snakes[i].pos[0].pos_row == 0)
            {
                No_of_Snakes[i].pos[0].pos_row = Height;
            }
        }
        if (No_of_Snakes[i].Dir == DOWN)
        {
            No_of_Snakes[i].pos[0].pos_row = No_of_Snakes[i].pos[0].pos_row + 1;
            if (No_of_Snakes[i].pos[0].pos_row==Height)
            {
                No_of_Snakes[i].pos[0].pos_row = 1;
            }
        }
        if (No_of_Snakes[i].Dir == RIGHT)
        {
            No_of_Snakes[i].pos[0].pos_col = No_of_Snakes[i].pos[0].pos_col + 1;
            if (No_of_Snakes[i].pos[0].pos_col==Width)
            {
                No_of_Snakes[i].pos[0].pos_col = 1;
            }
        }
        if (No_of_Snakes[i].Dir == LEFT)
        {
            No_of_Snakes[i].pos[0].pos_col = No_of_Snakes[i].pos[0].pos_col - 1;
            if (No_of_Snakes[i].pos[0].pos_col <= 1)
            {
                No_of_Snakes[i].pos[0].pos_col = Width;
            }
        }
    }
}
void Move_changer( Snake* &No_of_Snakes, int move ,int NOP)
{  
    // First Identifying that the key that is passed is of which snake 
    int which_snake = 0;
    int loop_breaker = 0;
    for (int i = 0; i < NOP; i++)
    {    
        if (No_of_Snakes[i].alive == true)
        {
            if (No_of_Snakes[i].DOWN_KEY == move || No_of_Snakes[i].UP_KEY == move || No_of_Snakes[i].LEFT_KEY == move || No_of_Snakes[i].RIGHT_KEY == move)
            {
                which_snake = i;
                loop_breaker = 1;
                break;

            }
        }
        
    }

    if (loop_breaker != 0)
    {
        
        
        // _______________
        if (No_of_Snakes[which_snake].UP_KEY == move)
        {
            if (No_of_Snakes[which_snake].Dir != DOWN)
            {
                No_of_Snakes[which_snake].Dir = UP;
                return;
            }
        }
       
        // __________________
        if (No_of_Snakes[which_snake].DOWN_KEY == move)
        {
            if (No_of_Snakes[which_snake].Dir != UP)
            {
                No_of_Snakes[which_snake].Dir = DOWN;
                return;
            }

        }
        //  ___________________
        if (No_of_Snakes[which_snake].RIGHT_KEY == move)
        {
            if (No_of_Snakes[which_snake].Dir != LEFT)
            {
                No_of_Snakes[which_snake].Dir = RIGHT;
                return;
            }
        }
        //_____________________
        if (No_of_Snakes[which_snake].LEFT_KEY == move)
        {
            if (No_of_Snakes[which_snake].Dir != RIGHT)
            {
                No_of_Snakes[which_snake].Dir = LEFT;
                return;
            }
        }
        
    }

}
void Movement_Maker(Snake*& No_of_Snakes, int NOP)
{
    
    for (int i = 0; i < NOP; i++)
    {
        for (int j = 0; j < No_of_Snakes[i].size && No_of_Snakes[i].alive==true; j++)
        {
            SetClr(No_of_Snakes[i].clr);
            gotoRowCol(No_of_Snakes[i].pos[j].pos_row, No_of_Snakes[i].pos[j].pos_col);
            cout << char(-37);
        }
        SetClr(15);
    }



}
void Movement_Erazer(Snake*& No_of_Snakes, int NOP)
{
    for (int i = 0; i < NOP; i++)
    {
        for (int j = 0; j < No_of_Snakes[i].size && No_of_Snakes[i].alive == true; j++)
        {

            gotoRowCol(No_of_Snakes[i].pos[j].pos_row, No_of_Snakes[i].pos[j].pos_col);
            cout << " ";
        }
    }
}



void all_Snake_grower(Snake*& No_of_Snakes, int which_snake, food& f, time_food& tf, bool x)
{
    if (No_of_Snakes[which_snake].alive == true)
    {

        position* decoy = new position[(No_of_Snakes[which_snake].size + 1)];
        if (x == false)
        {
            decoy[0].pos_row = f.fp.pos_row; decoy[0].pos_col = f.fp.pos_col;
        }
        else
        {
            decoy[0].pos_row = tf.fp.pos_row; decoy[0].pos_col = tf.fp.pos_col;
        }
        
        for (int i = 1; i < (No_of_Snakes[which_snake].size + 1); i++)
        {
            decoy[i].pos_row = No_of_Snakes[which_snake].pos[i - 1].pos_row;
            decoy[i].pos_col = No_of_Snakes[which_snake].pos[i - 1].pos_col;
        }
        delete[]No_of_Snakes[which_snake].pos;
        No_of_Snakes[which_snake].pos = nullptr;
        No_of_Snakes[which_snake].pos = decoy;
        decoy = nullptr;
        if (x == false)
        {
            No_of_Snakes[which_snake].size++;
            No_of_Snakes[which_snake].score += f.score;
        }
        else
        {
            No_of_Snakes[which_snake].size++;
            No_of_Snakes[which_snake].score += tf.score;
        }
    }
}

void Snake_killer(Snake* &No_of_snakes, int NOP, stage s)
{
    //____________________________
    for (int i = 0; i < NOP; i++)
    {
        if (No_of_snakes[i].alive == true)
        {
            //______ checking boundries
            if (s.boundry == true)
            {
                if (No_of_snakes[i].pos[0].pos_row <= 1 || No_of_snakes[i].pos[0].pos_row >= Height - 1 || No_of_snakes[i].pos[0].pos_col <= 1 || No_of_snakes[i].pos[0].pos_col >= Width - 1)
                {
                    No_of_snakes[i].score = 0;
                   // delete[]No_of_snakes[i].pos;
                   // No_of_snakes[i].pos = nullptr;
                    No_of_snakes[i].alive = false;
                    No_of_snakes[i].size = 0;
                    return;
                }

            }
            //______________ walls __________
            for (int j = 0; j < s.walls_num; j++)
            {
                for (int k = 0; k < s.wall_lnth[j]; k++)
                {
                    if (No_of_snakes[i].pos[0].pos_row == s.wall[j][k].pos_row && No_of_snakes[i].pos[0].pos_col == s.wall[j][k].pos_col)
                    {

                        No_of_snakes[i].score = 0;
                       // delete[]No_of_snakes[i].pos;
                       // No_of_snakes[i].pos = nullptr;
                        No_of_snakes[i].alive = false;
                        No_of_snakes[i].size = 0;
                        return;
                    }
                }
            }
            ///_________________other snakes __________
            for (int x = 0; x < NOP; x++)
            {
                for (int y = 0; y < No_of_snakes[x].size; y++)
                {
                    if (No_of_snakes[i].pos[0].pos_row == No_of_snakes[x].pos[y].pos_row && No_of_snakes[i].pos[0].pos_col == No_of_snakes[x].pos[y].pos_col)
                    {
                        if (!(x == i && y == 0))
                        {

                            No_of_snakes[i].score = 0;
                          //  delete[]No_of_snakes[i].pos;
                          //  No_of_snakes[i].pos = nullptr;

                            No_of_snakes[i].alive = false;
                            No_of_snakes[i].size = 0;
                            return;
                        }
                    }
                }

            }




        }

    }


}

void Meta_data(int stop_watch, Snake* No_of_Snakes, int NOP)
{
    
    gotoRowCol(20, 131);
    SetClr(4);
    cout << "Timer: " << stop_watch;
    SetClr(15);
    for (int i = 1; i <= NOP; i++)
    {
        gotoRowCol(20+ (i)*2, 120);
        cout << "Player " << i << " is: " << No_of_Snakes[i-1].P_name << " with SCORE: ";
        SetClr(No_of_Snakes[i-1].clr);
        cout << No_of_Snakes[i-1].score;
        SetClr(15);
        gotoRowCol((20 + (i)*2) + 1, 120);
        cout << "Keys are--> UP:" << char(No_of_Snakes[i-1].UP_KEY) << " DOWN:" << char(No_of_Snakes[i-1].DOWN_KEY) << " LEFT:" << char(No_of_Snakes[i-1].LEFT_KEY) << " RIGHT:" << char(No_of_Snakes[i-1].RIGHT_KEY);

    }
}

bool win_checker(Snake * No_of_Snakes,int Time_limit,int NOP,int stop_watch ,int Score_limit, string&msg, string & win_player)
{
    //__________________checking the timer 1st _______________
    if (stop_watch == Time_limit)
    {
        int draw_count = 0,h_score=0,h_score_index=0;
        int score_arr[4]{}, size=1;

        score_arr[0] = No_of_Snakes[0].score;
        h_score = score_arr[0];
        h_score_index = 0;
        for (int i = 1; i < NOP; i++)
        {
            score_arr[i] = No_of_Snakes[i].score;
            if (No_of_Snakes[i].score > h_score)
            {
                h_score = No_of_Snakes[i].score;
                h_score_index = i;
                msg = " THE WINNER IS: ";
                win_player = No_of_Snakes[i].P_name;
            }
        }
        // checking draw condition
        for (int i = 0; i < NOP; i++)
        {
            for (int j = 0; j < NOP; j++)
            {
                if (i != j)
                {
                    if (No_of_Snakes[i].score == No_of_Snakes[j].score)
                    {
                        draw_count++;

                    }
                }
            }
            
        }
        if (draw_count == NOP)
        {
            msg = "IT IS A ";
            win_player = "DRAW";
            return false;
        }
        return false;
    }
    //___________________checking score____________________
    for (int i = 0; i < NOP; i++)
    {
        if (No_of_Snakes[i].score == Score_limit)
        {
            msg = " THE WINNER IS:";
            win_player = No_of_Snakes[i].P_name;
            return false;
        }
    }
    //___________________ checking no of snakes dies__________
    
    
    {
        int snakes_death_count = 0;
        for (int i = 0; i < NOP; i++)
        {
            if (No_of_Snakes[i].alive == false)
            {
                snakes_death_count++;
            }
            else
            {
                msg = "THE WINNER IS: ";
                win_player = No_of_Snakes[i].P_name;
            }
        }
        if (NOP != 1)
        {
            if (snakes_death_count == NOP - 1)
            {
                return false;
            }
        }
        else
        {
            if (snakes_death_count == NOP)
            {
                msg = "You cant even win a single player";
                win_player = "game :(";
                return false;
            }
        }
    }
    
    return true;
}


int main()
    {
    /*  OLD
    NOTE:
    Window width = 195
    Window Height = 102
    */
   
    /*  NEW
    NOTE:
           on DESKTOP Window Width 197 Window Height 104
    Window width = 110
    Window Height = 100
     working width = 110
     working Height = 100
       Raster font 8x8 
    */
    srand(time(0));
    //__________________
    gotoRowCol(Height / 2, (Width / 2)+6);
    cout << "S N A K E       G A M E";
    _getch();
    system("Cls");
    Snake* No_of_Snakes{};
    stage s{};
    food f{};
    time_food tf{};
    //___________________
    int which_stage=0,which_snake=0;
    int Time_limit = 0,Score_limit=0;
    int NOP, timer=0,stop_watch=0, food_time=0, gen_count=0,music_play=0;
    char ch;
    int garbage;
    bool x = false;
    string win_player, msg;
    InIt(No_of_Snakes,NOP,s,which_stage,f,tf,Time_limit,Score_limit);
    system("Cls");
    stage_displayer(s, which_stage);
    all_food_displayer(f,tf,false);
    PlaySound(TEXT("JAZZ.wav"), NULL, SND_ASYNC);
    while (win_checker(No_of_Snakes,Time_limit,NOP,stop_watch,Score_limit,msg,win_player))
    {
        if (music_play==60)
        {
            PlaySound(TEXT("JAZZ.wav"), NULL, SND_ASYNC);
            music_play = 0;
        }
        if (timer == 18)
        {
            stop_watch++;
            music_play++;
            timer = 0;
        }
        
        if (_kbhit())
        {
            ch = _getch();
            if (ch == -32 || ch >= 200)
            {
                ch = _getch();
            }                      // ch is the direction of move of the snake, in which the player wants to turn it in
            Move_changer(No_of_Snakes,int(ch),NOP);
            
        }
        Meta_data(stop_watch,No_of_Snakes,NOP);
        Movement_Maker(No_of_Snakes,NOP);
        Sleep(66);
        Movement_Erazer(No_of_Snakes, NOP);
        Snake_Mover(No_of_Snakes, NOP);
        Snake_killer(No_of_Snakes,NOP,s);
        if (food_eaten(f, No_of_Snakes, which_snake, NOP))
        {
            all_Snake_grower(No_of_Snakes, which_snake, f, tf, false);
            all_food_eraser(f, tf, false);
            all_food_generator(f, tf, No_of_Snakes, NOP, s, false);
            all_food_displayer(f, tf, false);
            food_time++;

        }
        if (food_time == 5 && gen_count==0)
        {
            all_food_generator(f, tf, No_of_Snakes, NOP, s, true);
            all_food_displayer(f, tf, true);
            gen_count++;
        }
        
        if (food_time >= 5)
        {
            if (tfood_eaten(tf,No_of_Snakes,which_snake,NOP))
            {
                all_Snake_grower(No_of_Snakes, which_snake, f, tf, true);
                all_food_eraser(f, tf, true);
                food_time = 0;
                gen_count = 0;
            }
            else if (food_time == 7 * 18)
            {
                all_food_eraser(f, tf, true);
                food_time = 0;
                gen_count = 0;
            }
        }

        timer++;
    }
    PlaySound(nullptr, nullptr, 0);
    system("cls");
    gotoRowCol(Height / 2, Width / 2);
    SetClr(11);
    cout << msg << " " << win_player;
    SetClr(15);
    _getch();
    _getch();
    return 0;


}





//
//int main_1()
//{
//     // Remembetr No.of snakes is the array of sankes with max size 4 and min 1
//    char x;
//    while (true)
//    {
//        if (_kbhit())
//        {
//
//            x = _getch();
//            if (x == '_')
//            {
//                break;
//            }
//            cout << "Character pressed was: " << x << " with ASCII value: " << int(x) << endl;
//        }
//    }
//    return 0;
//}
//
//int main_3()
//{
//    while (true)
//    {
//        int rpos = 0, cpos = 0;
//        getRowColbyLeftClick(rpos, cpos);
//        cout << rpos << "  " << cpos << endl;
//
//    
//    }
//    return 0;
//}
//
//int main_2()
//{
//    for (int i = 45, j = (Width/4)+4; i < 75; i++, j++)
//    {
//        gotoRowCol(i, j);
//        cout << "x";
//    }
//    return 0;
//}