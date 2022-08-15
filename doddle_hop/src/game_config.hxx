#pragma once

#include <ge211.hxx>

using Velocity = ge211::Dims<int>;

struct game_config
{
    //Constructs an instance with default parameters
    game_config();

    //Dimensions of the window/screen
    ge211::Dims<int> scene_dims;

    //Dimensions of doodle
    ge211::Dims<int> doodle_dims;
    ge211::Dims<int> doodle_shoot_dims;
    int ball_radius;

    //Margins
    int vertical_margin;

    //Dimensions of the blocks
    ge211::Dims<int> block_dims;

    //Center Doodle in center of block for start of game
    ge211::Posn<int> top_left_startblock() const;

    //Initial Velocity of Doodle
    Velocity doodle_velocity_0;

    //Initial Block Velocity
    Velocity block_velocity_0;

    int doodle_x_velocity;
    Velocity ball_velocity;

    int initial_score;
    ge211::Dims<int> acceleration;

    ge211::Dims<int> obs_dims;

};
