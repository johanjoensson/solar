/******************************************************************************
 * Implementationen av samtliga klasser och funktioner definierade i system.h
 *****************************************************************************/

#include <iostream>
#include "system.h"
#include "body.h"
#include "sun.h"
#include "camera.h"
#include "ship.h"
#include "spacebox.h"
#include <time.h>
#include "cel_bodies.h"
#include "loadobj.h"

#include <glm/glm.hpp>

using namespace std;
using namespace glm;

void System::update(float dt)
{
    bodies.update(dt/1000);
    asteroids.update(dt/1000);
    visible.next = f.cull_frustum(bodies.next, c);
    update_collisions();
}

/******************************************************************************
 * Kollar om två kroppar kolliderar.
 *****************************************************************************/
int System::check_collision(Body *p, Body *q)
{
    int collide = 0;     
          
    vec3 diff = q->position - p->position;
    float dsquare = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
    float rsquare = (q->get_radius() + p->get_radius())*(q->get_radius() + p->get_radius());

    if(dsquare <= rsquare){
         collide = 1;
    }

    return collide;
}

/******************************************************************************
 * Uppdaterar kollisioner
 *****************************************************************************/
void System::update_collisions()
{
    Cel_bodies *current = this->bodies.next;
    Body *tmp;
    Cel_bodies *next, *tmp2;
    float r, rcube, mass;
    int collide;
    vec3 v;

    while(current->next != NULL){        
        next = current->next;
        while(next != NULL){

            collide = check_collision(current->planet, next->planet);

            if(collide == 1){          
                //räknar ut massa, radie och hastighet för sammanslagen kroppp
                mass = current->planet->mass + next->planet->mass;                
                rcube = pow(current->planet->get_radius(), 3) + pow(next->planet->get_radius(), 3);            
                r = pow(rcube, (1.0/3));
                v = (current->planet->mass*current->planet->velocity + next->planet->mass*next->planet->velocity)/mass;
                
                //Kollar vilken planet som ska tas bort
                if((current->planet->mass) <= (next->planet->mass)){  
                    tmp = current->planet;
                    current->planet = next->planet;
                    next->planet = tmp;
                }
 
                current->planet->mass = mass;
                current->planet->set_radius(r);
                current->planet->velocity = v;
                 
                tmp2 = next;
                next = next->next;
                bodies.remove_planet(tmp2->planet);
                 
            } else{
                next = next->next;
            }
        }

        if(current->next !=NULL){
            current = current->next;
        }
    }
}

System::System(){
    s = Spacebox("res/spacedome.obj", "res/spacedome.png");
    ship = Ship("res/cat.obj", "res/cat_diff.tga");
    f = Frustum(1, 300, 1, -1, -1, 1);
    bodies = Cel_bodies();
}

System::System(int program) : System()
{
    c = Camera(program);
    Model *model = LoadModelPlus((char*)"res/planet.obj");

    Body *a = new Body(model, "res/mercurymap.png");
    Sun *s = new Sun(model, "res/sunmap.png");

    s->specularExponent = 14;
    s->set_scale(10);
    s->mass = 5E10;
    s->emit_color = vec3(1,1,1);
    s->position = vec3(0,0,-2);
         
    bodies.add_planet(s);

    a->spin_x = 1;
    a->position = vec3(15.0, 0.0, -2.0);
    a->velocity = vec3(0, 0.0, 1.4);
    a->set_scale(1);
    a->mass = 2E2;
    bodies.add_planet(a);
    
    a = new Body(model, "res/venus.png");
    a->spin_y = 0.5;
    a->position = vec3(50.0, 0.0, -2.0);
    a->velocity = vec3(0, 0.0, 0.7);
    a->set_scale(1.5);
    a->mass = 1E3;
    bodies.add_planet(a);
    
    a = new Body(model, "res/earth.png");
    a->spin_y = -0.2;
    a->position = vec3(65.0, 0.0, -2.0);
    a->velocity = vec3(0, 0.0, -0.7);
    a->set_scale(1.8);
    a->mass = 4E3;
    bodies.add_planet(a);
    
    a = new Body(model, "res/mars.png");
    a->spin_y = 0.1;
    a->position = vec3(75.0, 0.0, -2.0);
    a->velocity = vec3(0, 0.0, 0.7);
    a->set_scale(1.4);
    a->mass = 2E3;
    bodies.add_planet(a);
    
    a = new Body(model, "res/jupiter.png");
    a->spin_y = -0.1;
    a->position = vec3(-105.0, 0.0, -2.0);
    a->velocity = vec3(0, 0.0, -0.6);
    a->set_scale(6);
    a->mass = 3E5;
    bodies.add_planet(a);
    
    a = new Body(model, "res/saturnmap.png");
    a->spin_y = 0.3;
    a->position = vec3(125.0, 0.0, -2.0);
    a->velocity = vec3(0, 0.0, -0.57);
    a->set_scale(4);
    a->mass = 3E4;
    bodies.add_planet(a);
    
    a = new Body(model, "res/uranusmap.png");
    a->spin_y = 0.1;
    a->position = vec3(-140.0, 0.0, -2.0);
    a->velocity = vec3(0, 0.0, 0.55);
    a->set_scale(3);
    a->mass = 1E4;
    bodies.add_planet(a);
    
    a = new Body(model, "res/neptunemap.png");
    a->spin_y = 0.23;
    a->position = vec3(155.0, 0.0, -2.0);
    a->velocity = vec3(0, 0.0, -0.53);
    a->set_scale(3);
    a->mass = 1E4;
    bodies.add_planet(a);
}

System::System(int program, int n_planets, int n_suns, int n_asteroids, long p_mass_range, long s_mass_range, float p_vel_range, int p_pos_range_in) :
    System()
{
    c = Camera(program);

    // Sätt fröet för slumpade värden
    srand(time(NULL));
    int p_pos_range = 4*sqrt(n_planets*(n_suns + 1));
    if(p_pos_range_in) {
        p_pos_range = p_pos_range_in;
    } 
    //int p_vel_range = 2;
    float p_spin_range = 0.1;

    //int s_pos_range = 25*sqrt(n_suns);
    float s_vel_range = 0.1;
    long int s_mass_min = 1E10;
    int s_spin_range = 1;

    float rand_value;

    Body *p;
    Model *model = LoadModelPlus((char*)"res/planet.obj");
    for(int i=0; i<n_planets; i++){
        rand_value = (float)rand() / (float)RAND_MAX;
        if(rand_value < 0.09) {
            p = new Body(model, "res/earth.png");
        } else if(rand_value < 0.18) {
            p = new Body(model, "res/mars.png");
        } else if(rand_value < 0.27) {
            p = new Body(model, "res/moon.png");
        } else if(rand_value < 0.36) {
            p = new Body(model, "res/venus.png");
        } else if (rand_value < 0.45) {
            p = new Body(model, "res/mars_elevation.png");
        } else if (rand_value < 0.55) {
            p = new Body(model, "res/saturnmap.png");
        } else if (rand_value < 0.64) {
            p = new Body(model, "res/uranusmap.png");
        } else if (rand_value < 0.73) {
            p = new Body(model, "res/neptunemap.png");
        } else if (rand_value < 0.82) {
            p = new Body(model, "res/plutomap1k.png");
        } else if (rand_value < 0.91) {
            p = new Body(model, "res/mercurymap.png");
        } else {
            p = new Body(model, "res/jupiter.png");
        }

        p->spin_x = (float)rand()/((float)RAND_MAX/p_spin_range) - p_spin_range/2.0;
        p->spin_y = (float)rand()/((float)RAND_MAX/p_spin_range) - p_spin_range/2.0;
        p->spin_z = (float)rand()/((float)RAND_MAX/p_spin_range) - p_spin_range/2.0;

        p->mass = rand() % p_mass_range;
        p->set_radius((1 + pow(3.0*p->mass/(4*M_PI), 1.0/3)/10)/8);

        p->position = vec3(
                (float)rand() / ((float)RAND_MAX/p_pos_range) - p_pos_range/2.0,
                (float)rand() / ((float)RAND_MAX/p_pos_range) - p_pos_range/2.0,
                (float)rand() / ((float)RAND_MAX/p_pos_range) - p_pos_range/2.0);

        p->velocity = vec3(
                rand() / (RAND_MAX/p_vel_range) - p_vel_range/2.0,
                rand() / (RAND_MAX/p_vel_range) - p_vel_range/2.0,
                rand() / (RAND_MAX/p_vel_range) - p_vel_range/2.0);
        bodies.add_planet(p);
    }

    // Kan bara ljussätta en sol, så vi kan bara ha en eller ingen sol. Men 
    // lämnar kvar det här för en tid när vi kan ha flera strålande solar :)
    Sun *s;
    for(int i=0; i<n_suns; i++){
        s = new Sun(model, "res/sunmap.png");
        s->emit_color = vec3(1,1,1);
        s->specularExponent = 5;

        s->spin_x = (float)rand()/(float)RAND_MAX/s_spin_range - s_spin_range/2.0;
        s->spin_y = (float)rand()/(float)RAND_MAX/s_spin_range - s_spin_range/2.0;
        s->spin_z = (float)rand()/(float)RAND_MAX/s_spin_range - s_spin_range/2.0;

        s->mass = rand() % s_mass_range + s_mass_min;

        s->set_radius((1 + pow(3.0*s->mass/(4*M_PI), 1.0/3)/30)/2);

        s->position = vec3(0,0,0);

        s->velocity = vec3(
                (float)rand() / ((float)RAND_MAX/s_vel_range) - s_vel_range/2.0,
                (float)rand() / ((float)RAND_MAX/s_vel_range) - s_vel_range/2.0,
                (float)rand() / ((float)RAND_MAX/s_vel_range) - s_vel_range/2.0);

        bodies.add_planet(s);
    }
    asteroids = Planetoids(n_asteroids, 90, "res/asteroid.obj", "res/asteroid.tga", "src/solar.vert", "src/solar.frag");
}

void System::draw(int program)
{
    s.draw(program);
    /* ship.draw(program); */
    Cel_bodies *current = this->visible.next;
    Cel_bodies *next;
   
    while(current != NULL){
        next = current->next;
        current->planet->draw(program);
        this->visible.remove_planet(current->planet);

        current = next;
    }
    asteroids.draw();
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
