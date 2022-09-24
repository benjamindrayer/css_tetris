/*__    __  __            __                                                         
 /  |  /  |/  |          /  |                                                        
 $$ |  $$ |$$/   ______  $$ |____    _______   _______   ______    ______    ______  
 $$ |__$$ |/  | /      \ $$      \  /       | /       | /      \  /      \  /      \ 
 $$    $$ |$$ |/$$$$$$  |$$$$$$$  |/$$$$$$$/ /$$$$$$$/ /$$$$$$  |/$$$$$$  |/$$$$$$  |
 $$$$$$$$ |$$ |$$ |  $$ |$$ |  $$ |$$      \ $$ |      $$ |  $$ |$$ |  $$/ $$    $$ |
 $$ |  $$ |$$ |$$ \__$$ |$$ |  $$ | $$$$$$  |$$ \_____ $$ \__$$ |$$ |      $$$$$$$$/ 
 $$ |  $$ |$$ |$$    $$ |$$ |  $$ |/     $$/ $$       |$$    $$/ $$ |      $$       |
 $$/   $$/ $$/  $$$$$$$ |$$/   $$/ $$$$$$$/   $$$$$$$/  $$$$$$/  $$/        $$$$$$$/ 
              /  \__$$ |                                                            
              $$    $$/                                                             
               $$$$$$/ 

    Handle the highscore / leaderboard of a game               
*/

#include <util_input_output.h>

//Structure for single entry 
#define HIGHSCORE_NAME_LENGTH 3
typedef struct
{
    char aName[HIGHSCORE_NAME_LENGTH];
    uint32_t score;
}UTIL_HIGHSCORE_Entry_t;

//Structure for the leaderboard
#define HIGHSCORE_LEADERBOARD_LENGTH 3
typedef struct
{
    UTIL_HIGHSCORE_Entry_t aEntries[HIGHSCORE_LEADERBOARD_LENGTH];
}UTIL_HIGHSCORE_Leaderboard_t;

/**
 * @brief Enter a name for the leaderboard
 * 
 * @param pData the pointer to the name
 * @param pInput the input from the device 
 */
extern bool UTIL_HIGHSCORE_enterName(UTIL_HIGHSCORE_Leaderboard_t* pLeaderBoard,
                                     const UTIL_IO_Input_t* pInput);
/**
 * @brief show the leaderboard 
 * 
 * @param pData the leaderboard, raw
 */
extern void UTIL_HIGHSCORE_showLeaderBoard(const void* const pData);
extern bool UTIL_HIGHSCORE_isNewHighscore(const void* const pData,
                                          const uint32_t score);
