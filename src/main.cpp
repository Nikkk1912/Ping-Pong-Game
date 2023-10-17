#include <deque>
#include <iostream>
#include <random>
#include <raylib.h>

using namespace std;

Color dark_blue = {31, 29, 52, 255};
Color light_yellow = {248, 232, 199, 255};

float cellSize = 25;
Rectangle screen = {0, 0, (float)(GetScreenWidth()), (float)(GetScreenHeight())};
Rectangle playArea = {cellSize * 3, cellSize * 3, screen.width - (6 * cellSize), screen.height - (6 * cellSize)};

double lastUpdateTime = 0;
/*
bool EventTrigger(double interval) {
  double currentTime = GetTime();
  if (currentTime - lastUpdateTime >= interval) {
    lastUpdateTime = currentTime;
    return true;
  }
  return false;
}

Vector2 Substract(Vector2 &first, const Vector2 &second) {
  first.x -= second.x;
  first.y -= second.y;
  return first;
}

int InvertX(int currentX) {
  int newX = 0;

  if (currentX == 1) {
    newX = -1;
  } else if (currentX == -1) {
    newX = 1;
  } else {
    currentX = 0;
  }
  return newX;
}

int InvertY(int currentY) {
  int newY = 0;

  if (currentY == 1) {
    newY = -1;
  } else if (currentY == -1) {
    newY = 1;
  } else {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(0, 1);
    currentY = distribution(gen) == 0 ? 1 : -1;
  }
  return newY;
}
*/
class Ball {
private:
  Rectangle body;
  Vector2 direction;

public:
  Ball(Rectangle &playArea) {
    body = {playArea.width / 2, playArea.height / 2, cellSize * 2, cellSize * 2};
    direction = {1, 0};
  }

  void Draw() {
    DrawRectangleRounded(body, 5, 1, light_yellow);
    cout << body.x << " " << body.y << endl;
  }

  void Update() {
  }

  void Reset() {
  }

  Vector2 GiveDirection() { return direction; }

  void ChangeDirection(Vector2 newDirection) { direction = newDirection; }
};
/*
class Panel {
private:
  Rectangle panelBody = {0, 0, (float)cellCount, (float)(cellCount * 6)};
  int panelDirectionY = 0;

public:
  void Draw() {
    DrawRectangleRounded(panelBody, 0.5, 1, light_yellow);
  }

  void Update() {
    cout << panelDirectionY << endl;

    if (panelDirectionY == 1 && panelBody.y >= lowBorder) {
      return;
    }
    if (panelDirectionY == -1 && panelBody.y <= topBorder) {
      return;
    }
    panelBody.y += (panelDirectionY * 20);
  }

  void Reset() {
    panelBody = {(float)(cellCount * (cellSize + 1)), (float)(cellCount / 2 * cellSize), (float)cellCount, (float)(cellCount * 6)};
  }

  Vector2 BodyCords() {
    Vector2 cords;
    cords.x = panelBody.x;
    cords.y = panelBody.y;
    return cords;
  }

  int GiveDirection() { return panelDirectionY; }

  void ChangeDirection(int newDirection) { panelDirectionY = newDirection; }
};
*/
class Game {
private:
  Ball ball = Ball(playArea);
  Rectangle playArea;
  int loosePoints = 0;

public:
  Game(Rectangle playAre) {
    playArea = playAre;
  }
  void Draw() {
    ball.Draw();
  }

  void Update() {
    ball.Update();

    CheckCollisionWithEdgeLeft();
    CheckCollisionWithEdgeRight();
    CheckCollisionWithEdgeUp();
    CheckCollisionWithEdgeDown();
    CheckCollisionWithPanel();
  }

  int GiveLoosePoints() { return loosePoints; }

  void CheckCollisionWithEdgeLeft() {
  }

  void CheckCollisionWithEdgeRight() {
  }

  void CheckCollisionWithEdgeUp() {
  }

  void CheckCollisionWithEdgeDown() {
  }

  void CheckCollisionWithPanel() {
  }
};

Rectangle GetScreenInfo(Rectangle screen) {
  return screen;
}

Rectangle GetPlayAreanInfo(Rectangle playArea) {
  return playArea;
}

int main() {

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1200, 1200, "Pong");
  SetTargetFPS(60);

  Game game = Game(playArea);

  while (!WindowShouldClose()) {
    BeginDrawing();

    screen = {0, 0, (float)(GetScreenWidth()), (float)(GetScreenHeight())};
    playArea = {cellSize * 3, cellSize * 3, screen.width - (6 * cellSize), screen.height - (6 * cellSize)};
    SetWindowSize(screen.height, screen.height);

    game.Draw();

    ClearBackground(dark_blue);
    DrawRectangleLinesEx(playArea, 5, light_yellow);
    DrawText("Pong", cellSize * 3, cellSize / 2, cellSize * 2, light_yellow);
    DrawText(TextFormat("%i", game.GiveLoosePoints()), cellSize * 3, playArea.height + cellSize * 3 + cellSize / 2, cellSize * 2, light_yellow);
    DrawText("- ball missed", cellSize * 5, playArea.height + cellSize * 3 + cellSize / 2, cellSize * 2, light_yellow);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}