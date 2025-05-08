
#include "resource_dir.h" // utility header for SearchAndSetResourceDir
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

Color yellow = {255, 255, 0, 255};
Color blue = {0, 0, 255, 255};
Color black = {0, 0, 0, 255};

int Cellsize = 30;
int Cellcount = 25;

double oldtime = 0;

bool eventrigger(double intervall) {
  double zeitspanne = GetTime() - oldtime;
  if (zeitspanne >= intervall) {
    oldtime = GetTime();
    return true;
  } else {
    return false;
  }
}

struct vec {
  int x;
  int y;
};

class Brick {
public:
  int x;
  int y;
  const int widthb = 10;
  const int heightb = Cellsize * 2;
  void Draw() { DrawRectangle(x, y, widthb, heightb, yellow); }
};

class Blop {
public:
  struct vec cords;
  struct vec speed;
  // warum kontte ich nicht hier drinnen das x und y definieren
  void Draw() { DrawRectangle(cords.x, cords.y, 10, 10, yellow); }
};

int main() {

  // Create the window and OpenGL context
  InitWindow(Cellsize * Cellcount, Cellsize * Cellcount, "Hello Raylib");
  SetTargetFPS(80);
  // game loop
  //
  const int speedb = 5;
  Blop kugel = Blop();
  Brick brick = Brick();
  brick.x = 13;
  brick.y = 3;
  Brick brick2 = Brick();
  brick2.x = Cellcount * Cellsize - 23;
  brick2.y = 3;

  // kugel.cords.x = Cellcount * Cellsize * 0.5;
  kugel.cords.x = 4;
  kugel.cords.y = 3;
  // kugel.speed.x = rand() % 4;
  kugel.speed.x = 4;
  kugel.speed.y = rand() % 3;
  Vector2 score = {0, 0};
  printf(" test score %f %f \n", score.x, score.y);
  while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE or
                               // presses the Close button on the window
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
    kugel.cords.x = kugel.cords.x + kugel.speed.x;
    kugel.cords.y = kugel.cords.y + kugel.speed.y;

    // tunneling
    // printf(" %d pre cords after cord %d \n", cordspre.x, kugel.cords.x);

    int yhit2 = ((kugel.cords.y - cordspre.y) / (kugel.cords.x - cordspre.x)) *
                    brick.x +
                kugel.cords.y;
    if ((yhit2 < brick2.y + brick2.heightb && yhit2 > brick2.y) &&
        cordspre.x < brick2.x && kugel.cords.x > brick2.x) {
      printf("HIT FOR SURE  %d y now %d y forsure\n", kugel.cords.y, yhit2);
      kugel.cords.y = yhit2;
      kugel.cords.x = brick2.x;
    }

    if ((kugel.cords.x < brick.x && cordspre.x > brick.x &&
         cordspre.x < brick.x + brick.widthb) &&
        ((kugel.cords.y > brick.y) &&
         kugel.cords.y < brick.y + brick.heightb) &&
        (cordspre.y < brick.y + kugel.speed.y ||
         cordspre.y > brick.y - kugel.speed.y)) {
      printf("Tunnel  ing \n");
      int yhit = ((kugel.cords.y - cordspre.y) / (kugel.cords.x - cordspre.x)) *
                 brick.x;
      printf("yhit an kugel y %d  %d %d\n", kugel.cords.y, cordspre.y, yhit);
      kugel.cords.y = kugel.cords.y + yhit;
      kugel.cords.x = brick.x;

      printf("yhit an 22  kugel y %d  %d %d\n", kugel.cords.y, kugel.cords.x,
             yhit);
    }

    if ((kugel.cords.x < brick2.x && cordspre.x > brick2.x &&
         cordspre.x < brick2.x + brick2.widthb) &&
        ((kugel.cords.y > brick2.y) &&
         kugel.cords.y < brick2.y + brick2.heightb) &&
        (cordspre.y < brick2.y + kugel.speed.y ||
         cordspre.y > brick2.y - kugel.speed.y)) {
      printf("Tunneling \n");
      // int yhit = ((kugel.cords.y - cordspre.y) / (kugel.cords.x -
      // cordspre.x)) *
      //           brick2.x;

      // kugel.cords.y = kugel.cords.y + yhit; kugel.cords.x = brick2.x;
    }

    //
    if (kugel.cords.x >= Cellcount * Cellsize || kugel.cords.x < 0) {
      printf(" Rechte Wand \n");
      kugel.speed.x = kugel.speed.x * -1;
      kugel.speed.y = kugel.speed.x * -1;
    }

    if (kugel.cords.x == brick.x &&
        kugel.cords.y <= (brick.heightb + brick.y) &&
        kugel.cords.y >= brick.y) {
      kugel.speed.x = kugel.speed.x * -1;
    }
    if (kugel.cords.x == brick2.x &&
        kugel.cords.y <= (brick2.heightb + brick2.y) &&
        kugel.cords.y >= brick2.y) {
      kugel.speed.x = kugel.speed.x * -1;
    }

    if (kugel.cords.x < 0) {
      printf(" Linke Wand \n");
    }

    if (kugel.cords.y >= Cellcount * Cellsize || kugel.cords.y < 0) {
      printf(" untere Wand \n");
      kugel.speed.y = kugel.speed.y * -1;
    }

    if (kugel.cords.y < 0) {
      printf(" obere  Wand \n");
    }

    BeginDrawing();

    kugel.Draw();
    brick.Draw();
    brick2.Draw();

    ClearBackground(black);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
