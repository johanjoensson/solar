/******************************************************************************
 * Implementationen av samtliga klasser och funktioner definierade i system.h
 *****************************************************************************/

#include <iostream>
#include "system.h"
#include "body.h"
#include "camera.h"
#include "spacebox.h"
#include "VectorUtils3.h"
#include <time.h>
#include "cel_bodies.h"

System::System(){
}


void System::update(float dt)
{
    bodies.update(dt/1000);
}

System::System(int program){
    s = Spacebox("res/spacedome.obj", "res/spacedome.png");
    c = Camera(program);
    bodies = Cel_bodies();

    Body *a = new Body("res/planet.obj", "res/grass.tga");
    Body *q = new Body("res/planet.obj", "res/skyBox512.tga");

    a->set_scale(1);
    a->spin_x = 1;
    a->mass = 1;
    a->position = vec3(0.0, 5.0, -2.0);
    a->velocity = vec3(0, 0, 0.0);

    q->set_scale(3);
    q->spin_x = 1;
    q->mass = 1;
    q->position = vec3(5.0, 0.0, -2.0);
    q->velocity = vec3(0.0, 0, 0.0);

    bodies.add_planet(a);
    bodies.add_planet(q);
}

System::System(int program, int n_planets, int n_suns)
{
    // Sätt fröet för slumpade värden
    srand(time(NULL));
    int p_pos_range = 4*sqrt(n_planets*n_suns);
    int p_vel_range = 2;
    int p_mass_range = 2E6;
    int p_spin_range = 3;
    int p_radius_range = 3;

    int s_pos_range = 25*sqrt(n_suns);
    int s_vel_range = 1;
    long int s_mass_range = 6E10;
    long int s_mass_min = 1E10;
    int s_spin_range = 1;
    int s_radius_range = 6;

    s = Spacebox("res/spacedome.obj", "res/spacedome.png");
    c = Camera(program);
    bodies = Cel_bodies();
    Body *p;
    for(int i=0; i<n_planets; i++){
        p = new Body("res/planet.obj", "res/grass.tga");

        p->spin_x = (float)rand()/(float)RAND_MAX/p_spin_range - p_spin_range/2.0;
        p->spin_y = (float)rand()/(float)RAND_MAX/p_spin_range - p_spin_range/2.0;;
        p->spin_z = (float)rand()/(float)RAND_MAX/p_spin_range - p_spin_range/2.0;;

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
    Cel_bodies *current = this->bodies.next;
    while(current != NULL){
        current->planet->draw(program);

        current = current->next;
    }
}
