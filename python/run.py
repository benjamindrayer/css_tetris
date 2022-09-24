# Run the complete Tetris game
#
#  1. Start screen
#       | | 2 seconds or key pressed
#        v
#  2. Game
#       | | Game lost
#        v
#  3. Game Over
#       | | 2 seconds or key pressed
#        v
#     New High score ?
# 4. Enter name
#
# 5. Show Highscore

# TODO
# 1. Display highscore and level during the tetris game
# 3. Highscore in txt file
# 4. Prepare the structure to save the name / score
# 5. Select the correct index for editing the name/score
# 8. Print new highscore, when score is good
# 9. Level up message
# 10. refactor void to leader_board
# 11.
from the_game.display import *
import ctypes


# Structure for the c-code to handle the input buttons
class UTIL_TETRIS_Input_t(ctypes.Structure):
    _fields_ = [
        ("buttonLeft", ctypes.c_uint8),
        ("buttonCenter", ctypes.c_uint8),
        ("buttonRight", ctypes.c_uint8),
        ("buttonDebug", ctypes.c_uint8),
    ]


# Structure for the c-code to handle the output
class UTIL_TETRIS_Output_t(ctypes.Structure):
    _fields_ = [
        ("musicCommand", ctypes.c_uint32),
        ("level", ctypes.c_uint32),
        ("score", ctypes.c_uint32),
        ("gameOver", ctypes.c_bool),
        ("debugState", ctypes.c_uint32),
    ]

class UTIL_TETRIS_Output_t(ctypes.Structure):
    _fields_ = [
        ("musicCommand", ctypes.c_uint32),
        ("level", ctypes.c_uint32),
        ("score", ctypes.c_uint32),
        ("gameOver", ctypes.c_bool),
        ("debugState", ctypes.c_uint32),
    ]

HIGHSCORE_NAME_LENGTH = 3
class UTIL_HIGHSCORE_Entry_t(ctypes.Structure):
    _fields_ = [
        ("aName", ctypes.c_uint8*HIGHSCORE_NAME_LENGTH),
        ("score", ctypes.c_uint32),
    ]

class UTIL_HIGHSCORE_Leaderboard_t(ctypes.Structure):
    _fields_ = [
        ("aEntries", UTIL_HIGHSCORE_Entry_t * 3)
    ]

leader_board = UTIL_HIGHSCORE_Leaderboard_t()
leader_board.aEntries[0].score = 12345678
leader_board.aEntries[1].score = 100
leader_board.aEntries[2].score = 10
leader_board.aEntries[0].aName[0] = 66
leader_board.aEntries[0].aName[1] = 66
leader_board.aEntries[0].aName[2] = 66
leader_board.aEntries[1].aName[0] = 67
leader_board.aEntries[1].aName[1] = 67
leader_board.aEntries[1].aName[2] = 67
leader_board.aEntries[2].aName[0] = 68
leader_board.aEntries[2].aName[1] = 68
leader_board.aEntries[2].aName[2] = 68

# Display configuration
CSS_DISPLAY_WIDTH = 128
CSS_DISPLAY_HEIGHT = 44

CSS_DISPLAY_OFFSET_X = 284
CSS_DISPLAY_OFFSET_Y = 66

SIMULATION_DISPLAY_WIDTH = 614
SIMULATION_DISPLAY_HEIGHT = 244

# DLL STUFF
PATH_TO_DLL = "../c/build/libtetris.dll"
niklas_dll = ctypes.CDLL(PATH_TO_DLL)
niklas_dll.getImageBuffer.restype = ctypes.POINTER(ctypes.c_uint32)
image_array = niklas_dll.getImageBuffer()


def display_css_image(screen, css_image):
    for x in range(CSS_DISPLAY_WIDTH):
        for y in range(CSS_DISPLAY_HEIGHT):
            color = css_image[y * CSS_DISPLAY_WIDTH + x]
            r = color & 255
            g = (color >> 8) & 255
            b = (color >> 16) & 255
            screen.set_pixel(CSS_DISPLAY_OFFSET_X + 2 * x, CSS_DISPLAY_OFFSET_Y + 2 * y, [r, g, b])
            screen.set_pixel(CSS_DISPLAY_OFFSET_X + 2 * x, CSS_DISPLAY_OFFSET_Y + 2 * y + 1, [r, g, b])
            screen.set_pixel(CSS_DISPLAY_OFFSET_X + 2 * x + 1, CSS_DISPLAY_OFFSET_Y + 2 * y, [r, g, b])
            screen.set_pixel(CSS_DISPLAY_OFFSET_X + 2 * x + 1, CSS_DISPLAY_OFFSET_Y + 2 * y + 1, [r, g, b])


def read_input(buttons):
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

screen = Display(SIMULATION_DISPLAY_WIDTH, SIMULATION_DISPLAY_HEIGHT, factor=1)

# Load the sensor images
image = pygame.image.load('images/sensor_2.png')
image_on = pygame.image.load('images/sensor_on.png')

# select and define the regions for the button and Q-LED
Q_LED_ROI = [254, 276, 164, 182]
BUTTON_LEFT_ROI = [284, 156, 362, 196]

pygame.init()
# input_control = InputControl(input_type=INPUT_TYPE_BOTH)
pygame.display.set_caption('CSS-Tetris')
screen.show()
screen.showImage(image)
screen.showImageMasked(image_on, Q_LED_ROI)
screen.show()

running = True
iter = 0
buttons = UTIL_TETRIS_Input_t()
output = UTIL_TETRIS_Output_t()
display_mask = [CSS_DISPLAY_OFFSET_X, CSS_DISPLAY_OFFSET_X + CSS_DISPLAY_WIDTH * 2, CSS_DISPLAY_OFFSET_Y,
                CSS_DISPLAY_OFFSET_Y + CSS_DISPLAY_HEIGHT * 2]
STATE_START_SCREEN = 1
STATE_GAME = 2
STATE_GAME_OVER = 3
STATE_NEW_HIGHSCORE = 4
STATE_SHOW_HIGHSCORE = 5

game_state = STATE_SHOW_HIGHSCORE
while running:
    iter += 1
    read_input(buttons)
    #Start screen state
    if game_state == STATE_START_SCREEN:
        niklas_dll.UTIL_TETRIS_showStartScreen()
        display_css_image(screen, image_array)
        screen.show(mask=display_mask)
        if buttons.buttonLeft==1 or buttons.buttonRight==1 or buttons.buttonCenter==1 or iter > 10:
            game_state = STATE_GAME
            niklas_dll.UTIL_TETRIS_init(1222, leader_board.aEntries[0].score)
            continue
    #Tetris game state
    if game_state == STATE_GAME:
        niklas_dll.UTIL_TETRIS_update(ctypes.pointer(buttons), ctypes.pointer(output))
        display_css_image(screen, image_array)
        screen.show(mask=display_mask)
        print(output.gameOver)
        #CHeck for game over here
        if output.gameOver:
            game_state = STATE_GAME_OVER
            iter = 0
            continue

    #Tetris game over state
    if game_state == STATE_GAME_OVER:
        niklas_dll.UTIL_TETRIS_showGameOverScreen()
        display_css_image(screen, image_array)
        screen.show(mask=display_mask)
        if buttons.buttonLeft==1 or buttons.buttonRight==1 or buttons.buttonCenter==1 or iter > 10:
            #Check if the score is new highscore
            output.score = 123
            is_highscore = niklas_dll.UTIL_HIGHSCORE_isNewHighscore(ctypes.pointer(leader_board), output.score)
            print("NEW HIGHSCORE", is_highscore)
            iter = 0
            niklas_dll.GUI_Clear()
            if is_highscore:
                game_state = STATE_NEW_HIGHSCORE
            else:
                game_state = STATE_SHOW_HIGHSCORE
            continue
    #Tetris new high score state
    if game_state == STATE_NEW_HIGHSCORE:
        res = niklas_dll.UTIL_HIGHSCORE_enterName(ctypes.pointer(leader_board), ctypes.pointer(buttons))
        display_css_image(screen, image_array)
        screen.show(mask=display_mask)
        if res == 0:
            iter = 0
            game_state = STATE_SHOW_HIGHSCORE
            continue

    #Tetris game over state
    if game_state == STATE_SHOW_HIGHSCORE:
        niklas_dll.UTIL_HIGHSCORE_showLeaderBoard(ctypes.pointer(leader_board))
        display_css_image(screen, image_array)
        screen.show(mask=display_mask)
        if buttons.buttonLeft==1 or buttons.buttonRight==1 or buttons.buttonCenter==1 or iter > 50:
            #END
            running = False
    time.sleep(0.1)
    print(iter)

niklas_dll.UTIL_TETRIS_qetLedState.restype = ctypes.c_bool;
running = True
while running:
    iter = iter + 1
    niklas_dll.UTIL_TETRIS_update(ctypes.pointer(buttons), ctypes.pointer(output))
    # handle the LED
    led_on = niklas_dll.UTIL_TETRIS_qetLedState()
    print(led_on)
    if led_on:
        # Turn on the led
        screen.showImageMasked(image, Q_LED_ROI)
    else:
        # Turn it off
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
