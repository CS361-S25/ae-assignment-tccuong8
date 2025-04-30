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

    // traits
    int GetSpecies() { return species; }
    std::string GetColor() { return "red"; }

    /**
     * Takes in nothing.
     * Zombies decay by a fraction of their energy every update.
     * Returns nothing.
     */
    void Process()
    {
        this->AddPoints(-(this->GetPoints() * decay / 100.0 + 10));
    }

    /**
     * Takes in a pointer to an Organism that's the target of interaction.
     * Zombies will eat Plants and gain energy from them. 
     * Zombies otherwise attack any other organism, including other Zombies.
     * If a Zombie kills a Human, they can turn the corpse into another Zombie.
     * Returns nothing.
     */
    void Interact(emp::Ptr<Organism> target)
    {
        double my_points = this->GetPoints();
        double target_points = target->GetPoints();
        if (target->GetSpecies() == 3)
        {
            this->AddPoints(target_points);
        }
        else
        {
            this->AddPoints(-target_points);
            if (target->GetSpecies() == 1 && my_points > target_points)
            {
                has_corpse = 1;
            }
        }
    }

    int CanReproduce()
    {
        return has_corpse;
    }

    /**
     * Takes in nothing.
     * If they can reproduce:
     * Zombies produce shambling Zombies that decay quite quickly.
     * Returns the created organism, if any.
     */
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