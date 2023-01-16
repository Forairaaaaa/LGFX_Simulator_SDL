/**
 * @file main.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-01-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <iostream>
#include <string>
#include <vector>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>

static LGFX Lcd(128, 128);
// static LGFX Lcd(320, 320);
static LGFX_Sprite Screen(&Lcd);

#define PIN_LEFT    39
#define PIN_DOWN    38
#define PIN_RIGHT   37
#define PIN_UP      36






struct Coordinate_t {
    int x;
    int y;
};

enum MoveDirection {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT
};


static unsigned int SnakeBodyWidth = 4;
std::vector<Coordinate_t> SnakeBodyList;
static MoveDirection SnakeMoveDirection = MOVE_RIGHT;
static MoveDirection SnakeMoveDirection_old = MOVE_RIGHT;






void SnakeMove()
{
    Coordinate_t Coor_New = *SnakeBodyList.begin();

    // SnakeMoveDirection_old = SnakeMoveDirection;
    if (((Coor_New.x + (SnakeBodyWidth / 2)) % SnakeBodyWidth) == 0)
    {
        if (((Coor_New.y + (SnakeBodyWidth / 2)) % SnakeBodyWidth) == 0)
            SnakeMoveDirection_old = SnakeMoveDirection;
    }


    switch (SnakeMoveDirection_old)
    {
        case MOVE_UP:
            Coor_New.y -= 1;
            break;
        case MOVE_DOWN:
            Coor_New.y += 1;
            break;
        case MOVE_LEFT:
            Coor_New.x -= 1;
            break;
        case MOVE_RIGHT:
            Coor_New.x += 1;
            break;
        default:
            break;
    }


    /* If hit wall */
    if (Coor_New.x < 0)
        Coor_New.x = Lcd.width();
    if (Coor_New.y < 0)
        Coor_New.y = Lcd.height();
    if (Coor_New.x > Lcd.width())
        Coor_New.x = 0;
    if (Coor_New.y > Lcd.height())
        Coor_New.y = 0;



    SnakeBodyList.insert(SnakeBodyList.begin(), Coor_New);
    SnakeBodyList.pop_back();
}


void SnakeGrow()
{
    for (int i = 0; i < SnakeBodyWidth; i++)
    {
        SnakeBodyList.push_back(SnakeBodyList.back());
    }
}


void SnakeScreenUpdate()
{
    for (auto i : SnakeBodyList)
    {
        // Screen.fillCircle(i.x, i.y, SnakeBodyWidth / 2, TFT_ORANGE);
        // Screen.fillRect(i.x - (SnakeBodyWidth / 2), i.y - (SnakeBodyWidth / 2), SnakeBodyWidth, SnakeBodyWidth, TFT_ORANGE);
        Screen.drawRect(i.x - (SnakeBodyWidth / 2), i.y - (SnakeBodyWidth / 2), SnakeBodyWidth, SnakeBodyWidth, TFT_ORANGE);
    }
}






void InputUpdate()
{
    MoveDirection SnakeMoveDirection_New = SnakeMoveDirection;

    if (lgfx::gpio_in(PIN_LEFT) == 0)
    {
        std::cout << "L\n";
        SnakeMoveDirection_New = MOVE_LEFT;
    }
    else if (lgfx::gpio_in(PIN_RIGHT) == 0)
    {
        std::cout << "R\n";
        SnakeMoveDirection_New = MOVE_RIGHT;
    }

    else if (lgfx::gpio_in(PIN_UP) == 0)
    {
        std::cout << "U\n";
        SnakeMoveDirection_New = MOVE_UP;
    }
    else if (lgfx::gpio_in(PIN_DOWN) == 0)
    {
        std::cout << "D\n";
        SnakeMoveDirection_New = MOVE_DOWN;
    }


    /* Direction lock */
    switch (SnakeMoveDirection_New)
    {
        case MOVE_UP:
            if (SnakeMoveDirection_old == MOVE_DOWN)
                return;
            break;
        case MOVE_DOWN:
            if (SnakeMoveDirection_old == MOVE_UP)
                return;
            break;
        case MOVE_LEFT:
            if (SnakeMoveDirection_old == MOVE_RIGHT)
                return;
            break;
        case MOVE_RIGHT:
            if (SnakeMoveDirection_old == MOVE_LEFT)
                return;
            break;
        default:
            break;
    }
    SnakeMoveDirection = SnakeMoveDirection_New;

}








void setup()
{
    Lcd.init();
    Screen.createSprite(Lcd.width(), Lcd.height());


    SnakeBodyList.push_back({10, 10});
    for (int i = 0; i < 10; i++)
    {
        SnakeGrow();
    }

}


uint32_t Time_Snake = 0;
uint32_t Time_Input = 0;


void loop()
{
    Time_Snake++;
    if (Time_Snake > 8)
    {
        Time_Snake = 0;
        SnakeMove();
        Screen.clear();
        SnakeScreenUpdate();
        Screen.pushSprite(0, 0);
    }

    // Time_Input++;
    // if (Time_Input > 8)
    // {
    //     Time_Input = 0;
    //     InputUpdate();
    // }
    InputUpdate();


    SDL_Delay(1);
}
