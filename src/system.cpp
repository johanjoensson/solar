#include <iostream>
#include "system.h"
#include "VectorUtils3.h"

System::System(){
}

System::Cel_bodies::Cel_bodies()
{
    this->next = NULL;
    
}

void System::Cel_bodies::add_planet(Body *p)
{
    Cel_bodies *tmp = new Cel_bodies;
    tmp->planet = p;
    tmp->next = this->next;

    this->next = tmp;
    std::cout << "mass: " << tmp->planet->mass << std::endl << "position: " << tmp->planet->position.x << " " << tmp->planet->position.y << " " << tmp->planet->position.z << " " << std::endl;

}

void System::Cel_bodies::remove_planet(Body *p)
{
    if(this->next == NULL){
        std::cout << "Inga planeter i listan" << std::endl;
        return;
    }

    Cel_bodies *current = this;
    Cel_bodies *prev, *next;
    next = current->next;
    
    while(next != NULL){
        if(current->planet == p){
            std::cout << "Radera planeten i listan." << std::endl;
            prev->next = next;
            delete[] current;
            return;
        }
        prev = current;
        current = next;
        next = current->next;
    }
            
    std::cout << "Radera sista  planeten i listan." << std::endl;
    prev->next = NULL;
    delete[] current;
}

System::Cel_bodies::~Cel_bodies()
{
    if(this->next == NULL){
        return;
    }
    std::cout << "Innan variabeldeklarationerna" << std::endl;

    Cel_bodies *current = this->next;;
    Cel_bodies *next = current->next;

    std::cout << "Innan loopen" << std::endl;
    while(current != NULL){
        std::cout << "Inuti loopen" << std::endl;
        delete[] current;
        current = next;
        next = current->next;
    }
    this->next = NULL;
}

vec3 System::Cel_bodies::rk4_accel(float h, vec3 k, Cel_bodies *universe)
{
    // No gravity in empty space
    // We ignore Casimir forces etc.
    if(universe->next == NULL){
        return vec3(0.0f, 0.0f, 0.0f);
    }
    Cel_bodies *tmp = universe->next;
    vec3 rn = this->planet->position;
    vec3 rp_i, a;
    float rp_icu;
    a = vec3(0.0, 0.0, 0.0);
    float Mi;

    double G = 6.6738480E-5;//E-11f;

    while(tmp != NULL){
        if(tmp->planet != this->planet){
            rp_i = tmp->planet->position - rn - h*k;
            Mi = tmp->planet->mass;
            rp_icu = Norm(rp_i)*Norm(rp_i)*Norm(rp_i);
            a = a + G*Mi*rp_i/rp_icu;
        }
        tmp = tmp->next;
    }


    return a;
}


vec3 System::Cel_bodies::rk4_velocity(float h, vec3 acc)
{
    vec3 v0 = this->planet->velocity;
    return v0 + acc*h;
;
}

void System::Cel_bodies::rk4_gravity(float dt, Cel_bodies *universe)
{
    vec3 res_r, res_v;
    vec3 rn = this->planet->position;
    vec3 vn = this->planet->velocity;
    // Intermediary slopes for position
    vec3 kr1, kr2, kr3, kr4;
    // Intermediary slopes for velocity
    vec3 kv1, kv2, kv3, kv4;

    kv1 = rk4_accel(0.0f, vec3(), universe);
    kv2 = rk4_accel(dt/2.0f, kv1, universe);
    kv3 = rk4_accel(dt/2.0f, kv2, universe);
    kv4 = rk4_accel(dt, kv3, universe);

    kr1 = rk4_velocity(0.0f, vec3(0.0, 0.0, 0.0));
    kr2 = rk4_velocity(dt/2.0f, kv1);
    kr3 = rk4_velocity(dt/2.0f, kv2);
    kr4 = rk4_velocity(dt/2.0f, kv3);

    res_r = rn + dt/6.0f*(kr1+2*kr2+2*kr3 + kr4);
    res_v = vn + dt/6.0f*(kv1 + 2*kv2 + 2*kv3 + kv4);
//    res_r =  dt/6.0f*(kr1+2*kr2+2*kr3 + kr4);
//    res_v =  dt/6.0f*(kv1 + 2*kv2 + 2*kv3 + kv4);

    this->planet->position = res_r;
    this->planet->velocity = res_v;
}


void System::Cel_bodies::update(float interval)
{
    std::cout << std::endl << std::endl;;
    Cel_bodies *tmp = this->next;
    while(tmp != NULL){
//        std::cout << "Planetens massa är: " << tmp->planet->mass << std::endl;
        tmp->rk4_gravity(interval, this);
        tmp->planet->translate(tmp->planet->position.x,tmp->planet->position.y,tmp->planet->position.z);
        std::cout << "Planetens position är: " << tmp->planet->position.x << " "  << tmp->planet->position.y << " " << tmp->planet->position.z << std::endl;
        std::cout << "Planetens hastighet är: " << tmp->planet->velocity.x << " "  << tmp->planet->velocity.y << " "  << tmp->planet->velocity.z << std::endl;
        tmp = tmp->next;
    }
    return;
}


void System::event_handler(SDL_Event event){
	switch(event.type){
		case SDL_VIDEORESIZE: 
			resize_window(event);
			break;
		case SDL_KEYDOWN:
			handle_keypress(event);
			break;
		case SDL_QUIT:
			exit_prog(0);
			break;
		case SDL_USEREVENT:
			handle_userevent(event);
			break;
		default:
			break;
	}
}
