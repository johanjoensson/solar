#include "cel_bodies.h"
#include "body.h"

#include <glm/gtx/norm.hpp>


Cel_bodies::Cel_bodies()
{
    this->next = NULL;
    this->planet = NULL;
}

/******************************************************************************
 * Lägg till objekt först i en länkad lista
 *****************************************************************************/
void Cel_bodies::add_planet(Body *p)
{
    Cel_bodies *tmp = new Cel_bodies;
    tmp->planet = p;
    tmp->next = this->next;

    this->next = tmp;
}

/******************************************************************************
 * Leta reda på planeten i listan och ta bort den från listan och radera.
 *****************************************************************************/
void Cel_bodies::remove_planet(Body *p)
{
    if(this->next == NULL){
        return;
    }

    Cel_bodies *current = this;
    current->planet = NULL;
    Cel_bodies *prev, *next;
    prev = NULL;
    next = current->next;
    
    while(next != NULL){
        if(current->planet == p){
            prev->next = next;
            delete current;
            return;
        }
        prev = current;
        current = next;
        next = current->next;
    }
            
    prev->next = NULL;
    delete current;
}

void Cel_bodies::clear_list()
{
    Cel_bodies *current = this->next;
    Cel_bodies *next;
    while(current != NULL){
        next = current->next;
        delete current;
        current = next;
    }
}

/******************************************************************************
 * Här börjar rk4-integrationen och gravitationsberäkningarna.
 * Here be dragons! Rör inget utan att först ha kollat med mig (Johan Jönsson).
 * Att sätta sig in i koden kan ge huvudvärk och akuta magsmärtor.
 * Du har blivit varnad, forsätt på egen risk.
 *****************************************************************************/

/******************************************************************************
 * Beräkna kraften på objekt first från objekt second
 * f(tn,yn) i rk4
 *****************************************************************************/
glm::vec3 Cel_bodies::force(Cel_bodies *second, float h, glm::vec3 first_k, glm::vec3 second_k)
{
    if(second == NULL){
        return glm::vec3(0.0,0.0,0.0);
    }
    Cel_bodies *first = this;
    glm::vec3 rn = first->planet->position + h*first_k;
    glm::vec3 rp_i, F;
    float rp_icu, rp_i_val;
    
    F = glm::vec3(0.0, 0.0, 0.0);
    float Mi,m;

    float G = 6.6738480E-10 ;//E-11f;

    rp_i = second->planet->position + h*second_k - rn;
    rp_i_val = glm::l2Norm(rp_i);
    Mi = second->planet->mass;
    m = first->planet->mass;
    rp_icu = rp_i_val*rp_i_val*rp_i_val;

    F = G*m*Mi*rp_i/rp_icu;

    return F;

}

/******************************************************************************
 * Beräkna hjälplutningen k1 för samtliga objekt.
 * k1 är egentligen 2 hjälplutningar, för hastighets- och positions-beräkningen
 * Utnyttja Newton III för att undvika dubbla beräkningar
 *****************************************************************************/
void Cel_bodies::calculate_k1()
{
    Cel_bodies *current = this->next;
    
    glm::vec3 F, zero;
    zero = glm::vec3(0.0, 0.0, 0.0);
    while(current != NULL){
        F = force(current, 0.0, zero, zero);
        this->planet->kv1 = this->planet->kv1 + F/this->planet->mass;
        current->planet->kv1 = current->planet->kv1 - F/current->planet->mass; 


        current = current->next;
    }
    this->planet->kr1 = this->planet->velocity;

}

/******************************************************************************
 * Beräkna hjälplutning k2 för samtliga objekt.
 * Utnyttja Newton III för att undvika dubbla beräkningar
 *****************************************************************************/
void Cel_bodies::calculate_k2(float h)
{
    Cel_bodies *current = this->next;
    
    glm::vec3 F;
    while(current != NULL){
        F = this->force(current, h, this->planet->kv1, current->planet->kv1);
        this->planet->kv2 = this->planet->kv2 + F/this->planet->mass;
        current->planet->kv2 = current->planet->kv2 - F/current->planet->mass ; 


        current = current->next;
    }
    this->planet->kr2 = this->planet->velocity + h*this->planet->kv1;

}

/******************************************************************************
 * Beräkna hjälplutning k3 för samtliga objekt.
 * Utnyttja Newton III för att undvika dubbla beräkningar
 *****************************************************************************/
void Cel_bodies::calculate_k3(float h)
{
    Cel_bodies *current = this->next;
    
    glm::vec3 F;
    while(current != NULL){
        F = this->force(current, h, this->planet->kv2, current->planet->kv2);
        this->planet->kv3 = this->planet->kv3 + F/this->planet->mass;
        current->planet->kv3 = current->planet->kv3 - F/current->planet->mass ; 

        current = current->next;
    }
    this->planet->kr3 = this->planet->velocity + h*this->planet->kv2;

}

/******************************************************************************
 * Beräkna hjälplutning k4 för samtliga objekt.
 * Utnyttja Newton III för att undvika dubbla beräkningar
 *****************************************************************************/
void Cel_bodies::calculate_k4(float h)
{
    Cel_bodies *current = this->next;
    
    glm::vec3 F;
    while(current != NULL){
        F = this->force(current, h, this->planet->kv3, current->planet->kv3);
        this->planet->kv4 = this->planet->kv4 + F/this->planet->mass;
        current->planet->kv4 = current->planet->kv4 - F/current->planet->mass ; 

        current = current->next;
    }

    this->planet->kr4 = this->planet->velocity + h*this->planet->kv3;

}

/******************************************************************************
 * Nollställ hjälplutnignarna på objektet
 *****************************************************************************/
void Cel_bodies::reset_k()
{
    glm::vec3 zero = glm::vec3(0.0, 0.0, 0.0);
    this->planet->kv1 = zero;
    this->planet->kv2 = zero;
    this->planet->kv3 = zero;
    this->planet->kv4 = zero;
    
    this->planet->kr1 = zero;
    this->planet->kr2 = zero;
    this->planet->kr3 = zero;
    this->planet->kr4 = zero;

}

/******************************************************************************
 * rk4-integrationen sker här. Beräkna ny position och ny hastighet.
 * Uppdatera objektens positions- och hastighets-fält samt nollställ 
 * hjälplutningarna
 *****************************************************************************/
void Cel_bodies::update_gravity(float h)
{
    Cel_bodies *current = this;
    glm::vec3 new_pos, new_vel;
    while(current != NULL){

        new_vel = current->planet->velocity + h/6*(current->planet->kv1 + float(2)*current->planet->kv2 + float(2)*current->planet->kv3 + current->planet->kv4);

        current->planet->velocity = new_vel;
        new_pos = current->planet->position + h/6*(current->planet->kr1 + float(2)*current->planet->kr2 + float(2)*current->planet->kr3 + current->planet->kr4);
        current->planet->position = new_pos;

        current->reset_k();
        /*HÄR HÄNDER NÅGOT MYSKO! */
        current->planet->place(current->planet->position);

        current = current->next;
    }
}

/******************************************************************************
 * Starta beräkningen av samtliga hjälplutningar
 *****************************************************************************/
void Cel_bodies::calculate_slopes(float dt)
{
    Cel_bodies *current = this;
    while(current != NULL){
        current->calculate_k1();
        current->calculate_k2(dt/2);
        current->calculate_k3(dt/2);
        current->calculate_k4(dt);

        current = current->next;
    }
}

/******************************************************************************
 * Uppdatera samtliga objekt i världen med nya hastigheter och positioner.
 *****************************************************************************/
void Cel_bodies::update(float dt)
{
    if(this->next == NULL){
        return;
    }
    Cel_bodies *current = this->next;
    current->calculate_slopes(dt);
    current->update_gravity(dt);

    current = this->next;
    while(current != NULL){
        current->planet->update(dt);

        current = current->next;
    }
}
