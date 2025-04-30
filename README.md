[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/-GCLf3vW)

# World of Life and Death

My world showcases the interaction between 3 species: Humans, Zombies, and Plants. In each update step, each organism can interact with another organism, give birth to something, and move somwhere.

Humans lose a certain amount of energy every update unless they interact with another Human, and gain energy from eating Plants. They are converted into Zombies when got into contact with Zombies, but will deal damage tot he Zombies corresponding to their own pool of energy.

Zombies decay a fraction plus some of their energy every update. They gain energy from eating Plants, and attack everything else, even other Zombies.

Plants synthesize points every update. They equalize their energy when interacting with fellow Plants (the interacting pair pools their energy and split it evenly), and got eaten otherwise.