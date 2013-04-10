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
#if 0
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
#endif
}

vec3 System::Cel_bodies::rk4_accel(float h, vec3 k, Cel_bodies *universe)
{
    // No gravity in empty space
    // We ignore Casimir forces etc.
    if(universe->next == NULL){
        return vec3(0.0f, 0.0f, 0.0f);
    }
    Cel_bodies *tmp = this;
    vec3 rn = this->planet->position;
    vec3 rp_i, a;
    float rp_icu;
    a = vec3(0.0, 0.0, 0.0);
    float Mi;

    double G = 6.6738480E-7 ;//E-11f;

    while(tmp != NULL){
        if(tmp->planet != this->planet){
            rp_i = tmp->planet->position - (rn + h*k);
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

    this->planet->position = res_r;
    this->planet->velocity = res_v;
}

vec3 System::Cel_bodies::acceleration(Cel_bodies *second, float h, vec3 first_k, vec3 second_k)
{
    Cel_bodies *first = this->next;
    vec3 rn = first->planet->position + h*first_k;
    vec3 rp_i, a;
    float rp_icu, rp_i_val;
    a = vec3(0.0, 0.0, 0.0);
    float Mi;

    double G = 6.6738480E-7 ;//E-11f;

    rp_i = second->planet->position + h*second_k - rn;
    rp_i_val = Norm(rp_i);
    Mi = second->planet->mass;
    rp_icu = rp_i_val*rp_i_val*rp_i_val;

    a = G*Mi*rp_i/rp_icu;

    std::cout << "Acceleration: " << a.x << ", " << a.y << ", " << a.z << std::endl;

    return a;

}

void System::Cel_bodies::calculate_k1()
{
    Cel_bodies *current = this->next;
    std::cout << "Calculate k1!" << std::endl;
    
    vec3 a, zero;
    zero = vec3(0.0, 0.0, 0.0);
    while(current != NULL){
        a = this->acceleration(current, 0.0, zero, zero);
        this->planet->kv1 = this->planet->kv1 + a;
        current->planet->kv1 = current->planet->kv1 - a ; 


        current = current->next;
    }
    this->planet->kr1 = this->planet->velocity;

}

void System::Cel_bodies::calculate_k2(float h)
{
    Cel_bodies *current = this->next;
    std::cout << "Calculate k2!" << std::endl;
    
    vec3 a;
    while(current != NULL){
        a = this->acceleration(current, h, this->planet->kv1, current->planet->kv1);
        this->planet->kv2 = this->planet->kv2 + a;
        current->planet->kv2 = current->planet->kv2 - a ; 


        current = current->next;
    }
    this->planet->kr2 = this->planet->velocity + h*this->planet->kv1;

}

void System::Cel_bodies::calculate_k3(float h)
{
    Cel_bodies *current = this->next;
    std::cout << "Calculate k3!" << std::endl;
    
    vec3 a;
    while(current != NULL){
        a = this->acceleration(current, h, this->planet->kv2, current->planet->kv2);
        this->planet->kv3 = this->planet->kv3 + a;
        current->planet->kv3 = current->planet->kv3 - a ; 

        current = current->next;
    }
    this->planet->kr3 = this->planet->velocity + h*this->planet->kv2;

}

void System::Cel_bodies::calculate_k4(float h)
{
    Cel_bodies *current = this->next;
    std::cout << "Calculate k4!" << std::endl;
    
    vec3 a;
    while(current != NULL){
        a = this->acceleration(current, h, this->planet->kv3, current->planet->kv3);
        this->planet->kv4 = this->planet->kv4 + a;
        current->planet->kv4 = current->planet->kv4 - a ; 

        current = current->next;
    }

    this->planet->kr4 = this->planet->velocity + h*this->planet->kv3;

}

void System::Cel_bodies::reset_k()
{
    vec3 zero = vec3(0.0, 0.0, 0.0);
    this->planet->kv1 = zero;
    this->planet->kv2 = zero;
    this->planet->kv3 = zero;
    this->planet->kv4 = zero;
    
    this->planet->kr1 = zero;
    this->planet->kr2 = zero;
    this->planet->kr3 = zero;
    this->planet->kr4 = zero;

}

void System::Cel_bodies::update_gravity(float h)
{
    Cel_bodies *current = this;
    vec3 new_pos, new_vel;
    while(current != NULL){
        std::cout << "k1: " << current->planet->kv1.x << ", " << current->planet->kv1.y << ", " << current->planet->kv1.z << "\t";
        std::cout  << current->planet->kr1.x << ", " << current->planet->kr1.y << ", " << current->planet->kr1.z << "\t" << std::endl;

        std::cout << "k2: " << current->planet->kv2.x << ", " << current->planet->kv2.y << ", " << current->planet->kv2.z << "\t";
        std::cout  << current->planet->kr2.x << ", " << current->planet->kr2.y << ", " << current->planet->kr2.z << "\t" << std::endl;

        std::cout << "k3: " << current->planet->kv3.x << ", " << current->planet->kv3.y << ", " << current->planet->kv3.z << "\t";
        std::cout  << current->planet->kr3.x << ", " << current->planet->kr3.y << ", " << current->planet->kr3.z << "\t" << std::endl;

        std::cout << "k4: " << current->planet->kv4.x << ", " << current->planet->kv4.y << ", " << current->planet->kv4.z << "\t";
        std::cout  << current->planet->kr4.x << ", " << current->planet->kr4.y << ", " << current->planet->kr4.z << "\t" << std::endl;



        new_vel = current->planet->velocity + h/6*(current->planet->kv1 + 2*current->planet->kv2 + 2*current->planet->kv3 + current->planet->kv4);
        current->planet->velocity = new_vel;

        new_pos = current->planet->position + h/6*(current->planet->kr1 + 2*current->planet->kr2 + 2*current->planet->kr3 + current->planet->kr4);
        current->planet->position = new_pos;

        current->reset_k();
        current->planet->place(current->planet->position);

        current = current->next;
    }
}
void System::Cel_bodies::calculate_slopes(float dt)
{
    Cel_bodies *current = this;
    std::cout << "Calculate slopes!" << std::endl;
    while(current != NULL){
        current->calculate_k1();
        current->calculate_k2(dt/2);
        current->calculate_k3(dt/2);
        current->calculate_k4(dt);

        current = current->next;
    }
}

void System::Cel_bodies::update(float interval)
{
    std::cout << std::endl << std::endl;;
    if(this->next == NULL){
        return;
    }
#if 0
    Cel_bodies *first = this->next;
    first->calculate_slopes(interval);
    first->update_gravity(interval);
#endif
#if 1
    Cel_bodies *current = this->next;
    while(current != NULL){
        current->rk4_gravity(interval, this);
        current->planet->place(current->planet->position);
        std::cout << "Planetens position är: " << current->planet->position.x << " "  << current->planet->position.y << " " << current->planet->position.z << std::endl;
        std::cout << "Planetens hastighet är: " << current->planet->velocity.x << " "  << current->planet->velocity.y << " "  << current->planet->velocity.z << std::endl;
        current = current->next;
    }
#endif
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
