#include <app.h>
#include <SDL2/SDL_image.h>

void init_app(App *app, int width, int height)
{
    int error_code;
    int inited_loaders;
    ShowCursor(FALSE);
    app->help = true;
    app->gameover = false;
    app->scene.isrunning = false;
    app->is_running = false;
    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0)
    {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
        "Fruit_ninja",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);
    if (app->window == NULL)
    {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0)
    {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL)
    {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);

    init_camera(&(app->camera));
    init_scene(&(app->scene));
    app->is_running = true;
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.8, 0.8, 0.8, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO)
    {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else
    {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -.08, .08,
        -.06, .06,
        .1, 1000);
}
void handle_app_events(App *app)
{
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                app->is_running = false;
                break;
            case SDL_SCANCODE_A:
                set_camera_side_speed(&(app->camera), app->scene.player_speed);
                break;
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), -app->scene.player_speed);
                break;
            case SDL_SCANCODE_W:
                set_camera_speed(&(app->camera), app->scene.player_speed);
                break;
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), -app->scene.player_speed);
                break;
            case SDL_SCANCODE_F1:
                if (app->gameover)
                {
                    app->gameover = !app->gameover;
                    restart(&(app->scene));
                    reset_targets(&(app->scene));
                    app->scene.isrunning = !app->scene.isrunning;
                    break;
                }
                app->help = !app->help;
                app->scene.isrunning = !app->scene.isrunning;
                break;
            case SDL_SCANCODE_KP_PLUS:
                app->scene.light += 0.1;
                break;
            case SDL_SCANCODE_KP_MINUS:
                app->scene.light -= 0.1;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), 0);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (!is_mouse_down)
            {
                if (checktarget(&(app->camera), &(app->scene)))
                {
                    app->gameover = !app->gameover;
                    app->scene.isrunning = false;
                }
            }
            is_mouse_down = true;
            SDL_GetMouseState(&x, &y);

            break;
        case SDL_MOUSEMOTION:
            if (app->scene.isrunning)
            {
                SDL_GetMouseState(&x, &y);
                if (x == (app->width) / 2 && y == (app->heigth) / 2)
                {
                    mouse_x = x;
                    mouse_y = y;
                    break;
                }
                if (mouse_x != 0 && mouse_y != 0)
                    rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);

                SDL_WarpMouseInWindow(app->window, (app->width) / 2, (app->heigth) / 2);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            is_mouse_down = false;
            break;
        case SDL_QUIT:
            app->is_running = false;
            break;
        default:
            break;
        }
    }
}

void update_app(App *app)
{
    double current_time;
    double elapsed_time;
    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;
    if (update_scene(&(app->scene), elapsed_time))
    {
        app->gameover = !app->gameover;
        app->scene.isrunning = false;
    }
    update_camera(&(app->camera), elapsed_time);
}

void render_app(App *app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, app->scene.textures.middle_dot);
    middle_dot();
    if (app->help)
    {
        glBindTexture(GL_TEXTURE_2D, app->scene.textures.f1_panel);
        f1_panel();
    }
    if (app->gameover)
    {
        glBindTexture(GL_TEXTURE_2D, app->scene.textures.gameoverscreen);
        f1_panel();
        int number_of_digits = app->scene.points > 0 ? (int)log10((double)app->scene.points) : 0;
        DrawNumber(app->scene.points, &(app->scene), 0, (-1 + number_of_digits));
    }
    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App *app)
{
    if (app->gl_context != NULL)
    {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL)
    {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}
