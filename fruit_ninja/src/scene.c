#include <scene.h>
#include <GL/gl.h>
#include <obj/model.h>
#include <obj/load.h>
#include <obj/draw.h>
#include <texture.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

void init_scene(Scene* scene)
{
	srand(time(0));
	scene->max_targets=3;
	scene->difficulty_increase=50;
	restart(scene);
	
	load_model(&(scene->model), "assets/models/sphere.obj");
	
	scene->textures.object_textures=(GLuint*)malloc(4*sizeof(GLuint));
	scene->targets=(Targets*)malloc(scene->max_targets*sizeof(Targets));
	reset_targets(scene);
	
	 for (int i = 1; i < 4; i++) {
        char path[50];
        snprintf(path, sizeof(path), "assets/textures/type_%d.jpg", i);
        scene->textures.object_textures[i] = load_texture(path);
    }
	scene->textures.object_textures[0]=load_texture("assets/textures/enemy.jpg");
	scene->textures.number_textures=(GLuint*)malloc(10*sizeof(GLuint));
	for (int i = 0; i < 10; i++) {
        char path[50];
        snprintf(path, sizeof(path), "assets/textures/%d.jpg", i);
        scene->textures.number_textures[i] = load_texture(path);
    }
	load_model(&(scene->map_model), "assets/models/house.obj");
	scene->textures.map_texture = load_texture("assets/textures/house.jpg");
	
	scene->textures.f1_panel= load_texture("assets/textures/help.jpg");
	scene->textures.middle_dot= load_texture("assets/textures/middle_dot.png");
	scene->textures.gameoverscreen= load_texture("assets/textures/gameover.jpg");
	scene->light=0.8f;
	scene->player_speed=6.0f;
    scene->material = (Material) {
        .ambient = {1.0, 1.0, 1.0},
        .diffuse = {1.0, 1.0, 0.0},
        .specular = {0.0, 0.0, 0.0},
        .shininess = 0.0
    };
}

void restart(Scene* scene){
	scene->isrunning=false;
	scene->current_targets=1;
	scene->points=0;
	scene->enemy_chance=0.0;
}

void reset_targets(Scene* scene){
	for(int i=0;i<scene->max_targets;i++)
		scene->targets[i].texture_type=-1;
}

void set_lighting(Scene* scene) {
    float light_color[] = {scene->light, scene->light, scene->light, 1.0f};
    float position[] = {0.0f, 0.0f, 10.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material* material) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (float*)&material->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (float*)&material->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (float*)&material->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);
}

bool update_scene(Scene* scene, double time)
{
	if(!scene->isrunning) return false;
	for(int i=0;i<scene->current_targets;i++)
		if(scene->targets[i].texture_type==-1){
			if(rand()%101<scene->enemy_chance) {
				scene->targets[i].isenemy=true;
				scene->targets[i].size=0.3;
				scene->targets[i].point=0;
				scene->targets[i].texture_type=0;
			}else{
				scene->targets[i].isenemy=false;
				scene->targets[i].texture_type=rand()%3+1;
				switch(scene->targets[i].texture_type){
					case 1:scene->targets[i].point=1;scene->targets[i].size=0.7;break;
					case 2:scene->targets[i].point=3;scene->targets[i].size=0.35;break;
					case 3:scene->targets[i].point=5;scene->targets[i].size=0.12;break;
				}
			}
			update_target(i,scene);
		}else{
			if(update_target_motion(i,scene,time)) return true;
		}
		return false;
}

bool update_target_motion(int i, Scene* scene,double time){
	scene->targets[i].position.x+=(scene->targets[i].velocity.x/2)*time;
	scene->targets[i].position.z+=(scene->targets[i].velocity.z)*time;
	scene->targets[i].velocity.z-=(7.0*time);
	if(scene->targets[i].position.z<-20.0 && scene->targets[i].texture_type!=0){
		return true;
	}else if(scene->targets[i].position.z<-20.0 && scene->targets[i].texture_type==0){
		scene->targets[i].texture_type=-1;
	}
	return false;
}

void update_target(int i,Scene* scene){
	scene->targets[i].position.x=rand()%5-2;
	scene->targets[i].position.y=8.0;
	scene->targets[i].position.z=-5.0;
	scene->targets[i].velocity.x=0.0;
	scene->targets[i].velocity.y=0.0;
	scene->targets[i].velocity.z=0.0;
	while(scene->targets[i].velocity.x==0.0)
		scene->targets[i].velocity.x=((double)(rand()%13-6));
	while(scene->targets[i].velocity.z==0.0)
		scene->targets[i].velocity.z=((double)(rand()%3+10));
}

void render_scene(Scene* scene)
{
    set_material(&(scene->material));
    set_lighting(scene);
	for(int i=0;i<scene->current_targets;i++){
		glBindTexture(GL_TEXTURE_2D, scene->textures.object_textures[scene->targets[i].texture_type]);
		glPushMatrix();
		glTranslatef(scene->targets[i].position.x,scene->targets[i].position.y,scene->targets[i].position.z);
		glScalef(scene->targets[i].size,scene->targets[i].size,scene->targets[i].size);
		draw_model(&(scene->model));
		glPopMatrix();
	}
	
	 glPushMatrix();
	 glBindTexture(GL_TEXTURE_2D, scene->textures.map_texture);
	 glTranslatef(0.0,0.0,-8.0);
	 glRotatef(90.0,1.0,0.0,0.0);
	 glRotatef(275.0,0.0,1.0,0.0);
	 glTranslatef(15.0,0.0,0.0);
	 glScalef(3.0,3.0,3.0);
	 draw_model(&(scene->map_model));
	 glPopMatrix();
	 DrawNumber(scene->points,scene,14,22);
}

bool checktarget(Camera* camera, Scene* scene){
	vec3 dir=camera_direction(camera);
	for(int i=0;i<scene->current_targets;i++){
		vec3 d = {camera->position.x + dir.x - camera->position.x, camera->position.y + dir.y - camera->position.y, camera->position.z + dir.z - camera->position.z};
		vec3 e = {camera->position.x - scene->targets[i].position.x, camera->position.y - scene->targets[i].position.y, camera->position.z - scene->targets[i].position.z};
		float a = d.x * d.x + d.y * d.y + d.z * d.z;
		float b = 2 * (d.x * e.x + d.y * e.y + d.z * e.z);
		float c = e.x * e.x + e.y * e.y + e.z * e.z - scene->targets[i].size * scene->targets[i].size;
		float discriminant = b * b - 4 * a * c;
		if (discriminant < 0)
			continue;
		if(scene->targets[i].isenemy) return true;
		scene->points+=scene->targets[i].point;
		scene->targets[i].texture_type=-1;
	}
	scene->current_targets=(scene->points/scene->difficulty_increase)+1;
	if(scene->current_targets>scene->max_targets) scene->current_targets=scene->max_targets;
	if(scene->points>scene->difficulty_increase) scene->enemy_chance=10.0;
	if(scene->points>scene->difficulty_increase*2) scene->enemy_chance=30.0;
	return false;
}
void DrawDigit(float x,float y) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);glVertex3f(y-2, x, -30);
    glTexCoord2f(1, 0);glVertex3f(y, x, -30);
    glTexCoord2f(1, 1);glVertex3f(y, x-2, -30);
    glTexCoord2f(0, 1);glVertex3f(y-2, x-2, -30);
    glEnd();

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void DrawNumber(int number,Scene* scene,float x, float y) {
	y=y*-1;
	x=x*-1;
    if (number == 0) {
		glBindTexture(GL_TEXTURE_2D, scene->textures.number_textures[0]);
        DrawDigit(x,y);
        return;
    }
    
    int digits[10];
    int numDigits = 0;

    while (number > 0) {
        digits[numDigits++] = number % 10;
        number /= 10;
    }

    for (int i = numDigits - 1; i >= 0; --i) {
		glBindTexture(GL_TEXTURE_2D, scene->textures.number_textures[digits[i]]);
        DrawDigit(x,y);
        y += 2;
    }
}
