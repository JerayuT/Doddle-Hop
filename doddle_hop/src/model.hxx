#pragma once

#include <ge211.hxx>
#include "doodle.hxx"
#include "game_config.hxx"

class Model
{
public:

//Model(const std::string);
    game_config const config = game_config();
    int score;


    explicit Model();

    Doodle const& doodle() const {
        return doodle_;
    }

    Doodle& get_doodle_test() {
        return doodle_;
    }

    void on_frame(double dt);

    //SETTER
    void set_right()
    {
        doodle_.set_right();
    }
    void set_left()
    {
        doodle_.set_left();
    }
    /*void no_dir()
    {
        doodle_.no_dir();
    }*/
    void set_shoot()
    {
        doodle_.set_shoot_true();
    }
    void set_shoot_motion()
    {
        doodle_.set_shoot_motion_true();
    }

    void launch();
    void block_off_screen();

    Position generate_pos() const;

    std::vector<Block> const& get_block() const
    {
        return blocks_;
    }

    std::vector<Block>& get_block_test()
    {
        return blocks_;
    }

    std::vector<Ball> const& get_ball() const
    {
        return balls;
    }

    bool const& get_game_over() const
    {
        return is_game_over;
    }

    std::vector<Obstacle> get_obstacle()
    {
        return obstacles_;
    }

    bool get_is_game_over()
    {
        return is_game_over;
    }

    void generate_block(Position pos);

    bool bounce_blocks();

    void update_score(int apex, int new_pos, int old_pos);

    Position warp(Doodle doodle);

    std::vector<Ball> balls;

    Block translate_velocity(Block b);
    Obstacle obs_trans_vel(Obstacle obs);

    void shoot_ball(double dt);

    void game_over();

    void v_to_doodle(Block b);

    void doodle_0_y_motion();

    void remove_obstacle();

    Obstacle obstacle_;

private:

    Doodle doodle_;
    double duration1_ = 0;
    double duration2_ = 0;
    int prev_index_ = 0;
    std::vector<Block> blocks_;
    std::vector<Ball> balls_;
    std::vector<Obstacle> obstacles_;
    bool is_game_over;
    int previous_dist;
    Block previous_block;
};
