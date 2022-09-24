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
#include<stdio.h>
#include<stdint.h>
#include <stdbool.h>

#include "util_tetris.h"

#ifdef SIMULATION
#include "mock_gui.h"
#else
#include <fw_lui_tft.h>
#include <GUI.h>
#endif

//#define SIMULATION                               ///Flag to use the simulation display

#define SCREEN_WIDTH                128         ///Screen width
#define SCREEN_HEIGHT                44         ///Screen height

#define N_COLOR_MAPS                 12         ///Number of different color maps

#define BUTTON_STATE_OFF 0
#define BUTTON_STATE_ON  1

//Colormap to color the tetris stones
const uint32_t aColorMap[N_COLOR_MAPS][8] = {{0x242424, 0xf11c00, 0xe33800, 0xd45500, 0xc67100, 0xb88e00, 0xaaaa00, 0x9bc700},
                                             {0x242424, 0x001cff, 0x0038ff, 0x0055ff, 0x0071ff, 0x008eff, 0x00aaff, 0x00c7ff},
                                             {0x242424, 0xe31cff, 0xc738ff, 0xaa55ff, 0x8e71ff, 0x718eff, 0x55aaff, 0x38c7ff},
                                             {0x242424, 0x668d1c, 0x669b38, 0x66aa55, 0x66b871, 0x66c68e, 0x66d4aa, 0x66e3c7},
                                             {0x242424, 0xffe31c, 0xffc738, 0xffaa55, 0xff8e71, 0xff718e, 0xff55aa, 0xff38c7},
                                             {0x242424, 0x4ff4ef, 0x25eafb, 0x11d9ff, 0x05c6ff, 0x00b6ff, 0x00a9ff, 0x009bfe},
                                             {0x242424, 0xe37d0d, 0x94bc00, 0x77dd55, 0x8df3c5, 0x89d9e2, 0x6b92aa, 0x686f8f},
                                             {0x242424, 0xc70038, 0x8f0070, 0x5500aa, 0x1d00e2, 0x001de2, 0x0055aa, 0x008f70},
                                             {0x242424, 0xfe0000, 0xff6000, 0xffd400, 0xaaff4c, 0x4cffaa, 0x00e5ff, 0x007aff},
                                             {0x242424, 0x9e0345, 0xa80072, 0x9e179b, 0x8536bc, 0x6b57d7, 0x5378ec, 0x3a9ffa},
                                             {0x242424, 0x006eff, 0x00fff8, 0x00ff5b, 0x3bff00, 0xd7ff00, 0xff8f00, 0xff000e},
                                             {0x242424, 0x773012, 0x7c6625, 0x6f8736, 0x4d9743, 0x52a77b, 0x5bb3a9, 0x64a4bf}};

#define N_ROCKS            7                            ///Number of different tetris blocks

//offset map for the tetris blocks, each block has 4 orientation/rotations
const int8_t aOffsets[N_ROCKS][4][8] = {{{0,0,-1, 1, 0, 1, 1, 1}, {0,0, 1, 1, 1, 0, 1,-1}, {0,0,-1,-1, 0,-1, 1,-1}, {0,0,-1, 1,-1, 0,-1,-1}},                   //Alpha-Block
                                        {{-1, 0, 0, 0, 1, 0, 2, 0}, {0, -3, 0, -2, 0, -1, 0, 0}, {2, 0, 1, 0, 0, 0, -1, 0}, {0, 0, 0, -1, 0, -2, 0, -3}},       //India
                                        {{1,0,1, 1, 0, 1, 0, 0}, {1, 1, 0, 1, 0, 0, 1, 0}, {0,1,0,0, 1,0, 1,1}, {0,0,1, 0,1, 1,0,1}},                           //Oscar-Block
                                        {{0, 0, 0, 1, 0, 2, -1, 2}, {-1, 1, 0, 1, 1, 1, 1, 2}, {0, 0, -1, 0, -1, 1, -1, 2}, {-1, 1, -1, 2, 0, 2, 1, 2}},        //Juliett
                                        {{0, 0, 0, 1, 0, 2, 1, 2}, {-1, 2, 0, 2, 1, 2, 1, 1}, {0, 0, 1, 0, 1, 1, 1, 2}, {-1, 2, -1, 1, 0, 1, 1, 1}},            //Lima
                                        {{0, 0, 0, 1, 1, 1, 1, 2}, {0, 1, 1, 1, 0, 2, -1, 2}, {1, 2, 1, 1, 0, 1, 0, 0}, {-1, 2, 0, 2, 1, 1, 0, 1}},             //Sierra
                                        {{0, 0, 0, 1, -1, 1, -1, 2}, {0, 1, -1, 1, 0, 2, 1, 2}, {-1, 2, -1, 1, 0, 1, 0, 0}, {1, 2, 0, 2, -1, 1, 0, 1}}};        //Zulu


const uint32_t aScores[] = {0, 40, 100, 300, 1200};                      ///Original scoring

//The start screent in RLE encoding
#define SIZE_START_SCREEN SCREEN_WIDTH*SCREEN_HEIGHT/4                   ///Size of the bit encoded start screen
const uint32_t aStartScreen[SIZE_START_SCREEN] = {32964608, 261888, 507904, 2016, 129990656, 261376, 980992, 3832, 502890496, 248064, 2087936, 4047, 3754524672, 130271488, 3760190464, 8189, 4266229760, 1055117063, 3089097728, 32767, 4065296384, 2007235855, 4162839552, 32767, 2457698304, 2099505423, 3357535744, 32767, 2481553408, 2032388367, 3357539200, 32767, 3217653760, 2036583183, 3424649088, 32575, 4090462208, 2078531391, 4280286848, 32563, 2482864128, 2133064055, 4280286336, 32755, 2481553408, 2032388477, 4255120576, 32659, 2479456256, 2032388473, 4188012528, 32563, 4090462208, 2044972921, 4188012350, 32563, 4093607936, 2133065087, 4280287207, 32755, 2482601984, 2099497343, 4255121405, 32755, 2479456256, 2032388473, 4188012537, 32563, 3016327168, 2044972921, 4188008441, 32563, 4157571072, 2141453689, 4213171707, 8048, 2650374144, 2099509631, 1042279486, 4046, 2680782848, 2032388473, 3223321600, 8189, 3192487936, 2036583289, 3223315456, 32767, 4065296384, 2078531449, 1075831808, 32766, 2457698304, 2133063999, 1075831808, 32766, 2481553408, 2032388367, 1075837952, 32766, 3217653760, 2032388367, 3223315456, 32575, 4090462208, 2044972863, 2089984, 32560, 2482864128, 2141453687, 2089984, 32752, 2481553408, 2099497341, 2096128, 32688, 2479456256, 2032388473, 3762287616, 32561, 4090462208, 2044972921, 3162498048, 32563, 4091510784, 2141453695, 4152353792, 32755, 2482601984, 2099505535, 4294962688, 32755, 2479456256, 2032388473, 4194302848, 32563, 3016589312, 2036583289, 4194303872, 32563, 4157571072, 2078793337, 4185914496, 16179, 2684125184, 2134767743, 4286577792, 4095, 2680782848, 2034223229, 4194303104, 4095, 3192487936, 2034124921, 4185915264, 4095, 4065263616, 2046183545, 4194303616, 4095, 2457600000, 2141616255, 4279237760, 4095, 2480930816, 522706959, 3691248768, 511, 3204448256, 255721487, 3355475840, 63, 4160749568, 267386887, 4160753152, 7};
#define SIZE_GAME_OVER_SCREEN SIZE_START_SCREEN*2                       ///Size of the 2bit encoded end screen
const uint32_t aGameOverScreen[SIZE_GAME_OVER_SCREEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 353697792, 1414812693, 1364283728, 1162166609, 1162166277, 352326917, 1347703892, 337, 353697792, 1414812693, 1364283728, 1162166609, 1162166277, 352326917, 1347703892, 337, 353697792, 1414813653, 1364283728, 1162166609, 1162166277, 352326917, 1347703892, 337, 983040, 264241392, 61440, 0, 0, 0, 4026531840, 0, 1425408, 1455445232, 22035792, 344400, 1074085888, 352326917, 1342180436, 86017, 1425408, 1442862320, 3243261264, 344415, 1074085888, 352326917, 1342180436, 86017, 14020608, 1459640316, 3243248976, 201670995, 1074085948, 352326917, 1342177364, 86017, 63172608, 201339900, 3233812224, 50331651, 15, 805322496, 4026531840, 3, 64352256, 1414813653, 3243248976, 88424799, 1074088960, 3573552389, 1342198868, 349, 265678848, 1414813653, 3444575568, 88437087, 1074101248, 3573552389, 1342198868, 349, 265678848, 1414813653, 4249891152, 88437087, 1074098176, 3573552389, 1342198871, 786769, 1073737728, 65472, 4227923904, 50344191, 258048, 4026531840, 786435, 3149823, 1070985216, 1409308629, 4048813392, 50691420, 1074085888, 3573552389, 1342963799, 15818749, 1070985264, 1409308629, 4049403216, 201686367, 1074085888, 3573552389, 1345323095, 66150397, 1070973888, 1409308437, 3244293456, 359775, 1882537984, 3573552389, 1358692439, 66936829, 1073729472, 16320, 3288334320, 1056968655, 4228841472, 4026531888, 16515087, 66912255, 366345984, 1409308629, 224390480, 1967488351, 1970618373, 4027909109, 1398035543, 16605181, 3587571456, 1409308629, 1029672272, 1967488351, 1970618373, 4229235701, 1398232919, 66936829, 3587570688, 1409308629, 1029619024, 1967488351, 1970618373, 4292153333, 1347901271, 66936813, 4294966272, 4278452223, 1073659888, 4294967215, 4294950915, 4293922815, 4093628223, 67108847, 4294966272, 4294967295, 4294622972, 4294966975, 4294967247, 4294721535, 4294918143, 66060207, 4227857408, 4294966271, 1073393407, 4294966927, 4290756607, 4294967295, 4294705151, 61865919, 4211014656, 4294966271, 4293570239, 4294965951, 4273975295, 4294967295, 4294705151, 61864703, 4211014656, 4294962175, 3219827375, 4273990335, 4273974271, 4294962175, 4293656575, 65710847, 2868575232, 3221203199, 2951391914, 4273973951, 4205816831, 4294946815, 3957587903, 65711103, 2867526656, 2751441658, 2951391914, 4272597695, 4205489151, 4290691071, 3937398335, 66759679, 2863332352, 2885593834, 2951391914, 3131747006, 4205488842, 4289609647, 2864704170, 16690175, 2863332364, 2868554410, 2885331626, 2863311530, 4205488890, 4272865167, 2864687786, 67021823, 2863332412, 2863311530, 2885331626, 2863311530, 4205488890, 3131749055, 2868685482, 67021562, 2863303920, 2863311530, 2854922922, 2863311530, 707439274, 2863311539, 2868882090, 13544106, 2863304688, 2863311530, 2863311530, 2863311530, 2863311530, 2863311535, 2868882090, 51030698, 2863316736, 2863311530, 2863311530, 2863311530, 2863311530, 2863311530, 2864622250, 66759338, 2863316928, 2863311530, 2863311530, 2863311530, 2862656170, 2863311530, 2863311530, 66759338, 2863316736, 2863311530, 2863311530, 2863311530, 2863311530, 2863311530, 2863311530, 66759338, 2863311872, 2863311530, 2854922922, 2863311530, 2863311530, 2863311530, 2863311530, 15641258, 2863329280, 2863311530, 2863311530, 2863311530, 2863311530, 2863311530, 2863311530, 3058346, 2863267840, 2863311530, 2863311530, 2863311530, 2863311530, 2863311530, 2863311530, 43690};


#define BOARD_HEIGHT             10
#define BOARD_WIDTH_VISIBLE      20
#define BOARD_WIDTH              25

#define BOARD_ANCHOR_X 4
#define BOARD_ANCHOR_Y 2

//Structure for the game board
typedef struct
{
    uint32_t width;
    uint32_t height;
    uint8_t aData[BOARD_WIDTH*BOARD_HEIGHT];
}UTIL_TETRIS_Board_t;

//structure for a block/rock
typedef struct
{
    int8_t x;                       ///x-position on the game board
    int8_t y;                       ///y-position on the game board
    uint8_t type;                   ///tpe resp. id of the block
    uint8_t rotation;               ///current rotation
    uint8_t rotationGoal;           ///target rotation
} UTIL_TETRIS_Rock_t;

typedef struct
{
    uint32_t nBlinks;               ///blinks to be executed
    uint32_t nextHighTime;          ///Time to turn on the LED
    uint32_t nextLowTime;           ///Time to turn off the LED
    uint8_t state;                  ///high or low state of the led
} UTIL_TETRIS_Led_t;

typedef struct
{
	float r;
	float g;
	float b;
} UTIL_TETRIS_Rgb_t;

//internal structure keeping the game
static struct
{
    UTIL_TETRIS_Board_t board;      ///the board
    UTIL_TETRIS_Rock_t block;       ///the current block
    UTIL_TETRIS_Rock_t blockNext;   ///the future block
    int32_t level;                  ///level
    uint32_t nLines;                ///lines
    uint32_t score;                 ///score
    uint32_t highScore;             ///highscore 
    bool gameOver;                  ///game over
    UTIL_TETRIS_Led_t blinker;      ///LED handler
} m;

///////////////////////////////////////////////////////////////////////////////////////////
// Simulation stuff - mocking and simulating display function
///////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
//static functions
/////////////////////////////////////////////////////////////////////////////////////////////////
static void getNextBlock(UTIL_TETRIS_Rock_t* const pRock);
static uint16_t randomgen();
static void placeBlockOnBoard(const UTIL_TETRIS_Rock_t* const pRock,
                              UTIL_TETRIS_Board_t* const pBoard,
                              const uint8_t blockId);

#define BLINK_DELAY 4
static void addBlinks(UTIL_TETRIS_Led_t* const pLed,
                      const uint32_t counter,
                      const uint32_t nBlinks)
{
    if(pLed->nBlinks == 0 && counter>pLed->nextLowTime)
    {
        pLed->nextHighTime=counter;
        pLed->nextLowTime=counter+BLINK_DELAY;
        pLed->state = 1;
        LUI_setQLEDDirect(true);
        pLed->nBlinks += (nBlinks - 1);
    }
    else
    {
        pLed->nBlinks += nBlinks;
    }
}

static void updateBlink(UTIL_TETRIS_Led_t* const pLed,
                        const uint32_t counter)
{
    if(pLed->state==1)
    {
        if(counter>pLed->nextLowTime)
        {
            //Turn off
            pLed->state = 0;
            LUI_setQLEDDirect(false);
            if(pLed->nBlinks>0)
            {
                pLed->nBlinks--;
                pLed->nextHighTime = counter + BLINK_DELAY;
                pLed->nextLowTime = counter + (2*BLINK_DELAY);
            }
            else
            {
                pLed->nextHighTime = (uint32_t) (-1);
            }
        }
    }
    if(pLed->state==0)
    {
        if(counter>pLed->nextHighTime)
        {
            //turn on
            pLed->state = 1;
            LUI_setQLEDDirect(true);
        }
    }
}
static uint16_t lfsr = 0xACE1u;
static uint16_t randomgen()
{
    uint16_t bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    return lfsr =  (lfsr >> 1) | (bit << 15);
}

/**
 * \brief get the next block
 * \param[out] pRock: the next randomly selected block
 */
static void getNextBlock(UTIL_TETRIS_Rock_t* const pRock)
{
    pRock->rotation = 0;
    pRock->rotationGoal = 0;
    pRock->y = (BOARD_HEIGHT>>1)-1;
    pRock->x = BOARD_WIDTH_VISIBLE + 1;
    pRock->type = (randomgen() % N_ROCKS);
}

#define DIRECTION_LEFT 0
#define DIRECTION_UP   1
#define DIRECTION_DOWN 2

/**
 * \brief Places a Rock on the Board.
 * \note  blockId=0=> erases the block
 * \param[in] pRock: the tetris block to be placed
 * \param[out] pBoard: the game board
 */
inline
static void placeBlockOnBoard(const UTIL_TETRIS_Rock_t* const pRock,
                              UTIL_TETRIS_Board_t* const pBoard,
                              const uint8_t blockId)
{
    for(uint32_t i=0Lu;i<4;i++)
    {
        int8_t xTemp = pRock->x + aOffsets[pRock->type][pRock->rotation][i*2+0];
        int8_t yTemp = pRock->y + aOffsets[pRock->type][pRock->rotation][i*2+1];
        pBoard->aData[yTemp*BOARD_WIDTH+xTemp] = blockId;
    }
}

/**
 * \brief CHeck if a block can be placed
 */
inline
static bool blockCanBePlaced(int8_t offsetX,
                             int8_t offsetY,
                             const UTIL_TETRIS_Rock_t* const pRock,
                             UTIL_TETRIS_Board_t* const pBoard)
{
    for(uint32_t i=0Lu;i<4;i++)
    {
        int8_t xTemp = pRock->x + aOffsets[pRock->type][pRock->rotation][i*2+0] + offsetX;
        int8_t yTemp = pRock->y + aOffsets[pRock->type][pRock->rotation][i*2+1] + offsetY;
        if(xTemp>=0 && xTemp<BOARD_WIDTH && yTemp>=0 && yTemp<BOARD_HEIGHT)
        {
            if(pBoard->aData[yTemp*BOARD_WIDTH+xTemp] > 0)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    return true;
}

static bool moveBlock(UTIL_TETRIS_Rock_t* const pRock,
                      UTIL_TETRIS_Board_t* const pBoard,
                      const uint8_t direction)
{
    //1. Configure the delta
    int8_t deltaX = 0;
    int8_t deltaY = 0;
    if(direction == DIRECTION_LEFT)
    {
        deltaX = -1;
    }
    if(direction == DIRECTION_UP)
    {
        deltaY = -1;
    }
    if(direction == DIRECTION_DOWN)
    {
        deltaY = 1;
    }
    //2. remove stone0
    placeBlockOnBoard(pRock, pBoard, 0);
    //3. check if it can be placed
    bool ok = blockCanBePlaced(deltaX, deltaY, pRock, pBoard);
    if(ok)
    {
        pRock->x += deltaX;
        pRock->y += deltaY;
    }
    //4. Place block
    placeBlockOnBoard(pRock, pBoard, pRock->type+1);
    return ok;
}

static bool placeBlock(UTIL_TETRIS_Rock_t* pRock,
                       UTIL_TETRIS_Board_t* pBoard)
{
    bool ok = true;
    for(uint32_t i=0Lu;i<4;i++)
    {
        int8_t xTemp = pRock->x + aOffsets[pRock->type][pRock->rotation][i*2+0];
        int8_t yTemp = pRock->y + aOffsets[pRock->type][pRock->rotation][i*2+1];
        if(xTemp>=0 && xTemp<BOARD_WIDTH && yTemp>=0 && yTemp<BOARD_HEIGHT)
        {
            if(pBoard->aData[yTemp*BOARD_WIDTH+xTemp] > 0)
            {
                ok = false;
                break;
            }
        }
        else
        {
            ok = false;
            break;
        }
    }
    if(ok)
    {
        //Place block
        placeBlockOnBoard(pRock, pBoard, pRock->type + 1);
    }
    return ok;
}

static bool rotateBlock(UTIL_TETRIS_Rock_t* pRock,
                        UTIL_TETRIS_Board_t* pBoard)
{
    pRock->rotationGoal++;
    pRock->rotationGoal%=4;
    //1. remove
    placeBlockOnBoard(pRock, pBoard, 0);
    //Temp move
    bool ok = true;
    for(uint32_t i=0Lu;i<4;i++)
    {
        int8_t xTemp = pRock->x + aOffsets[pRock->type][pRock->rotationGoal][i*2+0];
        int8_t yTemp = pRock->y + aOffsets[pRock->type][pRock->rotationGoal][i*2+1];
        if(xTemp>=0 && xTemp<BOARD_WIDTH && yTemp>=0 && yTemp<BOARD_HEIGHT)
        {
            if(pBoard->aData[yTemp*BOARD_WIDTH+xTemp] > 0)
            {
                ok = false;
                break;
            }
        }
        else
        {
            ok = false;
            break;
        }
    }
    //If ok: remove block
    if(ok)
    {
        pRock->rotation = pRock->rotationGoal;
    }
    //Place block
    placeBlockOnBoard(pRock, pBoard, pRock->type+1);

    return ok;
}

///////////////////////////////////////////////////////////////////////////////////
// Tetris Board
///////////////////////////////////////////////////////////////////////////////////

static void initBoard(UTIL_TETRIS_Board_t* pBoard)
{
    for(uint32_t i = 0Lu; i<BOARD_WIDTH*BOARD_HEIGHT; i++)
    {
        pBoard->aData[i] = 0;
    }
    getNextBlock(&(m.block));
    getNextBlock(&(m.blockNext));
}

static void draw3by3(int x, int y)
{
    GUI_DrawPixel(x  , y);
    GUI_DrawPixel(x+1, y);
    GUI_DrawPixel(x  , y+1);
    GUI_DrawPixel(x+1, y+1);
}
#define WIDTH_OF_FONT 5
const uint8_t aNumbers[10][5] = {{0b11111000, 0b10011000, 0b10101000, 0b11001000, 0b11111000},    //0
                                 {0b00100000, 0b01100000, 0b00100000, 0b00100000, 0b01110000},    //1
                                 {0b11110000, 0b00001000, 0b01110000, 0b10000000, 0b11111000},    //2
                                 {0b11111000, 0b00001000, 0b01110000, 0b00001000, 0b11111000},    //3
                                 {0b10000000, 0b10000000, 0b10100000, 0b11111000, 0b00100000},    //4
                                 {0b11111000, 0b10000000, 0b11110000, 0b00001000, 0b11110000},    //5
                                 {0b11111000, 0b10000000, 0b11111000, 0b10001000, 0b11111000},    //6
                                 {0b11111000, 0b00001000, 0b00010000, 0b00100000, 0b00100000},    //7
                                 {0b11111000, 0b10001000, 0b11111000, 0b10001000, 0b11111000},    //8
                                 {0b11111000, 0b10001000, 0b11111000, 0b00001000, 0b11111000}};   //9

static void printSingleNumberAt(int x, int y, uint32_t number)
{
    GUI_SetColor(0);
    for(uint32_t line=0Lu;line<5;line++)
    {
        for(uint32_t col=0Lu;col<5;col++)
        {
            GUI_DrawPixel(x+col, y+line);

        }
    }
    GUI_SetColor(0xFFFFFFFF);
    for(uint32_t line=0Lu;line<5;line++)
    {
        for(uint32_t col=0Lu;col<5;col++)
        {
            if(aNumbers[number][line]>>(7-col) & 1 == 1)
            {
                GUI_DrawPixel(x+col, y+line);
            }
        }
    }
}

/**
 * Print the current game status
 *    HI 1234567
 *    L        0
 *    S   123456
 */
inline
static void printStatus(const uint32_t x, const uint32_t y)
{
    GUI_SetBkColor(0x00000000);
    GUI_SetColor(0x00FFFFFF);
    char aText[10];
    for(uint32_t i = 0Lu;i<10;i++)
    {
        aText[i] = ' ';
    }
    aText[0] = 'H';
    aText[1] = 'I';
    aText[2] = 0;
    GUI_DispStringAt(aText, x+30, y-8);
    aText[0] = ' ';
    aText[1] = ' ';
    aText[2] = ' ';

    uint32_t n=m.highScore;
    for(uint32_t i=0Lu;i<7;i++)
    {
        uint32_t remainder = n % 10;
        aText[6-i] = remainder + '0';
        n = n / 10;
        if(n==0)
        {
            break;
        }
    }
    aText[7] = 0;
    GUI_DispStringAt(aText, x, y);
    //Display level
    for(uint32_t i = 0Lu;i<10;i++)
    {
        aText[i] = ' ';
    }
    aText[0] = 'L';
    aText[1] = ' ';
    n=m.nLines;
    for(uint32_t i=0Lu;i<6;i++)
    {
        uint32_t remainder = n % 10;
        aText[6-i] = remainder + '0';
        n = n / 10;
        if(n==0)
        {
            break;
        }
    }
    aText[7] = 0; 
    GUI_DispStringAt(aText, x, y+9);
    //Display score
    for(uint32_t i = 0Lu;i<10;i++)
    {
        aText[i] = ' ';
    }
    aText[0] = 'S';
    aText[1] = ' ';
    n=m.score;
    for(uint32_t i=0Lu;i<7;i++)
    {
        uint32_t remainder = n % 10;
        aText[6-i] = remainder + '0';
        n = n / 10;
        if(n==0)
        {
            break;
        }
    }
    aText[7] = 0; 
    GUI_DispStringAt(aText, x, y+2*9);
}

static void showBoard(UTIL_TETRIS_Board_t* pBoard, uint32_t level)
{
    for(uint32_t y=0;y<BOARD_HEIGHT;y++)
    {
        for(uint32_t x=0;x<BOARD_WIDTH_VISIBLE;x++)
        {
            GUI_SetColor(aColorMap[level][pBoard->aData[y*BOARD_WIDTH+x]]);
            uint32_t xTemp = BOARD_ANCHOR_X + (4*x);
            uint32_t yTemp = BOARD_ANCHOR_Y + (4*y);

            GUI_FillRect(xTemp, yTemp, xTemp+2, yTemp+2);
        }
    }
}
#define PREVIEW_ANCHOR_X 95
#define PREVIEW_ANCHOR_Y 2

static void showPreview(const UTIL_TETRIS_Rock_t* const pRock,
                        const uint32_t level)
{
    //CLR
    for(uint32_t x=0Lu;x<4;x++)
    {
        for(uint32_t y=0Lu;y<4;y++)
        {
            GUI_SetColor(aColorMap[level][0]);
            int8_t xTemp = PREVIEW_ANCHOR_X + x*3;
            int8_t yTemp = PREVIEW_ANCHOR_Y + y*3;
            draw3by3(xTemp, yTemp);
        }
    }
    //PLot Stone
    for(uint32_t i=0Lu;i<4;i++)
    {
        int8_t xTemp = PREVIEW_ANCHOR_X + (aOffsets[pRock->type][pRock->rotation][i*2+0]+1) * 3;
        int8_t yTemp = PREVIEW_ANCHOR_Y + (aOffsets[pRock->type][pRock->rotation][i*2+1]+1) * 3;
        GUI_SetColor(aColorMap[level][pRock->type+1]);
        draw3by3(xTemp, yTemp);
    }
}

static void eraseLine(UTIL_TETRIS_Board_t* pBoard,
                      const uint32_t line)
{
    for(uint32_t y=0;y<BOARD_HEIGHT;y++)
    {
        for(uint32_t x=line;x<BOARD_WIDTH-1;x++)
        {
            pBoard->aData[y*BOARD_WIDTH+x] = pBoard->aData[y*BOARD_WIDTH+x+1];
        }
    }
    for(uint32_t y=0;y<BOARD_HEIGHT;y++)
    {
        pBoard->aData[y*BOARD_WIDTH+BOARD_WIDTH-1] = 0;
    }
}

static bool checkLineComplete(UTIL_TETRIS_Board_t* pBoard,
                              const uint32_t line)
{
    for(uint32_t y=0;y<BOARD_HEIGHT;y++)
    {
        if(pBoard->aData[y*BOARD_WIDTH+line] == 0)
        {
            return false;
        }
    }
    return true;
}

static uint32_t evaluateBoard(UTIL_TETRIS_Board_t* pBoard)
{
    uint32_t nLines = 0Lu;
    for(int32_t x = BOARD_WIDTH-1; x>=0;x--)
    {
        if(checkLineComplete(pBoard, x))
        {
            eraseLine(pBoard, x);
            nLines++;
        }
    }
    return nLines;
}

/**
 * \brief shows the BU13 start screen
 *
 */
extern void UTIL_TETRIS_showStartScreen(void)
{

    float alpha = 1.f;
    float beta = 0.f;

    const float red0 = 255.f;
    const float green0 = 255.f;
    const float blue0 = 0.f;

    const float red1 = 0.f;
    const float green1 = 255.f;
    const float blue1 = 255.f;

    uint32_t currentColor = 0x00FFFF;
    uint32_t xCurrent = 0Lu;
    uint32_t yCurrent = 0Lu;
    uint8_t foreground = 0u;

    uint32_t index = 0Lu;
    for(uint32_t y=0Lu;y<SCREEN_HEIGHT;y++)
    {
        for(uint32_t xWord=0Lu;xWord<4Lu;xWord++)
        {
            uint32_t colorValue = aStartScreen[index];
            index++;
            for(uint32_t i=0Lu;i<32Lu;i++)
            {
                if((colorValue&(1<<i))>0Lu)
                {
                    GUI_SetColor(currentColor);
                }
                else
                {
                    GUI_SetColor(0x00000000);
                }
                GUI_DrawPixel((xWord*32)+i, y);
            }
        }
        beta = (float)y/((float)SCREEN_HEIGHT);
        alpha = 1.f-beta;
        uint8_t red = (uint8_t)((red0*alpha) + (red1*beta));
        uint8_t green = (uint8_t)((green0*alpha) + (green1*beta));
        uint8_t blue = (uint8_t)((blue0*alpha) + (blue1*beta));
        currentColor = blue<<16 | green<<8 | red;
    }
}

static uint32_t interpolateColor(const UTIL_TETRIS_Rgb_t* pCol1, const UTIL_TETRIS_Rgb_t* pCol2, const float alpha)
{
	const uint8_t red = (uint8_t) (255.f * ((pCol1->r * alpha) + (pCol2->r * (1.f-alpha))));
	const uint8_t green = (uint8_t) (255.f * ((pCol1->g * alpha) + (pCol2->g * (1.f-alpha))));
	const uint8_t blue = (uint8_t) (255.f * ((pCol1->b * alpha) + (pCol2->b * (1.f-alpha))));
	return (uint32_t)(blue<<16 | green<<8 | red);
}
/**
 * \brief shows the tetris game over screen
 *
 */
extern void UTIL_TETRIS_showGameOverScreen(void)
{

	UTIL_TETRIS_Rgb_t flameRedStart, flameRedStop, flameYellowStart, flameYellowStop, bgStart, bgStop, textStart, textStop;
	flameRedStart.r = 
	bgStart.r = 0.f;	bgStart.g = 0.f;	bgStart.b = 0.f;
	bgStop.r = 0.4f;	bgStop.g = 0.4f;	bgStop.b = 0.4f;

	textStart.r = 0.8f;	textStart.g = 0.8f;	textStart.b = 0.8f;
	textStop.r = 1.f;	textStop.g = 1.f;	textStop.b = 1.f;

	flameYellowStart.r = 1.f;	flameYellowStart.g = 1.f;	flameYellowStart.b = 0.f;
	flameYellowStop.r = 1.0f;	flameYellowStop.g = 1.0f;	flameYellowStop.b = 0.5f;

	flameRedStart.r = 1.f;	flameRedStart.g = 0.f;	flameRedStart.b = 0.f;
	flameRedStop.r = 1.0f;	flameRedStop.g = 0.5f;	flameRedStop.b = 0.5f;

    float alpha = 1.f;
    float beta = 0.f;

    uint32_t currentColorBg = 0x00000000;
    uint32_t currentColorText = 0x00000000;
    uint32_t currentColorFlameYellow = 0x00000000;
    uint32_t currentColorFlameRed = 0x00000000;
    uint32_t index = 0Lu;
    for(uint32_t y=0Lu;y<SCREEN_HEIGHT;y++)
    {
		
        for(uint32_t xWord=0Lu;xWord<8Lu;xWord++)
        {
            const uint32_t colorValue = aGameOverScreen[index];
            index++;
            for(uint32_t i=0Lu;i<16Lu;i++)
            {
				uint32_t colorIndex = (colorValue&(3<<(i*2)))>>(i*2);
				//0 Background 
				if(colorIndex == 0)
				{
                    GUI_SetColor(currentColorBg);
				}
				//1 Game Over String
				if(colorIndex == 1)
				{
                    GUI_SetColor(currentColorText);
				}
				//2 Inner Flames
				if(colorIndex == 2)
				{
                    GUI_SetColor(currentColorFlameYellow);
				}
				//3 Outer Flames
				if(colorIndex == 3)
				{
                    GUI_SetColor(currentColorFlameRed);
				}				
                GUI_DrawPixel((xWord*16)+i, y);
            }
        }
        beta = (float)y/((float)SCREEN_HEIGHT);
		currentColorBg = interpolateColor(&bgStart, &bgStop, 1-beta);
		currentColorText = interpolateColor(&textStart, &textStop, beta);
		currentColorFlameYellow = interpolateColor(&flameYellowStart, &flameYellowStop, 1.f-beta);
		currentColorFlameRed = interpolateColor(&flameRedStart, &flameRedStop, 1.f-beta);
    }
}

static uint32_t counter = 0;

/**
 * \brief Initializes the module
 * \param[in] seed: Seed for the random number generation
 */
extern void UTIL_TETRIS_init(const uint16_t seed,
                             const uint32_t highScore)
{
    GUI_SetBkColor(0x00000000u);
    GUI_Clear();
    lfsr = seed;
    //Init Game board
    initBoard(&m.board);
    m.level = 0;
    m.nLines = 0;
    m.score = 0;
    m.highScore = highScore;
    m.blinker.nBlinks=0;
    m.blinker.state=0;
    m.blinker.nextLowTime = 0;
    m.blinker.nextHighTime = 0;
    m.gameOver = false;
    showBoard(&m.board, m.level);
    showPreview(&m.blockNext, m.level);
    printStatus(85, 15);
}
#define LINES_PER_LEVEL 2

extern void UTIL_TETRIS_update(const UTIL_TETRIS_Input_t* const pButtons,
                               UTIL_TETRIS_Output_t* pOutput)
{
    counter++;
    showBoard(&m.board, m.level);
    updateBlink(&m.blinker, counter);
    if(pButtons->buttonLeft==1)
    {
        moveBlock(&m.block, &m.board, DIRECTION_UP);
    }
    if(pButtons->buttonRight==1)
    {
        moveBlock(&m.block, &m.board, DIRECTION_DOWN);
    }
    if(pButtons->buttonDebug==1)
    {
        moveBlock(&m.block, &m.board, DIRECTION_LEFT);
    }
    if(pButtons->buttonCenter==1)
    {
        rotateBlock(&m.block, &m.board);
    }
    pOutput->debugState |= 1;
    if(counter%2 == 0)
    {
        bool movementOk = moveBlock(&m.block, &m.board, DIRECTION_LEFT);
        //If we cannot move the block anymore it has landed
        if(!movementOk)
        {
            //1. Check line completeness and do stuff
            uint32_t nLines = evaluateBoard(&m.board);
            if(nLines>0)
            {
                addBlinks(&m.blinker, counter, nLines);
            }
            m.nLines += nLines;
            m.score += aScores[nLines]*(m.level+1);
            pOutput->score = m.score;
            if(m.nLines>(m.level+1)*LINES_PER_LEVEL)
            {
                m.level++;
            }
            printStatus(85, 15);
            //2. Check game over
            pOutput->debugState |= 2;
            m.block = m.blockNext;  //Current Block is next block
            getNextBlock(&m.blockNext);
            showPreview(&m.blockNext, m.level);
            //Try to put it on the board
            bool newBlockOk = placeBlock(&m.block, &m.board);
            if(!newBlockOk)
            {
                m.gameOver = true;
            }
            pOutput->gameOver = m.gameOver;
        }
    }
}

//int main() {
//    printf("Hello World\n");
//    return 0;
//}
