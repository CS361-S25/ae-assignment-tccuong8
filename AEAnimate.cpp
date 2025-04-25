#include "emp/math/Random.hpp"
#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"
#include "World.h"
#include "Org.h"
#include "Zombie.h"
#include "Human.h"
#include "Plant.h"

emp::web::Document doc{"target"};

class AEAnimator : public emp::web::Animate
{

    // arena width and height
    const int num_h_boxes = 75;
    const int num_w_boxes = 75;
    const double RECT_SIDE = 10;
    const double width{num_w_boxes * RECT_SIDE};
    const double height{num_h_boxes * RECT_SIDE};

    emp::web::Canvas canvas{width, height, "canvas"};

    emp::Random random{5};
    OrgWorld world{random};

public:
    AEAnimator()
    {
        // shove canvas into the div
        // along with a control button
        // and the grid structure
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");
        world.SetPopStruct_Grid(num_w_boxes, num_h_boxes);

        // Initial state
        this->SeedWorld(1, (num_h_boxes + num_w_boxes));
        this->SeedWorld(2, (num_h_boxes + num_w_boxes));
        this->SeedWorld(3, (num_h_boxes + num_w_boxes));

    }

    /**
     * Takes in a species id and a number.
     * Randomly insert the stated number of the corresponding species into the world.
     * Only at unoccupied spaces.
     * Returns nothing.
    */
    void SeedWorld(int species, int num)
    {
        int counts_left = num;
        emp::vector<size_t> schedule = emp::GetPermutation(random, world.GetSize());
        for (size_t i : schedule)
        {
            std::cout << "i: " << i << std::endl;
            if (!world.IsOccupied(i) && counts_left > 0)
            {
                counts_left--;

                emp::Ptr<Organism> org;
                if (species == 1)
                {
                    org = new Human(&random);
                }
                else if (species == 2)
                {
                    org = new Zombie(&random);
                }
                else if (species == 3)
                {
                    org = new Plant(&random);
                }
                world.AddOrgAt(org, i);
            }
        }
    }

    /**
     * Takes in an x coordinante, a y coordinate, and a string describing an organism's color
     * Draws that organism at the stated address.
     * Returns nothing.
    */
    void DrawOrganism(int x, int y, std::string color)
    {
        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, color, "black");
    }

    /**
     * Takes in an x coordinante, and a y coordinate.
     * Draws a blank, default white cell at the stated address.
     * Returns nothing.
    */
    void DrawBlank(int x, int y)
    {
        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "white", "black");
    }

    /**
     * Takes in nothing. 
     * Clears the canvas, updates the world, and draws the result. 
     * Returns nothing. 
    */
    void DoFrame() override
    {
        canvas.Clear();
        world.Update();

        int org_num = 0;
        for (int x = 0; x < num_w_boxes; x++)
        {
            for (int y = 0; y < num_h_boxes; y++)
            {
                if (world.IsOccupied(org_num))
                {
                    this->DrawOrganism(x, y, world.GetOrg(org_num).GetColor());
                }
                else
                {
                    this->DrawBlank(x, y);
                }
                org_num++;
            }
        }
    }
};

AEAnimator animator;

int main() { animator.DoFrame(); }