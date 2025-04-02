#ifndef APP_H
#define APP_H
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "camera.h"
#include "scene.h"

#include <SDL2/SDL.h>

#include <GL/gl.h>

#include <stdbool.h>

#define VIEWPORT_RATIO (16.0 / 9.0)
#define VIEWPORT_ASPECT 50.0

typedef struct App
{
    SDL_Window* window;
    SDL_GLContext gl_context;
    bool is_running;
	bool help;
	bool gameover;
    double uptime;
    Camera camera;
    Scene scene;
	int width;
	int heigth;
} App;

/**
 * Initialize the application.
 */
void init_app(App* app, int width, int height);

/**
 * Initialize the OpenGL context.
 */
void init_opengl();

/**
 * Reshape the window.
 */
void reshape(GLsizei width, GLsizei height);

/**
 * Handle the events of the application.
 */
void handle_app_events(App* app);

/**
 * Update the application.
 */
void update_app(App* app);

/**
 * Render the application.
 */
void render_app(App* app);

/**
 * Destroy the application.
 */
void destroy_app(App* app);

#endif /* APP_H */
