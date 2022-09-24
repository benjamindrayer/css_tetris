/*
_____/\\\\\\\\\\\\__/\\\________/\\\__/\\\\\\\\\\\_        
 ___/\\\//////////__\/\\\_______\/\\\_\/////\\\///__       
  __/\\\_____________\/\\\_______\/\\\_____\/\\\_____      
   _\/\\\____/\\\\\\\_\/\\\_______\/\\\_____\/\\\_____     
    _\/\\\___\/////\\\_\/\\\_______\/\\\_____\/\\\_____    
     _\/\\\_______\/\\\_\/\\\_______\/\\\_____\/\\\_____   
      _\/\\\_______\/\\\_\//\\\______/\\\______\/\\\_____  
       _\//\\\\\\\\\\\\/___\///\\\\\\\\\/____/\\\\\\\\\\\_ 
        __\////////////_______\/////////_____\///////////__
    
    Functions for the GUI in simulation mode
*/

#include <mock_gui.h>
#include <font_5x5.h>
#include <stdbool.h>

#define SCREEN_WIDTH                128         ///Screen width
#define SCREEN_HEIGHT                44         ///Screen height

#define SCREEN_BYTES_PER_PIXEL        4
#define SCREEN_SIZE_IN_BYTES SCREEN_WIDTH * SCREEN_HEIGHT * SCREEN_BYTES_PER_PIXEL

//Simulated version of the display
static struct
{
    uint32_t aScreenData[SCREEN_SIZE_IN_BYTES / 4Lu];   ///The screen
    uint32_t color;                                     ///Foreground color
    uint32_t bkColor;                                   ///Background color
    bool qLedState;                                     ///State of the Q-LED
} mSimulation;

static void display_char_at(const char, const int x, const int y);

extern void GUI_DrawPixel(int x, int y)
{
    mSimulation.aScreenData[(y*SCREEN_WIDTH)+x] = mSimulation.color;
}

extern void GUI_FillRect(int x0, int y0, int x1, int y1)
{
    for(uint32_t x = x0; x<=x1; x++)
    {
        for(uint32_t y = y0; y<=y1; y++)
        {
            GUI_DrawPixel(x, y);
        }
    }
}

extern void GUI_Clear()
{
    for(uint32_t i = 0Lu; i<(SCREEN_WIDTH*SCREEN_HEIGHT); i++)
    {
        mSimulation.aScreenData[i] = mSimulation.bkColor;
    }
}

extern void GUI_SetColor(const uint32_t color)
{
    mSimulation.color = color;
}

extern void GUI_SetBkColor(const uint32_t color)
{
    mSimulation.bkColor = color;
}

uint32_t* getImageBuffer()
{
    return &mSimulation.aScreenData[0];
}

/**
 * \brief Set the LED directly
 */
extern void LUI_setQLEDDirect(const bool state)
{
    mSimulation.qLedState = state;
}

extern bool UTIL_TETRIS_qetLedState()
{
    return mSimulation.qLedState;
}

extern void GUI_DispStringAt(const char * s, int x, int y)
{
    uint32_t xCurrent = x;
    while(*s != 0)
    {
        display_char_at(*s, xCurrent, y);
        printf("Print char %c\n", *s);
        xCurrent+=6;
        s++;
    }
}

///////////////////////////////////////////////////////////////////
// static functions
///////////////////////////////////////////////////////////////////

static void display_char_at(const char myCharacter, const int x, const int y)
{
    //1. Get the index
    uint32_t index = 0Lu;
    //1a. char is a digit
    if('0'<=myCharacter && myCharacter<='9')
    {
        index = (uint32_t)(myCharacter-'0');
    }
    //1b. char is upper case
    if('A'<=myCharacter && myCharacter<='Z')
    {
        index = (uint32_t)(myCharacter-'A'+10);
    }
    //1c. char is lower case
    if('a'<=myCharacter && myCharacter<='z')
    {
        index = (uint32_t)(myCharacter-'a'+10);
    }
    //1d. special characters
    if(' '== myCharacter)
    {
        index = 36;
    }
    if('_'== myCharacter)
    {
        index = 37;
    }
    if('.'== myCharacter)
    {
        index = 38;
    }
    //2. Display the char
    for(uint32_t j=0Lu;j<5;j++)
    {
        for(uint32_t i=0Lu;i<5;i++)
        {
            uint32_t color = mSimulation.bkColor;
            if(aFont5x5[index][j]&(1<<(7-i)))
            {
                color = mSimulation.color;
            }
            mSimulation.aScreenData[((y+j)*SCREEN_WIDTH)+(x+i)] = color;
        }
    }
}