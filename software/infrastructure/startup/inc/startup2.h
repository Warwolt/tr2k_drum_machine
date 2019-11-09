/*
****************************************************************************************************
* brief : This file contains the instantiation of all classes used in the application, and provides
*         an interface for accessing references to selected classes needed in main.
****************************************************************************************************
*/

#ifndef STARTUP_2_H
#define STARTUP_2_H

#include "tempo_control_view.h"
#include "tempotimingmanager.h"
#include "led_group.h"
#include "button_group.h"
#include "gpio_matrix.h"
#include "gpiopin.h"

namespace Startup2
{

void init();
LedGroup& getStepLeds();
ButtonGroup& getStepButtons();

};

#endif /* STARTUP_2_H */
