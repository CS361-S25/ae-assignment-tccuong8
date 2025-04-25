#ifndef WORLD_H
#define WORLD_H

#include "emp/Evolve/World.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"
#include "emp/math/math.hpp"

#include "Org.h"

class OrgWorld : public emp::World<Organism>
{

    emp::Random &random;
    emp::Ptr<emp::Random> random_ptr;

public:
    OrgWorld(emp::Random &_random) : emp::World<Organism>(_random), random(_random)
    {
        random_ptr.New(_random);
    }

    ~OrgWorld()
    {
    }

    /**
     * Takes in an index of a known organism.
     * Removes that organism form the world.
     * Returns the organism, which is meant to be added back to the world in a new spot to simulate movement.
    */
    emp::Ptr<Organism> ExtractOrganism(int i)
    {
        emp::Ptr<Organism> org = pop[i];
        pop[i] = nullptr;
        return org;
    }

    /**
     * Takes in an index of a known organism and optionally if it's looking for another organism to interact with.
     * Tries 3 times to randomly find either an interactible organism or an empty spot to move to.
     * Returns the index of the found target/empty spot. If none was found, the organism stays put.
    */
    int NextMove(int i, int findingTarget = 0)
    {
        for (int x = 0; x < 3; ++x)
        {
            emp::WorldPosition pos = GetRandomNeighborPos(i);
            if (pos.IsValid())
            {
                int pos_num = pos.GetIndex();
                if (findingTarget == 1 && IsOccupied(pos_num))
                {
                    return pos_num;
                }
                else if (findingTarget == 0 && !IsOccupied(pos_num))
                {
                    return pos_num;
                }
            }
        }
        return i;
    }

    /**
     * Takes in nothing.
     * Simulates the actions done by eeach organism in a random order.
     * Modifies the global world to reflect results.
     * Returns nothing.
    */
    void Update()
    {
        emp::World<Organism>::Update();

        // Organisms die or progress in life
        emp::vector<size_t> schedule = emp::GetPermutation(random, GetSize());
        for (int i : schedule)
        {
            if (!IsOccupied(i))
            {
                continue;
            }
            emp::Ptr<Organism> org = pop[i];
            if (org->GetPoints() <= 0)
            {
                std::cout << "Org [" << i << "] Species [" << pop[i]->GetSpecies() << "] has died!" << std::endl;
                pop[i] = nullptr;
            }
            else
            {
                std::cout << "Org [" << i << "] Species [" << pop[i]->GetSpecies() << "] (" << pop[i]->GetPoints() << ")" << std::endl;
                org->Process();
            }
        }

        // Organisms try to interact, propagate, and then move in that order.
        schedule = emp::GetPermutation(random, GetSize());
        for (int i : schedule)
        {
            if (!IsOccupied(i))
            {
                continue;
            }
            int interact_pos = this->NextMove(i, 1);
            if (interact_pos != i)
            {
                pop[i]->Interact(pop[interact_pos]);
            }
            int propagate_pos = this->NextMove(i);
            if (propagate_pos == i)
            {
                continue;
            }
            emp::Ptr<Organism> propagate = pop[i]->CheckReproduction();
            if (propagate)
            {
                AddOrgAt(propagate, propagate_pos);
            }
            emp::Ptr<Organism> movedOrg = ExtractOrganism(i);
            AddOrgAt(movedOrg, GetRandomNeighborPos(i));
        }
    }
};
#endif
