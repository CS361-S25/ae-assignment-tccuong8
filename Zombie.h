#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Org.h"

class Zombie : public Organism
{
    double decay;
    int has_corpse;
    int species;

public:
    Zombie(emp::Ptr<emp::Random> _random, double _points = 20.0, double _decay = 30) : Organism(_random, _points), decay(_decay), has_corpse(0), species(2) { ; }

    int GetSpecies() { return species; }

    std::string GetColor() { return "red"; }

    void Process()
    {
        this->AddPoints(-(this->GetPoints() * decay / 100.0 + 10));
    }

    void Interact(emp::Ptr<Organism> target)
    {
        double my_points = this->GetPoints();
        double target_points = target->GetPoints();
        if (target->GetSpecies() == 1) // If the target is a human
        {
            if (my_points <= target_points)
            {
                this->AddPoints(-target_points); // Harmed by the human
            }
            else
            {
                has_corpse = 1; // The zombie has a corpse
            }
        }
        // else if (target->GetSpecies() == 3 && my_points < 0) // If the target is a plant and I'm a compost
        // {
        //     if (my_points < 0)
        //     {
        //         this->SetPoints(0); // The compost is depleted
        //     }
        //     else
        //     {
        //         // this->AddPoints(target_points); // Gain points from absorbing the plant
        //     }
        // }
        else {
            this->AddPoints(target_points); // The zombie eats anything, including other zombies
        }
    }

    int CanReproduce()
    {
        return has_corpse;
    }

    emp::Ptr<Organism> CheckReproduction()
    {
        if (this->CanReproduce() == 1)
        {
            emp::Ptr<Organism> offspring = new Zombie(this->GetRandom());
            has_corpse = 0; // Used the corpse for a new zombie
            return offspring;
        }
        return nullptr;
    }
};

#endif