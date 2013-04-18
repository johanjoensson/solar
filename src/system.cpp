/******************************************************************************
 * Implementationen av samtliga klasser och funktioner definierade i system.h
 *****************************************************************************/

#include <iostream>
#include "system.h"
#include "body.h"
#include "camera.h"
#include "spacebox.h"
#include "VectorUtils3.h"
#include "cel_bodies.h"

System::System(){
}


void System::update(float dt)
{
    bodies.update(dt/1000);
}

System::System(int program){
    b = Body("res/bunnyplus.obj", "res/grass.tga");
    s = Spacebox("res/spacedome.obj", "res/spacedome.png");
    b.translate(0,0,-2);
    b.position = vec3(0.0,0.0,-2.0);
    b.spin_y = 1;
    c = Camera(program);
    bodies = Cel_bodies();

    Body *p = new Body("res/bunnyplus.obj", "res/grass.tga");
    Body *q = new Body("res/planet.obj", "res/skyBox512.tga");
    p->spin_y = 1;
    p->mass = 1E10;
    p->position = vec3(0.0, 0.0, -2.0);


    q->spin_x = 1;
    q->mass = 1;
    q->position = vec3(5.0, 0.0, -2.0);
    q->velocity = vec3(0.0, 0.78, 0.0);

    bodies.add_planet(q);
    bodies.add_planet(p);
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
