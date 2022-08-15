#include "model.hxx"
#include<iostream>

Model::Model()
    : score(config.initial_score),
      obstacle_(config, {100, 400}, config.obs_dims),
      doodle_(config),
      is_game_over(false),
      previous_dist(doodle().doodle_pos.y),
      previous_block(Block::stationary_block(config.top_left_startblock()))
{
    Block start_block = Block::stationary_block(config.top_left_startblock());
    blocks_.push_back(start_block);
    for(int i = 0; i < 15; i++)
    {
        Position random_pos = generate_pos();
        generate_block(random_pos);
    }
    obstacles_.push_back(obstacle_);

}


void
Model::on_frame(double dt)
{
    int y_least = config.scene_dims.height;
    if(doodle_.live)
    {
        Doodle next = doodle_.next_doodle(dt);
        Block bounced_block = previous_block;


        bool no_block_on_screen = false;
        unsigned int counter = 0;
        for(auto block : blocks_)
        {
            if(block.block_pos.y > config.scene_dims.height || block.block_pos.y < 0)
            {
                counter++;
            }
        }
        if (counter == blocks_.size())
        {
            no_block_on_screen = true;
        }

        if (no_block_on_screen)
        {
            doodle_.doodle_acc.height = config.acceleration.height;
            /*for (unsigned int i = 0; i < blocks_.size(); i++)
            {
                for(auto& block : blocks_)
                {
                    block.block_velocity.height = 0;
                }
            }*/
        }


        if (bounce_blocks()) {
            for(auto& block : blocks_)
            {
                if (doodle_.bounce(config, block))
                {
                    bounced_block = block;
                    break;
                }
            }
            if (bounced_block.block_pos.y != previous_block.block_pos.y)
            {
                int y_dist = abs(bounced_block.block_pos.y);
                if(y_dist - abs(doodle_.doodle_pos.y) < y_least)
                {
                    y_least = y_dist;
                }
                update_score(250, abs(previous_dist), y_least);
                previous_dist = y_dist;
                previous_block = bounced_block;
            }
            doodle_.jump(config);
        }


        if (doodle_.get_left())
        {
            doodle_.set_index(0);
            doodle_.doodle_velocity.width = -1 * config.doodle_x_velocity;
            doodle_.doodle_acc.width = config.acceleration.width;
        }
        if (doodle_.get_right())
        {
            doodle_.set_index(1);
            doodle_.doodle_velocity.width = config.doodle_x_velocity;
            doodle_.doodle_acc.width = -1 * config.acceleration.width;
        }
        if (doodle_.doodle_velocity.width == 0)
        {
            doodle_.doodle_acc.width = 0;
        }
        double update_rate1 = 0.1;
        double update_rate2 = 0.001;
        /*int dist_ball_doodle = config.scene_dims.height;
        for (auto ball: balls)
        {
            int dist = ball.posn.y;
            if (abs(dist) < dist_ball_doodle)
            {
                dist_ball_doodle = abs(dist);
            }
        }*/

        if (doodle_.get_shoot()) {
            Position ball_pos = {doodle_.doodle_pos.x + doodle_.doodle_shoot_dims.width/2
                                 -config.ball_radius,
                                 doodle_.doodle_pos.y};
            Ball shooting_ball = Ball(ball_pos, config);
            balls.push_back(shooting_ball);

            duration2_ += dt;
            if (duration2_ > update_rate2)
            {
                duration2_ = 0;
                doodle_.set_shoot_false();
            }
        }
        if (doodle_.get_shoot_motion())
        {
            if (doodle_.get_prev_dir())
            {
                prev_index_ = 0;
            }
            else
            {
                prev_index_ = 1;
            }
            doodle_.set_index(2);
            duration1_ += dt;
            if (duration1_ > update_rate1)
            {
                duration1_ = 0;
                doodle_.set_shoot_motion_false();
                doodle_.set_index(prev_index_);
            }
        }
        for(auto& ball : balls)
        {
            if (ball.posn.y < 0)
            {
                ball = balls.back();
                balls.pop_back();
            }
        }

        if (obstacle_.posn.y > config.scene_dims.height && !no_block_on_screen)
        {
            if(score > 1000 && score < 100000) {
                ge211::Random_source<int> random_num2(1, 100000 / score);
                ge211::Random_source<int> random_num3(1, 100000 / score);
                int rand2 = random_num2.next();
                int rand3 = random_num3.next();
                if (rand2 == rand3)
                {
                    obstacle_.posn = generate_pos();
                }
            }
            else
            {
                obstacle_.posn = generate_pos();
            }
            //obstacles_.push_back(obstacle_);
        }
        if (blocks_.size() < 15)
        {
            Position new_block_pos = generate_pos();
            generate_block(new_block_pos);
        }
        Position warp_pos = warp(doodle_);
        doodle_.doodle_pos = warp_pos;

        if (next.doodle_pos.y < config.scene_dims.height/2 && doodle_.doodle_velocity.height != 0
                                                              && !no_block_on_screen)
        {
            for (auto& block : blocks_)
            {
                block = translate_velocity(block);
            }
            obstacle_ = obs_trans_vel(obstacle_);
            doodle_0_y_motion();
        }
        if (blocks_[0].block_velocity.height == 0 && !no_block_on_screen)
        {
            for (auto& block : blocks_)
            {
                v_to_doodle(block);
            }
            obstacle_.y_acc = 0;
            obstacle_.y_vel = 0;
            doodle_.doodle_acc.height = config.acceleration.height;
        }
        next = doodle_.next_doodle(dt);
        doodle_ = next;
        if (!no_block_on_screen)
        {
            for (auto& block : blocks_)
            {
                Block next_block = block.next_block(dt);
                if (next_block.block_hit_wall(config, block))
                {
                    block.block_velocity.width *= -1;
                    next_block = block.next_block(dt);
                }
                block = next_block;
            }
        }
        block_off_screen();
        doodle_.destroy_block(config,blocks_);
        shoot_ball(dt);
        remove_obstacle();
        Obstacle next_obs = obstacle_.next_obstacle(dt);
        obstacle_ = next_obs;
        doodle_.reset_x_velocity();
        game_over();
    }
}


Position
Model::generate_pos() const
{
    int smallest_y = config.scene_dims.height;
    for(auto block : blocks_)
    {
        int y_value = block.block_pos.y;
        if(y_value < smallest_y)
        {
            smallest_y = y_value;
        }
    }
    ge211::Random_source<int>
    random_height(smallest_y - 200, smallest_y - 100);
    ge211::Random_source<int>
    random_width(0, config.scene_dims.width - config.block_dims.width);
    int random_x = random_width.next();
    int random_y = random_height.next();
    Position pos = {random_x,random_y};
    return pos;
}

void
Model::launch()
{
    if (!doodle_.live)
    {
        doodle_.live = true;
        doodle_.jump(config);
        is_game_over = false;
    }
}

void
Model::block_off_screen()
{
    for(Block &block: blocks_)
    {
        if(block.block_pos.y + block.block_dims_.height >
           config.scene_dims.height)
        {
            block = blocks_.back();
            blocks_.pop_back();
        }
    }
}

void
Model::generate_block(Position pos)
{
    ge211::Random_source<int> random_num(1, 3);
    int type_block = random_num.next();
    if(score > 1000 && score < 100000)
    {
        ge211::Random_source<int> random_num2(1, 100000/score);
        ge211::Random_source<int> random_num3(1, 100000/score);
        int rand2 = random_num2.next();
        int rand3 = random_num3.next();
        if (rand2 == rand3)
        {
            type_block = 2;
        }
    }
    if(score > 100000)
    {
        type_block = 3;
    }
    if (type_block == 1) {
        Block initial_block = Block::stationary_block(pos);
        blocks_.push_back(initial_block);
    }
    if (type_block == 2)
    {
        Block initial_block = Block::moving_block(pos,
                                                  {100,0});
        blocks_.push_back(initial_block);
    }
    if (type_block == 3)
    {
        Block initial_block = Block::breakable_block(pos,
                                                     {0,0});
        blocks_.push_back(initial_block);
    }
}

void
Model::update_score(int apex, int new_pos, int old_pos)
{
    score += abs(apex - new_pos - (new_pos - old_pos));
}

bool
Model::bounce_blocks()
{
    for (auto& block : blocks_)
    {
        if (doodle_.bounce(config, block))
        {
            return true;
        }
    }
    return false;
}

Position
Model::warp(Doodle doodle)
{
    Position pos (doodle.doodle_pos);
    if(doodle.doodle_pos.x > config.scene_dims.width)
    {
        pos.x = doodle.doodle_pos.x - config.scene_dims.width + 10;
    }
    if(doodle.doodle_pos.x < 0)
    {
        pos.x = config.scene_dims.width + ( -1 *doodle.doodle_pos.x) - 10;
    }
    return pos;
}

void
Model::shoot_ball(double dt)
{
    for (auto& ball: balls)
    {
        ball = ball.next_ball(dt);
    }
}

void
Model::game_over(){
    if (doodle_.hits_obs(obstacle_) || doodle_.doodle_pos.y > config.scene_dims.height)
    {
        doodle_.live = false;
        while(!blocks_.empty())
        {
            blocks_.pop_back();
        }
        while(!balls.empty())
        {
            balls.pop_back();
        }
        while(!obstacles_.empty())
        {
            obstacles_.pop_back();
        }
        Block start_block = Block::stationary_block(config.top_left_startblock());
        //doodle().stationary_block(config,config.top_left_startblock());
        blocks_.push_back(start_block);
        obstacle_ = Obstacle(config, generate_pos(), config.obs_dims);
        obstacles_.push_back(obstacle_);
        for(int i = 0; i < 15; i++)
        {
            Position random_pos = generate_pos();
            //Block initial_block = Block::stationary_block(random_pos);
            //blocks_.push_back(initial_block);
            generate_block(random_pos);
        }
        doodle_.doodle_pos = {config.top_left_startblock().x - config.doodle_dims.width/4,
                              config.top_left_startblock().y -config.doodle_dims.height};
        score = config.initial_score;
        is_game_over = true;
    }
}


/*void
Model::blockmove_down(Doodle doodle)
{
    if(doodle.doodle_pos.y < config.scene_dims.height)
    {
        for(auto block : blocks_)
        {

        }
    }
}*/

Block
Model::translate_velocity(Block b)
{
    b.y_acc = -1 * config.acceleration.height;
    b.block_velocity.height = -1 * doodle_.doodle_velocity.height;
    return b;
}

Obstacle
Model::obs_trans_vel(Obstacle obs)
{
    obs.y_acc = -1 * config.acceleration.height;
    obs.y_vel = -1 * doodle_.doodle_velocity.height;
    return obs;
}

void
Model::v_to_doodle(Block b)
{
    b.y_acc = -1 * config.acceleration.height;
    b.block_velocity.height = -1 * doodle_.doodle_velocity.height;
}

void
Model::doodle_0_y_motion()
{
    doodle_.doodle_acc.height = 0;
    doodle_.doodle_velocity.height = 0;
}

void
Model::remove_obstacle()
{
    for(auto ball : balls)
    {
        if(ball.hits_obs_ball(obstacle_))
            {
                obstacle_.posn = {-10, 1000};
            }
    }
}

