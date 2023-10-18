#include <deque>
#include <iostream>
#include <random>
#include <raylib.h>

using namespace std;

Color dark_blue = {31, 29, 52, 255};
Color light_yellow = {248, 232, 199, 255};

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
*/
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
    newY = distribution(gen) == 0 ? 1 : -1;
  }
  return newY;
}

class Ball {
  friend class Game;

private:
  Vector2 direction = {-1, 0};
  float speedChange = 10;
  float cellSize = 25;
  Rectangle playArea = {cellSize * 3, cellSize * 3, GetScreenWidth() - (6 * cellSize), GetScreenHeight() - (6 * cellSize)};
  Rectangle body = {playArea.width / 2, playArea.height / 2 + cellSize * 2, cellSize * 2, cellSize * 2};
  float screen = 1200;

public:
  void Draw() {
    DrawRectangleRounded(body, 5, 1, light_yellow);
    // cout << body.x << " " << body.y << endl;
  }

  void Update() {

    if (cellSize != GetScreenWidth() / (1200 / 25)) {
      cellSize = GetScreenWidth() / (1200 / 25);
      float scale = (float)GetScreenWidth() / screen;
      screen = (float)GetScreenWidth();
      // cout <<screen << endl;
      // cout << scale << endl;
      body.x = body.x * scale;
      body.y = body.y * scale;
      speedChange = 10.0 * scale;
      body.width = cellSize * 2;
      body.height = cellSize * 2;
      playArea = {cellSize * 3, cellSize * 3, GetScreenWidth() - (6 * cellSize), GetScreenHeight() - (6 * cellSize)};
    }

    body.x += direction.x * speedChange;
    body.y += direction.y * speedChange;
  }

  void Reset() {
    body = {playArea.width / 2, playArea.height / 2 + cellSize * 2, cellSize * 2, cellSize * 2};
    direction = {-1, 0};
  }

  Vector2 GiveDirection() { return direction; }

  void ChangeDirection(Vector2 newDirection) { direction = newDirection; }
};

class Panel {
  friend class Game;

private:
  float speedChange = 15;
  float cellSize = 25;
  float screen = 1200;
  Rectangle playAreaPanel = {cellSize * 3, cellSize * 3, GetScreenWidth() - (6 * cellSize), GetScreenHeight() - (6 * cellSize)};
  Rectangle panelBody = {playAreaPanel.width - cellSize / 2.0f, playAreaPanel.height / 2.0f - cellSize * 2.0f, (float)(cellSize * 2), (float)(cellSize * 10.0)};
  int panelDirectionY = 0;

public:
  void Draw() {
    DrawRectangleRounded(panelBody, 0.5, 1, light_yellow);
  }

  void Update() {
    panelDirectionY = 0;
    if (cellSize != GetScreenWidth() / (1200 / 25)) {
      cellSize = GetScreenWidth() / (1200 / 25);
      float scale = (float)GetScreenWidth() / screen;
      screen = (float)GetScreenWidth();
      // cout <<screen << endl;
      // cout << scale << endl;
      panelBody.x = panelBody.x * scale;
      panelBody.y = panelBody.y * scale;
      speedChange = 10.0 * scale;
      panelBody.width = (float)cellSize * 2;
      panelBody.height = (float)cellSize * 10;
      playAreaPanel = {cellSize * 3, cellSize * 3, GetScreenWidth() - (6 * cellSize), GetScreenHeight() - (6 * cellSize)};
    }
  }


  int GiveDirection() { return panelDirectionY; }

  void ChangeDirection(int newDirection) { panelDirectionY = newDirection; }
};

class Game {
private:
  Ball ball = Ball();
  Panel panel = Panel();
  float cellSize = 25;
  Rectangle playArea = {cellSize * 3, cellSize * 3, GetScreenWidth() - (6 * cellSize), GetScreenHeight() - (6 * cellSize)};
  int loosePoints = 0;
  Rectangle topBorder = {cellSize * 3, cellSize * 3, playArea.width, cellSize / 5};
  Rectangle bottomBorder = {cellSize * 3, playArea.height + cellSize * 3 - cellSize / 5, playArea.width, cellSize / 5};
  Rectangle leftBorder = {cellSize * 3, cellSize * 3, cellSize / 5, playArea.height};
  Rectangle rightBorder = {playArea.width + cellSize * 3, cellSize * 3, cellSize / 5, playArea.height};
  Rectangle biggerPanel = {playArea.width - cellSize / 2.0f, playArea.height / 2.0f - cellSize * 2.0f, (float)(cellSize * 3), (float)(cellSize * 12)};

public:
  void Draw() {
    //DrawRectangleRounded(topBorder, 1, 1, RED);
    //DrawRectangleRounded(bottomBorder, 1, 1, BLUE);
    //DrawRectangleRounded(leftBorder, 1, 1, GREEN);
    //DrawRectangleRounded(rightBorder, 1, 1, YELLOW);
    // DrawRectangleLinesEx(playArea, cellSize/5, WHITE);
    ball.Draw();
    panel.Draw();
    DrawRectangleRounded(biggerPanel, 1, 1, WHITE);
  }

  void UpdateValues(float newcellSize) {
    if (cellSize != newcellSize) {
      cellSize = newcellSize;
      playArea = {cellSize * 3, cellSize * 3, GetScreenWidth() - (6 * cellSize), GetScreenHeight() - (6 * cellSize)};

      topBorder = {cellSize * 3, cellSize * 3, playArea.width, cellSize / 5};
      bottomBorder = {cellSize * 3, playArea.height + cellSize * 3 - cellSize / 5, playArea.width, cellSize / 5};
      leftBorder = {cellSize * 3, cellSize * 3, cellSize / 5, playArea.height + cellSize * 3};
      leftBorder = {cellSize * 3, cellSize * 3, cellSize / 5, playArea.height};
      rightBorder = {playArea.width + cellSize * 3, cellSize * 3, cellSize / 5, playArea.height};
      biggerPanel = {playArea.width - cellSize / 2.0f, playArea.height / 2.0f - cellSize * 2.0f, (float)(cellSize * 3), (float)(cellSize * 12)};
    }
  }

  void Update(float &newcellSize) {
    UpdateValues(newcellSize);

    ball.Update();
    panel.Update();

    CheckCollisionWithEdgeLeft();
    CheckCollisionWithEdgeRight();
    CheckCollisionWithEdgeUp();
    CheckCollisionWithEdgeDown();
    CheckCollisionWithPanel();
  }

  int GiveLoosePoints() { return loosePoints; }

  void CheckCollisionWithEdgeLeft() {
    if (CheckCollisionRecs(ball.body, leftBorder)) {
      ball.direction.x = InvertX(ball.direction.x);
      if (ball.direction.y == 0) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> distribution(0, 1);
        ball.direction.y = distribution(gen) == 0 ? 1 : -1;
      }
    }
  }

  void CheckCollisionWithEdgeRight() {
    if (CheckCollisionRecs(ball.body, rightBorder)) {
      ball.Reset();
      loosePoints++;
    }
  }

  void CheckCollisionWithEdgeUp() {
    if (CheckCollisionRecs(ball.body, topBorder)) {
      ball.direction.y = InvertY(ball.direction.y);
    }
  }

  void CheckCollisionWithEdgeDown() {
    if (CheckCollisionRecs(ball.body, bottomBorder)) {
      ball.direction.y = InvertY(ball.direction.y);
    }
  }

  void CheckCollisionWithPanel() {
    if (CheckCollisionRecs(ball.body, biggerPanel)) {
      cout <<"collision" << endl;
      ball.direction.x = InvertX(ball.direction.x);
      if (panel.panelDirectionY == 1) {
        ball.direction.y = -1;
      } else if(panel.panelDirectionY == -1) {
        ball.direction.y = 1;
      } 
    }
  }
  void PanelChangeDirection(int direction) {
    if (direction == -1 && panel.panelBody.y > topBorder.y + (cellSize / 5) * 2) {
      panel.panelBody.y += direction * panel.speedChange;
      biggerPanel.y += direction * panel.speedChange;
      panel.panelDirectionY = 1;
    }

    else if (direction == 1 && panel.panelBody.y + (cellSize * 10) < bottomBorder.y - cellSize / 5) {
      panel.panelBody.y += direction * panel.speedChange;
      biggerPanel.y += direction * panel.speedChange;
      panel.panelDirectionY = -1;
    }
  }
};

void WindowCheckAndUpdate(Rectangle &screen, Rectangle &playArea, float &cellSize) {
  if (screen.width != (float)GetScreenWidth()) {
    screen = {0, 0, (float)(GetScreenWidth()), (float)(GetScreenHeight())};
    playArea = {cellSize * 3, cellSize * 3, screen.width - (6 * cellSize), screen.height - (6 * cellSize)};
    SetWindowSize(screen.height, screen.height);
    cellSize = GetScreenWidth() / (1200 / 25);
  } else {
    return;
  }
}

int main() {

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1200, 1200, "Pong");
  SetTargetFPS(60);

  float cellSize = 25;
  Rectangle screen = {0, 0, (float)(GetScreenWidth()), (float)(GetScreenHeight())};
  Rectangle playArea = {cellSize * 3, cellSize * 3, screen.width - (6 * cellSize), screen.height - (6 * cellSize)};
  SetWindowSize(screen.height, screen.height);

  Game game = Game();

  while (!WindowShouldClose()) {

    if (IsKeyDown(KEY_W)) {
      game.PanelChangeDirection(-1);
    }
    if (IsKeyDown(KEY_S)) {
      game.PanelChangeDirection(1);
    }

    BeginDrawing();

    WindowCheckAndUpdate(screen, playArea, cellSize);

    game.Update(cellSize);
    game.Draw();

    ClearBackground(dark_blue);
    //DrawLine(0, screen.height / 2, screen.width, screen.height / 2, light_yellow);
    DrawRectangleLinesEx(playArea, cellSize / 5, light_yellow);
    DrawText("Pong", cellSize * 3, cellSize / 2, cellSize * 2, light_yellow);
    DrawText(TextFormat("%i", game.GiveLoosePoints()), cellSize * 3, playArea.height + cellSize * 3 + cellSize / 2, cellSize * 2, light_yellow);
    DrawText("- ball missed", cellSize * 5, playArea.height + cellSize * 3 + cellSize / 2, cellSize * 2, light_yellow);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}