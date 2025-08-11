
#include "resource_dir.h" // utility header for SearchAndSetResourceDir
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <cmath>
#define _USE_MATH_DEFINES

double Maxwinkel = M_PI*0.4;
int ballspeed = 2;

Color yellow = {255, 255, 0, 255};
Color blue = {0, 0, 255, 255};
Color black = {0, 0, 0, 255};

typedef enum Screens {Title ,Pause,Game} Screens;

int Cellsize = 30;
int Cellcount = 25;

double oldtime = 0;



struct vec {
  double x;
  double y;
};

// Klasse fuer die Kugel
// Daten : Kordinaten,Richtung und Geschwindigkeitsvektor, Tod = ob die kugel ausserhalb des bildschirms ist
// Funktionen : Malen,Spawn


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
  double einheitsx = (( (double) ((rand()) % 9)+1)) / 10;
  double einheitsy = 1- std::pow(einheitsx,2);
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

//Klasse Bloecke
//Daten : x-Koordinate,y-Koordinate,Blockbreite,Blockhoehe
//Funktion : Draw,Hit

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
    if( brick == 1){
      kugel->speed.x = ballspeed * std::cos(winkel);
      kugel->speed.y = -ballspeed * std::sin(winkel);
    }else {
      kugel->speed.x = -ballspeed * std::cos(winkel);
      kugel->speed.y = ballspeed * std::sin(winkel);
    }


  }
};

int main() {

  // Create the window and OpenGL context
  InitWindow(Cellsize * Cellcount, Cellsize * Cellcount, "Pong");
  SetTargetFPS(90);
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
  //war fuer eine auswahl im menue davor "noetig"
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
      

      if(Titleposition.y == 0 && IsKeyDown(KEY_ENTER)){
        Mainscreen = Game;
      }
      break;
    case Game:
      {
        //Dient zur Berechnung der Sekante
    struct vec oldvecs;
    oldvecs.x = kugel.cords.x;
    oldvecs.y = kugel.cords.y;

        //Kugel bewegen
    kugel.cords.x = kugel.cords.x + kugel.speed.x*ballspeed;
    kugel.cords.y = kugel.cords.y + kugel.speed.y*ballspeed;


        //Sekante
    int steigung = (kugel.cords.y-oldvecs.y)/(kugel.cords.x-oldvecs.x);
    int yhit = steigung*(brick.x-oldvecs.x)+oldvecs.y;
    int yhit2 = steigung*(brick2.x-oldvecs.x)+oldvecs.y;






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



    if( kugel.cords.x <= brick.x && oldvecs.x >= brick.x)
    {

      if( yhit >= brick.y-10 && yhit <= (brick.y+brick.heightb+10)){
      kugel.cords.y = yhit;
      kugel.cords.x = brick.x;
      brick.hit(&kugel,1);
      }

    }

    
    if( kugel.cords.x >= brick2.x && oldvecs.x <= brick2.x)
    { 

      if( yhit2 >= brick2.y-10 && yhit2 <= (brick2.y+brick2.heightb+10)){
      kugel.cords.y = yhit2;
      kugel.cords.x = brick2.x;
      brick2.hit(&kugel, 2);

      }


    }




    if (kugel.cords.x >= Cellcount * Cellsize || kugel.cords.x < 0) {
      kugel.speed.x = kugel.speed.x * -1;
      kugel.speed.y = kugel.speed.x * -1;
      kugel.death = 1;
    }

    if (kugel.cords.x < 0) {
      kugel.death = 2;
    }

    if (kugel.cords.y >= Cellcount * Cellsize || kugel.cords.y < 0) {
      kugel.speed.y = kugel.speed.y * -1;
    }

    if (kugel.cords.y < 0) {
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
      if(Titleposition.y ==0 ){
      DrawText("Play", Cellcount*Cellcount/2,Cellcount*Cellsize/2,40,RED);
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
