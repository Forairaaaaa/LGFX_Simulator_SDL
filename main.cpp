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

static LGFX lcd(240, 240);
// static LGFX lcd(480, 320);

static LGFX_Sprite* _canvas;
#define _canvas_update() _canvas->pushSprite(0, 0)

#define PIN_LEFT    39
#define PIN_DOWN    38
#define PIN_RIGHT   37
#define PIN_UP      36

std::random_device rd;
std::mt19937 gen(rd());
int Game_random(int low, int high)
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}



void setup()
{
    lcd.init();
    _canvas = new LGFX_Sprite(&lcd);
    _canvas->createSprite(lcd.width(), lcd.height());
}


void loop()
{
    _canvas->fillSmoothCircle(Game_random(0, _canvas->width()), Game_random(0, _canvas->height()), Game_random(1, 24), Game_random(TFT_BLACK, TFT_WHITE));
    _canvas_update();
}


