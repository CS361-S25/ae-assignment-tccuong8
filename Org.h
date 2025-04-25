#ifndef ORG_H
#define ORG_H

#include "emp/math/Random.hpp"
#include "emp/tools/string_utils.hpp"

class Organism
{
    int species;
    double points;
    emp::Ptr<emp::Random> random;

public:
    Organism(emp::Ptr<emp::Random> _random, double _points = 0.0) : random(_random), points(_points) { ; }

    // Data handling traits of an organism
    void SetPoints(double _in) { points = _in; }
    void AddPoints(double _in) { points += _in; }
    double GetPoints() { return points; }
    emp::Ptr<emp::Random> GetRandom() { return random; }
    virtual int GetSpecies() { return 0; }
    virtual std::string GetColor() { return "black"; }

    // Virtual methods of how each organism will behave.
    virtual void Process()
    {
        ;
    }

    virtual void Interact(emp::Ptr<Organism> target)
    {
        ;
    }

    virtual int CanReproduce()
    {
        return 0;
    }

    virtual emp::Ptr<Organism> CheckReproduction()
    {
        return nullptr;
    }
};
#endif