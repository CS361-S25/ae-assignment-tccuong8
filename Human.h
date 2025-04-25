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

    // traits
    int GetSpecies() { return species; }
    std::string GetColor() { return "blue"; }

    /**
     * Takes in nothing
     * Human loses some energy everyday from working out.
     * Returns nothing.
     */
    void Process()
    {
        this->AddPoints(-exhaustion);
    }

    /**
     * Takes in a pointer to an Organism that's the target of interaction.
     * Humans will support each other and won't be exhaused working as a team. 
     * Humans die fighting against stronger Zombies.
     * Humans eat Plants. 
     * Returns nothing.
     */
    void Interact(emp::Ptr<Organism> target)
    {
        double my_points = this->GetPoints();
        double target_points = target->GetPoints();
        if (target->GetSpecies() == 1)
        {
            this->AddPoints(exhaustion);
        }
        else if (target->GetSpecies() == 2) 
        {
            if (my_points <= target_points)
            {
                this->SetPoints(0);
            }
        }
        else if (target->GetSpecies() == 3)
        {
            this->AddPoints(target_points);
        }
    }

    /**
     * Takes in nothing.
     * Humans can afford to reproduce when they have more than twice the energy needed to give birth.
     * Returns if the human is ready to reproduce. 
    */
    int CanReproduce()
    {
        if (this->GetPoints() > birth_cost * 2)
        {
            return 1;
        }
        return 0;
    }

    /**
     * Takes in nothing.
     * If they can reproduce: 
     * Humans give birth and perfectly passes on the spent energy to the offspring.
     * Humans might randomly mutate into Zombies if they didn't give birth.
     * Returns the created organism, if any. 
    */
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
            emp::Ptr<Organism> mutation = new Zombie(this->GetRandom(), this->GetPoints() / 2);
            this->SetPoints(0); // The human spontaneously mutates into a zombie
            return mutation;
        }
        return nullptr;
    }
};

#endif