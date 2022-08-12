#include<stdio.h>
#include<stdint.h>
#define SIMULATION

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 44
#define SCREEN_BYTES_PER_PIXEL 4
#define SCREEN_SIZE_IN_BYTES SCREEN_WIDTH * SCREEN_HEIGHT * SCREEN_BYTES_PER_PIXEL

#define BUTTON_STATE_OFF 0
#define BUTTON_STATE_ON  1

// Structure for the input of the game
typedef struct
{
	uint8_t buttonLeft;			
	uint8_t buttonCenter;			
	uint8_t buttonRight;			
}UTIL_TETRIS_Input_t;

#ifdef SIMULATION
static struct
{
	uint32_t aScreenData[SCREEN_SIZE_IN_BYTES / 4Lu];
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

static uint32_t counterX = 0;
static uint32_t counterY = 0;
static uint32_t counterColor = 0;

uint32_t update(const UTIL_TETRIS_Input_t* const pButtons)
{
	GUI_SetBkColor(0x00008000);
	GUI_Clear();
	GUI_SetColor(0x00FF0000);
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
	uint32_t y = counterY%SCREEN_HEIGHT;
	uint32_t x = counterX%SCREEN_WIDTH;
	GUI_DrawPixel(x, y);
}

int main() {
	printf("Hello World\n");
	return 0;
}