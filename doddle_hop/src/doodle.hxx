#pragma once

#include <ge211.hxx>
#include "game_config.hxx"

using Position = ge211::Posn<int>;
using Velocity = ge211::Dims<int>;

struct Obstacle
{
    Obstacle(game_config config, Position posn, ge211::Dims<int> dims);
    Position get_obs_pos() const {
        return posn;
    }
public:
    Position posn;
    ge211::Dims<int> dims;
    int y_vel = 0;
    int y_acc = 0;

    Obstacle next_obstacle(double dt);

    Obstacle createnewobstacle(game_config config, Position pos);
};

struct Block
{
public:
    //Blocks(game_config config);
    ge211::Dims<int> block_dims_ = game_config().block_dims;
    Position block_pos;
    Velocity block_velocity;
    bool breakable;
    int type;
    int y_acc = 0;

    Block(Position posn, bool breaks, Velocity v, int val)
            : block_pos(posn),
            block_velocity(v),
            breakable(breaks),
            type(val)
    {
    }

    static Block stationary_block(Position posn)
    {
        return Block(posn, false, game_config().block_velocity_0, 1);
    }

    static Block moving_block(Position posn, Velocity v)
    {
        return Block(posn, false, v, 2);
    }

    static Block breakable_block(Position posn, Velocity v)
    {
        return Block(posn, true, v, 3);
    }



    int const& get_block() const
    {
        return type;
    }

    static bool block_hit_wall(game_config config, Block block);
    Block reflect_velocity(Block b);

    Block next_block(double dt);

};

struct Doodle
{
public:
    Doodle(game_config config);
    ge211::Dims<int> doodle_dims;
    ge211::Dims<int> doodle_shoot_dims = game_config().doodle_shoot_dims;
    Position doodle_pos;
    Velocity doodle_velocity;
    bool live = false;
    ge211::Dims<int> doodle_acc;

    bool hits_obs (Obstacle obs);
    bool bounce(game_config const& config, Block block) const;
    void jump(game_config config);
    //void warp(game_config config, Doodle doodle);
    bool destroy_block(game_config config, std::vector<Block>& blocks);
    Doodle next_doodle(double dt) const;

    ge211::Image_sprite const& get_sprite() const
    {
        return img_sprites_[index_];
    }
    //Doodle next(double dt);
    ge211::Image_sprite const& get_obs_sprite() const
    {
        return img_sprites_[3];
    }

    void set_right()
    {
        right_ = true;
        left_ = false;
        is_prev_dir_left_ = false;
    }
    void set_left()
    {
        right_ = false;
        left_ = true;
        is_prev_dir_left_ = true;
    }

    void reset_x_velocity()
    {
        right_ = false;
        left_ = false;
    }

    bool get_left()
    {
        return left_;
    }
    bool get_right()
    {
        return right_;
    }
    bool get_prev_dir()
    {
        return is_prev_dir_left_;
    }

    void set_shoot_motion_true()
    {
        shoot_motion_ = true;
    }
    void set_shoot_motion_false()
    {
        shoot_motion_ = false;
    }
    void set_shoot_true()
    {
        shoot_ = true;
    }
    void set_shoot_false()
    {
        shoot_ = false;
    }
    bool const& get_shoot() const
    {
        return shoot_;
    }
    bool get_shoot_motion()
    {
        return shoot_motion_;
    }
    void set_index(int val)
    {
        index_ = val;
    }
    void set_position(Position pos)
    {
        doodle_pos = pos;
    }
    int get_index()
    {
        return index_;
    }

private:
    bool left_ = false;
    bool right_ = false;
    bool shoot_ = false;
    bool shoot_motion_ = false;
    std::vector<ge211::Image_sprite> img_sprites_;
    int index_;
    bool is_prev_dir_left_;
};

/*struct Ball
{
public:
    Position ball_pos;

    Ball(Position doodle_pos, game_config config)
        :   ball_pos()

};*/

struct Ball
{
    Ball(Position posn, game_config config);

    Ball next_ball(double dt) const;

    bool hits_obs_ball(Obstacle obs);

    Position posn;
    int radius;
    Velocity velocity;
};


