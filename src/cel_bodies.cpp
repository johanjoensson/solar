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
#ifndef GRAV_OPT
/******************************************************************************
 * 2 olika implementationer av integratorn finns, den första är inte särskilt
 * optimerad, men ganska rättfram och begriplig. Gör n^2-n beräkningar för att
 * räkna ut ny position och hastighet på n objekt.
 *****************************************************************************/

/******************************************************************************
 * Räkna ut ett objekts acceleration utifrån gravitationskrafterna som verkar
 * på objektet. f(tn, yn) i rk4.
 *****************************************************************************/
glm::vec3 Cel_bodies::rk4_accel(float h, glm::vec3 k, Cel_bodies *universe)
{
    // No gravity in empty space
    // We ignore Casimir forces etc.
    if(universe->next == NULL){
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
    Cel_bodies *tmp = this;
    glm::vec3 rn = this->planet->position;
    glm::vec3 rp_i, a;
    float rp_icu;
    a = glm::vec3(0.0, 0.0, 0.0);
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


/******************************************************************************
 * Räkna ut hastigheten på ett objekt utifrån dess acceleration.
 * f(tn, yn) i rk4 (vi gör 2 separata rk4-integrationer)
 *****************************************************************************/
glm::vec3 Cel_bodies::rk4_velocity(float h, glm::vec3 acc)
{
    glm::vec3 v0 = this->planet->velocity;
    return v0 + acc*h;
;
}

/******************************************************************************
 * Genomför 2 separata rk4-integrationer.
 * Den första ger hastigheten utifrån accelerationen.
 * Den andra ger positionen utifrån hastigheten.
 *****************************************************************************/
void Cel_bodies::rk4_gravity(float dt, Cel_bodies *universe)
{
    glm::vec3 res_r, res_v;
    glm::vec3 rn = this->planet->position;
    glm::vec3 vn = this->planet->velocity;
    // Intermediary slopes for position
    glm::vec3 kr1, kr2, kr3, kr4;
    // Intermediary slopes for velocity
    glm::vec3 kv1, kv2, kv3, kv4;

    /**************************************************************************
     * Hjälptlutningarna för hastighetsberäkningen
     *************************************************************************/
    kv1 = rk4_accel(0.0f, glm::vec3(), universe);
    kv2 = rk4_accel(dt/2.0f, kv1, universe);
    kv3 = rk4_accel(dt/2.0f, kv2, universe);
    kv4 = rk4_accel(dt, kv3, universe);

    /**************************************************************************
     * Hjälptlutningarna för positionsberäkningen.
     * Beror på accelerationsuppskattningarna ovan.
     *************************************************************************/
    kr1 = rk4_velocity(0.0f, glm::vec3(0.0, 0.0, 0.0));
    kr2 = rk4_velocity(dt/2.0f, kv1);
    kr3 = rk4_velocity(dt/2.0f, kv2);
    kr4 = rk4_velocity(dt/2.0f, kv3);

    /**************************************************************************
     * Beräkna resultaten som viktade medelvärden.
     *************************************************************************/
    res_r = rn + dt/6.0f*(kr1+2*kr2+2*kr3 + kr4);
    res_v = vn + dt/6.0f*(kv1 + 2*kv2 + 2*kv3 + kv4);

    /**************************************************************************
     * Uppdatera planetens hastighets- och positions-fält
     *************************************************************************/
    this->planet->position = res_r;
    this->planet->velocity = res_v;
}
#else //GRAV_OPT
/******************************************************************************
 * Den andra rk4-integratorn börjar här. Den här är en hel del grötigare.
 * Gör (n(n-3) + 2)/2 beräkningar för n objekt.
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
#endif //GRAV_OPT

/******************************************************************************
 * Uppdatera samtliga objekt i världen med nya hastigheter och positioner.
 *****************************************************************************/
void Cel_bodies::update(float dt)
{
    if(this->next == NULL){
        return;
    }
    Cel_bodies *current = this->next;
#ifdef GRAV_OPT 
    current->calculate_slopes(dt);
    current->update_gravity(dt);
#else // ! GRAV_OPT
    while(current != NULL){
        current->rk4_gravity(dt, this);
        current->planet->place(current->planet->position);
        current = current->next;
    }
#endif

    current = this->next;
    while(current != NULL){
        current->planet->update(dt);

        current = current->next;
    }

}
