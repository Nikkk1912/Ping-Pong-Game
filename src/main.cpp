#include <deque>
#include <iostream>
#include <random>
#include <raylib.h>

using namespace std;

Color dark_blue = {31, 29, 52, 255};
Color light_yellow = {248, 232, 199, 255};
Color light_green = {160, 212, 171, 255};

float cellSize = 25;
Rectangle screen;
Rectangle playArea;

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

class Ball {
private:
  deque<Vector2> body = {Vector2{15, 15}, Vector2{16, 15}, Vector2{15, 16},
                         Vector2{16, 16}};
  //
  //  {15,15} {16,15}
  //  {15,16} {16,16}
  //
  Vector2 direction = {1, 0};

public:
  void Draw() {

  }

  void Update() {
    for (auto i = 0; i < (int)body.size(); i++) {
      body[i] = Substract(body[i], direction);
    }
  }

  void Reset() {
    body = {Vector2{15, 15}, Vector2{16, 15}, Vector2{15, 16}, Vector2{16, 16}};
    direction = {1, 0};
  }

  deque<Vector2> BodyCords() { return body; }

  Vector2 GiveDirection() { return direction; }

  void ChangeDirection(Vector2 newDirection) { direction = newDirection; }
};

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

    if(panelDirectionY == 1 && panelBody.y >= lowBorder) {
      return;
    }
    if(panelDirectionY == -1 && panelBody.y <= topBorder) {
      return;
    }
    panelBody.y += (panelDirectionY*20);

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

class Game {
private:
  Ball ball = Ball();
  Panel panel = Panel();
  int loosePoints = 0;

public:
  void Draw() {
    ball.Draw();
    panel.Draw();
  }

  void Update() {
    ball.Update();
    panel.Update();
    CheckCollisionWithEdgeLeft();
    CheckCollisionWithEdgeRight();
    CheckCollisionWithEdgeUp();
    CheckCollisionWithEdgeDown();
    // CheckCollisionWithPanel();
  }

  // int PanelGetDirection() { return panel.GiveDirection(); }

  void PanelChangeDirection(int dir) { panel.ChangeDirection(dir); }

  deque<Vector2> GetBallCords() { return ball.BodyCords(); }

  int GiveLoosePoints() { return loosePoints; }

  void CheckCollisionWithEdgeLeft() {

    if (ball.BodyCords()[0].x == 0) {

      if (ball.GiveDirection().y == 1) {
        Vector2 newDirection = {(float)InvertX(ball.GiveDirection().x), 1};
        ball.ChangeDirection(newDirection);
      } else {
        Vector2 newDirection = {(float)InvertX(ball.GiveDirection().x), -1};
        ball.ChangeDirection(newDirection);
      }
    }
  }

  void CheckCollisionWithEdgeRight() {

    if (ball.BodyCords()[1].x == cellCount + 2) {

      ball.Reset();
      loosePoints++;
    }
  }

  void CheckCollisionWithEdgeUp() {

    if (ball.BodyCords()[0].y == 0 && ball.BodyCords()[0].x != -3 &&
        ball.BodyCords()[1].x != cellCount + 2) {
      if (ball.GiveDirection().x == 1) {
        Vector2 newDirection = {1, (float)InvertY(ball.GiveDirection().y)};
        ball.ChangeDirection(newDirection);
      } else {
        Vector2 newDirection = {-1, (float)InvertY(ball.GiveDirection().y)};
        ball.ChangeDirection(newDirection);
      }
    }
  }

  void CheckCollisionWithEdgeDown() {

    if (ball.BodyCords()[3].y == cellCount && ball.BodyCords()[0].x != -3 &&
        ball.BodyCords()[1].x != cellCount) {
      if (ball.GiveDirection().x == 1) {
        Vector2 newDirection = {1, (float)InvertY(ball.GiveDirection().y)};
        ball.ChangeDirection(newDirection);
      } else {
        Vector2 newDirection = {-1, (float)InvertY(ball.GiveDirection().y)};
        ball.ChangeDirection(newDirection);
      }
    }
  }

  void CheckCollisionWithPanel() {
    for (auto i = 0; i < (int)panel.BodyCords().size(); i++) {
      if (ball.BodyCords()[1].x == panel.BodyCords()[i].x &&
          ball.BodyCords()[1].y == panel.BodyCords()[i].y) {

        if (panel.GiveDirection() == 1) {
          Vector2 newDirection = {(float)InvertX(ball.GiveDirection().x), 1};
          ball.ChangeDirection(newDirection);
        } else if (panel.GiveDirection() == -1) {
          Vector2 newDirection = {(float)InvertX(ball.GiveDirection().x), -1};
          ball.ChangeDirection(newDirection);
        } else {
          if (ball.GiveDirection().y == 1) {
            Vector2 newDirection = {(float)InvertX(ball.GiveDirection().x), 1};
            ball.ChangeDirection(newDirection);
          } else {
            Vector2 newDirection = {(float)InvertX(ball.GiveDirection().x), -1};
            ball.ChangeDirection(newDirection);
          }

          // cout << ball.GiveDirection().x << " " <<ball.GiveDirection().y <<
          // endl; ball.ChangeDirection(InvertDirection(ball.GiveDirection()));
          // cout <<InvertDirection(ball.GiveDirection()).x << " " <<
          // InvertDirection(ball.GiveDirection()).y <<endl;
        }
      }
    }
  }
};

*/
int main() {

 
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1000, 1000, "Pong");
  SetTargetFPS(60);

  screen = {0, 0, (float)(GetScreenWidth() ), (float)(GetScreenHeight())};
  playArea = {cellSize, cellSize, screen.width - (2 * cellSize), screen.height - (2 * cellSize)};
  SetWindowSize(screen.height, screen.height);


  // Game game = Game();

  while (!WindowShouldClose()) {
    BeginDrawing();
    /*
    if (EventTrigger(0.05)) {
      game.Update();
    }

    if (IsKeyPressed(KEY_W)) {
      game.PanelChangeDirection(-1);
    }
    if (IsKeyPressed(KEY_S)) {
      game.PanelChangeDirection(1);
    }

    */

    ClearBackground(dark_blue);
    

    EndDrawing();
  }

  CloseWindow();
  return 0;
}