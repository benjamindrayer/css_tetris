#Run me
from the_game.display import *
import ctypes

class UTIL_TETRIS_Input_t(ctypes.Structure):
    _fields_ = [
        ("buttonLeft", ctypes.c_uint8),
        ("buttonCenter", ctypes.c_uint8),
        ("buttonRight", ctypes.c_uint8),
    ]

DISPLAY_WIDTH = 128
DISPLAY_HEIGHT = 44
PATH_TO_DLL = "C:/Development/css_tetris/c/b3/libtetris.dll"
niklas_dll = ctypes.CDLL(PATH_TO_DLL)
niklas_dll.main()
niklas_dll.getImageBuffer.restype = ctypes.POINTER(ctypes.c_uint32)
image_array = niklas_dll.getImageBuffer()

def display_image(screen, image):
    for x in range(DISPLAY_WIDTH):
        for y in range(DISPLAY_HEIGHT):
            color = image_array[y*DISPLAY_WIDTH+x]
            r = color & 255
            g = (color>>8)&255
            b = (color>>16)&255
            screen.set_pixel(x, y, [r, g, b])


screen = Display(DISPLAY_WIDTH, DISPLAY_HEIGHT)


pygame.init()
#input_control = InputControl(input_type=INPUT_TYPE_BOTH)
pygame.display.set_caption('Niklas')
screen.show()
show_arrows = True
running = True
iter = 0
buttons = UTIL_TETRIS_Input_t()
while running:
    iter = iter + 1
    niklas_dll.update(ctypes.pointer(buttons))
    display_image(screen, image_array)
    screen.show()
    time.sleep(0.01)

    events = pygame.event.get()
    buttons.buttonLeft = 0
    buttons.buttonCenter = 0
    buttons.buttonRight = 0
    for event in events:
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                buttons.buttonLeft = 1
            if event.key == pygame.K_RIGHT:
                buttons.buttonRight = 1
            if event.key == pygame.K_DOWN:
                buttons.buttonCenter = 1
    print(iter)
