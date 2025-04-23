#ifndef WORLD_H
#define WORLD_H

#include "emp/Evolve/World.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"
#include "emp/math/math.hpp"

#include "Org.h"

class OrgWorld : public emp::World<Entity>
{

    emp::Random &random;
    emp::Ptr<emp::Random> random_ptr;

public:
    OrgWorld(emp::Random &_random) : emp::World<Entity>(_random), random(_random)
    {
        random_ptr.New(_random);
    }

    ~OrgWorld()
    {
    }

    // First, you’ll need to make a new method in your World subclass that removes an organism from the population and returns it. I recommend calling it ExtractOrganism. You already know how to get an organism at a particular position in the world, and ‘removing’ it from the population just involves setting its spot to null:
    // pop[i] = nullptr;
    // Then you just need to return the organism that you grabbed.
    emp::Ptr<Entity> ExtractOrganism(int i)
    {
        emp::Ptr<Entity> org = pop[i];
        pop[i] = nullptr;
        return org;
    }

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

    void Update()
    {
        emp::World<Entity>::Update();

        // Natural organism's existential process
        emp::vector<size_t> schedule = emp::GetPermutation(random, GetSize());
        for (int i : schedule)
        {
            if (!IsOccupied(i))
            {
                continue;
            }
            emp::Ptr<Entity> org = pop[i];
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

        // Try to interact, propagate, then move
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
                std::cout << "Org [" << i << "] Species [" << pop[i]->GetSpecies() << "] has no space to give birth." << std::endl;
                continue;
            }
            emp::Ptr<Entity> propagate = pop[i]->CheckReproduction();
            // this is implemented in Organism
            if (propagate)
            {
                std::cout << "Org [" << i << "] Species [" << pop[i]->GetSpecies() << "] has reproduced." << std::endl;
                AddOrgAt(propagate, propagate_pos);
                // i is the parent's position in the world
            }
            emp::Ptr<Entity> movedOrg = ExtractOrganism(i);

            AddOrgAt(movedOrg, this->NextMove(i));
        }
    }
};
#endif
