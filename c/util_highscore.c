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
#include<stdio.h>
#include<stdint.h>
#include <stdbool.h>
#include <util_highscore.h>

#ifdef SIMULATION
#include <mock_gui.h>
#else
#include <fw_lui_tft.h>
#include <GUI.h>
#endif

static struct
{
    uint32_t positionInName;
    uint32_t positionInLeaderBoard;
} m;

extern void UTIL_HIGHSCORE_showLeaderBoard(const void* const pData)
{
    GUI_SetColor(0x00FFFFFF);
    GUI_SetBkColor(0x00000000);    
    GUI_Clear();
    UTIL_HIGHSCORE_Leaderboard_t* pLeaderBoard = (UTIL_HIGHSCORE_Leaderboard_t*) pData;
    GUI_DispStringAt("High Score", 20, 2);
    char aText[20];
    uint32_t aColors[HIGHSCORE_LEADERBOARD_LENGTH] = {0x00ffdf, 0xc1cdbe, 0x409bcd};
    for(uint32_t i = 0Lu; i<HIGHSCORE_LEADERBOARD_LENGTH; i++)
    {
        for(uint32_t i=0Lu; i<20;i++)
        {
            aText[i] = ' ';
        }
        GUI_SetColor(aColors[i]);
        //Place
        aText[0] = i + '1';
        aText[1] = '.';
        //Name
        for(uint32_t j = 0; j<HIGHSCORE_NAME_LENGTH; j++)
        {
            aText[j+2] = pLeaderBoard->aEntries[i].aName[j];             
        }        
        //Score
        uint32_t score = pLeaderBoard->aEntries[i].score;
        for(uint32_t j = 8; j>0; j--)
        {
            uint32_t digit = score % 10;
            score = score / 10;
            aText[HIGHSCORE_NAME_LENGTH+4+j] = digit + '0';             
            if(score == 0)
            {
                break;
            }
        }        
        aText[HIGHSCORE_NAME_LENGTH+4+9] = 0;             
        GUI_DispStringAt(&(aText[0]), 10, 12+(8*i));
    }
}

extern bool UTIL_HIGHSCORE_isNewHighscore(const void* const pData,
                                          const uint32_t score)
{
    UTIL_HIGHSCORE_Leaderboard_t* pLeaderBoard = (UTIL_HIGHSCORE_Leaderboard_t*) pData;
    uint32_t indexInLeaderBoard = HIGHSCORE_LEADERBOARD_LENGTH;
    bool retVal = false;
    for(int32_t i = 0; i<HIGHSCORE_LEADERBOARD_LENGTH;i++)
    {
        if(score>pLeaderBoard->aEntries[i].score)
        {
            indexInLeaderBoard = i;
            retVal = true;
            break; 
        }
    }    

    if(indexInLeaderBoard<HIGHSCORE_LEADERBOARD_LENGTH)
    {
        m.positionInLeaderBoard = indexInLeaderBoard;
        m.positionInName = 0;
        for(uint32_t i=HIGHSCORE_LEADERBOARD_LENGTH-1;i>indexInLeaderBoard;i--)
        {
            pLeaderBoard->aEntries[i] = pLeaderBoard->aEntries[i-1];
        }
        for(uint32_t i=0;i<HIGHSCORE_NAME_LENGTH;i++)
        {
            pLeaderBoard->aEntries[indexInLeaderBoard].aName[i]='_';
            pLeaderBoard->aEntries[indexInLeaderBoard].score = score;
        }
    }

    return retVal;
}


static char getNextCharacter(const char c)
{
    if(c == '_')
    {
        return 'A';
    }
    if(c >= 'A' && c < 'Z')
    {
        return c + 1;
    }
    if(c == 'Z')
    {
        return '0';
    }
    if(c >= '0' && c < '9')
    {
        return c + 1;
    }
    if(c == '9')
    {
        return '_';
    }
}

static char getPreviousCharacter(const char c)
{
    if(c == '_')
    {
        return '9';
    }
    if(c > 'A' && c <= 'Z')
    {
        return c - 1;
    }
    if(c == 'A')
    {
        return '_';
    }
    if(c > '0' && c <= '9')
    {
        return c - 1;
    }
    if(c == '0')
    {
        return 'Z';
    }
}

extern bool UTIL_HIGHSCORE_enterName(UTIL_HIGHSCORE_Leaderboard_t* pLeaderBoard,
                                     const UTIL_IO_Input_t* pInput)
{

    char* pString = (char*) &(pLeaderBoard->aEntries[m.positionInLeaderBoard].aName[0]);

    //Left key -> move cursor left
    if(pInput->buttonLeft == 1)
    {
        if(m.positionInName > 0)
        {
            m.positionInName--;
        }
    }
    //Right key -> move cursor right
    if(pInput->buttonRight == 1)
    {
        if(m.positionInName < HIGHSCORE_NAME_LENGTH)
        {
            m.positionInName++;
        }
    }
    //Up key -> increment position in alphabet
    //or exit
    if(pInput->buttonCenter == 1)
    {
        //Exit
        if(m.positionInName == HIGHSCORE_NAME_LENGTH)
        {
            m.positionInName = 0;
            return false;
        }
        else
        {
            pString[m.positionInName] = getNextCharacter(pString[m.positionInName]);
        }
    }
    //Down key -> decrement position in alphabet
    //or exit
    if(pInput->buttonDown == 1)
    {
        //Exit
        if(m.positionInName == HIGHSCORE_NAME_LENGTH)
        {
            m.positionInName = 0;
            return false;
        }
        else
        {
            pString[m.positionInName] = getPreviousCharacter(pString[m.positionInName]);
        }
    }
    //Display all
    GUI_SetColor(0x00FFFFFF);
    GUI_SetBkColor(0x00000000);    
    GUI_DispStringAt("NEW HIGHSCORE", 20, 2);
    GUI_DispStringAt("Enter Your Name", 20, 12);
    char aText[10];
    aText[0] = pString[0];
    aText[1] = pString[1];
    aText[2] = pString[2];
    aText[3] = ' ';
    aText[4] = 'E';
    aText[5] = 'N';
    aText[6] = 'D';
    aText[7] = 0;   
    GUI_DispStringAt(aText, 20, 22);
    GUI_SetColor(0x00000FF0);    
    if(m.positionInName == 0)
    {
        aText[0] = pString[0];
        aText[1] = 0;
        GUI_DispStringAt(aText, 20, 22);
    }
    if(m.positionInName == 1)
    {
        aText[0] = pString[1];
        aText[1] = 0;
        GUI_DispStringAt(aText, 20+6, 22);
    }
    if(m.positionInName == 2)
    {
        aText[0] = pString[2];
        aText[1] = 0;
        GUI_DispStringAt(aText, 20+12, 22);
    }
    if(m.positionInName == HIGHSCORE_NAME_LENGTH)
    {
        aText[0] = 'E';
        aText[1] = 'N';
        aText[2] = 'D';
        aText[3] = 0;   
        GUI_DispStringAt(aText, 20+(HIGHSCORE_NAME_LENGTH+1)*6, 22);
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
//Static functions
///////////////////////////////////////////////////////////////////////////////////////////