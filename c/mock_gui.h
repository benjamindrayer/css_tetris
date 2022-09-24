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
#include <stdint.h>
extern void GUI_Clear();
extern void GUI_SetColor(const uint32_t color);
extern void GUI_SetBkColor(const uint32_t color);
extern void GUI_DrawPixel(int x, int y);
extern void GUI_FillRect(int x0, int y0, int x1, int y1);

/**
 * @brief Display string at position (x,y)
 * 
 * @param s string
 * @param x position
 * @param y position
 */
extern void GUI_DispStringAt(const char * s, int x, int y);
