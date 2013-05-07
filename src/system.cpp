/******************************************************************************
 * Implementationen av samtliga klasser och funktioner definierade i system.h
 *****************************************************************************/

#include <iostream>
#include "system.h"
#include "body.h"
#include "camera.h"
#include "ship.h"
#include "spacebox.h"
#include "VectorUtils3.h"
#include <time.h>
#include "cel_bodies.h"

System::System(){
}


void System::update(float dt)
{
    bodies.update(dt/1000);
    visible.next = f.cull_frustum(bodies.next, c);
}

System::System(int program){
    s = Spacebox("res/spacedome.obj", "res/spacedome.png");
    c = Camera(program);
    bodies = Cel_bodies();

    ship = Ship("res/spaceship.obj", "res/spaceship.png");
}

System::System(int program, int n_planets, int n_suns)
{
    // Sätt fröet för slumpade värden
    srand(time(NULL));
    int p_pos_range = 4*sqrt(n_planets*n_suns);
    int p_vel_range = 2;
    int p_mass_range = 2E6;
    float p_spin_range = 0.1;
    int p_radius_range = 3;

    int s_pos_range = 25*sqrt(n_suns);
    float s_vel_range = 0.1;
    long int s_mass_range = 6E10;
    long int s_mass_min = 1E10;
    int s_spin_range = 1;
    int s_radius_range = 6;

    float rand_value;

    s = Spacebox("res/spacedome.obj", "res/spacedome.png");
    c = Camera(program);
    bodies = Cel_bodies();
    Body *p;
    for(int i=0; i<n_planets; i++){
        rand_value = (float)rand() / (float)RAND_MAX;
        if(rand_value < 0.167) {
            p = new Body("res/planet.obj", "res/earth.png");
        } else if(rand_value < 0.33) {
            p = new Body("res/planet.obj", "res/mars.png");
        } else if(rand_value < 0.5) {
            p = new Body("res/planet.obj", "res/moon.png");
        } else if(rand_value < 0.667) {
            p = new Body("res/planet.obj", "res/venus.png");
        } else if (rand_value < 0.83) {
            p = new Body("res/planet.obj", "res/mars_elevation.png");
        } else {
            p = new Body("res/planet.obj", "res/jupiter.png");
        }

        p->spin_x = (float)rand()/((float)RAND_MAX/p_spin_range) - p_spin_range/2.0;
        p->spin_y = (float)rand()/((float)RAND_MAX/p_spin_range) - p_spin_range/2.0;;
        p->spin_z = (float)rand()/((float)RAND_MAX/p_spin_range) - p_spin_range/2.0;;

        p->mass = rand() % p_mass_range;
        p->set_radius(0.3 + (float)rand()/((float)RAND_MAX/p_radius_range));

        p->position = vec3(
                (float)rand() / ((float)RAND_MAX/p_pos_range) - p_pos_range/2.0,
                (float)rand() / ((float)RAND_MAX/p_pos_range) - p_pos_range/2.0,
                (float)rand() / ((float)RAND_MAX/p_pos_range) - p_pos_range/2.0);

        p->velocity = vec3(
                (float)rand() / ((float)RAND_MAX/p_vel_range) - p_vel_range/2.0,
                (float)rand() / ((float)RAND_MAX/p_vel_range) - p_vel_range/2.0,
                (float)rand() / ((float)RAND_MAX/p_vel_range) - p_vel_range/2.0);
        bodies.add_planet(p);
    }

    for(int i=0; i<n_suns; i++){
        p = new Body("res/bunnyplus.obj", "res/grass.tga");

        p->spin_x = (float)rand()/(float)RAND_MAX/s_spin_range - s_spin_range/2.0;
        p->spin_y = (float)rand()/(float)RAND_MAX/s_spin_range - s_spin_range/2.0;;
        p->spin_z = (float)rand()/(float)RAND_MAX/s_spin_range - s_spin_range/2.0;;

        p->mass = rand() % s_mass_range + s_mass_min;

        p->set_radius(1 + (float)rand()/((float)RAND_MAX/s_radius_range));

        p->position = vec3(
                (float)rand() / ((float)RAND_MAX/s_pos_range) - s_pos_range/2.0,
                (float)rand() / ((float)RAND_MAX/s_pos_range) - s_pos_range/2.0,
                (float)rand() / ((float)RAND_MAX/s_pos_range) - s_pos_range/2.0);

        p->velocity = vec3(
                (float)rand() / ((float)RAND_MAX/s_vel_range) - s_vel_range/2.0,
                (float)rand() / ((float)RAND_MAX/s_vel_range) - s_vel_range/2.0,
                (float)rand() / ((float)RAND_MAX/s_vel_range) - s_vel_range/2.0);
        bodies.add_planet(p);
    }
}

void System::draw(int program)
{
    s.draw(program);
    ship.draw(program);
    Cel_bodies *current = this->visible.next;
    Cel_bodies *next;
    while(current != NULL){
        next = current->next;
        current->planet->draw(program);
        this->visible.remove_planet(current->planet);

        current = next;
    }
}

int System::check_distance(Body *b, int max_distance)
{
    vec3 pos = b->position;
    return ((pos.x * pos.x + pos.y * pos.y + pos.z * pos.z) > max_distance * max_distance);
}

void System::clean(int max_distance)
{
    Cel_bodies *current = bodies.next;
    Cel_bodies *tmp;

    while(current != NULL){
        // Om planet längre bort från origo än max_distance, ta bort den
        if(check_distance(current->planet, max_distance)){
            tmp = current;
            bodies.remove_planet(tmp->planet);
        }
        current = current->next;
    }
}
