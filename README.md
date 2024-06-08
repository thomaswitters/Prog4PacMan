# Some specifics

The engine includes a collision detection component to identify collisions between objects, such as pickups or ghosts. Colliding with a pickup results in a score increase, while colliding with a ghost causes the player to lose a life.

The ghosts in the game utilize an A* pathfinding algorithm for their movement. Both Pac-Man and the ghosts can only move along a grid, avoiding cells marked as walls. The A* algorithm ensures that the ghosts find the fastest path without passing through walls.

There are three types of pathfinding strategies for the ghosts:

Chase Mode: Finds the fastest path to Pac-Man.
Ambush Mode: Targets a position four cells ahead of Pac-Man to cut him off.
Flee Mode: Moves towards the farthest cell from Pac-Man for fleeing purposes.
Additionally, the ghosts have a field of vision (FOV) component, enabling them to detect Pac-Man and determine his position within their line of sight.

# link
https://github.com/thomaswitters/Prog4PacMan.git

# Minigin

Minigin is a very small project using [SDL2](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D c++ game projects. It is in no way a game engine, only a barebone start project where everything sdl related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes.

[![Build Status](https://github.com/avadae/minigin/actions/workflows/msbuild.yml/badge.svg)](https://github.com/avadae/msbuild/actions)
[![GitHub Release](https://img.shields.io/github/v/release/avadae/minigin?logo=github&sort=semver)](https://github.com/avadae/minigin/releases/latest)

# Goal

Minigin can/may be used as a start project for the exam assignment in the course 'Programming 4' at DAE. In that assignment students need to recreate a popular 80's arcade game with a game engine they need to program themselves. During the course we discuss several game programming patterns, using the book '[Game Programming Patterns](https://gameprogrammingpatterns.com/)' by Robert Nystrom as reading material. 

# Disclaimer

Minigin is, despite perhaps the suggestion in its name, not a game engine. It is just a very simple sdl2 ready project with some of the scaffolding in place to get started. None of the patterns discussed in the course are used yet (except singleton which use we challenge during the course). It is up to the students to implement their own vision for their engine, apply patterns as they see fit, create their game as efficient as possible.

# Use

Download the latest release of this project and compile/run in visual studio. Since students need to have their work on github too, they can use this repository as a template.

