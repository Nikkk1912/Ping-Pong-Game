#include <iostream>
#include <deque>
#include <raylib.h>

using namespace std;

int cellCount = 32;
int cellSize = 25;
int offset = 75;
double lastUpdateTime = 0;

Color light_green = {160, 212, 171, 255};

bool EventTrigger(double interval)
{
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

Vector2 Substract(Vector2& first,const Vector2& second) 
{
    first.x -= second.x;
    first.y -= second.y;

    return first;
}


class Ball{
    private:
        deque<Vector2> body = {Vector2{16,16}, Vector2{15,15}, Vector2{15,16}, Vector2{16,15}};
        Vector2 direction = {1, 0};

    public:
        


        void Draw()
        {
            for(auto i = 0; i <body.size();i++)
            {
               float x = body[i].x;
               float y = body[i].y;
               Rectangle segment = Rectangle{offset + x*cellSize, offset+ y*cellSize, (float)cellSize, (float)cellSize};
               DrawRectangleRounded(segment, 0.5, 6, BLACK); 
            }
        }

        void Update()
        {
            for(auto i = 0; i < body.size();i++)
            {
                body[i] = Substract(body[i], direction);
            }
        }

        void Reset()
        {
            body = {Vector2{16,16}, Vector2{15,15}, Vector2{15,16}, Vector2{16,15}};

        }

};

class Game{
    private:
        Ball ball = Ball();

    public:


        void Draw()
        {
            ball.Draw();
        }

        void Update()
        {
            ball.Update();
        }

};

int main() {
    InitWindow(2*offset + cellSize * cellCount, 2*offset + cellSize * cellCount, "Pong");
    SetTargetFPS(60);

    Game game = Game();

    while(!WindowShouldClose())
    {
         BeginDrawing();

            if(EventTrigger(0.15))
            {
                game.Update();
            }

            ClearBackground(light_green);

            game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}