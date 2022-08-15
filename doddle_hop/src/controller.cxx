#include "controller.hxx"

Controller::Controller()
        : model_(),
        view_(model_)
{ }

void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set);
}

void
Controller::on_frame(double dt)
{
    model_.on_frame(dt);
}

void
Controller::on_key(ge211::Key key)
{
    if (key == ge211::Key::right())
    {
        model_.set_right();
    }
    else if (key == ge211::Key::left())
    {
        model_.set_left();
    }

    if (key == ge211::Key::code(' '))
    {
        if(model_.doodle().live)
        {
            model_.set_shoot();
            model_.set_shoot_motion();
        }
        model_.launch();
    }

    if (key == ge211::Key::up())
    {
        model_.launch();
    }
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}