#include "stdio.h"
#include "stdbool.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

typedef enum platform_button_state
{
    KEY_DEFAULT_STATE = 0,
    KEY_PRESSED_DOWN  = 0x1,
    KEY_HOLD_DOWN     = 0x2,
    KEY_RELEASE       = 0x4,
} platform_button_state;

typedef struct platform_input_state
{
    int mouse_x;
    int mouse_y;
    float mouse_scroll_y;
    platform_button_state left_mouse_btn;
    platform_button_state right_mouse_btn;

    platform_button_state keys[GLFW_KEY_MENU + 1];
    bool any_key_pressed;
} platform_input_state;

platform_input_state input_state;

int window_width = 1280;
int window_height = 720;

static void key_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(glfw_window, GLFW_TRUE);
    }

    if (action == GLFW_PRESS && input_state.keys[key] == KEY_DEFAULT_STATE)
    {
        input_state.keys[key] = KEY_PRESSED_DOWN;
        input_state.any_key_pressed = true;
    }

    if (action == GLFW_RELEASE)
    {
        input_state.keys[key] = KEY_RELEASE;
    }
}

static void scroll_callback(GLFWwindow* glfw_window, double xoffset, double yoffset)
{
    input_state.mouse_scroll_y = yoffset;
}

static void mouse_button_callback(GLFWwindow* glfw_window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT  && input_state.left_mouse_btn  == KEY_DEFAULT_STATE) input_state.left_mouse_btn  = KEY_PRESSED_DOWN;
        if (button == GLFW_MOUSE_BUTTON_RIGHT && input_state.right_mouse_btn == KEY_DEFAULT_STATE) input_state.right_mouse_btn = KEY_PRESSED_DOWN;
    }

    if (action == GLFW_RELEASE)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT ) input_state.left_mouse_btn  = KEY_RELEASE;
        if (button == GLFW_MOUSE_BUTTON_RIGHT) input_state.right_mouse_btn = KEY_RELEASE;
    }
}

static void cursor_position_callback(GLFWwindow* glfw_window, double xpos, double ypos)
{
    input_state.mouse_x = (int) xpos;
    input_state.mouse_y = (int) ypos;
}

static void window_size_callback(GLFWwindow* glfw_window, int width, int height)
{
    window_width = width;
    window_height = height;
    glViewport(0, 0, window_width, window_height);
}

int main()
{
    // GLFW init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* glfw_window = glfwCreateWindow(window_width, window_height, "OpenGL experiment", NULL, NULL);
    glfwSetKeyCallback(glfw_window, key_callback);
    glfwSetScrollCallback(glfw_window, scroll_callback);
    glfwSetMouseButtonCallback(glfw_window, mouse_button_callback);
    glfwSetCursorPosCallback(glfw_window, cursor_position_callback);
    glfwSetWindowSizeCallback(glfw_window, window_size_callback);
    glfwMakeContextCurrent(glfw_window);
    gladLoadGL();
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(glfw_window))
    {
        glfwSwapBuffers(glfw_window);
        glfwPollEvents();

        // Rendering
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        // Input handling
        {
            for (int i = 0; i < sizeof(input_state.keys) / sizeof(input_state.keys[0]); i++)
            {
                if (input_state.keys[i] == KEY_RELEASE)
                {
                    input_state.keys[i] = KEY_DEFAULT_STATE;
                }

                if (input_state.keys[i] == KEY_PRESSED_DOWN)
                {
                    input_state.keys[i] = KEY_HOLD_DOWN;
                }
            }

            input_state.any_key_pressed = 0;

            if (input_state.mouse_scroll_y != 0) input_state.mouse_scroll_y = 0;
            if (input_state.left_mouse_btn  == KEY_RELEASE)      input_state.left_mouse_btn  = KEY_DEFAULT_STATE;
            if (input_state.left_mouse_btn  == KEY_PRESSED_DOWN) input_state.left_mouse_btn  = KEY_HOLD_DOWN;
            if (input_state.right_mouse_btn == KEY_RELEASE)      input_state.right_mouse_btn = KEY_DEFAULT_STATE;
            if (input_state.right_mouse_btn == KEY_PRESSED_DOWN) input_state.right_mouse_btn = KEY_HOLD_DOWN;
        }
    }
}