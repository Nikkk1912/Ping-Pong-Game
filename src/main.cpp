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

Vector2 InvertDirection(Vector2 currentDirection)
{
    Vector2 newDirection;
    if(currentDirection.x == 1)
    {
        newDirection.x = -1;
    } else if (currentDirection.x == -1)
    {
        newDirection.x = 1;
    } else 
    {
        currentDirection.x =0;
    }

    if(currentDirection.y == 1)
    {
        newDirection.y = -1;
    } else if (currentDirection.y == -1)
    {
        newDirection.y = 1;
    } else 
    {
        currentDirection.y =0;
    }

    return newDirection;
    
}

class Ball{
    private:
        deque<Vector2> body = {Vector2{16,16}, Vector2{15,15}, Vector2{15,16}, Vector2{16,15}};
        Vector2 direction = {1, 0};

    public:
        


        void Draw() 
        {
            for(auto i = 0; i < (int)body.size();i++)
            {
               float x = body[i].x;
               float y = body[i].y;
               Rectangle segment = Rectangle{offset + x*cellSize, offset+ y*cellSize, (float)cellSize, (float)cellSize};
               DrawRectangleRounded(segment, 0.5, 6, BLACK); 
            }
        }

        void Update()
        {
            for(auto i = 0; i < (int)body.size();i++)
            {
                body[i] = Substract(body[i], direction);
            }
        }

        void Reset()
        {
            body = {Vector2{16,16}, Vector2{15,15}, Vector2{15,16}, Vector2{16,15}};

        }

        deque<Vector2> BodyCords()
        {
            return body;
        }

        Vector2 GiveDirection()
        {
            return direction;
        }

        void ChangeDirection(Vector2 newDirection)
        {
            direction = newDirection;
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
            CheckCollisionWithEdgeLeft();
        }

        void CheckCollisionWithEdgeLeft()
        {
            deque<Vector2> ballBody = ball.BodyCords();

            for(auto i = 0; i < (int)ballBody.size(); i++)
            {
                if (ballBody[i].x == -1)
                {
                    ball.ChangeDirection(InvertDirection(ball.GiveDirection()));
                }
            }
        }

};

class Panel{
    private:


    public:

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