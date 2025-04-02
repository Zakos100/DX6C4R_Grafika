#ifndef SCENE_H
#define SCENE_H
#include "utils.h"
#include <obj/model.h>
#include "texture.h"
#include <stdlib.h>
#include "camera.h"

typedef struct Targets
{
	vec3 velocity;
	vec3 position;
	double size;
	int point;
	int texture_type;
	bool isenemy;
}Targets;

typedef struct Textures
{
	GLuint* object_textures;
	GLuint map_texture;
	GLuint f1_panel;
	GLuint middle_dot;
	GLuint* number_textures;
	GLuint gameoverscreen;
}Textures;

typedef struct Scene
{
	bool isrunning;
	int max_targets;
	int current_targets;
	int difficulty_increase;
	int points;
	double enemy_chance;
	Targets* targets;
	Model model;
	Model map_model;
	Material material;
	float light;
	double player_speed;
	Textures textures;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting(Scene* scene);

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Render the scene objects.
 */
void render_scene(Scene* scene);


/**
 * Update the scene objects.
 */
bool update_scene(Scene* scene, double time);

/**
 * Update the target;
 */
void update_target(int i,Scene* scene);

/**
 * Update the target motion;
 */
bool update_target_motion(int i, Scene* scene,double time);
/**
 * Check target hit.
 * Return: true, if it is a bomb. false othervise
 */
bool checktarget(Camera* camera, Scene* scene);

/**
 * Restart the game.
 */
void  restart(Scene* scene);

/**
 * Resets the targets.
 */
void  reset_targets(Scene* scene);
/**
 * Draw digit to hud.
 */
void DrawDigit(float y,float x);

/**
 * Helper function for DrawDigit.
 */
void DrawNumber(int number,Scene* scene,float x, float y);
#endif /* SCENE_H */
