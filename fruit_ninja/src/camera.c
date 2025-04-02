#include "camera.h"

#include <GL/gl.h>
#include <stdio.h>
#include <math.h>

void init_camera(Camera* camera)
{
    camera->position.x = 0.0;
    camera->position.y = 0.0;
    camera->position.z = 1.0;
    camera->rotation.x = 0.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = 90.0;
    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;
}

void update_camera(Camera* camera, double time)
{
    double angle;
    double side_angle;
    angle = degree_to_radian(camera->rotation.z);
    side_angle = degree_to_radian(camera->rotation.z + 90.0);

    camera->position.x += cos(angle) * camera->speed.y * time;
    camera->position.y += sin(angle) * camera->speed.y * time;
    camera->position.x += cos(side_angle) * camera->speed.x * time;
    camera->position.y += sin(side_angle) * camera->speed.x * time;
	
	if(camera->position.x>5.0){
		camera->position.x=5.0;
	}
	if(camera->position.x<-5.0){
		camera->position.x=-5.0;
	}
	if(camera->position.y>4.0){
		camera->position.y=4.0;
	}
	if(camera->position.y<-35.0){
		camera->position.y=-35.0;
	}
}

void set_view(const Camera* camera)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(camera->rotation.x + 90), 1.0, 0, 0);
    glRotatef(-(camera->rotation.z - 90), 0, 0, 1.0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}

void rotate_camera(Camera* camera, double horizontal, double vertical)
{
    camera->rotation.z += horizontal/10;
    camera->rotation.x += vertical/10;

    if (camera->rotation.z < 0) {
        camera->rotation.z += 360.0;
    }

    if (camera->rotation.z > 360.0) {
        camera->rotation.z -= 360.0;
    }
	
	if (camera->rotation.x < -90.0) {
        camera->rotation.x = -90.0;
    }

    if (camera->rotation.x > 90.0) {
        camera->rotation.x =90.0;
    }
}

void set_camera_speed(Camera* camera, double speed)
{
    camera->speed.y = speed;
}

void set_camera_side_speed(Camera* camera, double speed)
{
    camera->speed.x = speed;
}


void f1_panel()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-1, 1, -3);
    glTexCoord2f(1, 0);
    glVertex3f(1, 1, -3);
    glTexCoord2f(1, 1);
    glVertex3f(1, -1, -3);
    glTexCoord2f(0, 1);
    glVertex3f(-1, -1, -3);
    glEnd();

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}
void middle_dot()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-3, 3, -1000);
    glTexCoord2f(1, 0);
    glVertex3f(3, 3, -1000);
    glTexCoord2f(1, 1);
    glVertex3f(3, -3, -1000);
    glTexCoord2f(0, 1);
    glVertex3f(-3, -3, -1000);
    glEnd();
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

vec3 camera_direction(Camera *camera)
{
    vec3 dir = {cos(degree_to_radian(camera->rotation.x)) *
                 cos(degree_to_radian(camera->rotation.z)), 
                 sin(degree_to_radian(camera->rotation.z)) *
                 cos(degree_to_radian(camera->rotation.x)),
                 sin(degree_to_radian(camera->rotation.x))};
                 
    float w = sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    dir.x /= w;
    dir.y /= w;
    dir.z /= w;
    return dir;
}
