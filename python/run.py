#Run me
from the_game.display import *
import ctypes

#Structure for the c-code to handle the input buttons
class UTIL_TETRIS_Input_t(ctypes.Structure):
    _fields_ = [
        ("buttonLeft", ctypes.c_uint8),
        ("buttonCenter", ctypes.c_uint8),
        ("buttonRight", ctypes.c_uint8),
        ("buttonDebug", ctypes.c_uint8),
    ]

#Structure for the c-code to handle the output
class UTIL_TETRIS_Output_t(ctypes.Structure):
    _fields_ = [
        ("musicCommand", ctypes.c_uint32),
        ("level", ctypes.c_uint32),
        ("score", ctypes.c_uint32),
        ("debugState", ctypes.c_uint32),
    ]

#Display configuration
CSS_DISPLAY_WIDTH = 128
CSS_DISPLAY_HEIGHT = 44

CSS_DISPLAY_OFFSET_X = 284
CSS_DISPLAY_OFFSET_Y = 66

SIMULATION_DISPLAY_WIDTH = 614
SIMULATION_DISPLAY_HEIGHT = 244

#DLL STUFF
PATH_TO_DLL = "../c/build/libtetris.dll"
niklas_dll = ctypes.CDLL(PATH_TO_DLL)
niklas_dll.getImageBuffer.restype = ctypes.POINTER(ctypes.c_uint32)
image_array = niklas_dll.getImageBuffer()

def display_css_image(screen, css_image):
    for x in range(CSS_DISPLAY_WIDTH):
        for y in range(CSS_DISPLAY_HEIGHT):
            color = css_image[y*CSS_DISPLAY_WIDTH+x]
            r = color & 255
            g = (color>>8)&255
            b = (color>>16)&255
            screen.set_pixel(CSS_DISPLAY_OFFSET_X + 2*x, CSS_DISPLAY_OFFSET_Y + 2*y, [r, g, b])
            screen.set_pixel(CSS_DISPLAY_OFFSET_X + 2*x, CSS_DISPLAY_OFFSET_Y + 2*y+1, [r, g, b])
            screen.set_pixel(CSS_DISPLAY_OFFSET_X + 2*x+1, CSS_DISPLAY_OFFSET_Y + 2*y, [r, g, b])
            screen.set_pixel(CSS_DISPLAY_OFFSET_X + 2*x+1, CSS_DISPLAY_OFFSET_Y + 2*y+1, [r, g, b])


screen = Display(SIMULATION_DISPLAY_WIDTH, SIMULATION_DISPLAY_HEIGHT, factor=1)

#Load the sensor images
image = pygame.image.load('images/sensor_2.png')
image_on = pygame.image.load('images/sensor_on.png')

#select and define the regions for the button and Q-LED
Q_LED_ROI = [254, 276, 164, 182]
BUTTON_LEFT_ROI = [284, 156, 362, 196]

pygame.init()
#input_control = InputControl(input_type=INPUT_TYPE_BOTH)
pygame.display.set_caption('CSS-Tetris')
screen.show()
screen.showImage(image)
screen.showImageMasked(image_on, Q_LED_ROI)
screen.show()

running = True
iter = 0
buttons = UTIL_TETRIS_Input_t()
output = UTIL_TETRIS_Output_t()
display_mask = [CSS_DISPLAY_OFFSET_X, CSS_DISPLAY_OFFSET_X+CSS_DISPLAY_WIDTH*2, CSS_DISPLAY_OFFSET_Y, CSS_DISPLAY_OFFSET_Y+CSS_DISPLAY_HEIGHT*2]
niklas_dll.UTIL_TETRIS_init(1222)
niklas_dll.UTIL_TETRIS_qetLedState.restype = ctypes.c_bool;
while running:
    iter = iter + 1
    niklas_dll.UTIL_TETRIS_update(ctypes.pointer(buttons), ctypes.pointer(output))
    #handle the LED
    led_on = niklas_dll.UTIL_TETRIS_qetLedState()
    print(led_on)
    if led_on:
        #Turn on the led
        screen.showImageMasked(image, Q_LED_ROI)
    else:
        #Turn it off
        screen.showImageMasked(image_on, Q_LED_ROI)
    screen.show(mask=Q_LED_ROI)

    print(iter, output.debugState, output.musicCommand)
    display_css_image(screen, image_array)
    screen.show(mask=display_mask)
    time.sleep(0.01)

    events = pygame.event.get()
    buttons.buttonLeft = 0
    buttons.buttonCenter = 0
    buttons.buttonRight = 0
    buttons.buttonDebug = 0
    for event in events:
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                buttons.buttonLeft = 1
            if event.key == pygame.K_RIGHT:
                buttons.buttonRight = 1
            if event.key == pygame.K_UP:
                buttons.buttonCenter = 1
            if event.key == pygame.K_DOWN:
                buttons.buttonDebug = 1
