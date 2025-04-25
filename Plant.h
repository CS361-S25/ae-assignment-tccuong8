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
    Plant(emp::Ptr<emp::Random> _random, double _synthesis = 100, double _birth_cost = 100, double _points = 100.0) : Organism(_random, _points), synthesis(_synthesis), birth_cost(_birth_cost), species(3) { ; }

    int GetSpecies() { return species; }

    std::string GetColor() { return "green"; }

    void Process()
    {
        this->AddPoints(synthesis);
    }

    void Interact(emp::Ptr<Organism> target)
    {
        double my_points = this->GetPoints();
        double target_points = target->GetPoints();
        if (target->GetSpecies() == 1) // If the target is a human
        {
            if (my_points > 100)
            {
                this->SetPoints(0); // Eaten by the human
            }
        }
        else if (target->GetSpecies() == 2) // If the target is a compost (killed zombie)
        {
            if (target_points > 0)
            {
                this->SetPoints(0); // Trampled by the zombie
            }
            else
            {
                this->AddPoints(-target_points); // Gain points from absorbing compost
            }
        }
        else if (target->GetSpecies() == 3) // If the target is a plant
        {
            this->SetPoints((my_points + target_points) / 2); // Share points with the other plant
        }
    }

    int CanReproduce()
    {
        if (this->GetPoints() > birth_cost)
        {
            return 1;
        }
        return 0;
    }

    emp::Ptr<Organism> CheckReproduction()
    {
        if (this->CanReproduce() == 1)
        {
            emp::Ptr<Organism> offspring = new Plant(this->GetRandom());
            this->AddPoints(-birth_cost);
            return offspring;
        }
        return nullptr;
    }
};

#endif