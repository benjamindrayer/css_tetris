/*
__/\\\\\\\\\\\\\\\__/\\\\\\\\\\\\\\\__/\\\\\\\\\\\\\\\____/\\\\\\\\\______/\\\\\\\\\\\_____/\\\\\\\\\\\___        
 _\///////\\\/////__\/\\\///////////__\///////\\\/////___/\\\///////\\\___\/////\\\///____/\\\/////////\\\_       
  _______\/\\\_______\/\\\___________________\/\\\_______\/\\\_____\/\\\_______\/\\\______\//\\\______\///__      
   _______\/\\\_______\/\\\\\\\\\\\___________\/\\\_______\/\\\\\\\\\\\/________\/\\\_______\////\\\_________     
    _______\/\\\_______\/\\\///////____________\/\\\_______\/\\\//////\\\________\/\\\__________\////\\\______    
     _______\/\\\_______\/\\\___________________\/\\\_______\/\\\____\//\\\_______\/\\\_____________\////\\\___   
      _______\/\\\_______\/\\\___________________\/\\\_______\/\\\_____\//\\\______\/\\\______/\\\______\//\\\__  
       _______\/\\\_______\/\\\\\\\\\\\\\\\_______\/\\\_______\/\\\______\//\\\__/\\\\\\\\\\\_\///\\\\\\\\\\\/___ 
        _______\///________\///////////////________\///________\///________\///__\///////////____\///////////_____
*/

/**
 * \file
 *
 * \brief   TODO Add a short file description
 * 
 * \author  fehrepa               (responsible)
 * \date    05.09.2022               (created)
 *
 * \copyright (c) 2022 SICK AG Germany. All rights reserved.
 *
 *
 * \details TODO Add additional details. Use markdown for longer descriptions.
 *  
 * \note    TODO Add additional notes.
 *
 * \see     TODO Add references to external documents here (if needed)
 */




#ifndef INCLUDE_UTIL_TETRIS_H_
#define INCLUDE_UTIL_TETRIS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// Structure for the input of the game
typedef struct
{
    uint8_t buttonLeft;
    uint8_t buttonCenter;
    uint8_t buttonRight;
    uint8_t buttonDebug;
}UTIL_TETRIS_Input_t;

// Structure for the output of the game
typedef struct
{
    uint32_t musicCommand;
    int32_t level;
    uint32_t score;
    bool gameOver;
    uint32_t debugState;    
}UTIL_TETRIS_Output_t;

extern void UTIL_TETRIS_showStartScreen(void);

/**
 * \brief Initializes the module
 * \param[in] seed: Seed for the random number generation
 */
extern void UTIL_TETRIS_init(const uint16_t seed,
                             const uint32_t highScore);

extern void UTIL_TETRIS_update(const UTIL_TETRIS_Input_t* const pButtons,
                                   UTIL_TETRIS_Output_t* pOutput);



#ifdef UNITTEST

/**
 * \brief   Runs the unit tests of this module.
 * \return  Number of failed tests.
 */
extern uint32_t MODULE_runTests(void); // TODO change MODULE prefix

#endif

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_UTIL_TETRIS_H_ */
