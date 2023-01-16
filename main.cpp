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
// static LGFX Lcd(320, 320);
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


static unsigned int SnakeBodyWidth = 8;
std::vector<Coordinate_t> SnakeBodyList = {{(int)(SnakeBodyWidth + SnakeBodyWidth / 2), (int)(SnakeBodyWidth + SnakeBodyWidth / 2)}};
static MoveDirection SnakeMoveDirection = MOVE_RIGHT;
static MoveDirection SnakeMoveDirection_old = MOVE_RIGHT;
static Coordinate_t Food_coor = {0, 0};





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






void InputUpdate()
{
    MoveDirection SnakeMoveDirection_New = SnakeMoveDirection;

    if (lgfx::gpio_in(PIN_LEFT) == 0)
    {
        // std::cout << "L\n";
        SnakeMoveDirection_New = MOVE_LEFT;
    }
    else if (lgfx::gpio_in(PIN_RIGHT) == 0)
    {
        // std::cout << "R\n";
        SnakeMoveDirection_New = MOVE_RIGHT;
    }

    else if (lgfx::gpio_in(PIN_UP) == 0)
    {
        // std::cout << "U\n";
        SnakeMoveDirection_New = MOVE_UP;
    }
    else if (lgfx::gpio_in(PIN_DOWN) == 0)
    {
        // std::cout << "D\n";
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



uint8_t Food_Size = 0;
uint32_t Food_UpdateTime_old = 0;

void FoodUpdate()
{
    /* Get random coordinante */
    // Food_coor.x = random(0 + (SnakeBodyWidth / 2), Lcd.width() - (SnakeBodyWidth / 2));
    // Food_coor.y = random(0 + (SnakeBodyWidth / 2), Lcd.height() - (SnakeBodyWidth / 2));

    // if (((Food_coor.x + (SnakeBodyWidth / 2)) % SnakeBodyWidth) == 0)
    // {
    //     if (((Food_coor.y + (SnakeBodyWidth / 2)) % SnakeBodyWidth) == 0)
            
    // }

    // do {
    //     Food_coor.x = random(0 + (SnakeBodyWidth / 2), Lcd.width() - (SnakeBodyWidth / 2));
    // } while (((Food_coor.x + (SnakeBodyWidth / 2)) % SnakeBodyWidth) != 0);
    // do {
    //     Food_coor.y = random(0 + (SnakeBodyWidth / 2), Lcd.height() - (SnakeBodyWidth / 2));
    // } while (((Food_coor.y + (SnakeBodyWidth / 2)) % SnakeBodyWidth) != 0);


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


    Food_Size = 0;

    std::cout << "(" << Food_coor.x << "," << Food_coor.y << ")\n";

}


void FoodScreenUpdate()
{
    // Screen.fillRect(Food_coor.x - (SnakeBodyWidth / 2), Food_coor.y - (SnakeBodyWidth / 2), SnakeBodyWidth, SnakeBodyWidth, FOOD_COLOR);
    // Screen.fillRoundRect(Food_coor.x - (SnakeBodyWidth / 2), Food_coor.y - (SnakeBodyWidth / 2), SnakeBodyWidth, SnakeBodyWidth, 1, FOOD_COLOR);



    Screen.fillRoundRect(Food_coor.x - (Food_Size / 2), Food_coor.y - (Food_Size / 2), Food_Size, Food_Size, 1, FOOD_COLOR);


    /* Food row up */
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
            SnakeGrow();
            FoodUpdate();
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

    // /* Draw frame */
//     Screen.drawRect(0, 0, Lcd.width(), Lcd.height(), BG_FRAME_COLOR);
//     Screen.drawRect(3, 3, Lcd.width() - 6, Lcd.height() - 6, BG_FRAME_COLOR);
//     Screen.floodFill(1, 1, BG_FRAME_COLOR);
}


void IndicatorScreenUpdate()
{
    // switch (SnakeMoveDirection)
    // {
    //     case MOVE_UP:
    //         Screen.fillRoundRect((Lcd.width() / 4), -2, (Lcd.width() / 2), 4, 1, INDICATOR_COLOR);
    //         break;
    //     case MOVE_DOWN:
    //         Screen.fillRoundRect((Lcd.width() / 4), Lcd.height() - 2, (Lcd.width() / 2), 4, 1, INDICATOR_COLOR);
    //         break;
    //     case MOVE_LEFT:
    //         Screen.fillRoundRect(-2, (Lcd.height() / 4), 4, (Lcd.height() / 2), 1, INDICATOR_COLOR);
    //         break;
    //     case MOVE_RIGHT:
    //         Screen.fillRoundRect(Lcd.width() - 2, (Lcd.height() / 4), 4, (Lcd.height() / 2), 1, INDICATOR_COLOR);
    //         break;
    //     default:
    //         break;
    // }

    Screen.fillRoundRect((Lcd.width() / 4), -2, (Lcd.width() / 2), 4, 1, 
                        SnakeMoveDirection == MOVE_UP ? INDICATOR_COLOR_ACTIVE : INDICATOR_COLOR_DISABLE);
    Screen.fillRoundRect((Lcd.width() / 4), Lcd.height() - 2, (Lcd.width() / 2), 4, 1,
                        SnakeMoveDirection == MOVE_DOWN ? INDICATOR_COLOR_ACTIVE : INDICATOR_COLOR_DISABLE);
    Screen.fillRoundRect(-2, (Lcd.height() / 4), 4, (Lcd.height() / 2), 1, 
                        SnakeMoveDirection == MOVE_LEFT ? INDICATOR_COLOR_ACTIVE : INDICATOR_COLOR_DISABLE);
    Screen.fillRoundRect(Lcd.width() - 2, (Lcd.height() / 4), 4, (Lcd.height() / 2), 1, 
                        SnakeMoveDirection == MOVE_RIGHT ? INDICATOR_COLOR_ACTIVE : INDICATOR_COLOR_DISABLE);

}


void setup()
{
    Lcd.init();
    Screen.createSprite(Lcd.width(), Lcd.height());

    // int StartCoor = (int)(SnakeBodyWidth + SnakeBodyWidth / 2);
    // SnakeBodyList.push_back({StartCoor, StartCoor});
    // for (int i = 0; i < 1; i++)
    // {
    //     SnakeGrow();
    // }


    /* Update first food */
    FoodUpdate();
}


uint32_t Time_Snake = 0;
uint32_t Time_Input = 0;


void loop()
{
    


    Time_Snake++;
    if (Time_Snake > 10)
    {
        Time_Snake = 0;

        SnakeMove();
        Check_EatFood();

        Screen.clear();
        BackGroudScreenUpdate();
        FoodScreenUpdate();
        SnakeScreenUpdate();
        // IndicatorScreenUpdate();

        Screen.pushSprite(0, 0);
    }

    



    // Time_Input++;
    // if (Time_Input > 1000)
    // {
    //     Time_Input = 0;
    //     // InputUpdate();
    //     FoodUpdate();
    // }
    InputUpdate();




    
    SDL_Delay(1);
}
