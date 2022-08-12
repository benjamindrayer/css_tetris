#include<stdio.h>
#include<stdint.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 44
#define SCREEN_BYTES_PER_PIXEL 4
#define SCREEN_SIZE_IN_BYTES SCREEN_WIDTH * SCREEN_HEIGHT * SCREEN_BYTES_PER_PIXEL

#define SIMULATION

#ifdef SIMULATION
static struct
{
	uint32_t aScreenData[SCREEN_SIZE_IN_BYTES / 4Lu];
	uint32_t color;
	uint32_t bkColor;
} m;

static int counter = 0;

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

uint32_t update(uint8_t* pData)
{
	GUI_SetBkColor(0);
	GUI_SetColor(0x00FF0000);
	counter++;
	int y = 10;
	int x = counter%SCREEN_WIDTH;
	GUI_DrawPixel(x, y);
}

int main() {
	printf("Hello World\n");
	return 0;
}