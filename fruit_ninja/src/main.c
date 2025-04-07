#include <app.h>
#include <stdio.h>

/**
 * Main function
 */
int main(int argc, char* argv[])
{
    App app;
	argc=argc;
	argv=argv;
	app.width=1920;
	app.heigth=1080;
    init_app(&app, app.width, app.heigth);
    while (app.is_running) {
        handle_app_events(&app);
        update_app(&app);
        render_app(&app);
    }
    destroy_app(&app);

    return 0;
}
