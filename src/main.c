#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stdbool.h"

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

typedef GLuint gl_shader;

static gl_shader load_shader(const char* vertex_source, const char* fragment_source)
{
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const char**) &vertex_source, NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const char**) &fragment_source, NULL);
    glCompileShader(fragment_shader);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
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

    // Renderer init
    glViewport(0, 0, window_width, window_height);

    // Exercise 1
    #if 0
    float vertices[] = {
        -0.5f, +1.0f, +0.0f, // top left
        +0.0f, +0.0f, +0.0f, // center
        -1.0f, +0.0f, +0.0f, // mid left
        +0.5f, +1.0f, +0.0f, // top right
        +1.0f, +0.0f, +0.0f, // mid right
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,   // first triangle
        1, 3, 4,   // second triangle
    };

    GLuint vbo = 0;
    GLuint ebo = 0;
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(/*attribute location*/ 0, /*floats per vertex*/3, /*data type*/GL_FLOAT, GL_FALSE, /*stride*/3 * sizeof(float), /*offset*/0);
        glEnableVertexAttribArray(/*attribute location*/ 0);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    char* vertex_source = "#version 330 core\n"
                           "layout (location = 0) in vec3 aPos;\n"
                           "\n"
                           "void main()\n"
                           "{\n"
                           "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                           "}\0";

    char* fragment_source = "#version 330 core\n"
                            "out vec4 FragColor;\n"
                            "\n"
                            "void main()\n"
                            "{\n"
                            "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                            "}";

    gl_shader shader_program = load_shader(vertex_source, fragment_source);
    #endif

    // Exercise 2
    #if 1
    char* vertex_source = "#version 330 core\n"
                           "layout (location = 0) in vec3 aPos;\n"
                           "\n"
                           "void main()\n"
                           "{\n"
                           "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                           "}\0";

    char* fragment_source = "#version 330 core\n"
                            "out vec4 FragColor;\n"
                            "\n"
                            "void main()\n"
                            "{\n"
                            "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                            "}";

    gl_shader shader_program = load_shader(vertex_source, fragment_source);

    float vertices1[] = {
        -0.5f, +1.0f, +0.0f, // top left
        +0.0f, +0.0f, +0.0f, // center
        -1.0f, +0.0f, +0.0f, // mid left
        //+0.5f, +1.0f, +0.0f, // top right
        //+1.0f, +0.0f, +0.0f, // mid right
    };

    GLuint vbo1 = 0;
    GLuint vao1 = 0;
    glGenVertexArrays(1, &vao1);
    glBindVertexArray(vao1);
    {
        glGenBuffers(1, &vbo1);
        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
        glBufferData(vbo1, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
    }
    glBindVertexArray(0);

    float vertices2[] = {
        -0.5f, +1.0f, +0.0f, // top left
        +0.0f, +0.0f, +0.0f, // center
        -1.0f, +0.0f, +0.0f, // mid left
        //+0.5f, +1.0f, +0.0f, // top right
        //+1.0f, +0.0f, +0.0f, // mid right
    };

    GLuint vbo1 = 0;
    GLuint vao1 = 0;
    glGenVertexArrays(1, &vao1);
    glBindVertexArray(vao1);
    {
        glGenBuffers(1, &vbo1);
        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
        glBufferData(vbo1, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
    }
    glBindVertexArray(0);
    #endif

    while (!glfwWindowShouldClose(glfw_window))
    {
        glfwSwapBuffers(glfw_window);
        glfwPollEvents();

        // Rendering
        {
            // Exercise 1
            #if 0
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(shader_program);
            glBindVertexArray(vao);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            #endif
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