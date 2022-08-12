#include<stdio.h>
#include<stdint.h>
#define SIMULATION

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 44
#define SCREEN_BYTES_PER_PIXEL 4
#define SCREEN_SIZE_IN_BYTES SCREEN_WIDTH * SCREEN_HEIGHT * SCREEN_BYTES_PER_PIXEL

#define BUTTON_STATE_OFF 0
#define BUTTON_STATE_ON  1

const uint32_t aColorMap[8] = {0x00000000,
                               0x000000FF,
                               0x0000FF00,
                               0x00FF0000,
                               0x000000FF,
                               0x0000FF00,
                               0x00FF0000,
                               0x00FF0000}; 

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

#ifdef SIMULATION
static struct
{
	uint32_t aScreenData[SCREEN_SIZE_IN_BYTES / 4Lu];
	UTIL_TETRIS_Board_t board;
	uint32_t color;
	uint32_t bkColor;
} m;


static void GUI_DrawPixel(int x, int y)
{
	m.aScreenData[(y*SCREEN_WIDTH)+x] = m.color;	
}

static void GUI_Clear()
{
	for(uint32_t i = 0Lu; i<(SCREEN_WIDTH*SCREEN_HEIGHT); i++)
	{
		m.aScreenData[i] = m.bkColor;
	}
}

static void GUI_SetColor(const uint32_t color)
{
	m.color = color;
}

static void GUI_SetBkColor(const uint32_t color)
{
	m.bkColor = color;
}

uint32_t* getImageBuffer()
{
	return &m.aScreenData[0];
}

#endif

///////////////////////////////////////////////////////////////////////////////////
// Tetris Board
///////////////////////////////////////////////////////////////////////////////////

static void initBoard(UTIL_TETRIS_Board_t* pBoard)
{
	for(uint32_t i = 0Lu; i<BOARD_WIDTH*BOARD_HEIGHT; i++)
	{
		pBoard->aData[i] = 0;
	}
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
	for(int i=0;i<200;i++)
	{
		m.board.aData[i] = i%8;
	}
}

uint32_t update(const UTIL_TETRIS_Input_t* const pButtons)
{
	showBoard(&m.board, 0);	
	if(pButtons->buttonLeft==1)
	{
     	counterX--;
	}
	if(pButtons->buttonRight==1)
	{
     	counterX++;
	}
	if(pButtons->buttonCenter==1)
	{
     	counterY++;
	}
	
	counterY = counterY%20;
	counterX = counterX%10;
}

int main() {
	printf("Hello World\n");
	return 0;
}