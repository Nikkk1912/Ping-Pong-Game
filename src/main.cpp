#include <iostream>
#include <deque>
#include <random>
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
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

Vector2 Substract(Vector2 &first, const Vector2 &second)
{
    // cout << first.x << " " <<first.y << endl;
    first.x -= second.x;
    first.y -= second.y;
    // cout << first.x << " " <<first.y << endl <<endl;
    return first;
}

/*Vector2 InvertDirection(Vector2 currentDirection)
{
    Vector2 newDirection;

    if (currentDirection.x == 1)
    {
        newDirection.x = -1;
    }
    else if (currentDirection.x == -1)
    {
        newDirection.x = 1;
    }
    else
    {
        currentDirection.x = 0;
    }

    if (currentDirection.y == 1)
    {
        newDirection.y = -1;
    }
    else if (currentDirection.y == -1)
    {
        newDirection.y = 1;
    }
    else
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> distribution(0, 1);
        currentDirection.y = distribution(gen) == 0 ? 1 : -1;
    }
    // cout << newDirection.x << " " << newDirection.y << endl;
    return newDirection;
}
*/

int InvertX(int currentX)
{
    int newX = 0;

    if (currentX == 1)
    {
        newX = -1;
    }
    else if (currentX == -1)
    {
        newX = 1;
    }
    else
    {
        currentX = 0;
    }
    return newX;
}

int InvertY(int currentY)
{
    int newY = 0;

    if (currentY == 1)
    {
        newY = -1;
    }
    else if (currentY == -1)
    {
        newY = 1;
    }
    else
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> distribution(0, 1);
        currentY = distribution(gen) == 0 ? 1 : -1;
    }
    // cout << newDirection.x << " " << newDirection.y << endl;
    return newY;
}

class Ball
{
private:
    deque<Vector2> body = {Vector2{15, 15}, Vector2{16, 15}, Vector2{15, 16}, Vector2{16, 16}};
    //
    //  {15,15} {16,15}
    //  {15,16} {16,16}
    //
    Vector2 direction = {1, 0};

public:
    void Draw()
    {
        for (auto i = 0; i < (int)body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5, 6, BLACK);
        }
    }

    void Update()
    {
        for (auto i = 0; i < (int)body.size(); i++)
        {
            body[i] = Substract(body[i], direction);
        }
    }

    void Reset()
    {
        body = {Vector2{15, 15}, Vector2{16, 15}, Vector2{15, 16}, Vector2{16, 16}};
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

class Game
{
private:
    Ball ball = Ball();

public:
    void Draw()
    {
        ball.Draw();
    }

    void Update()
    {
        //cout << ball.GiveDirection().x << " " << ball.GiveDirection().y << endl;
        ball.Update();
        CheckCollisionWithEdgeLeft();
        CheckCollisionWithEdgeRight();
        CheckCollisionWithEdgeUp();
        CheckCollisionWithEdgeDown();
    }

    void CheckCollisionWithEdgeLeft()
    {

        if (ball.BodyCords()[0].x == -3)
        {

            if (ball.GiveDirection().y == 1)
                    {
                        Vector2 newDirection = {(float)InvertX(ball.GiveDirection().x) , 1};
                        ball.ChangeDirection(newDirection);
                    } else 
                    {
                        Vector2 newDirection = {(float)InvertX(ball.GiveDirection().x) , -1};
                        ball.ChangeDirection(newDirection);
                    }

            // cout<<"Left";
            // cout << ball.GiveDirection().x << " " <<ball.GiveDirection().y << endl;
            //ball.ChangeDirection(InvertDirection(ball.GiveDirection()));
            // cout <<InvertDirection(ball.GiveDirection()).x << " " << InvertDirection(ball.GiveDirection()).y <<endl;
        }
    }

    void CheckCollisionWithEdgeRight()
    {
        

        if (ball.BodyCords()[1].x == cellCount + 2)
        {

            if (ball.GiveDirection().y == 1)
                    {
                        Vector2 newDirection = {(float)InvertX(ball.GiveDirection().x) , 1};
                        ball.ChangeDirection(newDirection);
                    } else 
                    {
                        Vector2 newDirection = {(float)InvertX(ball.GiveDirection().x) , -1};
                        ball.ChangeDirection(newDirection);
                    }

            // cout << ball.GiveDirection().x << " " <<ball.GiveDirection().y << endl;
            //ball.ChangeDirection(InvertDirection(ball.GiveDirection()));
            // cout <<InvertDirection(ball.GiveDirection()).x << " " << InvertDirection(ball.GiveDirection()).y <<endl;
        }
    }

    void CheckCollisionWithEdgeUp()
    {

        if (ball.BodyCords()[0].y == -3 && ball.BodyCords()[0].x != -3 && ball.BodyCords()[1].x != cellCount + 2)
        {
            if (ball.GiveDirection().x == 1)
            {
                Vector2 newDirection = {1, (float)InvertY(ball.GiveDirection().y)};
                ball.ChangeDirection(newDirection);
            }
            else
            {
                Vector2 newDirection = {-1, (float)InvertY(ball.GiveDirection().y)};
                ball.ChangeDirection(newDirection);
            }
            // ball.ChangeDirection(InvertDirection(ball.GiveDirection()));
            // cout <<InvertDirection(ball.GiveDirection()).x << " " << InvertDirection(ball.GiveDirection()).y <<endl;
        }
    }

    void CheckCollisionWithEdgeDown()
    {

        if (ball.BodyCords()[3].y == cellCount + 2 && ball.BodyCords()[0].x != -3 && ball.BodyCords()[1].x != cellCount + 2)
        {
            if (ball.GiveDirection().x == 1)
            {
                Vector2 newDirection = {1, (float)InvertY(ball.GiveDirection().y)};
                ball.ChangeDirection(newDirection);
            }
            else
            {
                Vector2 newDirection = {-1, (float)InvertY(ball.GiveDirection().y)};
                ball.ChangeDirection(newDirection);
            }

            // cout << ball.GiveDirection().x << " " <<ball.GiveDirection().y << endl;
            // ball.ChangeDirection(InvertDirection(ball.GiveDirection()));
            // cout <<InvertDirection(ball.GiveDirection()).x << " " << InvertDirection(ball.GiveDirection()).y <<endl;
        }
    }
};

class Panel
{
private:
public:
};

int main()
{
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Pong");
    SetTargetFPS(60);

    Game game = Game();

    while (!WindowShouldClose())
    {
        BeginDrawing();

        if (EventTrigger(0.15))
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