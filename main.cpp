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
static LGFX_Sprite Screen(&Lcd);

#define PIN_LEFT    39
#define PIN_DOWN    38
#define PIN_RIGHT   37
#define PIN_UP      36






struct Coordinate_t {
    int x;
    int y;
};



static unsigned int SnakeBodyWidth = 6;
std::vector<Coordinate_t> SnakeBodyList = {{10, 10}, {16, 10}, {22, 10}, {28, 10} ,{34, 10}};






void SnakeMove(uint8_t direction)
{
    Coordinate_t Coor_New = *SnakeBodyList.begin();

    switch (direction)
    {
        case 0:
            Coor_New.y -= SnakeBodyWidth;
            break;
        case 1:
            Coor_New.y += SnakeBodyWidth;
            break;
        case 2:
            Coor_New.x -= SnakeBodyWidth;
            break;
        case 3:
            Coor_New.x += SnakeBodyWidth;
            break;
        default:
            break;
    }

    SnakeBodyList.insert(SnakeBodyList.begin(), Coor_New);
    SnakeBodyList.pop_back();
}


void SnakeGrow()
{
    SnakeBodyList.push_back(SnakeBodyList.back());
}



void SnakeScreenUpdate()
{
    for (auto i : SnakeBodyList)
    {
        // Screen.fillCircle(i.x, i.y, SnakeBodyWidth / 2, TFT_ORANGE);
        Screen.fillRect(i.x - (SnakeBodyWidth / 2), i.y - (SnakeBodyWidth / 2), SnakeBodyWidth, SnakeBodyWidth, TFT_ORANGE);
    }
}




void InputUpdate()
{
    if (lgfx::gpio_in(PIN_LEFT) == 0)
    {
        std::cout << "L\n";
        SnakeMove(2);
    }
        

    else if (lgfx::gpio_in(PIN_RIGHT) == 0)
    {
        std::cout << "R\n";
        SnakeMove(3);
    }
        

    else if (lgfx::gpio_in(PIN_UP) == 0)
    {
        std::cout << "U\n";
        SnakeMove(0);
    }
        

    else if (lgfx::gpio_in(PIN_DOWN) == 0)
    {
        std::cout << "D\n";
        SnakeMove(1);
    }
        

}






void setup()
{
    Lcd.init();
    Screen.createSprite(Lcd.width(), Lcd.height());




}


uint8_t ddd_list[] = {1, 3, 0, 2};
uint8_t ddd = 0;

void loop()
{

    // SnakeMove(1);
    // Screen.clear();
    // SnakeScreenUpdate();
    // Screen.pushSprite(0, 0);


    // SDL_Delay(1000);







    for (int i = 0; i < 16; i++)
    {
        
        // SnakeMove(ddd_list[ddd]);
        SnakeScreenUpdate();


        InputUpdate();


        Screen.pushSprite(0, 0);
        // SDL_Delay(100);
        SDL_Delay(50);
        Screen.clear();
    }


    ddd++;
    if (ddd > 3)
        ddd = 0;
}
