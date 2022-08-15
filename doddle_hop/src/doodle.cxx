#include <doodle.hxx>

Doodle::Doodle(game_config config) :
    doodle_dims(config.doodle_dims),
    doodle_pos(config.top_left_startblock().x - config.doodle_dims.width/4,
                config.top_left_startblock().y -config.doodle_dims.height),
    doodle_velocity(config.doodle_velocity_0),
    live(false),
    doodle_acc(config.acceleration),
    index_(0),
    is_prev_dir_left_(false)
{
    ge211::Image_sprite img0("doodle_left.png");
    ge211::Image_sprite img1("doodle_right.png");
    ge211::Image_sprite img2("doodle_shoot.png");
    ge211::Image_sprite obs("alien.png");
    img_sprites_.push_back(img0);
    img_sprites_.push_back(img1);
    img_sprites_.push_back(img2);
    img_sprites_.push_back(obs);
}

Obstacle::Obstacle(game_config config, Position posn, ge211::Dims<int> dims)
    :   posn(posn),
        dims(config.obs_dims)
{ }

Ball::Ball(Position posn, game_config config)
        :   posn(posn),
            radius(config.ball_radius),
            velocity(config.ball_velocity)
{}

bool
Doodle::bounce(game_config const& config, Block block) const
{
    int doodle_top = doodle_pos.y;
    int doodle_right = doodle_pos.x + doodle_dims.width;
    int doodle_left = doodle_pos.x;
    int doodle_bottom = doodle_pos.y + doodle_dims.height;

    if(doodle_velocity.height < 0)
    {
        return false;
    }
    if (doodle_right < block.block_pos.x || doodle_bottom < block.block_pos.y ||
        doodle_left > block.block_pos.x + block.block_dims_.width ||
        doodle_top + doodle_dims.height > block.block_pos.y + block.block_dims_.height)
    {
        return false;
    }

    return true;
}


void
Doodle::jump(game_config config)
{
    doodle_velocity = config.doodle_velocity_0;
}

bool
Doodle::destroy_block(game_config config, std::vector<Block>& blocks) {
    for (Block &block: blocks)
    {
        if (bounce(config, block) && block.breakable)
        {
            jump(config);
            block = blocks.back();
            blocks.pop_back();
            return true;
        }
    }
    return false;
}

bool
Block::block_hit_wall(game_config config, Block block)
{
    if(block.type == 2) {
        int right_sidewall = config.scene_dims.width;
        int left_sidewall = 0;
        int right_block = block.block_pos.x + config.block_dims.width;
        int left_block = block.block_pos.x;
        if ((right_block > right_sidewall && block.block_velocity.width > 0))
        {
            return true;
        }
        if (left_block < left_sidewall && block.block_velocity.width < 0)
        {
            return true;
        }
    }
    return false;
}

Block
Block::reflect_velocity(Block b)
{
    b.block_velocity.width *= -1;
    return b;
}

bool
Doodle::hits_obs(Obstacle obs)
{
    int doodle_top = doodle_pos.y;
    int doodle_right = doodle_pos.x + doodle_dims.width;
    int doodle_left = doodle_pos.x;
    int doodle_bottom = doodle_pos.y + doodle_dims.height;

    if (doodle_right < obs.posn.x || doodle_bottom < obs.posn.y ||
        doodle_left > obs.posn.x + obs.dims.width ||
        doodle_top > obs.posn.y + obs.dims.height)
    {
        return false;
    }

    return true;
}

bool
Ball::hits_obs_ball(Obstacle obs)
{
    int ball_top = posn.y;
    int ball_right = posn.x + 2*radius;
    int ball_left = posn.x;
    int ball_bottom = posn.y + 2*radius;

    if (ball_right < obs.posn.x || ball_bottom < obs.posn.y ||
        ball_left > obs.posn.x + obs.dims.width ||
        ball_top > obs.posn.y + obs.dims.height)
    {
        return false;
    }

    return true;
}

Doodle
Doodle::next_doodle(double dt) const
{
    Doodle result(*this);

    int x = result.doodle_pos.x + (doodle_velocity.width*dt);
    int y = result.doodle_pos.y + (doodle_velocity.height*dt);
    result.doodle_pos = {x, y};
    int vy = result.doodle_velocity.height + (doodle_acc.height*dt);
    int vx = result.doodle_velocity.width + doodle_acc.width*dt;
    result.doodle_velocity.width = vx;
    result.doodle_velocity.height = vy;

    return result;
}

Block
Block::next_block(double dt)
{
    Block result(*this);
    int x = result.block_pos.x + (result.block_velocity.width * dt);
    int y = result.block_pos.y + (block_velocity.height * dt);
    result.block_pos.x = x;
    result.block_pos.y = y;
    int vy = result.block_velocity.height + (y_acc * dt);
    result.block_velocity.height = vy;

    return result;
}

Ball
Ball::next_ball(double dt) const
{
    Ball result(*this);
    int x = result.posn.x;
    int y = result.posn.y + (result.velocity.height*dt);
    result.posn = {x, y};
    return result;
}

/*void
Doodle::warp(game_config config, Doodle doodle)
{
    if(doodle.doodle_pos.x + doodle_dims.width/2 > config.scene_dims.width -
                                                   config.side_margin)
    {
        doodle.doodle_pos = {doodle.doodle_pos.x +config.side_margin + 10,
                             doodle.doodle_pos.y};
    }
    if(doodle.doodle_pos.x + doodle_dims.width/2 < config.side_margin)
    {
        doodle.doodle_pos = { config.scene_dims.width - config.side_margin -
                              doodle.doodle_pos.x - 10,
                              doodle.doodle_pos.y};
    }
}
*/

Obstacle
Obstacle::next_obstacle(double dt)
{
    Obstacle result(*this);
    int y = result.posn.y + (y_vel * dt);
    result.posn.y = y;
    int vy = result.y_vel + (y_acc * dt);
    result.y_vel = vy;

    return result;
}