#include <iostream>
#include "system.h"

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
    std::cout << "mass: " << tmp->planet->mass << std::endl << "radius: " << tmp->planet->radius << std::endl;

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
//    this->next = NULL;
}

void rk4_gravity(float dt)
{
    
}


void System::Cel_bodies::update()
{
    std::cout << "Hejsan!" << std::endl;
    Cel_bodies *tmp = this->next;
    while(tmp != NULL){
        std::cout << "Planetens massa är: " << tmp->planet->mass << std::endl;
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
