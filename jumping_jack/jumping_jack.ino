
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "Tinyfont.h"

Arduboy2 arduboy;
Arduboy2Audio audio;
ArduboyTones sound(arduboy.audio.enabled);

Tinyfont tinyfont = Tinyfont(arduboy.sBuffer, Arduboy2::width(), Arduboy2::height());

#define GAME_TITLE 0
#define GAME_MENU 1
#define GAME 2
#define GAME_OVER 3
#define STAGE_LEVEL_COMPLETED 4

bool Enable_Audio = true;

byte Game_Stage = GAME_MENU;

#define LEFT 0
#define RIGHT 1

#define Screen_Width 128
#define Screen_Height 64

int Lines_Position_Y[9] = {63, 42, 21, 0, -21, -42, -63, -84, -105 };
int Jack_Lines_Position_Y[9] = {47, 26, 5, -16, -37, -58, -79, -100, -121}; 
#define Height  21

#define Jack_Standing 0
#define Jack_Running_Right 1
#define Jack_Running_Left 2
#define Jack_Jumping 3
#define Jack_Falling 4
#define Jack_Stunned 5
#define Jack_Crashing 6

#define Jack_Width 16
#define Jack_Height 16
#define Jack_Middle_Screen_Position_Y  26

#define Max_Speed_Game 5
#define Min_Speed_Game 1
byte Speed_Game = 2;
unsigned int Time_Game = 0;
unsigned int Hi_Time_Game[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};



#define Number_of_Levels 2-;

bool Classic_Version = true; // true -> classic version, false -> modern - you can shoot to hazzards
byte Lives = 6;
byte Level = 0;
bool Unlimited_Lives = false;
bool Invisibility = false;
bool Easy_Mode = false;
unsigned int Score = 0;
unsigned int Hi_Score[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


byte Jack_Status=Jack_Standing;
byte Jack_Last_Direction_Status=Jack_Running_Right;
byte Jack_Line_Positon = 0;
int Jack_x = 0;
int Jack_y = Jack_Lines_Position_Y[Jack_Line_Positon];
int Last_Jack_y;
int Jack_Speed = Speed_Game;
byte Jack_Frame_Rate = 10;
int Jack_Stunned_Timer = 0;
#define Jack_Stunned_Timeout 2

int Shoot_x;
int Shoot_y;
byte Shoot_Line_Position;
bool Shooting = false;
byte Shoot_direction;   //Shoot_direction 0 = LEFT, 1 = RIGHT
byte Shooting_Down_Counter = 0; //Count downed Hazzards
int Shooting_Counter = 0; //Count Shoots

bool Scrolling = false;
int Jack_Frame = 0;
byte frameRate = 30;

#define Gaps_Max_On_Scren 8
byte Gaps_On_Screen = 2;
int Gaps_x[8] = {32, 64, 0, 0, 0, 0, 0};
int Gaps_Line_Y_Position[8]={4, 5, 0, 0, 0, 0, 0};
byte Gaps_Size = 20;
byte Gaps_Direction[8] = {0, 1, 1, 1, 1, 0, 0, 0}; //0 - to left / 1 - to right // prawa ida w dol na koncu ekranu, lewe ida do gory
#define Gaps_Color BLACK
byte Gaps_Speed = Speed_Game;
byte Last_Jumped_Gap;

byte Hazzards_Frame_Rate = 5;
#define Hazzards_Width 16
#define Hazzards_Height 16
#define Hazzards_Types_No 10

byte Hazzards_on_Screen;
int Hazzards_x[20] = {};
int Hazzards_Line_Y_Position[20] = {};
byte Hazzards_Speed = Speed_Game;
byte Hazzards_Type[20] = {0, 1, 2, 3, 4};
int Hazzards_Frame = 0;
bool Hazzards_Life_Status[20] = {};
bool Hazzards_Jack_Collide[20] = {};
int Hazzards_Jack_Collide_Counter = 0;


void select_game_menu(){
    
    bool exit_loop = false;
    byte Selected_Item = 16;
    do{
        arduboy.pollButtons();
        arduboy.clear();
        
        arduboy.setCursor(30, 0);
        arduboy.print("GAME SETTINGS");
        arduboy.drawLine(0,8,128,8, WHITE);

        
        
        tinyfont.setCursor(4, Selected_Item);
        tinyfont.print(">");
        
        tinyfont.setCursor(12, 16);
        tinyfont.print("TYPE:");
        tinyfont.setCursor(40, 16);
        if (Classic_Version){tinyfont.print("CLASSIC");
        }else{tinyfont.print("EXTENDED");}
            
            
        tinyfont.setCursor(12, 22);
        tinyfont.print("LEVEL:");
        tinyfont.setCursor(45, 22);
        tinyfont.print(Level);
        
        tinyfont.setCursor(12, 28);
        tinyfont.print("SPEED:");
        tinyfont.setCursor(45, 28);
        tinyfont.print(Speed_Game);
        
        tinyfont.setCursor(12, 34);
        tinyfont.print("UNLIMITED LIVES:");     
        tinyfont.setCursor(95, 34);
        if (Unlimited_Lives){tinyfont.print("YES");
        }else {tinyfont.print("NO");}
        
        
        tinyfont.setCursor(12, 40);
        tinyfont.print("INVISIBILITY:");  
        tinyfont.setCursor(80, 40);
        if (Invisibility){tinyfont.print("YES");
        }else {tinyfont.print("NO");}     
        
        tinyfont.setCursor(12, 46);
        tinyfont.print("EASY MODE:");  
        tinyfont.setCursor(65, 46);
        if (Easy_Mode){tinyfont.print("YES");
        }else {tinyfont.print("NO");}          
        
        tinyfont.setCursor(12, 52);
        tinyfont.print("SOUND:");     
        tinyfont.setCursor(45, 52);
        if (Enable_Audio){tinyfont.print("YES");
        }else {tinyfont.print("NO");}
        

        if (arduboy.justPressed(UP_BUTTON)) {
                Selected_Item = Selected_Item - 6;
            if (Selected_Item < 16) {
                Selected_Item = 52;
            }
        }   
        if (arduboy.justPressed(DOWN_BUTTON)) {
            Selected_Item = Selected_Item + 6;
            if (Selected_Item > 52) {
               Selected_Item = 16;
            }
        }


        if (arduboy.justPressed(RIGHT_BUTTON)) {
            switch (Selected_Item){
                case 16:    //Type: Classic/Modern
                    Classic_Version = !Classic_Version;
                    break;
                case 22:    //Level: 0 - 20
                    Level = Level + 1;
                    if (Level > 20) {
                        Level = 0;
                    }
                    break;
                case 28:    //Speed: 1-5
                    Speed_Game = Speed_Game + 1;
                    if (Speed_Game > Max_Speed_Game) {
                        Speed_Game = Min_Speed_Game;
                    }   
                    break;
                case 34:    // Unlimited Lives: Y/N
                    Unlimited_Lives = !Unlimited_Lives; 
                    break;  
                case 40:    //Invisibility:  Y/N
                    Invisibility = !Invisibility;
                    break;
                case 46:    //Easy_Mode:  Y/N - Jack not stunned after falling or jumming and catch line.
                    Easy_Mode = !Easy_Mode;
                    break;
                case 52:    //Audio enable:  Y/N - J.
                    Enable_Audio = !Enable_Audio;
                    break;
                default:
                    break;
            }
        }   
        
        if (arduboy.justPressed(LEFT_BUTTON)) {
            switch (Selected_Item){
                case 16:    //Type: Classic/Modern
                    Classic_Version = !Classic_Version;
                    break;
                case 22:    //Level: 0 - 20
                    Level = Level - 1;
                    if (Level == 255) {
                        Level = 20;
                    }
                    break;
                case 28:    //Speed: 1-5
                    Speed_Game = Speed_Game - 1;
                    if (Speed_Game < Min_Speed_Game) {
                        Speed_Game = Max_Speed_Game;
                    }  
                    break;
                case 34:    // Unlimited Lives: Y/N
                    Unlimited_Lives = !Unlimited_Lives;
                    break;
                case 40:    //Invisibility:  Y/N
                    Invisibility = !Invisibility;
                    break;
                case 46:    //Easy_Mode:  Y/N - Jack not stunned after falling or jumming and catch line.
                    Easy_Mode = !Easy_Mode;
                    break;
                case 52:    //Audio enable:  Y/N - J.
                    Enable_Audio = !Enable_Audio;
                    break;
                default:
                    break;
            }
            
        }
        if (arduboy.justPressed(A_BUTTON)) {
            
            exit_loop = true;
        }
        arduboy.display();

                
    }while(!exit_loop);
    
    if(Enable_Audio){ audio.on(); }
    else { audio.off(); }
    
    Hazzards_on_Screen = Level;
    randomSeed(millis());
    
    generate_gaps_parameters();
    generate_hazards_parameters();
    
    Lives = 6;
    Time_Game = 0;
    
    Jack_Speed = Speed_Game;
    Gaps_Speed = Speed_Game;
    Hazzards_Speed = Speed_Game;
    Hazzards_on_Screen = Level;
    
    Lines_Position_Y[0] = 63;
    Lines_Position_Y[1] = 42;
    Lines_Position_Y[2] = 21;
    Lines_Position_Y[3] = 0;
    Lines_Position_Y[4] = -21;
    Lines_Position_Y[5] = -42;
    Lines_Position_Y[6] = -63;
    Lines_Position_Y[7] = -84;
    Lines_Position_Y[8] = -105;

    Jack_Lines_Position_Y[0] = 47;
    Jack_Lines_Position_Y[1] = 26;
    Jack_Lines_Position_Y[2] = 5;
    Jack_Lines_Position_Y[3] = -16;
    Jack_Lines_Position_Y[4] = -37;
    Jack_Lines_Position_Y[5] = -58;
    Jack_Lines_Position_Y[6] = -79;
    Jack_Lines_Position_Y[7] = -100;
    Jack_Lines_Position_Y[8] = -121;
    
    Jack_Status=Jack_Standing;
    Jack_Line_Positon = 0;
    Jack_x = 56;
    Jack_y = Jack_Lines_Position_Y[Jack_Line_Positon];
    
    Score = 0;
    Shooting = false;
    Shooting_Down_Counter = 0;
    Shooting_Counter = Level;   //number of shoots = number of hazzards
    Hazzards_Jack_Collide_Counter = 0;
    

   
   Game_Stage = GAME;
}



void level_completed(){


    bool exit_loop = false;
    

     
       
        arduboy.clear();
        arduboy.setCursor(9, 0);
        arduboy.print("LEVEL");
        arduboy.setCursor(45, 0);
        arduboy.print(Level);
        arduboy.setCursor(63, 0);
        arduboy.print("COMPLETED");

        

        if (Score > Hi_Score[Level]) {
            Hi_Score[Level] =  Score;
            tinyfont.setCursor(24, 10);
            tinyfont.print("!!! NEW HIGH !!!");
        } 
        
        if ((Time_Game < Hi_Time_Game[Level]) || (Hi_Time_Game[Level] == 0)) {
            Hi_Time_Game[Level] =  Time_Game;
            tinyfont.setCursor(15, 16);
            tinyfont.print("!!! TIME RECORD !!!");
        } 
         
        tinyfont.setCursor(10, 24);
        tinyfont.print("SCORE:");
        tinyfont.setCursor(40, 24);
        tinyfont.print(Score);
        
        tinyfont.setCursor(10, 30);
        tinyfont.print("HI SCORE:");
        tinyfont.setCursor(55, 30);
        tinyfont.print(Hi_Score[Level]);
        
        tinyfont.setCursor(10, 36);
        tinyfont.print("TIME:");
        tinyfont.setCursor(35, 36);
        tinyfont.print(Time_Game);
        
        tinyfont.setCursor(10, 42);
        tinyfont.print("TIME RECORD:");
        tinyfont.setCursor(70, 42);
        tinyfont.print(Hi_Time_Game[Level]);
        
        tinyfont.setCursor(10, 48);
        tinyfont.print("FAILS:");
        tinyfont.setCursor(40, 48);
        tinyfont.print(Hazzards_Jack_Collide_Counter);
        
        if (!Classic_Version){
            tinyfont.setCursor(10, 54);
            tinyfont.print("HITS:");
            tinyfont.setCursor(35, 54);
            tinyfont.print(Shooting_Down_Counter); 
          
        }
    
        if (Level < 20) {
            tinyfont.setCursor(12, 59);
            tinyfont.print("PRESS A TO CONTINUE");
        }
        if (Level == 20) {
            tinyfont.setCursor(24, 59);
            tinyfont.print("CONGRATULATIONS");
            
        }   
        arduboy.display();
        do{
             arduboy.pollButtons();
            if (arduboy.justPressed(A_BUTTON)) {
                exit_loop = true;
            }
        

    
                
    }while(!exit_loop);
  
   Level = Level + 1;
   if (Level > 20) {Level = 0;}
   
   Game_Stage = GAME_MENU;
}




void stage_game_over(){

    bool exit_loop = false;

   
        
        
        arduboy.clear();
        arduboy.setCursor(36, 0);
        arduboy.print("GAME OVER");
         
        tinyfont.setCursor(10, 18);
        tinyfont.print("LEVEL:");
        tinyfont.setCursor(40, 18);
        tinyfont.print(Level);
        
        tinyfont.setCursor(10, 24);
        tinyfont.print("SCORE:");
        tinyfont.setCursor(40, 24);
        tinyfont.print(Score);
        
        tinyfont.setCursor(10, 30);
        tinyfont.print("HI SCORE:");
        tinyfont.setCursor(55, 30);
        tinyfont.print(Hi_Score[Level]);
        
        tinyfont.setCursor(10, 36);
        tinyfont.print("TIME:");
        tinyfont.setCursor(35, 36);
        tinyfont.print(Time_Game);
        
        tinyfont.setCursor(10, 42);
        tinyfont.print("TIME RECORD:");
        tinyfont.setCursor(70, 42);
        tinyfont.print(Hi_Time_Game[Level]);
        
        tinyfont.setCursor(10, 48);
        tinyfont.print("FAILS:");
        tinyfont.setCursor(40, 48);
        tinyfont.print(Hazzards_Jack_Collide_Counter);
        
        if (!Classic_Version){
            tinyfont.setCursor(10, 54);
            tinyfont.print("HITS:");
            tinyfont.setCursor(35, 54);
            tinyfont.print(Shooting_Down_Counter); 
          
        }
    
    
        tinyfont.setCursor(11, 59);
        tinyfont.print("PRESS A TO TRY AGAIN");
        
       arduboy.display();
        do{
            arduboy.pollButtons();
            if (arduboy.justPressed(A_BUTTON)) {
            exit_loop = true;
        }
        
        
                
    }while(!exit_loop);
  
   
   Game_Stage = GAME_MENU;
}

void check_jack_hazzards_collide(){ //check if Jack collide with hazzards
    if ((Jack_x >= 0) && (Jack_x<=Screen_Width-Jack_Width)){        //tylko jezeli Jack jest na ekranie
    for(int i=0; i < Hazzards_on_Screen; i++){
        if((!Hazzards_Jack_Collide[i]) && (Hazzards_Life_Status[i])){
                                                                   
            if (arduboy.collide(Rect(Jack_x+6, Jack_y, 4, 5), Rect(Hazzards_x[i]+10, Jack_Lines_Position_Y[Hazzards_Line_Y_Position[i]], 4, 5 ))){
                Hazzards_Jack_Collide[i] = true;
          //      Hazzards_Life_Status[i] = false;        //zabija jak sie spotkaaja - tylko test
                Hazzards_Jack_Collide_Counter = Hazzards_Jack_Collide_Counter + 1;
                if ((!Invisibility) && (Jack_Status != Jack_Jumping) && (Jack_Status != Jack_Crashing)){
                    Jack_Status = Jack_Stunned;
                    Jack_Stunned_Timer = Jack_Stunned_Timeout;
                }
                 sound.tone(100,100,50,400,10, 500);

            }
                
        }
    }
    }
    
}

void check_if_jack_shot_down_the_hazzard(){ //check if Jack collide with hazzards
    for(int i=0; i<Hazzards_on_Screen; i++){
        if(Hazzards_Life_Status[i]){

            if (arduboy.collide(Rect(Shoot_x, Jack_Lines_Position_Y[Shoot_Line_Position]+7, 1, 1), Rect(Hazzards_x[i]+2, Jack_Lines_Position_Y[Hazzards_Line_Y_Position[i]], Hazzards_Width-4, Hazzards_Height ))){
                Hazzards_Life_Status[i] = false;        //zabija jak trafi
                Shooting_Down_Counter = Shooting_Down_Counter + 1;
                sound.tone(80, 50, 50,50); 
                Shooting = false;
            }
                
        }
    }
    
}

void jack_shooting(){
    if(Shooting){
        check_if_jack_shot_down_the_hazzard();
        
        if (Shoot_direction == RIGHT){
            arduboy.drawPixel(Shoot_x+6, Jack_Lines_Position_Y[Shoot_Line_Position]+7, WHITE);
            Shoot_x = Shoot_x + Speed_Game+1;
            if (Shoot_x > Screen_Width) { Shooting = false;}
        } 
         if (Shoot_direction == LEFT){
            arduboy.drawPixel(Shoot_x-6, Jack_Lines_Position_Y[Shoot_Line_Position]+7, WHITE);
            Shoot_x = Shoot_x - (Speed_Game+1);
            if (Shoot_x < 0) { Shooting = false;}
        }    
    }

}

bool check_hazzards_collide(byte hazzard_no){   //check if the hazzards do not collide with each other
    bool hazzards_collide = false;
    for(int i=0; i<Hazzards_on_Screen; i++){
        if (i != hazzard_no){
            if (arduboy.collide(Rect(Hazzards_x[i], Jack_Lines_Position_Y[Hazzards_Line_Y_Position[i]], Hazzards_Width, Hazzards_Height ), Rect(Hazzards_x[hazzard_no], Jack_Lines_Position_Y[Hazzards_Line_Y_Position[hazzard_no]], Hazzards_Width, Hazzards_Height ))){

                hazzards_collide = true;
            }
        }
    }
    return hazzards_collide;
}

void generate_hazards_parameters(){
    
   for (int i=0; i<20; i++){
       Hazzards_Life_Status[i] = false;
       Hazzards_Jack_Collide[i] = false;
       Hazzards_x[i] = 250;
       Hazzards_Line_Y_Position[i] = 8;
    }
    
    
    for(int i=0; i<Hazzards_on_Screen; i++){
        Hazzards_x[i] = random(0, (128-16));
        Hazzards_Line_Y_Position[i] = random(1, 8);
        Hazzards_Type[i] = random(0, Hazzards_Types_No);
        Hazzards_Life_Status[i] = true;
        Hazzards_Jack_Collide[i] = false;
        
        while (check_hazzards_collide(i)){
            Hazzards_x[i] = random(0, (128-16));
            Hazzards_Line_Y_Position[i] = random(1, 8);
        }
    }    
}

void generate_gaps_parameters(){
    Gaps_x[0] = 32; 
    Gaps_x[1] = 64;
    Gaps_Line_Y_Position[0] = 4;
    Gaps_Line_Y_Position[1] = 5;
    Gaps_On_Screen = 2;
    
    for(int i=2; i< 8; i++){
        Gaps_x[i] = random(0, (128-Gaps_Size));
        Gaps_Line_Y_Position[i] = random(1, 9);
  
    }
}

const uint8_t PROGMEM Jack[7][130] = {
    
    {   //Jack Standing
        16, 16,
        0x00, 0x00, 0x00, 0x00, 0x3c, 0x4a, 0xdb, 0xd7, 0xdb, 0x4a, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x08, 0x84, 0xc2, 0x31, 0x0d, 0x03, 0x0d, 0x31, 0xc2, 0x84, 0x08, 0x00, 0x00, 0x00, 

        0x00, 0x00, 0x10, 0x18, 0x3c, 0x5a, 0xdb, 0xcf, 0xef, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x08, 0x84, 0xc2, 0x31, 0x0d, 0x03, 0x0d, 0xb1, 0xc6, 0x08, 0x00, 0x00, 0x00, 0x00,

        0x00, 0x00, 0x00, 0x00, 0x3c, 0x4a, 0xdb, 0xd7, 0xdb, 0x4a, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x08, 0x84, 0xc2, 0x31, 0x0d, 0x03, 0x0d, 0x31, 0xc2, 0x84, 0x08, 0x00, 0x00, 0x00, 

        0x00, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xef, 0xcf, 0xdb, 0x5a, 0x3c, 0x18, 0x10, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x08, 0xc6, 0xb1, 0x0d, 0x03, 0x0d, 0x31, 0xc2, 0x84, 0x08, 0x00, 0x00, 0x00, 
    },
    
    {   //Jack Running Right
        16, 16,
        0x00, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xef, 0xcf, 0xdb, 0x5a, 0x3c, 0x98, 0x10, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x02, 0x63, 0x23, 0xff, 0xa7, 0x2a, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 

        0x00, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xef, 0xcf, 0xdb, 0x5a, 0x3c, 0x98, 0x10, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0xc4, 0xb6, 0x0f, 0x03, 0x03, 0x02, 0x03, 0x1c, 0x10, 0x00, 0x00, 0x00, 

        0x00, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xef, 0xcf, 0xdb, 0x5a, 0x3c, 0x18, 0x10, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x66, 0x21, 0x21, 0x31, 0x0d, 0x03, 0x05, 0x09, 0x12, 0x24, 0xc2, 0x81, 0x00, 0x00, 

        0x00, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xef, 0xcf, 0xdb, 0x5a, 0x3c, 0x98, 0x10, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x04, 0x6a, 0x29, 0x23, 0x2f, 0x32, 0xc1, 0x80, 0x00, 0x00, 0x00, 0x00, 
    },
    {   //Jack Running Right
        16, 16,
        0x00, 0x00, 0x10, 0x98, 0x3c, 0x5a, 0xdb, 0xcf, 0xef, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x31, 0x2a, 0xa7, 0xff, 0x23, 0x63, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 

        0x00, 0x00, 0x10, 0x98, 0x3c, 0x5a, 0xdb, 0xcf, 0xef, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x10, 0x1c, 0x03, 0x02, 0x03, 0x03, 0x0f, 0xb6, 0xc4, 0x00, 0x00, 0x00, 0x00, 0x00, 

        0x00, 0x00, 0x10, 0x18, 0x3c, 0x5a, 0xdb, 0xcf, 0xef, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x81, 0xc2, 0x24, 0x12, 0x09, 0x05, 0x03, 0x0d, 0x31, 0x21, 0x21, 0x66, 0x00, 0x00, 0x00, 
       
        0x00, 0x00, 0x10, 0x98, 0x3c, 0x5a, 0xdb, 0xcf, 0xef, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x80, 0xc1, 0x32, 0x2f, 0x23, 0x29, 0x6a, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 

    },
    {   //Jack Jumping - 2 i 3 klatka taka sama
        16, 16,
        0x00, 0x00, 0x00, 0x00, 0x3c, 0x4a, 0xdb, 0xd7, 0xdb, 0x4a, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x08, 0x84, 0xc2, 0x31, 0x0d, 0x03, 0x0d, 0x31, 0xc2, 0x84, 0x08, 0x00, 0x00, 0x00, 

        0x00, 0x00, 0x00, 0x00, 0x3c, 0x6a, 0xeb, 0xe7, 0xeb, 0x6a, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x1e, 0x81, 0x7d, 0x03, 0x7d, 0x81, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 
         
        0x00, 0x00, 0x00, 0x00, 0x3c, 0x4a, 0xdb, 0xd7, 0xdb, 0x4a, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x1e, 0x81, 0x7d, 0x03, 0x7d, 0x81, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 

        0x00, 0x00, 0x00, 0x00, 0x3c, 0x4a, 0xdb, 0xd7, 0xdb, 0x4a, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x1e, 0x81, 0x7d, 0x03, 0x7d, 0x81, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00,
    },
    {   //Jack Falling
        16, 16,
        0x00, 0x00, 0x20, 0x40, 0xbc, 0x7a, 0xdb, 0xd7, 0xdb, 0x7a, 0xbc, 0x40, 0x20, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x80, 0xc0, 0x31, 0x0d, 0x03, 0x0d, 0x31, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00,  
        
        0x00, 0x00, 0x00, 0x00, 0x3c, 0x7a, 0xdb, 0xd7, 0xdb, 0x7a, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x08, 0x84, 0xc2, 0x31, 0x0d, 0x03, 0x0d, 0x31, 0xc2, 0x84, 0x08, 0x00, 0x00, 0x00, 

        0x00, 0x00, 0x00, 0x00, 0x3c, 0x7a, 0xdb, 0xd7, 0xdb, 0x7a, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x08, 0x84, 0xc2, 0x31, 0x0d, 0x03, 0x0d, 0x31, 0xc2, 0x84, 0x08, 0x00, 0x00, 0x00, 

        0x00, 0x00, 0x00, 0x00, 0x3c, 0x7a, 0xdb, 0xd7, 0xdb, 0x7a, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x08, 0x84, 0xc2, 0x31, 0x0d, 0x03, 0x0d, 0x31, 0xc2, 0x84, 0x08, 0x00, 0x00, 0x00, 
    },
    {   //Jack Stunned
        
        16, 16,
        0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x40, 0x00, 0x00, 0x82, 0x20, 0x00, 0x04, 0x00, 
        0x01, 0x06, 0x18, 0x20, 0x20, 0x23, 0x2c, 0x30, 0x70, 0x7c, 0xc2, 0xff, 0xff, 0xf6, 0x7c, 0x38, 
        
        0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x00, 0xa0, 0x02, 0x00, 0x08, 0x00, 
        0x01, 0x06, 0x18, 0x20, 0x20, 0x23, 0x2c, 0x30, 0x70, 0x7c, 0xc2, 0xff, 0xff, 0xf6, 0x7c, 0x38, 

        0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x10, 0x00, 0x00, 0x04, 0x20, 0x80, 0x00, 0x02, 0x10, 0x00, 
        0x00, 0x00, 0x00, 0x3f, 0x20, 0x20, 0x20, 0x3f, 0x70, 0x7c, 0xc2, 0xff, 0xff, 0xf6, 0x7c, 0x38,

        0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x08, 0x00, 0x40, 0x02, 0x80, 0x00, 0x20, 0x02, 0x00, 
        0x00, 0x00, 0x00, 0x3f, 0x20, 0x20, 0x20, 0x3f, 0x70, 0x7c, 0xc2, 0xff, 0xff, 0xf6, 0x7c, 0x38, 
    },
    {   //Jack Crashing
        16, 16,
        0x00, 0x00, 0x00, 0x00, 0x3c, 0x7a, 0xdb, 0xd7, 0xdb, 0x7a, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x08, 0x84, 0xc2, 0x31, 0x0d, 0x03, 0x0d, 0x31, 0xc2, 0x84, 0x08, 0x00, 0x00, 0x00,

        0x00, 0x00, 0x20, 0x40, 0xbc, 0x7a, 0xdb, 0xd7, 0xdb, 0x7a, 0xbc, 0x40, 0x20, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x03, 0x02, 0x04, 0x05, 0x09, 0x0f, 0x09, 0x05, 0x04, 0x02, 0x03, 0x00, 0x00, 0x00, 

        0x08, 0x14, 0x20, 0x40, 0x84, 0x48, 0x50, 0x20, 0x70, 0xf8, 0x84, 0xff, 0xfe, 0xe4, 0xf8, 0x70, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 

        0x08, 0x14, 0x20, 0x40, 0x84, 0x48, 0x50, 0x20, 0x70, 0xf8, 0x84, 0xff, 0xfe, 0xe4, 0xf8, 0x70, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 
     
    }

};

const uint8_t PROGMEM Hazzards[Hazzards_Types_No][130] = {
    
    {   //0-Aeroplane
        16, 16, 
        0x00, 0x80, 0xc0, 0x40, 0xc0, 0x40, 0x40, 0xc0, 0x40, 0x40, 0xc0, 0x40, 0x60, 0xf0, 0xf8, 0x80, 
        0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x0f, 0x1f, 0x1f, 0x3b, 0x33, 0x03, 0x03, 0x01, 

        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0x00, 
        0x04, 0x0e, 0x0f, 0x0d, 0x0f, 0x0d, 0x0d, 0x1f, 0x3d, 0x7d, 0x7f, 0xed, 0xcd, 0x0f, 0x0f, 0x06, 

        0x00, 0x80, 0xc0, 0x40, 0xc0, 0x40, 0x40, 0xc0, 0x40, 0x40, 0xc0, 0x40, 0x60, 0xf0, 0xf8, 0x80, 
        0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x0f, 0x1f, 0x1f, 0x3b, 0x33, 0x03, 0x03, 0x01, 

        0x40, 0xe0, 0xf0, 0xd0, 0xf0, 0xd0, 0xd0, 0xf0, 0xd0, 0xd0, 0xf0, 0xd0, 0xd8, 0xfc, 0xfe, 0x60, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x0e, 0x0c, 0x00, 0x00, 0x00, 
    },
    {   //1-Bus
        16, 16,
        0x00, 0x80, 0x00, 0x00, 0xf8, 0x88, 0x88, 0xf8, 0x88, 0x88, 0xf8, 0x08, 0x08, 0x08, 0xf8, 0x00, 
        0x0f, 0x6f, 0x1f, 0x4f, 0xef, 0x4f, 0x1f, 0x7f, 0x7f, 0x7f, 0x1f, 0x48, 0xe8, 0x48, 0x1f, 0x60, 

        0x00, 0x00, 0x00, 0x00, 0xf0, 0x10, 0x10, 0xf0, 0x10, 0x10, 0xf0, 0x10, 0x10, 0x10, 0xf0, 0x00, 
        0x0e, 0x6f, 0x1e, 0xae, 0x4f, 0xaf, 0x1f, 0x7f, 0x7f, 0x7f, 0x1f, 0xa8, 0x48, 0xa8, 0x1f, 0x60, 

        0x00, 0x00, 0x00, 0x00, 0xe0, 0x20, 0x20, 0xe0, 0x20, 0x20, 0xe0, 0x20, 0x20, 0x20, 0xe0, 0x00, 
        0x0c, 0x6e, 0x1c, 0x4c, 0xef, 0x4e, 0x1e, 0x7f, 0x7e, 0x7e, 0x1f, 0x48, 0xe8, 0x48, 0x1f, 0x60, 
    
        0x00, 0x00, 0x00, 0x00, 0xf0, 0x10, 0x10, 0xf0, 0x10, 0x10, 0xf0, 0x10, 0x10, 0x10, 0xf0, 0x00, 
        0x0e, 0x6f, 0x1e, 0xae, 0x4f, 0xaf, 0x1f, 0x7f, 0x7f, 0x7f, 0x1f, 0xa8, 0x48, 0xa8, 0x1f, 0x60, 
    },
    {   //2-Axe
        16, 16,
        0x00, 0x18, 0x78, 0xf8, 0xfc, 0xfc, 0xfc, 0xff, 0x7f, 0x3e, 0xfe, 0xc0, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x01, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x3c, 0xf0, 0xc0, 

        0x00, 0x00, 0x80, 0xc0, 0xc0, 0xf0, 0xf0, 0xf0, 0xf0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x02, 0x07, 0x0f, 0x1f, 0x3f, 0x0f, 0x03, 0x00, 0x01, 0x03, 0x07, 0x0e, 0x1c, 0x38, 0x70, 0xe0, 

        0x00, 0x00, 0x00, 0xb0, 0xf0, 0xf0, 0xf0, 0xf0, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x0c, 0x1e, 0x3f, 0x7f, 0xff, 0x3f, 0x07, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x06, 0x06, 0x0c, 
        
        0x00, 0x00, 0x80, 0xc0, 0xc0, 0xf0, 0xf0, 0xf0, 0xf0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x02, 0x07, 0x0f, 0x1f, 0x3f, 0x0f, 0x03, 0x00, 0x01, 0x03, 0x07, 0x0e, 0x1c, 0x38, 0x70, 0xe0, 
    },
    {   //3-Ray
        16, 16,
        0x30, 0xf8, 0xfc, 0xfc, 0xf8, 0xcc, 0xce, 0xff, 0xff, 0xce, 0xcc, 0xf8, 0xf8, 0xfc, 0xf8, 0x30, 
        0x00, 0x01, 0x1f, 0x3f, 0x7f, 0x7f, 0x7f, 0xff, 0xff, 0x7f, 0x7f, 0x7f, 0x3f, 0x1f, 0x01, 0x00, 

        0x00, 0x30, 0xf0, 0xf8, 0xfc, 0xcc, 0xce, 0xff, 0xff, 0xce, 0xcc, 0xfc, 0xf8, 0xf0, 0x00, 0x00, 
        0x00, 0x00, 0x01, 0x1f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x0f, 0x00, 0x00, 0x00, 

        0x00, 0x00, 0x00, 0x38, 0xfc, 0xfe, 0xe6, 0xff, 0xff, 0xe6, 0xfe, 0xfe, 0x7c, 0x1c, 0x08, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x3f, 0x7f, 0x7f, 0xff, 0xc7, 0x80, 0x00, 0x00, 0x00, 

        0x00, 0x30, 0xf0, 0xf8, 0xfc, 0xcc, 0xce, 0xff, 0xff, 0xce, 0xcc, 0xfc, 0xf8, 0xf0, 0x00, 0x00, 
        0x00, 0x00, 0x01, 0x1f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x0f, 0x00, 0x00, 0x00, 
    },
    {   //4-Farmer
        16, 16,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x28, 0x78, 0xac, 0xbf, 0xbf, 0xff, 0x3c, 0x08, 0x08, 
        0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x0e, 0x9f, 0xff, 0x26, 0x06, 0x06, 0x0e,
   
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x28, 0x78, 0xac, 0xbf, 0xbf, 0xff, 0x3c, 0x08, 0x08, 
        0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x42, 0x42, 0x3e, 0x0f, 0x0f, 0xb6, 0xc6, 0x06, 0x0e,         
           
        0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x08, 0x28, 0x38, 0xec, 0xbf, 0xbf, 0xff, 0x3c, 0x08, 0x08, 
        0x15, 0x0a, 0x05, 0x00, 0x02, 0x02, 0x82, 0x82, 0x62, 0x1e, 0x0f, 0x9f, 0xe6, 0x06, 0x06, 0x0e, 

        0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x08, 0x28, 0x78, 0xac, 0xbf, 0xbf, 0xff, 0x3c, 0x08, 0x08, 
        0x15, 0x0a, 0x05, 0x00, 0x02, 0x02, 0x02, 0x02, 0x82, 0xfe, 0x0f, 0x1f, 0x16, 0xa6, 0x46, 0x0e,         
    },
    {   //5-Octopus
        16, 16,
        0x00, 0x00, 0x18, 0x3c, 0xb6, 0xf6, 0xff, 0xff, 0xff, 0xf6, 0xb6, 0x3c, 0x18, 0x00, 0x00, 0x00, 
        0xe0, 0x18, 0x06, 0xc1, 0x3c, 0x03, 0xcf, 0x3f, 0xcf, 0x03, 0x3c, 0xc1, 0x06, 0x38, 0xc0, 0x00, 
                
        0x00, 0x00, 0x60, 0xf0, 0xd8, 0xd8, 0xfc, 0xfc, 0xfc, 0xd8, 0xd8, 0xf0, 0x60, 0x00, 0x00, 0x00, 
        0xc0, 0x30, 0x08, 0xc4, 0x32, 0x0f, 0xbf, 0x7f, 0xbf, 0x0f, 0x32, 0xc4, 0x08, 0x30, 0xc0, 0x00, 
                
        0x00, 0x00, 0x80, 0xc0, 0x60, 0x60, 0xf0, 0xf0, 0xf0, 0x60, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x00, 
        0x80, 0x40, 0x21, 0x93, 0x4b, 0x3f, 0xbf, 0x7f, 0xbf, 0x3f, 0x4b, 0x93, 0x21, 0x40, 0x80, 0x00, 
      
        0x00, 0x00, 0x60, 0xf0, 0xd8, 0xd8, 0xfc, 0xfc, 0xfc, 0xd8, 0xd8, 0xf0, 0x60, 0x00, 0x00, 0x00, 
        0xc0, 0x30, 0x08, 0xc4, 0x32, 0x0f, 0xbf, 0x7f, 0xbf, 0x0f, 0x32, 0xc4, 0x08, 0x30, 0xc0, 0x00, 
      
        
    },
    {   //6-Dinosaur
        16, 16,
        0x00, 0x70, 0x68, 0x38, 0xf0, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x08, 0x84, 0xc4, 0xc6, 0xe7, 0x7f, 0x3f, 0x3f, 0xfe, 0xf8, 0xe0, 0xc0, 0x60, 0x18, 0x04, 0x04, 

        0x00, 0x70, 0x68, 0x38, 0xf0, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x08, 0x84, 0xc4, 0xc2, 0xe7, 0x7f, 0x3f, 0x3f, 0xfe, 0xf8, 0xe0, 0xc0, 0x60, 0x18, 0x04, 0x04, 

        0x00, 0x00, 0x18, 0x38, 0x28, 0xf0, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x81, 0xc2, 0xc2, 0xe6, 0x7f, 0x3f, 0xbf, 0x7f, 0xf8, 0xe0, 0xc0, 0x60, 0x20, 0x20, 0x10, 

        0x00, 0x18, 0x38, 0x28, 0xf0, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x01, 0x82, 0xc2, 0xc6, 0xe7, 0x7f, 0x3f, 0xbf, 0x7e, 0xf8, 0xe0, 0xc0, 0x60, 0x20, 0x20, 0x10, 
    },
    {   //7-Snake
        16, 16,
        0x10, 0x30, 0x08, 0x06, 0x05, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0x78, 0x1e, 0x07, 0x1e, 0x78, 0xe0, 0x78, 0x1e, 

        0x08, 0x06, 0x05, 0x07, 0xfe, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x1f, 0x3f, 0x1e, 0x07, 0x1e, 0x78, 0xe0, 0x78, 0x1e, 0x07, 0x1e, 0x78, 

        0x00, 0x06, 0x05, 0x07, 0xfe, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x3f, 0x7f, 0xe0, 0x78, 0x1e, 0x07, 0x1e, 0x78, 0xe0, 0x78, 0x1e, 0x07, 

        0x08, 0x06, 0x05, 0x07, 0xfe, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x1f, 0x3f, 0x1e, 0x07, 0x1e, 0x78, 0xe0, 0x78, 0x1e, 0x07, 0x1e, 0x78, 
    },
    {   //8-Train
        16, 16,
        0x00, 0x00, 0x80, 0x80, 0x81, 0xff, 0xff, 0x81, 0xe0, 0xf0, 0xe0, 0x80, 0x80, 0xfe, 0x82, 0x82, 
        0x38, 0x10, 0x1f, 0x1f, 0x7f, 0xdf, 0xbf, 0x7f, 0x1f, 0x1f, 0x7f, 0xdf, 0xbf, 0x7f, 0x1f, 0x1f,

        0x00, 0x00, 0x82, 0x80, 0x84, 0xfd, 0xfc, 0x85, 0xe0, 0xf2, 0xe0, 0x80, 0x80, 0xfe, 0x82, 0x82, 
        0x38, 0x10, 0x1f, 0x1f, 0x7f, 0xbf, 0xdf, 0x7f, 0x1f, 0x1f, 0x7f, 0xbf, 0xdf, 0x7f, 0x1f, 0x1f, 

        0x00, 0x00, 0x8a, 0x84, 0x91, 0xf4, 0xf1, 0x92, 0xe8, 0xf4, 0xe0, 0x80, 0x80, 0xfe, 0x82, 0x82, 
        0x38, 0x10, 0x1f, 0x1f, 0x7f, 0xdf, 0xbf, 0x7f, 0x1f, 0x1f, 0x7f, 0xdf, 0xbf, 0x7f, 0x1f, 0x1f, 

        0x00, 0x00, 0x82, 0x80, 0x84, 0xfd, 0xfc, 0x85, 0xe0, 0xf2, 0xe0, 0x80, 0x80, 0xfe, 0x82, 0x82, 
        0x38, 0x10, 0x1f, 0x1f, 0x7f, 0xbf, 0xdf, 0x7f, 0x1f, 0x1f, 0x7f, 0xbf, 0xdf, 0x7f, 0x1f, 0x1f, 
    },
    {   //9-Witch
        16, 16,
        0x00, 0x10, 0x90, 0xd0, 0xb8, 0xfe, 0xff, 0xfe, 0x78, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 
        0x10, 0x10, 0x12, 0x14, 0x15, 0xbd, 0xff, 0x3d, 0x10, 0x10, 0x18, 0x34, 0x52, 0x5a, 0xaa, 0xaa, 
                
        0x00, 0x10, 0x90, 0xd0, 0xb8, 0xfe, 0xff, 0xfe, 0x78, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 
        0x10, 0x10, 0x12, 0x14, 0x15, 0xbd, 0xff, 0x3d, 0x10, 0x10, 0x18, 0x34, 0x52, 0x5a, 0xaa, 0xaa, 

        0x00, 0x10, 0x90, 0xd0, 0xb8, 0xfe, 0xff, 0xfe, 0x78, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 
        0x10, 0x10, 0x18, 0x14, 0x15, 0xbd, 0xff, 0x3d, 0x10, 0x10, 0x18, 0x34, 0x52, 0x5a, 0xaa, 0xaa, 

        0x00, 0x10, 0x90, 0xd0, 0xb8, 0xfe, 0xff, 0xfe, 0x78, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 
        0x10, 0x10, 0x18, 0x14, 0x15, 0xbd, 0xff, 0x3d, 0x10, 0x10, 0x18, 0x34, 0x52, 0x5a, 0xaa, 0xaa, 
    }
};


void title_screen(){
    
    byte Selected = 0;
    
    Jack_Frame_Rate = 4;
        bool exit_loop = false;
    
    do{
        arduboy.pollButtons();
        do{
            delay(1);
        }while (!(arduboy.nextFrame()));
    
        if (arduboy.everyXFrames(45)) {
            if (Selected < 9){Selected=Selected + 1;}
            else {Selected = 0;}
        }
        
        arduboy.clear();
     

        arduboy.setTextSize(2);
        arduboy.setCursor(0,0);
        arduboy.print("JUMPING");
        arduboy.setCursor(0,21);
        arduboy.print("JACK");
        arduboy.setTextSize(1);
        arduboy.setCursor(98,30);
        arduboy.print("smBIT");
         
        arduboy.drawLine(0,40,128,40, WHITE);
        arduboy.drawLine(0,42,128,42, WHITE);
        
        
        Sprites::drawSelfMasked(105, 8, Hazzards[Selected], Jack_Frame);

        for (int i=0; i<7; i++){    
            Sprites::drawSelfMasked(2+(i*18), 47, Jack[i], Jack_Frame);
            
            
        
        }
        update_jack_frame();
    
        if (arduboy.justPressed(A_BUTTON)) {
            exit_loop = true;
        }
        arduboy.display();

                
    }while(!exit_loop);
    
    Game_Stage == GAME_MENU;
    
    
}

void setup() {
    arduboy.begin();
    arduboy.setFrameRate(frameRate);
    arduboy.setTextColor(WHITE);
    arduboy.setTextBackground(WHITE);
    
    audio.begin();
    audio.on();
                     
    title_screen();
    
    
    

}


void update_jack_frame(){
       
        
        if (arduboy.everyXFrames(Jack_Frame_Rate)) {
            Jack_Frame=Jack_Frame+1;
            if(Jack_Frame>3){ Jack_Frame=0; }
        } 
}

void update_hazzards_frame(){
       
        
        if (arduboy.everyXFrames(Hazzards_Frame_Rate)) {
            Hazzards_Frame=Hazzards_Frame+1;
            if(Hazzards_Frame>3){ Hazzards_Frame=0; }
        } 
}


void scroll_the_screen(){
    if (Scrolling == true){
        //Jack go to up
        if ((Jack_y < Last_Jack_y) && (Last_Jack_y > 5) && (Last_Jack_y <47) && (Jack_Line_Positon < 7)) {
            Jack_y = Jack_y + 3;  
            
            for (int i=0; i<9; i++){
                Lines_Position_Y[i] = Lines_Position_Y[i] + 3;
                Jack_Lines_Position_Y[i] = Jack_Lines_Position_Y[i] + 3;
            }
                
        }else if ((Jack_y > Last_Jack_y) && (Last_Jack_y > 5) && (Last_Jack_y <47) && (Jack_Line_Positon > 0)){
            Jack_y = Jack_y - 3;  
            for (int i=0; i<9; i++){
                Lines_Position_Y[i] = Lines_Position_Y[i] - 3;
                Jack_Lines_Position_Y[i] = Jack_Lines_Position_Y[i] - 3;
            }
    
        }else{
            Scrolling = false;
        }
    }   
}

void animate_gaps(){
   for (int i=0; i<Gaps_On_Screen; i++){
        arduboy.drawLine(Gaps_x[i],Lines_Position_Y[Gaps_Line_Y_Position[i]], Gaps_x[i]+Gaps_Size, Lines_Position_Y[Gaps_Line_Y_Position[i]], BLACK);
        
        
        
        
        ///////////////////////////// PRINT GAP REFLECTION THE LAST LINE (TOP) ON THE BOTTOM LINE - 0
        
        if(Gaps_Line_Y_Position[i] == 8){
            arduboy.drawLine(Gaps_x[i],Lines_Position_Y[0], Gaps_x[i]+Gaps_Size, Lines_Position_Y[0], BLACK);    
        
            for (int i=0; i<128;i=i+2){
                arduboy.drawPixel(i,Lines_Position_Y[0], WHITE);
            }
        }
        /////////////////////////
        
        if (Gaps_Direction[i] == 0) {   // <= to left
            Gaps_x[i] = Gaps_x[i] - Gaps_Speed;
            if (Gaps_x[i] < -(Gaps_Size)) { 
                Gaps_x[i] = Screen_Width; 
                if (Gaps_Line_Y_Position[i] < 8){Gaps_Line_Y_Position[i] = Gaps_Line_Y_Position[i] + 1;} else { Gaps_Line_Y_Position[i] = 1;}
               
            }
            
        }
        if (Gaps_Direction[i] == 1) {   // => to right
            Gaps_x[i] = Gaps_x[i] + Gaps_Speed;
            if (Gaps_x[i] > Screen_Width) {
                Gaps_x[i] = -(Gaps_Size); 
                if (Gaps_Line_Y_Position[i] > 1){Gaps_Line_Y_Position[i] = Gaps_Line_Y_Position[i] - 1;} else { Gaps_Line_Y_Position[i] = 8;}
                }
            
        }

    } 
    
}


void animate_hazzards(){
    for (int i=0; i<Hazzards_on_Screen; i++){
        if(Hazzards_Life_Status[i]){    //show only if Hazzard is Alive
            Sprites::drawSelfMasked(Hazzards_x[i], Jack_Lines_Position_Y[Hazzards_Line_Y_Position[i]], Hazzards[Hazzards_Type[i]], Hazzards_Frame);
            
            
            if (Hazzards_Line_Y_Position[i] == 7){
                Sprites::drawSelfMasked(Hazzards_x[i], Jack_Lines_Position_Y[0], Hazzards[Hazzards_Type[i]], Hazzards_Frame);

            for(int j=Jack_Lines_Position_Y[0]; j<Jack_Lines_Position_Y[0]+16; j = j + 1)
            for (int i=0; i<128; i=i+2)
                if( i%2 == 0){arduboy.drawPixel(i, j, BLACK);}
                else {arduboy.drawPixel(i+1, j, BLACK);}      
            }

        
       
        
        
     
            Hazzards_x[i] = Hazzards_x[i] - Hazzards_Speed;
            if (Hazzards_x[i] < -(Hazzards_Width)) { 
                Hazzards_x[i] = Screen_Width; 
                if (Hazzards_Line_Y_Position[i] < 7){Hazzards_Line_Y_Position[i] = Hazzards_Line_Y_Position[i] + 1;} else { Hazzards_Line_Y_Position[i] = 1;}
               
            }
        }
    }
}



bool Jack_Can_Jump = false;
bool check_jack_jump(){
    Jack_Can_Jump = false;
    for (int i=0; i<Gaps_On_Screen; i++){
            if (arduboy.collide(Rect(Jack_x+6, Jack_y, 4, 1 ), Rect(Gaps_x[i], Lines_Position_Y[Gaps_Line_Y_Position[i]], 20, 6 ))){
                    Jack_Can_Jump = true;
                    Last_Jumped_Gap = i;
                    Score = Score + 5 + (5 * Level);
                }
    }
    return Jack_Can_Jump;
    
}

bool Jack_Fall = false;
bool check_jack_falling(){
    Jack_Fall = false;
   if ((Jack_x >= 0) && (Jack_x<=Screen_Width-Jack_Width)){        //tylko jezeli Jack jest na ekranie

    for (int i=0; i<Gaps_On_Screen; i++){
            if (i != Last_Jumped_Gap){
                if (arduboy.collide(Rect(Jack_x+7, Jack_y, 1, 17 ), Rect(Gaps_x[i]+7, Lines_Position_Y[Gaps_Line_Y_Position[i]], 4, 1 ))){
                    Jack_Fall = true;
                }
            }
    }
    }
    return Jack_Fall;
    
}

void check_keys(){
    
        if ((Jack_Status != Jack_Jumping) && (Jack_Status != Jack_Falling) && (Scrolling != true) && (Jack_Status != Jack_Crashing) && (Jack_Status != Jack_Stunned)){
        if (arduboy.pressed(LEFT_BUTTON)) {
            Jack_Status = Jack_Running_Left;
            Jack_Last_Direction_Status = Jack_Status;


        }
        if (arduboy.justReleased(LEFT_BUTTON)) {
            Jack_Status = Jack_Standing;


        }


        if (arduboy.pressed(RIGHT_BUTTON)) {
            Jack_Status = Jack_Running_Right;
            Jack_Last_Direction_Status = Jack_Status;
        }
        
        if (arduboy.justReleased(RIGHT_BUTTON)) {
            Jack_Status = Jack_Standing;
        }
        
        if ((arduboy.justPressed(A_BUTTON)) && (Scrolling == false)) {
            if (check_jack_jump()){
                Jack_Status = Jack_Jumping; 
                Jack_Frame = 0; 
                Jack_Line_Positon = Jack_Line_Positon + 1;
                Last_Jack_y = Jack_y;
            } else {
                Last_Jack_y = Jack_y;
                Jack_Frame = 0;
                Jack_Status = Jack_Crashing;
                Scrolling == false;
                            

            }
        }
        
     
    }
    
    
    if (arduboy.justReleased(UP_BUTTON)) {

    }
    
    if (arduboy.pressed(DOWN_BUTTON)) {

    }
    if (arduboy.justReleased(DOWN_BUTTON)) {

    }


    if (arduboy.justReleased(A_BUTTON)) {

    }
    
   

    if (arduboy.justPressed(B_BUTTON)) {
        if ((Shooting_Counter > 0) && (!Shooting) && (!Classic_Version)){     //only for Extended version
            sound.tone(200,100,50,400,10, 500);
            Shooting_Counter = Shooting_Counter - 1;

            Shooting = true;
            if (Jack_Last_Direction_Status == Jack_Running_Left){Shoot_direction = 0;}
            if (Jack_Last_Direction_Status == Jack_Running_Right){Shoot_direction = 1;}

            Shoot_x = Jack_x;
            Shoot_Line_Position = Jack_Line_Positon;
            
        }
     
    } 
    
    if ((arduboy.pressed(UP_BUTTON)) &&  (arduboy.pressed(B_BUTTON))){
            Game_Stage = GAME_MENU;
            


    }
    
}


void loop() {
    
    if (Game_Stage == GAME_MENU) {
        select_game_menu();
    } else if (Game_Stage == GAME_OVER){stage_game_over(); 
    } else if (Game_Stage == STAGE_LEVEL_COMPLETED){level_completed();
    } else if (Game_Stage == GAME) {
        
    
    
      if (!(arduboy.nextFrame())){
            return;
        }
            
    if (arduboy.everyXFrames(frameRate)) {
        Time_Game = Time_Game + 1;
    }  
        
    if (arduboy.everyXFrames(60)) {
        Last_Jumped_Gap = 99;
        for(int i=0; i<20; i++){
            Hazzards_Jack_Collide[i] = false;
        }
          
    }
    
    
        arduboy.pollButtons();
        scroll_the_screen();
        
        check_keys();
        
        
        arduboy.clear();
        
        update_jack_frame();
        update_hazzards_frame();
        
        
        //Hazzards can only reach Jack when his Status are Standing or Running Left or Right
        if ((Jack_Status == Jack_Standing) || (Jack_Status == Jack_Running_Left) || (Jack_Status  == Jack_Running_Right)){
            check_jack_hazzards_collide();
        }


  
    animate_hazzards();
    jack_shooting();

    ///////////////////PRINT LINES
    for (byte i = 0; i<9; i++){
        arduboy.drawLine(0,Lines_Position_Y[i],128,Lines_Position_Y[i], WHITE);
        
    }
    
    for (byte i = 0; i<9; i++){    //Print number of line
        if ((Lines_Position_Y[i]>=0) && (Lines_Position_Y[i]< 50)){
            tinyfont.setCursor(0, Lines_Position_Y[i]+2);
            tinyfont.print(i);
        }
    }
    /////////////////////////////
    
      animate_gaps();

    if ((Jack_Status != Jack_Jumping) && (Jack_Status != Jack_Falling) && (Scrolling == false)){
  
        if(check_jack_falling()){
            Jack_Status = Jack_Falling;
            Jack_Frame = 0;
            Jack_Line_Positon = Jack_Line_Positon - 1;
            Last_Jack_y = Jack_y;
            Scrolling = false;
        }
    }
   
   
        //Jack automatycznie przebiega przez krawedz okna jak sie zblizy za blisko
        if ((Jack_x <0)&& (Jack_Last_Direction_Status == Jack_Running_Left)){
            Jack_Status = Jack_Running_Left;
        }
        
        if ((Jack_x > Screen_Width - Jack_Width) && (Jack_Last_Direction_Status == Jack_Running_Right)){
           Jack_Status = Jack_Running_Right;
        }
        

     
    
    switch (Jack_Status){
        case Jack_Standing:
            Jack_Frame_Rate = 30;
            if (arduboy.everyXFrames(Jack_Frame_Rate)) {
                sound.tone(100,20,50,20);
            }
            
            break;
        
        case Jack_Running_Left:
            Jack_x = Jack_x - Jack_Speed;
            if (Jack_x <= -Jack_Width) { 
                Jack_x = Screen_Width - Jack_Width;
                
                if (arduboy.notPressed(LEFT_BUTTON)) {
                    Jack_Status = Jack_Standing;
                }
            }
          
            Jack_Frame_Rate = 3;
            if (arduboy.everyXFrames(Jack_Frame_Rate)) {
                sound.tone(100,20,50,20);
                }
            break;
        
        case Jack_Running_Right:
            Jack_x = Jack_x + Jack_Speed;
            if (Jack_x >= Screen_Width){
            Jack_x = 0; 
            if (arduboy.notPressed(RIGHT_BUTTON)) {
                    Jack_Status = Jack_Standing;
                }
        }
            
            Jack_Frame_Rate = 3;
            if (arduboy.everyXFrames(Jack_Frame_Rate)) {
                sound.tone(100,20,50,20);
                }
            break;
        
        case Jack_Jumping:
                Jack_Frame_Rate = 5;
                if (Jack_y > Jack_Lines_Position_Y[Jack_Line_Positon]){
                    Jack_y = Jack_y - 3;
                    sound.tone(100,50,300,100);

                } else {
                    Scrolling = true;
                    Jack_Status = Jack_Standing;
                    if (Gaps_On_Screen < 8) {Gaps_On_Screen = Gaps_On_Screen + 1;}   //udane przeskoczenie - kolejna dziura sie pojawia - max 8 na planszy
                    if(Jack_Line_Positon > 7){
                        Game_Stage = STAGE_LEVEL_COMPLETED;
                        }
                } 
                        
            
    
            break;
        case Jack_Falling:
            Jack_Frame_Rate = 5;
            if (!Scrolling){
                if (Jack_y < Jack_Lines_Position_Y[Jack_Line_Positon]){
                    Jack_y = Jack_y + 3;
                    sound.tone(300,50,100,100);
                } else {
                    Scrolling = true;
                    if ((!Unlimited_Lives) && (Jack_Line_Positon == 0)) {Lives = Lives -1; }    //odejmij zycie tylko jesli upadek nastapil na sam dol
                    if (!Easy_Mode){    //if easy mody Jack is not stunned after crashing
                        Jack_Status = Jack_Stunned;
                        Jack_Stunned_Timer = Jack_Stunned_Timeout;
                    }else {
                        Jack_Status = Jack_Standing;
                    }

                    
                }
            }
            
            break;
        case Jack_Stunned:
            Jack_Frame_Rate = 1;
            if (arduboy.everyXFrames(15)) {
                Jack_Stunned_Timer = Jack_Stunned_Timer - 1;
                sound.tone(790,20,700,20);
            }
            if (Jack_Stunned_Timer == 0){
                Jack_Status = Jack_Standing;
            }
         
    
            break;
            
        case Jack_Crashing:
            if (arduboy.everyXFrames(7)) {
                sound.tone(600, 40,500,10);
                Jack_y = Jack_y - 2;
                if (Jack_Frame < 3) {Jack_Frame = Jack_Frame +1;}
                if (Jack_y  ==  Last_Jack_y-6) {
                    Jack_y = Last_Jack_y;
                    if ((!Unlimited_Lives) && (Jack_Line_Positon == 0)) {Lives = Lives -1; }
                    if (!Easy_Mode){    //if easy mody Jack is not stunned after crashing
                        Jack_Status = Jack_Stunned;
                        Jack_Stunned_Timer = Jack_Stunned_Timeout;
                    }else {
                        Jack_Status = Jack_Standing;
                    }
                    }
                }
                
            break;
        default:
            break;
    }
    

    if (Jack_x < 0)  {
        Sprites::drawSelfMasked(Screen_Width + Jack_x, Jack_y, Jack[Jack_Status], Jack_Frame);
    }
    
    if (Jack_x > Screen_Width-Jack_x) {
        Sprites::drawSelfMasked(Jack_x - Screen_Width, Jack_y, Jack[Jack_Status], Jack_Frame);
    } 
 
    Sprites::drawSelfMasked(Jack_x, Jack_y, Jack[Jack_Status], Jack_Frame);

    
  
        if ((!Unlimited_Lives) && (Lives < 1)) {Game_Stage = GAME_OVER;}
    if (!Unlimited_Lives){       // only for unlimited time game mode - show the lives on screen
        arduboy.setTextRawMode(true);
        for(int i=0; i<Lives; i++){
            arduboy.setCursor(0+(i*7),56);
            arduboy.print("\x03");
        }    
        arduboy.setTextRawMode(false);
    }
    
    
    if (Level > 0){
        if (Hazzards_Jack_Collide_Counter < 10){tinyfont.setCursor(124, 2);}
        if ((Hazzards_Jack_Collide_Counter >= 10) && (Hazzards_Jack_Collide_Counter < 100)){tinyfont.setCursor(119, 2);}
        if ((Hazzards_Jack_Collide_Counter >= 100) && (Hazzards_Jack_Collide_Counter < 1000)){tinyfont.setCursor(112, 2);}
        if (Hazzards_Jack_Collide_Counter >= 1000) {Hazzards_Jack_Collide_Counter = 999; tinyfont.setCursor(112, 2);}
        tinyfont.print(Hazzards_Jack_Collide_Counter);
    }

    //DISPLAY SCORE ON SCREEN
    tinyfont.setCursor(8, 2);
    tinyfont.print("SC:");
    tinyfont.setCursor(22, 2);
    tinyfont.print(Score);
    
    
    //DISPLAY NUMBER OF SHOOTS
    if (!Classic_Version){
        if (Shooting_Counter < 10){tinyfont.setCursor(124, 58);}
        if ((Shooting_Counter >= 10) && (Shooting_Counter < 100)){tinyfont.setCursor(119, 58);}
        tinyfont.print(Shooting_Counter);
    }

    arduboy.display();
}
}
