#ifndef HUMAN_H
#define HUMAN_H

#include "Org.h"

class Human : public Organism
{
    double exhaustion;
    double birth_cost;
    int species;

public:
    Human(emp::Ptr<emp::Random> _random, double _points = 100.0, double _exhaustion = 1, double _birth_cost = 30) : Organism(_random, _points), exhaustion(_exhaustion), birth_cost(_birth_cost), species(1) { ; }

    int GetSpecies() { return species; }

    std::string GetColor() { return "blue"; }

    void Process()
    {
        this->AddPoints(-exhaustion);
    }

    void Interact(emp::Ptr<Organism> target)
    {
        double my_points = this->GetPoints();
        double target_points = target->GetPoints();
        if (target->GetSpecies() == 1) // If the target is a human
        {
            this->AddPoints(exhaustion); // Mutual support canceling out exhaustion
        }
        else if (target->GetSpecies() == 2) // If the target is a zombie
        {
            if (my_points <= target_points)
            {
                this->SetPoints(0); // Killed by the zombie
            }
        }
        else if (target->GetSpecies() == 3) // If the target is a plant
        {
            this->AddPoints(target_points); // Gain points from eating the plant
        }
    }

    int CanReproduce()
    {
        if (this->GetPoints() > birth_cost*2)
        {
            return 1;
        }
        return 0;
    }

    emp::Ptr<Organism> CheckReproduction()
    {
        if (this->CanReproduce() == 1)
        {
            emp::Ptr<Organism> offspring = new Human(this->GetRandom(), birth_cost);
            this->AddPoints(-birth_cost);
            return offspring;
        }
        double mutating = GetRandom()->GetDouble(0, 1);
        if (mutating < 0.3)
        {
            emp::Ptr<Organism> mutation = new Zombie(this->GetRandom(), this->GetPoints()/2);
            this->SetPoints(0); // The human spontaneously mutates into a zombie
            return mutation;
        }
        return nullptr;
    }
};

#endif