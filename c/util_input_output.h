/*        _____                           _______         
         /\    \                         /::\    \        
        /::\    \                       /::::\    \       
        \:::\    \                     /::::::\    \      
         \:::\    \                   /::::::::\    \     
          \:::\    \                 /:::/~~\:::\    \    
           \:::\    \               /:::/    \:::\    \   
           /::::\    \             /:::/    / \:::\    \  
  ____    /::::::\    \           /:::/____/   \:::\____\ 
 /\   \  /:::/\:::\    \         |:::|    |     |:::|    |
/::\   \/:::/  \:::\____\        |:::|____|     |:::|    |
\:::\  /:::/    \::/    /         \:::\    \   /:::/    / 
 \:::\/:::/    / \/____/           \:::\    \ /:::/    /  
  \::::::/    /                     \:::\    /:::/    /   
   \::::/____/                       \:::\__/:::/    /    
    \:::\    \                        \::::::::/    /     
     \:::\    \                        \::::::/    /      
      \:::\    \                        \::::/    /       
       \:::\____\                        \::/____/        
        \::/    /                         ~~              
         \/____/                                          

Structures used by several modules
*/                                                       



#ifndef INCLUDE_UTIL_INPUT_OUTPUT_H_
#define INCLUDE_UTIL_INPUT_OUTPUT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// Structure for the input of the game
typedef struct
{
    uint8_t buttonLeft;     //These buttons are both physically and color teach input
    uint8_t buttonCenter;
    uint8_t buttonRight;
    uint8_t buttonDown;     //This is only a virtual button, only available from color
}UTIL_IO_Input_t;

// Structure for the output of the game
typedef struct
{
    uint32_t musicCommand;
    int32_t level;
    uint32_t score;
    uint32_t debugState;
}UTIL_IO_Output_t;

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_UTIL_INPUT_OUTPUT_H_ */
