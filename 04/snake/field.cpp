/*
####################################################################
# TIE-02207 Programming 2: Basics, S2019                           #
#                                                                  #
# Name: Christen Blom-dahl Casanova                                #
# Student number: 282803                                           #
# email: christen.blom-dahlcasanova@tuni.fi                        #
#                                                                  #
# Project1: Snake                                                  #
# Program description: Implements a (broken) game called Snake.    #
#                                                                  #
# File: field.cpp                                                  #
# Description: Defines a class representing the game field.        #
#                                                                  #
# Author: First Last, s.num., first.last@tuni.fi                   #
####################################################################
*/

#include "field.hh"


using namespace std;

Field::Field(int width, int height, const std::default_random_engine& rng):
    width_(width),
    height_(height),
    rng_(rng) {

    // Create a snake whose head is about in the middle of the Field.
    const Point head ={(width_ - 1) / 2, (height_ - 1) / 2};
    snake_.push_back(head);

    // Put food somewhere.
    moveFood();
}

bool Field::gameLost() const {
    return dead_;
}

bool Field::gameOver() const {
    return gameLost() | gameWon();
}


bool Field::gameWon() const {
    return static_cast<int>(snake_.size()) >= width_ * height_;
}


void Field::move(const std::string& direction) {
    // If the snake can't / doesn't need to move, do nothing.
    if (gameOver()) {
        return;
    }

    // Figure out the destination coordinates.
    Point new_head = getHead();
    if (direction == UP) {
        --new_head.y;
    } else if (direction == LEFT) {
        --new_head.x;
    } else if (direction == DOWN) {
        ++new_head.y;
    } else if (direction == RIGHT) {
        ++new_head.x;
    }

    // Check if the snake died after running into a wall.new_head.x < 0 || new_head.x >= width_) && (new_head.y < 0 || new_head.y >= height_
    if (new_head.y < 0 || new_head.x < 0 || new_head.y >= height_ || new_head.x >= width_) {
       // cout << "We dead in the point: " << new_head.x << "." << new_head.y << endl;
        dead_ = true;
        return;
    }

    // Actually move the snake.
    moveSnake(new_head);
}

void Field::print() const {

    // Print the top wall.
    printHorizontalWall();
    // Print rows with playable area.

    for (int row = 0; row < height_; ++row) {
        std::cout << WALL;
        for (int col = 0; col < width_; ++col) {
            const Point position{col, row};
            if (position.x == food_.x && position.y == food_.y) {

                std::cout << FOOD;
            } else if (isInSquare(position)) {
                std::cout << EMPTY;
            } else if (gameLost()) {
                std::cout << DEAD;
            } else if (position.x == getHead().x && position.y == getHead().y) {
                std::cout << HEAD;
            } else if (position.x == getTail().x && position.y == getTail().y) {
                std::cout << TAIL;
            } else {
                std::cout << BODY ;
            }

        }
        std::cout << WALL << endl;

    }

    // Print the bottom wall.
    printHorizontalWall();
}

bool Field::isInSquare(const Point& square) const{
    // Check if any of the Points stored in snake_ match the given Point.
    for (const Point& part : snake_) {
        if (part.x == square.x && part.y == square.y) {
        //   cout << "if";
            return false;
        }
    }

    return true;
}

const Point Field::getHead() const {
    // The snake's head is the last one in the vector.
    return snake_.back();
}

const Point Field::getTail() const {
    // The snake's tail is the first one in the vector.
    return Point(snake_.front());
}

void Field::moveFood(){
    // Move food out of sight when it's no longer needed.
    if (gameWon()) {
        food_.x = -1;
        food_.y = -1;
        return;
    }

    // Keep trying random Points until an empty square is found.
    std::uniform_int_distribution<int> width_dist(0, width_-1);
    std::uniform_int_distribution<int> height_dist(0, height_-1);
    while (true) {
        food_.x = width_dist(rng_);
        food_.y = height_dist(rng_);
        if (isInSquare(food_)) {
            // Snakeless square found, stop moving the food around.
            return;
        }
    }
}

bool Field::eatItSelf(const Point& new_head){
    int i = 0;
    for (const Point& part : snake_) {
        if(i !=0 && new_head.x == part.x && new_head.y == part.y)
            return true;
        i++;
    }
    return false;
}

void Field::moveSnake(const Point& new_head) {
    // There shouldn't be any problems if the snake is only a head or
    // if it doesn't yet occupy the square it's moving into.
    if (snake_.size() > 1 || !isInSquare(new_head)) {
        const Point& neck = snake_[snake_.size() - 2];
        if (new_head.x == neck.x && new_head.y == neck.y) {
            // If the destination square is the square before the head,
            // don't move but don't die either.
            return;

        } else if (eatItSelf(new_head)) {
            // If the destination square contains anything but the "neck" or
            // the tail, the snake dies.
            dead_ = true;
            return;
        }
    }

    // Move the snake.
    snake_.push_back(new_head);
    // New food must be placed somewhere once one gets eaten.
    // Also, the snake should stretch a little.
    if (new_head.x == food_.x && new_head.y == food_.y) {
        moveFood();
    } else {
        snake_.erase(snake_.begin());
    }
}

void Field::printHorizontalWall() const {
    // Print a long enough wall to cover the Field and the walls at the sides.
    for (int i = 0; i < width_+2; ++i) {
        std::cout << WALL;
    }
    std::cout << std::endl;
}
