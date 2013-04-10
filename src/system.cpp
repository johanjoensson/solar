/******************************************************************************
 * Implementationen av samtliga klasser och funktioner definierade i system.h
 *****************************************************************************/

#include <iostream>
#include "system.h"
#include "body.h"
#include "camera.h"
#include "spacebox.h"
#include "VectorUtils3.h"

System::System(){
}

System::Cel_bodies::Cel_bodies()
{
    this->next = NULL;
    
}

/******************************************************************************
 * Lägg till objekt först i en länkad lista
 *****************************************************************************/
void System::Cel_bodies::add_planet(Body *p)
{
    Cel_bodies *tmp = new Cel_bodies;
    tmp->planet = p;
    tmp->next = this->next;

    this->next = tmp;

}

/******************************************************************************
 * Leta reda på planeten i listan och ta bort den från listan och radera.
 *****************************************************************************/
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
            
    prev->next = NULL;
    delete[] current;
}

/* TODO */
System::Cel_bodies::~Cel_bodies()
{
    return;
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


/******************************************************************************
 * Räkna ut hastigheten på ett objekt utifrån dess acceleration.
 * f(tn, yn) i rk4 (vi gör 2 separata rk4-integrationer)
 *****************************************************************************/
vec3 System::Cel_bodies::rk4_velocity(float h, vec3 acc)
{
    vec3 v0 = this->planet->velocity;
    return v0 + acc*h;
;
}

/******************************************************************************
 * Genomför 2 separata rk4-integrationer.
 * Den första ger hastigheten utifrån accelerationen.
 * Den andra ger positionen utifrån hastigheten.
 *****************************************************************************/
void System::Cel_bodies::rk4_gravity(float dt, Cel_bodies *universe)
{
    vec3 res_r, res_v;
    vec3 rn = this->planet->position;
    vec3 vn = this->planet->velocity;
    // Intermediary slopes for position
    vec3 kr1, kr2, kr3, kr4;
    // Intermediary slopes for velocity
    vec3 kv1, kv2, kv3, kv4;

    /**************************************************************************
     * Hjälptlutningarna för hastighetsberäkningen
     *************************************************************************/
    kv1 = rk4_accel(0.0f, vec3(), universe);
    kv2 = rk4_accel(dt/2.0f, kv1, universe);
    kv3 = rk4_accel(dt/2.0f, kv2, universe);
    kv4 = rk4_accel(dt, kv3, universe);

    /**************************************************************************
     * Hjälptlutningarna för positionsberäkningen.
     * Beror på accelerationsuppskattningarna ovan.
     *************************************************************************/
    kr1 = rk4_velocity(0.0f, vec3(0.0, 0.0, 0.0));
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
vec3 System::Cel_bodies::force(Cel_bodies *second, float h, vec3 first_k, vec3 second_k)
{
    if(second == NULL){
        return vec3(0.0,0.0,0.0);
    }
    Cel_bodies *first = this;
    vec3 rn = first->planet->position + h*first_k;
    vec3 rp_i, F;
    float rp_icu, rp_i_val;
    
    F = vec3(0.0, 0.0, 0.0);
    float Mi,m;

    double G = 6.6738480E-11 ;//E-11f;

    rp_i = second->planet->position + h*second_k - rn;
    rp_i_val = Norm(rp_i);
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
void System::Cel_bodies::calculate_k1()
{
    Cel_bodies *current = this->next;
    
    vec3 F, zero;
    zero = vec3(0.0, 0.0, 0.0);
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
void System::Cel_bodies::calculate_k2(float h)
{
    Cel_bodies *current = this->next;
    
    vec3 F;
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
void System::Cel_bodies::calculate_k3(float h)
{
    Cel_bodies *current = this->next;
    
    vec3 F;
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
void System::Cel_bodies::calculate_k4(float h)
{
    Cel_bodies *current = this->next;
    
    vec3 F;
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

/******************************************************************************
 * rk4-integrationen sker här. Beräkna ny position och ny hastighet.
 * Uppdatera objektens positions- och hastighets-fält samt nollställ 
 * hjälplutningarna
 *****************************************************************************/
void System::Cel_bodies::update_gravity(float h)
{
    Cel_bodies *current = this;
    vec3 new_pos, new_vel;
    while(current != NULL){

        new_vel = current->planet->velocity + h/6*(current->planet->kv1 + 2*current->planet->kv2 + 2*current->planet->kv3 + current->planet->kv4);

        current->planet->velocity = new_vel;
        new_pos = current->planet->position + h/6*(current->planet->kr1 + 2*current->planet->kr2 + 2*current->planet->kr3 + current->planet->kr4);
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
void System::Cel_bodies::calculate_slopes(float dt)
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
void System::Cel_bodies::update(float dt)
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
    p->spin_y = 1;
    p->position = vec3(0.0, 0.0, -2.0);
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
