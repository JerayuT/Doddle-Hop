#include "game_config.hxx"

game_config::game_config()
        :   scene_dims {300, 800},
            doodle_dims {42, 42},
            doodle_shoot_dims {30,59},
            ball_radius(4),
            vertical_margin(50),
            block_dims{50,10},
            doodle_velocity_0{0,-700},
            block_velocity_0{0,0},
            doodle_x_velocity(400),
            ball_velocity {0, -600},
            initial_score(0),
            acceleration{300,1000},
            obs_dims{30,30}
{ }


ge211::Posn<int>
game_config::top_left_startblock() const
{
    ge211::Posn<int> start_pos(scene_dims);
    start_pos.x /= 2;
    start_pos.x -= block_dims.width/2;
    start_pos.y -= (vertical_margin + block_dims.height);
    return start_pos;
}
