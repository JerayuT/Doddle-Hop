#pragma once

#include "model.hxx"

struct View
{
    void draw(ge211::Sprite_set& set);

    ge211::Font sans30{"sans.ttf", 20};

public:
    using Dimensions = ge211::Dims<int>;

    explicit View(Model const& model);

    ge211::Rectangle_sprite const stationary_block_sprite;
    ge211::Rectangle_sprite const moving_block_sprite;
    ge211::Rectangle_sprite const breakable_block_sprite;
    ge211::Rectangle_sprite const obstacle_sprite;
    ge211::Circle_sprite const ball_sprite;
    ge211::Text_sprite score_sprite;
    ge211::Text_sprite game_over_sprite1;
    ge211::Text_sprite game_over_sprite2;


    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;


private:
    Model const& model_;
};
