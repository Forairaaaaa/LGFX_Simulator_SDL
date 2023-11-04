/**
 * @file performance_window.hpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-11-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "lgfx/v1/platforms/sdl/common.hpp"
#include "lgfx/v1_autodetect/LGFX_AutoDetect_sdl.hpp"
#include <LGFX_AUTODETECT.hpp>
#include <LovyanGFX.hpp>
#include <SDL2/SDL.h>
#include <SDL_timer.h>
#include <cstdint>

class PerformanceWindow
{
private:
    LGFX* _display;
    LGFX_Sprite* _canvas;
    unsigned long _last_ticks;
    unsigned long _delta_ticks;
    int _fps;

public:
    PerformanceWindow()
    {
        _display = new LGFX(180, 60);
        _display->init();
        _canvas = new LGFX_Sprite(_display);
        _canvas->createSprite(_display->width(), _display->height());

        _canvas->setFont(&fonts::efontCN_24);
    }

    ~PerformanceWindow()
    {
        delete _canvas;
        delete _display;
    }

    void update()
    {
        _delta_ticks = lgfx::micros() - _last_ticks;

        _canvas->fillScreen(TFT_BLACK);
        _canvas->setCursor(0, 0);
        _canvas->printf("% 10f ms\n", (double)_delta_ticks / 1000);
        _canvas->printf("% 10ld fps", 1000000 / _delta_ticks);
        _canvas->pushSprite(0, 0);

        _last_ticks = lgfx::micros();
    }
};

