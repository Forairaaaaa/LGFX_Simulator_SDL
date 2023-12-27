/**
 * @file rachel.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-11-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "simu_project.h"
#include <mooncake.h>
#include "hal/hal.h"
#ifndef ESP_PLATFORM
#include "hal/hal_simulator/hal_simulator.hpp"
#else
#include "hal/hal_rachel/hal_rachel.h"
#endif
#include "apps/apps.h"


using namespace MOONCAKE;
static Mooncake* _mooncake = nullptr;


void SIMU_PROJECT::Setup()
{
    spdlog::info("simu project setup");

    // HAL injection 
    #ifndef ESP_PLATFORM
    HAL::Inject(new HAL_Simulator(240, 240));
    #else
    HAL::Inject(new HAL_Rachel);
    #endif

    // Mooncake framework 
    _mooncake = new Mooncake;
    _mooncake->init();

    // Install launcher 
    auto launcher = simu_project_launcher_install_callback();
    if (launcher != nullptr)
        _mooncake->installApp(launcher);

    // Install apps 
    simu_project_app_install_callback(_mooncake);

    // Create launcher 
    if (launcher != nullptr)
        _mooncake->createApp(launcher);
}


#include <random>
std::random_device rd;
std::mt19937 gen(rd());
int Game_random(int low, int high) 
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}


void SIMU_PROJECT::Loop()
{
    _mooncake->update();

    HAL::GetCanvas()->fillSmoothCircle(
        Game_random(0, HAL::GetCanvas()->width()), Game_random(0, HAL::GetCanvas()->height()),
        Game_random(1, 24), Game_random(TFT_BLACK, TFT_WHITE));
    HAL::CanvasUpdate();
}


void SIMU_PROJECT::Destroy()
{
    // Free 
    delete _mooncake;
    HAL::Destroy();
    spdlog::warn("simu project destroy");
}

