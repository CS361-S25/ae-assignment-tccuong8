#ifndef PLANT_H
#define PLANT_H

#include "Org.h"
#include "emp/math/math.hpp"

class Plant : public Organism
{
    double synthesis;
    double birth_cost;
    int species;

public:
    Plant(emp::Ptr<emp::Random> _random, double _points = 100.0, double _synthesis = 100, double _birth_cost = 100) : Organism(_random, _points), synthesis(_synthesis), birth_cost(_birth_cost), species(3) { ; }

    // traits
    int GetSpecies() { return species; }
    std::string GetColor() { return "green"; }

    /**
     * Takes in nothing.
     * Plants synthesize energy.
     * Returns nothing.
     */
    void Process()
    {
        this->AddPoints(synthesis);
    }

    /**
     * Takes in a pointer to an Organism that's the target of interaction.
     * Plants will share and average out their energy.
     * Plants are eeaten by Humans and Zombies.
     * Returns nothing.
     */
    void Interact(emp::Ptr<Organism> target)
    {
        double my_points = this->GetPoints();
        double target_points = target->GetPoints();
        
        if (target->GetSpecies() == 3) // If the target is a plant
        {
            this->SetPoints((my_points + target_points) / 2); // Share points with the other plant
        }
        else
        {
            this->SetPoints(0);
        }
    }

    /**
     * Takes in nothing.
     * Plants will reproduce when they have the energy needed to give birth.
     * Returns if the plant is ready to reproduce. 
    */
    int CanReproduce()
    {
        if (this->GetPoints() > birth_cost)
        {
            return 1;
        }
        return 0;
    }

    /**
     * Takes in nothing.
     * If they can reproduce: 
     * Plants give birth and perfectly passes on the spent energy to the offspring.
     * Returns the created organism, if any. 
    */
    emp::Ptr<Organism> CheckReproduction()
    {
        if (this->CanReproduce() == 1)
        {
            emp::Ptr<Organism> offspring = new Plant(this->GetRandom(), birth_cost);
            this->AddPoints(-birth_cost);
            return offspring;
        }
        return nullptr;
    }
};

#endif