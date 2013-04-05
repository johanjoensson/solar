#include <iostream>
#include "system.h"

System::System(){
    bodies.next = NULL;
}

void System::Cel_bodies::add_planet(Body *p)
{
    Cel_bodies *tmp = new Cel_bodies;
    tmp->planet = *p;
    tmp->next = NULL;

    this->next = tmp;
    std::cout << "mass: " << tmp->planet.mass << std::endl << "radius: " << tmp->planet.radius << std::endl;

}

void System::Cel_bodies::remove_planet(Body *p)
{
    Cel_bodies *current = this;
    Cel_bodies *prev, *next;
    next = current->next;
    
    while(next != NULL){
        if(&current->planet == p){
            prev->next = next;
            delete[] current;
            return;
        }
        std::cout << "Radera planeten i listan." << std::endl;
        prev = current;
        current = next;
        next = current->next;
    }

    delete[] current;
}

void System::Cel_bodies::update()
{
    std::cout << "Hejsan!" << std::endl;
    Cel_bodies *tmp = this;
    while(tmp != NULL){
        std::cout << "Planetens massa är: " << tmp->planet.mass << std::endl;
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
