
#include "resource_dir.h" // utility header for SearchAndSetResourceDir
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <cmath>
#define _USE_MATH_DEFINES

double Maxwinkel = M_PI*0.5;
int ballspeed = 2;

Color yellow = {255, 255, 0, 255};
Color blue = {0, 0, 255, 255};
Color black = {0, 0, 0, 255};

typedef enum Screens {Title ,Pause,Game} Screens;

int Cellsize = 30;
int Cellcount = 25;

double oldtime = 0;


/*bool eventrigger(double intervall) {
  double zeitspanne = GetTime() - oldtime;
  if (zeitspanne >= intervall) {
    oldtime = GetTime();
    return true;
  } else {
    return false;
  }
} */

struct vec {
  double x;
  double y;
};


class Blop {
public:
  struct vec cords;
  struct vec speed;
  int death = 0;

  void spawn() {
  cords.x = (rand() % 188)+200;
  cords.y = 300+ (rand() % 188);
  int direction = rand() % 2;
  if(direction == 0){
    direction = -1;
  }else{
    direction = 1;
  } 
  //speed.x=  ballspeed*(((rand()%10)+10)/10) *direction;
  //speed.y=  ballspeed*(((rand()%10)+10)/10) ;
  double einheitsx = (( (double) ((rand()) % 9)+1)) / 10;
  double einheitsy = 1- std::pow(einheitsx,2);
  std::cout << " Einheitstest " << einheitsx << " und " << einheitsy << std::endl;
  //speed.x=  ballspeed*direction*einheitsx;
  //speed.y=  ballspeed*einheitsy;
  speed.x=  ballspeed*direction*1;
  speed.y=  ballspeed*einheitsy*0;
  };
  int Draw() { 
    int olddeath = death;
    if(death != 0){
      spawn();
      death = 0;
    }
    DrawRectangle(cords.x, cords.y, 10, 10, yellow);  
    return olddeath;
  }
};

class Brick
{
public:
  int x;
  int y;
  const int widthb = 10;
  const int heightb = Cellsize * 2;
  void Draw() { DrawRectangle(x, y, widthb, heightb, yellow); }
  void hit(Blop *kugel,int brick)
  {
    double relativy = kugel->cords.y - y - (heightb / 2);
    double normrelativy = (relativy/(heightb/2));
    double winkel = Maxwinkel * normrelativy;
 //   std::cout << "rel " << relativy << " norm " <<normrelativy<< " winkel "<< winkel  <<std::endl;
//    std::cout<< "old " << kugel->speed.x << " und " << kugel->speed.y << std::endl;
    if( brick == 1){
      kugel->speed.x = ballspeed * std::cos(winkel);
      kugel->speed.y = -ballspeed * std::sin(winkel);
     // std::cout << "new 1 " << kugel->speed.x << "test "<< std::cos(winkel) << " und " << kugel->speed.y << " " << winkel<< std::endl;
    }else {
      kugel->speed.x = -ballspeed * std::cos(winkel);
      kugel->speed.y = ballspeed * std::sin(winkel);
      //std::cout << "new 2 " << kugel->speed.x << "test "<< std::cos(winkel) << " und " << kugel->speed.y << " " << winkel<< std::endl;
    }


  }
};

int main() {

  // Create the window and OpenGL context
  InitWindow(Cellsize * Cellcount, Cellsize * Cellcount, "Pong");
  SetTargetFPS(80);
  srand(time(0));
  
  //Werte Initialisierung
  std::string score1 = "0";
  std::string score2 = "0";
  const int speedb = 8;

  Brick brick = Brick();
  brick.x = 13;
  brick.y = 3;

  Brick brick2 = Brick();
  brick2.x = Cellcount * Cellsize - 23;
  brick2.y = 3;

  Blop kugel = Blop();
  kugel.spawn();

  Screens Mainscreen = Title;
  vec Titleposition;
  Titleposition.x = 0;
  Titleposition.y = 0;


  int framecounter = 0;
  //game loop
  while (!WindowShouldClose()) 
                              
  {
    switch (Mainscreen)
    {
    case Title:
      if(IsKeyDown(KEY_W)){
        Titleposition.y = 1;

      } else if(IsKeyDown(KEY_S)){
        Titleposition.y = 0;
      }
      

      if(Titleposition.y == 1 && IsKeyDown(KEY_ENTER)){
        Mainscreen = Game;
      }
      break;
    case Game:
      {







    const vec cordspre = kugel.cords;
    // Brick move
    if (IsKeyDown(KEY_S) && (brick.y + brick.heightb) <= Cellcount * Cellsize) {
      brick.y = brick.y + speedb;
    }
    if (IsKeyDown(KEY_W) && (brick.y >= 0)) {
      brick.y = brick.y - speedb;
    }
    if (IsKeyDown(KEY_DOWN) &&
        (brick2.y + brick2.heightb) <= Cellcount * Cellsize) {
      brick2.y = brick2.y + speedb;
    }
    if (IsKeyDown(KEY_UP) && (brick2.y >= 0)) {
      brick2.y = brick2.y - speedb;
    }
    // Kugel move
    struct vec oldvecs;
    oldvecs.x = kugel.cords.x;
    oldvecs.y = kugel.cords.y;

    //kugel.cords.x = kugel.cords.x + kugel.speed.x*kugel.schnell;
    //kugel.cords.y = kugel.cords.y + kugel.speed.y*kugel.schnell;

    kugel.cords.x = kugel.cords.x + kugel.speed.x*ballspeed;
    kugel.cords.y = kugel.cords.y + kugel.speed.y*ballspeed;


    int yhit = ((kugel.cords.y-oldvecs.y)/(oldvecs.x-kugel.cords.x) * brick.x) +kugel.cords.y;
    int yhit2 = ((kugel.cords.y-oldvecs.y)/(-oldvecs.x+kugel.cords.x) * brick2.x) +kugel.cords.y;

    if( kugel.cords.x <= brick.x && oldvecs.x >= brick.x)
    {

      if( yhit >= brick.y && yhit <= (brick.y+brick.heightb)){
    //std::cout << yhit<< " und "<< brick.y << " u " << brick.y +brick.heightb << std::endl;
      std::cout << " tunnel 1" << std::endl;
      kugel.cords.y = yhit;
      kugel.cords.x = brick.x;
      brick.hit(&kugel,1);
      //kugel.speed.x = -kugel.speed.x ;
      //kugel.speed.y = -kugel.speed.y ;
      }else {
        std::cout << "1  tunnel but no hit " << std::endl;
      }

    }

    
    if( kugel.cords.x >= brick2.x && oldvecs.x <= brick2.x)
    { 
      if( yhit >= brick2.y && yhit <= (brick2.y+brick2.heightb)){
        std::cout << " tunnel " << std::endl;
   // std::cout << yhit<< " und "<< brick.y << " u " << brick2.y +brick2.heightb << std::endl;
      kugel.cords.y = yhit;
      kugel.cords.x = brick2.x;
        brick2.hit(&kugel,2);
      //kugel.speed.x = -kugel.speed.x ;
      //kugel.speed.y = kugel.speed.y ;
      }else{

        std::cout << "  tunnel but no hit " << std::endl;
      }

    }




    if (kugel.cords.x >= Cellcount * Cellsize || kugel.cords.x < 0) {
      printf(" Rechte Wand \n");
      kugel.speed.x = kugel.speed.x * -1;
      kugel.speed.y = kugel.speed.x * -1;
      kugel.death = 1;
    }

    if (kugel.cords.x < 0) {
      printf(" Linke Wand \n");
      kugel.death = 2;
    }

    if (kugel.cords.y >= Cellcount * Cellsize || kugel.cords.y < 0) {
      printf(" untere Wand \n");
      kugel.speed.y = kugel.speed.y * -1;
    }

    if (kugel.cords.y < 0) {
      printf(" obere  Wand \n");
    }
      framecounter = framecounter +1;
      if(IsKeyDown(KEY_P) && framecounter > 50){
        Mainscreen = Pause;
        framecounter = 0;

      }

      }
      break;
    case Pause:
         
      framecounter = framecounter +1;
      if(IsKeyDown(KEY_P) && framecounter > 10){
        Mainscreen = Game;
        framecounter = 0;
      }

    break;
    }

    BeginDrawing();

    switch (Mainscreen)
    {
    case Title:
    {
      DrawRectangle(0,0,Cellsize*Cellcount,Cellcount*Cellsize,BLACK);
      if(Titleposition.y ==1 ){
      DrawText("Play", Cellcount*Cellcount/2,Cellcount*Cellsize/2,40,RED);
      DrawText("LOAD", Cellcount*Cellcount/2,Cellcount*Cellsize/2 +40,40,ORANGE);
      }else {
      DrawText("Play", Cellcount*Cellcount/2,Cellcount*Cellsize/2,40,ORANGE);
      DrawText("LOAD", Cellcount*Cellcount/2,(Cellcount*Cellsize/2) +40,40,RED);

      }
      }
      break;
    case Pause:
    {
      DrawText("Paused",(Cellcount*Cellsize/2)-80,(Cellcount*Cellsize/2)-60,50,GRAY);



    }
      break;
    case Game:
    {

    ClearBackground(black);
    DrawText(score1.c_str(), 200, 200, 40, WHITE);
    DrawText(score2.c_str(), 550, 200, 40, WHITE);
    int res = kugel.Draw();
    if(res == 1){
      score1 = std::to_string( res + std::stoi(score1));
    } else if (res == 2){
      score2 = std::to_string(res-1 + std::stoi(score2));
    }

    brick.Draw();
    brick2.Draw();
    ClearBackground(black);
    }    
    break;
    
    }
    EndDrawing();

  }

  CloseWindow();
  return 0;
}
