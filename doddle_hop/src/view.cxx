#include "view.hxx"

static ge211::Color const stationary {0, 255, 0};
static ge211::Color const moving {0 ,0 , 255};
static ge211::Color const breakable {255, 0, 0};
static ge211::Color const obstacle {255, 255, 255};
static ge211::Color const ball {255, 255, 255};


View::View(Model const& model)
        :   stationary_block_sprite(model.config.block_dims,stationary),
            moving_block_sprite(model.config.block_dims,moving),
            breakable_block_sprite(model.config.block_dims,breakable),
            obstacle_sprite(model.config.obs_dims, obstacle),
            ball_sprite(model.config.ball_radius, ball),
            model_(model)
{ }

void
View::draw(ge211::Sprite_set& set)
{
    initial_window_dimensions();
    initial_window_title();
    if(model_.get_game_over())
    {
        ge211::Text_sprite::Builder game_over_builder1(sans30);
        game_over_builder1.color({255,255,255}) << "GAME OVER";
        game_over_sprite1.reconfigure(game_over_builder1);
        set.add_sprite(game_over_sprite1, {80,400},4);
        ge211::Text_sprite::Builder game_over_builder2(sans30);
        game_over_builder2.color({255,255,255}) << "Restart By Pressing Spacebar";
        game_over_sprite2.reconfigure(game_over_builder2);
        set.add_sprite(game_over_sprite2, {15,450},4);

    }
    else {
        ge211::Text_sprite::Builder score_builder(sans30);
        score_builder.color({255,255,255}) << model_.score;
        score_sprite.reconfigure(score_builder);
        set.add_sprite(score_sprite, {20,760},1);
        ge211::Transform my_transform =
                ge211::Transform{}
                        .scale(0.5);
        ge211::Transform my_transform2 =
                ge211::Transform{}.scale(0.08);
        set.add_sprite(model_.doodle().get_sprite(), model_.doodle().doodle_pos, 2, my_transform);
        ///set.add_sprite(score_sprite,{150,100},1);
        for (auto block: model_.get_block()) {
            if (block.type == 1) {
                set.add_sprite(stationary_block_sprite, block.block_pos);
            } else if (block.type == 2) {
                set.add_sprite(moving_block_sprite, block.block_pos);
            } else if (block.type == 3) {
                set.add_sprite(breakable_block_sprite, block.block_pos);
            }
        }
        set.add_sprite(model_.doodle().get_obs_sprite(), model_.obstacle_.posn, 1, my_transform2);

        for (auto ball: model_.get_ball()) {
            set.add_sprite(ball_sprite, ball.posn, 3);
        }
    }
}

View::Dimensions
View::initial_window_dimensions() const
{
    // You can change this if you want:
    return model_.config.scene_dims;
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Doddle Hop";
}
