#include<stdio.h>
#include<stdint.h>
#include <stdbool.h>

#include "util_tetris.h"

#define SIMULATION

#define N_COLOR_MAPS            12      ///Number of different color maps

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 44
#define SCREEN_BYTES_PER_PIXEL 4
#define SCREEN_SIZE_IN_BYTES SCREEN_WIDTH * SCREEN_HEIGHT * SCREEN_BYTES_PER_PIXEL

#define BUTTON_STATE_OFF 0
#define BUTTON_STATE_ON  1

//Colormap to color the tetris stones, TODO: easy same as the colors from stefan's present

const uint32_t aColorMap[N_COLOR_MAPS][8] = {{0x141414, 0xf11c00, 0xe33800, 0xd45500, 0xc67100, 0xb88e00, 0xaaaa00, 0x9bc700},
                                             {0x141414, 0x001cff, 0x0038ff, 0x0055ff, 0x0071ff, 0x008eff, 0x00aaff, 0x00c7ff},
                                             {0x141414, 0xe31cff, 0xc738ff, 0xaa55ff, 0x8e71ff, 0x718eff, 0x55aaff, 0x38c7ff},
                                             {0x141414, 0x668d1c, 0x669b38, 0x66aa55, 0x66b871, 0x66c68e, 0x66d4aa, 0x66e3c7},
                                             {0x141414, 0xffe31c, 0xffc738, 0xffaa55, 0xff8e71, 0xff718e, 0xff55aa, 0xff38c7},
                                             {0x141414, 0x4ff4ef, 0x25eafb, 0x11d9ff, 0x05c6ff, 0x00b6ff, 0x00a9ff, 0x009bfe},
                                             {0x141414, 0xe37d0d, 0x94bc00, 0x77dd55, 0x8df3c5, 0x89d9e2, 0x6b92aa, 0x686f8f},
                                             {0x141414, 0xc70038, 0x8f0070, 0x5500aa, 0x1d00e2, 0x001de2, 0x0055aa, 0x008f70},
                                             {0x141414, 0xfe0000, 0xff6000, 0xffd400, 0xaaff4c, 0x4cffaa, 0x00e5ff, 0x007aff},
                                             {0x141414, 0x9e0345, 0xa80072, 0x9e179b, 0x8536bc, 0x6b57d7, 0x5378ec, 0x3a9ffa},
                                             {0x141414, 0x006eff, 0x00fff8, 0x00ff5b, 0x3bff00, 0xd7ff00, 0xff8f00, 0xff000e},
                                             {0x141414, 0x773012, 0x7c6625, 0x6f8736, 0x4d9743, 0x52a77b, 0x5bb3a9, 0x64a4bf}};


const int8_t aOffsets[][8] = {{0,0,-1, 1, 0, 1, 1, 1},
                              {0,0, 1, 1, 1, 0, 1,-1},
                              {0,0,-1,-1, 0,-1, 1,-1},
                              {0,0,-1, 1,-1, 0,-1,-1}};
// Structure for the input of the game
typedef struct
{
    uint8_t buttonLeft;
    uint8_t buttonCenter;
    uint8_t buttonRight;
}UTIL_TETRIS_Input_t;

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#define BOARD_ANCHOR_X 54
#define BOARD_ANCHOR_Y 2

//Structure for the game board
typedef struct
{
    uint32_t width;
    uint32_t height;
    uint8_t aData[BOARD_WIDTH*BOARD_HEIGHT]
} UTIL_TETRIS_Board_t;

//structure for a block/rock
typedef struct
{
    int8_t x;
    int8_t y;
    uint8_t type;
    uint8_t rotation;
    uint8_t rotationGoal;
} UTIL_TETRIS_Rock_t;

static struct
{
    UTIL_TETRIS_Board_t board;
    UTIL_TETRIS_Rock_t block;
} m;
#ifdef SIMULATION
static struct
{
    uint32_t aScreenData[SCREEN_SIZE_IN_BYTES / 4Lu];
    uint32_t color;
    uint32_t bkColor;
} mSimulation;


static void GUI_DrawPixel(int x, int y)
{
    mSimulation.aScreenData[(y*SCREEN_WIDTH)+x] = mSimulation.color;
}

static void GUI_Clear()
{
    for(uint32_t i = 0Lu; i<(SCREEN_WIDTH*SCREEN_HEIGHT); i++)
    {
        mSimulation.aScreenData[i] = mSimulation.bkColor;
    }
}

static void GUI_SetColor(const uint32_t color)
{
    mSimulation.color = color;
}

static void GUI_SetBkColor(const uint32_t color)
{
    mSimulation.bkColor = color;
}

uint32_t* getImageBuffer()
{
    return &mSimulation.aScreenData[0];
}

#endif

//TODO: defines for x and y init
//TODO: random selection of block type
static void getNextBlock(UTIL_TETRIS_Rock_t* pRock)
{
    pRock->rotation = 0;
    pRock->rotationGoal = 0;
    pRock->y = 4;
    pRock->x = 5;
    pRock->type = 0;
}

static bool moveBlockLeft(UTIL_TETRIS_Rock_t* pRock,
                          UTIL_TETRIS_Board_t* pBoard)
{
    //1. remove
    for(uint32_t i=0Lu;i<4;i++)
    {
        int8_t xTemp = pRock->x + aOffsets[4*pRock->type + pRock->rotation][i*2+0];
        int8_t yTemp = pRock->y + aOffsets[4*pRock->type + pRock->rotation][i*2+1];
        pBoard->aData[yTemp*BOARD_WIDTH+xTemp] = 0;
    }
    //Temp move
    bool ok = true;
    for(uint32_t i=0Lu;i<4;i++)
    {
        int8_t xTemp = pRock->x + aOffsets[4*pRock->type + pRock->rotation][i*2+0] - 1;
        int8_t yTemp = pRock->y + aOffsets[4*pRock->type + pRock->rotation][i*2+1];
        if(xTemp>=0 && yTemp>=0)
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
        pRock->x--;
    }
    //Place block
    for(uint32_t i=0Lu;i<4;i++)
    {
        int8_t xTemp = pRock->x + aOffsets[4*pRock->type + pRock->rotation][i*2+0];
        int8_t yTemp = pRock->y + aOffsets[4*pRock->type + pRock->rotation][i*2+1];
        pBoard->aData[yTemp*BOARD_WIDTH+xTemp] = pRock->type + 1;
    }
}

static bool moveBlockRight(UTIL_TETRIS_Rock_t* pRock,
                          UTIL_TETRIS_Board_t* pBoard)
{
    //1. remove
    for(uint32_t i=0Lu;i<4;i++)
    {
        int8_t xTemp = pRock->x + aOffsets[4*pRock->type + pRock->rotation][i*2+0];
        int8_t yTemp = pRock->y + aOffsets[4*pRock->type + pRock->rotation][i*2+1];
        pBoard->aData[yTemp*BOARD_WIDTH+xTemp] = 0;
    }
    //Temp move
    bool ok = true;
    for(uint32_t i=0Lu;i<4;i++)
    {
        int8_t xTemp = pRock->x + aOffsets[4*pRock->type + pRock->rotation][i*2+0] + 1;
        int8_t yTemp = pRock->y + aOffsets[4*pRock->type + pRock->rotation][i*2+1];
        if(xTemp<BOARD_WIDTH)
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
        pRock->x++;
    }
    //Place block
    for(uint32_t i=0Lu;i<4;i++)
    {
        int8_t xTemp = pRock->x + aOffsets[4*pRock->type + pRock->rotation][i*2+0];
        int8_t yTemp = pRock->y + aOffsets[4*pRock->type + pRock->rotation][i*2+1];
        pBoard->aData[yTemp*BOARD_WIDTH+xTemp] = pRock->type + 1;
    }
}

static bool rotateBlock(UTIL_TETRIS_Rock_t* pRock,
                        UTIL_TETRIS_Board_t* pBoard)
{
    pRock->rotationGoal++;
    pRock->rotationGoal%=4;
    //1. remove
    for(uint32_t i=0Lu;i<4;i++)
    {
        int8_t xTemp = pRock->x + aOffsets[4*pRock->type + pRock->rotation][i*2+0];
        int8_t yTemp = pRock->y + aOffsets[4*pRock->type + pRock->rotation][i*2+1];
        pBoard->aData[yTemp*BOARD_WIDTH+xTemp] = 0;
    }
    //Temp move
    bool ok = true;
    for(uint32_t i=0Lu;i<4;i++)
    {
        int8_t xTemp = pRock->x + aOffsets[4*pRock->type + pRock->rotationGoal][i*2+0];
        int8_t yTemp = pRock->y + aOffsets[4*pRock->type + pRock->rotationGoal][i*2+1];
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
    for(uint32_t i=0Lu;i<4;i++)
    {
        int8_t xTemp = pRock->x + aOffsets[4*pRock->type + pRock->rotation][i*2+0];
        int8_t yTemp = pRock->y + aOffsets[4*pRock->type + pRock->rotation][i*2+1];
        pBoard->aData[yTemp*BOARD_WIDTH+xTemp] = pRock->type + 1;
    }

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
}

static void showBoard(UTIL_TETRIS_Board_t* pBoard, uint32_t level)
{
    for(uint32_t y=0;y<BOARD_HEIGHT;y++)
    {
        for(uint32_t x=0;x<BOARD_WIDTH;x++)
        {
            GUI_SetColor(aColorMap[pBoard->aData[y*BOARD_WIDTH+x]]);
            uint32_t xTemp = BOARD_ANCHOR_X + (2*x);
            uint32_t yTemp = BOARD_ANCHOR_Y + (2*y);

            GUI_DrawPixel(xTemp  , yTemp);
            GUI_DrawPixel(xTemp+1, yTemp);
            GUI_DrawPixel(xTemp  , yTemp+1);
            GUI_DrawPixel(xTemp+1, yTemp+1);
        }
    }
}

static uint32_t counterX = 0;
static uint32_t counterY = 0;
static uint32_t counterColor = 0;

uint32_t init()
{
    //Init Game board
    initBoard(&m.board);
}

uint32_t update(const UTIL_TETRIS_Input_t* const pButtons)
{
    showBoard(&m.board, 0);
    if(pButtons->buttonLeft==1)
    {
        moveBlockLeft(&m.block, &m.board);
    }
    if(pButtons->buttonRight==1)
    {
        moveBlockRight(&m.block, &m.board);
    }
    if(pButtons->buttonCenter==1)
    {
        rotateBlock(&m.block, &m.board);
    }

    counterY = counterY%20;
    counterX = counterX%10;
}

int main() {
    printf("Hello World\n");
    return 0;
}
