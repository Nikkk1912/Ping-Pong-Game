#include <deque>
#include <iostream>
#include <random>
#include <raylib.h>

using namespace std;

Color dark_blue = {31, 29, 52, 255};
Color light_yellow = {248, 232, 199, 255};
Color light_green = {160, 212, 171, 255};

int cellCount = 32;
int cellSize = 25;
int offset = 75;
double lastUpdateTime = 0;

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
    for (auto i = 0; i < (int)body.size(); i++) {
      float x = body[i].x;
      float y = body[i].y;
      Rectangle segment =
          Rectangle{offset + x * cellSize, offset + y * cellSize,
                    (float)cellSize, (float)cellSize};
      DrawRectangleRounded(segment, 0.5, 6, light_yellow);
    }
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
  deque<Vector2> panelBody = {
      {(float)(cellCount - 1), 13}, {(float)(cellCount - 1), 14}, {(float)(cellCount - 1), 15}, {(float)(cellCount - 1), 16}, {(float)(cellCount - 1), 17}, {(float)(cellCount - 1), 18}};

  //
  // {cellCount, 13}
  // {cellCount, 14}
  // {cellCount, 15}
  // {cellCount, 16}
  // {cellCount, 17}
  // {cellCount, 18}
  //

  int panelDirectionY = 0;

public:
  void Draw() {
    for (auto i = 0; i < (int)panelBody.size(); i++) {
      float x = panelBody[i].x;
      float y = panelBody[i].y;
      Rectangle segment =
          Rectangle{offset + x * cellSize, offset + y * cellSize,
                    (float)cellSize, (float)cellSize};
      DrawRectangleRounded(segment, 0.5, 6, light_yellow);
    }
  }

  void Update() {
    for (auto i = 0; i < (int)panelBody.size(); i++) {
      Vector2 Paneldirection;
      // cout << panelDirectionY << endl;

      if (panelBody[5].y == 5) {
        if (panelDirectionY == 1) {
          Paneldirection = {0, 0};
        } else {
          Paneldirection = {0, (float)panelDirectionY};
        }

        panelBody[i] = Substract(panelBody[i], Paneldirection);

      } else if (panelBody[5].y == cellCount) {
        if (panelDirectionY == -1) {
          Paneldirection = {0, 0};
        } else {
          Paneldirection = {0, (float)panelDirectionY};
        }

        panelBody[i] = Substract(panelBody[i], Paneldirection);

      } else {
        Vector2 Paneldirection = {0, (float)panelDirectionY};
        panelBody[i] = Substract(panelBody[i], Paneldirection);
      }
    }
  }

  void Reset() {
    panelBody = {Vector2{15, 15}, Vector2{16, 15}, Vector2{15, 16},
                 Vector2{16, 16}};
  }

  deque<Vector2> BodyCords() { return panelBody; }

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
    CheckCollisionWithPanel();
  }

  int PanelGetDirection() { return panel.GiveDirection(); }

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

int main() {

  // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(2 * offset + cellSize * cellCount,
             2 * offset + cellSize * cellCount, "Pong");
  SetTargetFPS(60);

  Game game = Game();

  while (!WindowShouldClose()) {
    BeginDrawing();

    if (EventTrigger(0.05)) {
      game.Update();
    }

    if (IsKeyPressed(KEY_W)) {
      game.PanelChangeDirection(1);
    }
    if (IsKeyPressed(KEY_S)) {
      game.PanelChangeDirection(-1);
    }

    // ClearBackground(light_green);

    ClearBackground(dark_blue);
    DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 29, (float)cellSize * cellCount + 29}, 3, light_yellow);
    DrawText("Pong", offset - 5, 20, 40, light_yellow);
    DrawText(TextFormat("%i", game.GiveLoosePoints()), offset - 5, offset + cellSize * cellCount + 29, 40, light_yellow);
    // DrawTexture(textureExample,
    //  offset + 100, offset + cellSize*cellCount+15, WHITE);
    // DrawText(" - food", offset + 130 , offset + cellSize*cellCount+15, 30, light_yellow);
    //  DrawRectangle(offset + 320, offset + cellSize*cellCount+15, cellSize,
    //  cellSize, light_yellow); DrawText(" - bonus food", offset + 350 , offset
    //  + cellSize*cellCount+15, 30, light_yellow);

    game.Draw();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}