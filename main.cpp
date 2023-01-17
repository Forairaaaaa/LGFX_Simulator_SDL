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
#include <vector>
#include <random>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>

static LGFX Lcd(128, 128);
// static LGFX Lcd(320, 240);
// static LGFX Lcd(1080, 720);
static LGFX_Sprite Screen(&Lcd);

#define PIN_LEFT    39
#define PIN_DOWN    38
#define PIN_RIGHT   37
#define PIN_UP      36

std::random_device rd;
std::mt19937 gen(rd());
int random(int low, int high)
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}










#define SNAKE_COLOR             0xff8153U
#define FOOD_COLOR              0x88d35eU
#define BG_GRID_COLOR           0x61497eU
#define BG_FRAME_COLOR          0x61497eU
#define INDICATOR_COLOR_ACTIVE  0x88d35eU
#define INDICATOR_COLOR_DISABLE 0x61497eU

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


static unsigned int SnakeBodyWidth = 16;
std::vector<Coordinate_t> SnakeBodyList;
static MoveDirection SnakeMoveDirection = MOVE_RIGHT;
static MoveDirection SnakeMoveDirection_old = MOVE_RIGHT;
static Coordinate_t Food_coor = {0, 0};
static unsigned int SnakeScore = 0;
static uint8_t Food_Size = 0;
static uint32_t Food_UpdateTime_old = 0;

void GameOver();



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
        // Screen.drawRect(i.x - (SnakeBodyWidth / 2), i.y - (SnakeBodyWidth / 2), SnakeBodyWidth, SnakeBodyWidth, SNAKE_COLOR);
        Screen.fillRoundRect(i.x - (SnakeBodyWidth / 2), i.y - (SnakeBodyWidth / 2), SnakeBodyWidth, SnakeBodyWidth, 1, SNAKE_COLOR);
    }
}




void FoodUpdate()
{
    /* Get a random Y */
    while (1)
    {
        Food_coor.x = random(0 + (SnakeBodyWidth / 2), Lcd.width() - (SnakeBodyWidth / 2));
        /* Check if fit grid */
        if (((Food_coor.x + (SnakeBodyWidth / 2)) % SnakeBodyWidth) != 0)
            continue;
        /* Check if hit snake */
        for (auto i : SnakeBodyList) 
        {
            if (Food_coor.x == i.x)
                continue;
        }
        break;
    }
    /* Get a random Y */
    while (1)
    {
        Food_coor.y = random(0 + (SnakeBodyWidth / 2), Lcd.height() - (SnakeBodyWidth / 2));
        /* Check if fit grid */
        if (((Food_coor.y + (SnakeBodyWidth / 2)) % SnakeBodyWidth) != 0)
            continue;
        /* Check if hit snake */
        for (auto i : SnakeBodyList) 
        {
            if (Food_coor.y == i.y)
                continue;
        }
        break;
    }
    /* Resize food */
    Food_Size = 0;
    // std::cout << "(" << Food_coor.x << "," << Food_coor.y << ")\n";
}


void FoodScreenUpdate()
{
    Screen.fillRoundRect(Food_coor.x - (Food_Size / 2), Food_coor.y - (Food_Size / 2), Food_Size, Food_Size, 1, FOOD_COLOR);

    /* Food grow up */
    if (Food_UpdateTime_old == 0)
        Food_UpdateTime_old = SDL_GetTicks();
    else
    {
        if ((SDL_GetTicks() - Food_UpdateTime_old) > 10)
        {
            Food_UpdateTime_old = SDL_GetTicks();
            if (Food_Size < SnakeBodyWidth)
                Food_Size++;
        }
    }
}


void Check_EatFood()
{
    for (auto i : SnakeBodyList)
    {
        if ((Food_coor.x == i.x) && (Food_coor.y == i.y))
        {
            SnakeScore++;
            SnakeGrow();
            FoodUpdate();
        }
    }
}


void Check_EatItself()
{
    /* ***Check bug, not yet figured out */
    if (SnakeBodyList.size() < (3 * (SnakeBodyWidth / 2)))
        return;

    Coordinate_t Coor_Head = *SnakeBodyList.begin();
    for (auto i = SnakeBodyList.begin() + 1; i <= SnakeBodyList.end(); i++)
    {
        if ((Coor_Head.x == i->x) && (Coor_Head.y == i->y))
        {
            GameOver();
        }
    }
}


void BackGroudScreenUpdate()
{
    /* Draw grid */
    for (int x = -(SnakeBodyWidth / 2) - 1; x < Lcd.width(); x += SnakeBodyWidth)
    {
        for (int y = -(SnakeBodyWidth / 2); y < Lcd.height(); y += SnakeBodyWidth)
        {
            Screen.drawPixel(x, y, BG_GRID_COLOR);
        }
    }
}


void GameReset()
{
    SnakeBodyList.clear();
    SnakeBodyList.push_back({(int)(SnakeBodyWidth + SnakeBodyWidth / 2), (int)(SnakeBodyWidth + SnakeBodyWidth / 2)});

    SnakeMoveDirection = MOVE_RIGHT;
    SnakeMoveDirection_old = MOVE_RIGHT;

    SnakeScore = 0;

    FoodUpdate();
}


void GameOver()
{
    /* Draw dialog framwork */
    Screen.fillRoundRect(Lcd.width() / 12 + 3, Lcd.height() / 10 + 3, (Lcd.width() * 5) / 6, (Lcd.height() * 7) / 10, SnakeBodyWidth / 2, 0x443454U);
    Screen.fillRoundRect(Lcd.width() / 12, Lcd.height() / 10, (Lcd.width() * 5) / 6, (Lcd.height() * 7) / 10, SnakeBodyWidth / 2, BG_GRID_COLOR);
    
    char TextBuff[10];
    Screen.setFont(&fonts::Font8x8C64);
    Screen.setTextDatum(top_centre);
    Screen.setTextColor(TFT_WHITE, BG_GRID_COLOR);
    Screen.setTextSize(Lcd.width() / 42);
    snprintf(TextBuff, sizeof(TextBuff), "%d", SnakeScore);
    Screen.drawCenterString(TextBuff, Lcd.width() / 2 - 3, (Lcd.height() / 2) - (Screen.fontHeight() / 2 * 3));
    Screen.setTextSize(Lcd.width() / 128);
    snprintf(TextBuff, sizeof(TextBuff), "GAME OVER");
    Screen.drawCenterString(TextBuff, Lcd.width() / 2 - 3, (Lcd.height() / 2));

    Screen.pushSprite(0, 0);


    /* Wait reset condition */
    int PressTime_Count = 0;
    while (1)
    {
        if (!lgfx::gpio_in(PIN_RIGHT))
        {
            PressTime_Count++;
            if (PressTime_Count > 30)
            {
                GameReset();
                break;
            }
        }
        else 
            PressTime_Count = 0;
        SDL_Delay(10);
    }
}


void InputUpdate()
{
    if (lgfx::gpio_in(PIN_LEFT) == 0)
    {
        if (SnakeMoveDirection_old == MOVE_RIGHT)
            return;
        SnakeMoveDirection = MOVE_LEFT;
    }
    else if (lgfx::gpio_in(PIN_RIGHT) == 0)
    {
        if (SnakeMoveDirection_old == MOVE_LEFT)
            return;
        SnakeMoveDirection = MOVE_RIGHT;
    }
    else if (lgfx::gpio_in(PIN_UP) == 0)
    {
        if (SnakeMoveDirection_old == MOVE_DOWN)
            return;
        SnakeMoveDirection = MOVE_UP;
    }
    else if (lgfx::gpio_in(PIN_DOWN) == 0)
    {
        if (SnakeMoveDirection_old == MOVE_UP)
            return;
        SnakeMoveDirection = MOVE_DOWN;
    }
}




void setup()
{
    Lcd.init();
    Screen.createSprite(Lcd.width(), Lcd.height());


    GameReset();
}



uint32_t Time_Snake = 0;

void loop()
{
    /* Frame update */
    if (Time_Snake == 0)
        Time_Snake = SDL_GetTicks();
    else if ((SDL_GetTicks() - Time_Snake) > 5)
    {
        Time_Snake = SDL_GetTicks();

        SnakeMove();
        Check_EatFood();
        Check_EatItself();

        Screen.clear();
        BackGroudScreenUpdate();
        FoodScreenUpdate();
        SnakeScreenUpdate();

        Screen.pushSprite(0, 0);
    }

    /* Input update */
    InputUpdate();
}
