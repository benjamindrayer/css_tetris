#include<stdio.h>
#include<stdint.h>
#include <stdbool.h>

#include "util_tetris.h"

#define SIMULATION                              ///Flag to use the simulation display

#define SCREEN_WIDTH                128         ///Screen width
#define SCREEN_HEIGHT                44         ///Screen height

#define N_COLOR_MAPS                 12         ///Number of different color maps

#define BUTTON_STATE_OFF 0
#define BUTTON_STATE_ON  1

//Colormap to color the tetris stones
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

#define N_ROCKS            7                            ///Number of different tetris blocks

//offset map for the tetris blocks, each block has 4 orientation/rotations
const int8_t aOffsets[N_ROCKS][4][8] = {{{0,0,-1, 1, 0, 1, 1, 1}, {0,0, 1, 1, 1, 0, 1,-1}, {0,0,-1,-1, 0,-1, 1,-1}, {0,0,-1, 1,-1, 0,-1,-1}},                   //Alpha-Block
                                        {{-1, 0, 0, 0, 1, 0, 2, 0}, {0, -3, 0, -2, 0, -1, 0, 0}, {2, 0, 1, 0, 0, 0, -1, 0}, {0, 0, 0, -1, 0, -2, 0, -3}},       //India
                                        {{1,0,1, 1, 0, 1, 0, 0}, {1, 1, 0, 1, 0, 0, 1, 0}, {0,1,0,0, 1,0, 1,1}, {0,0,1, 0,1, 1,0,1}},                           //Oscar-Block
                                        {{0, 0, 0, 1, 0, 2, -1, 2}, {-1, 1, 0, 1, 1, 1, 1, 2}, {0, 0, -1, 0, -1, 1, -1, 2}, {-1, 1, -1, 2, 0, 2, 1, 2}},        //Juliett
                                        {{0, 0, 0, 1, 0, 2, 1, 2}, {-1, 2, 0, 2, 1, 2, 1, 1}, {0, 0, 1, 0, 1, 1, 1, 2}, {-1, 2, -1, 1, 0, 1, 1, 1}},            //Lima
                                        {{0, 0, 0, 1, 1, 1, 1, 2}, {0, 1, 1, 1, 0, 2, -1, 2}, {1, 2, 1, 1, 0, 1, 0, 0}, {-1, 2, 0, 2, 1, 1, 0, 1}},             //Sierra
                                        {{0, 0, 0, 1, -1, 1, -1, 2}, {0, 1, -1, 1, 0, 2, 1, 2}, {-1, 2, -1, 1, 0, 1, 0, 0}, {1, 2, 0, 2, -1, 1, 0, 1}}};        //Zulu


const uint32_t aScores[] = {0, 40, 100, 300, 1200};     ///Original scoring

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
    uint32_t debugState;
}UTIL_TETRIS_Output_t;

#define BOARD_HEIGHT             10
#define BOARD_WIDTH_VISIBLE      20
#define BOARD_WIDTH              24

#define BOARD_ANCHOR_X 4
#define BOARD_ANCHOR_Y 2

//Structure for the game board
typedef struct
{
    uint32_t width;
    uint32_t height;
    uint8_t aData[BOARD_WIDTH*BOARD_HEIGHT]
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

//internal structure keeping the game
static struct
{
    UTIL_TETRIS_Board_t board;      ///the board
    UTIL_TETRIS_Rock_t block;       ///the current block
    UTIL_TETRIS_Rock_t blockNext;   ///the future block
    int32_t level;                  ///level
    uint32_t nLines;                ///lines
    uint32_t score;                 ///score
} m;

///////////////////////////////////////////////////////////////////////////////////////////
// Simulation stuff - mocking and simulating display function
///////////////////////////////////////////////////////////////////////////////////////////

#ifdef SIMULATION

#define SCREEN_BYTES_PER_PIXEL        4
#define SCREEN_SIZE_IN_BYTES SCREEN_WIDTH * SCREEN_HEIGHT * SCREEN_BYTES_PER_PIXEL

//Simulated version of the display
static struct
{
    uint32_t aScreenData[SCREEN_SIZE_IN_BYTES / 4Lu];   ///The screen
    uint32_t color;                                     ///Foreground color
    uint32_t bkColor;                                   ///Background color
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

/////////////////////////////////////////////////////////////////////////////////////////////////
//static functions
/////////////////////////////////////////////////////////////////////////////////////////////////
static void getNextBlock(UTIL_TETRIS_Rock_t* const pRock);
static uint16_t randomgen();
static void placeBlockOnBoard(const UTIL_TETRIS_Rock_t* const pRock,
                              UTIL_TETRIS_Board_t* const pBoard,
                              const uint8_t blockId);

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
    pRock->y = BOARD_HEIGHT>>1;
    pRock->x = BOARD_WIDTH_VISIBLE;
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

static void draw4by4(int x, int y)
{
    GUI_DrawPixel(x  , y);
    GUI_DrawPixel(x+1, y);
    GUI_DrawPixel(x+2, y);

    GUI_DrawPixel(x  , y+1);
    GUI_DrawPixel(x+1, y+1);
    GUI_DrawPixel(x+2, y+1);

    GUI_DrawPixel(x  , y+2);
    GUI_DrawPixel(x+1, y+2);
    GUI_DrawPixel(x+2, y+2);

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
 * Print a number with a ceratin amount of digits
 */
inline
static void printNumberAt(const int x,
                          const int y,
                          const uint32_t number,
                          const uint32_t nDigits)
{
    uint32_t n=number;
    for(uint32_t i=0Lu;i<nDigits;i++)
    {
        uint32_t remainder = n % 10;
        printSingleNumberAt(x+(nDigits-1-i)*(WIDTH_OF_FONT+1), y, remainder);
        n = n / 10;
        if(n==0)
        {
            break;
        }
    }
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

            draw4by4(xTemp, yTemp);
        }
    }
}
#define PREVIEW_ANCHOR_X 88
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

static uint32_t counter = 0;

/**
 * \brief Initializes the module
 * \param[in] seed: Seed for the random number generation
 */
extern void UTIL_TETRIS_init(const uint16_t seed)
{
    lfsr = seed;
    //Init Game board
    initBoard(&m.board);
    m.level = 0;
    m.nLines = 0;
    m.score = 0;
    showBoard(&m.board, m.level);
    showPreview(&m.blockNext, m.level);
    printNumberAt(85, 35, m.score, 7);
}
#define LINES_PER_LEVEL 2

extern uint32_t UTIL_TETRIS_update(const UTIL_TETRIS_Input_t* const pButtons,
                                   UTIL_TETRIS_Output_t* pOutput)
{
    counter++;
    showBoard(&m.board, m.level);
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
    if(counter%4 == 0)
    {
        bool movementOk = moveBlock(&m.block, &m.board, DIRECTION_LEFT);
        //If we cannot move the block anymore it has landed
        if(!movementOk)
        {
            //1. Check line completeness and do stuff
            uint32_t nLines = evaluateBoard(&m.board);
            m.nLines += nLines;
            m.score += aScores[nLines]*(m.level+1);
            printNumberAt(85, 35, m.score, 7);
            pOutput->score = m.score;
            if(m.nLines>(m.level+1)*LINES_PER_LEVEL)
            {
                m.level++;
            }
            //2. Check game over
            pOutput->debugState |= 2;
            m.block = m.blockNext;  //Current Block is next block
            getNextBlock(&m.blockNext);
            showPreview(&m.blockNext, m.level);
            //Try to put it on the board
            bool newBlockOk = placeBlock(&m.block, &m.board);
            if(!newBlockOk)
            {
                pOutput->level = -1;
            }
        }
    }
}

int main() {
    printf("Hello World\n");
    return 0;
}
