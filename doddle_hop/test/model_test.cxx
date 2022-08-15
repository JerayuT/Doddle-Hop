#include "model.hxx"
#include "game_config.hxx"
#include <catch.hxx>

TEST_CASE("example test (TODO: replace this)")
{
    CHECK(1 + 1 == 2);
}

//
//
// These tests should demonstrate your functional requirements.
//
TEST_CASE("game over")
{
    Model m;
    m.launch();
    m.get_doodle_test().set_position({100,900});
    m.game_over();
    CHECK(m.get_is_game_over() == true);
}

TEST_CASE("shootball")
{
    Model m;
    double dt = 0.1;
    m.launch();
    CHECK(m.balls.empty());
    CHECK(m.doodle().get_shoot() == false);
    m.set_shoot();
    CHECK(m.doodle().get_shoot() == true);
    m.on_frame(0.1);
    CHECK(!m.balls.empty());
    m.balls[0].posn = {100,300};
    m.obstacle_ = Obstacle({m.config, {100, 100},m.config.obs_dims});
    CHECK(m.balls[0].hits_obs_ball(m.obstacle_) == false);
    m.on_frame(dt);
    m.on_frame(dt);
    m.on_frame(dt);
    CHECK(m.obstacle_.posn.x != 100);
    CHECK(m.obstacle_.posn.y != 100);
}

TEST_CASE("move screen")
{
    Model m;
    ge211::geometry::Dims<int> scene_dims = m.config.scene_dims;
    m.get_doodle_test().doodle_pos = {scene_dims.width/2,
                                      scene_dims.height/2 + 1};
    m.launch();
    std::vector<Block> blocks = m.get_block();
    CHECK(blocks[0].block_velocity.height == 0);
    m.on_frame(0.1);
    m.on_frame(0.1);
    m.on_frame(0.1);
    //m.on_frame(0.001);
    for (auto& block : m.get_block())
    {
        CHECK(block.block_velocity.height > 0);
    }
    CHECK(m.get_block()[0].y_acc == -1 * m.config.acceleration.height);
}

TEST_CASE("move doodle")
{
    Model m;
    m.launch();
    Position initial_pos = m.get_doodle_test().doodle_pos;

    CHECK(m.get_block()[0].block_velocity.height == 0);
    m.on_frame(0.1);
    CHECK(initial_pos.y > m.get_doodle_test().doodle_pos.y);

    CHECK(m.get_doodle_test().doodle_velocity.height < 0);
    m.on_frame(0.1);
    CHECK(initial_pos.y > m.get_doodle_test().doodle_pos.y);

    CHECK(m.get_doodle_test().doodle_velocity.height < 0);
    m.on_frame(0.1);
    CHECK(initial_pos.y > m.get_doodle_test().doodle_pos.y);

    CHECK(m.get_doodle_test().doodle_velocity.height < 0);
    CHECK(initial_pos.y > m.get_doodle_test().doodle_pos.y);

    for (auto& block : m.get_block())
    {
        CHECK(block.block_velocity.height == 0);
    }
    CHECK(m.get_doodle_test().doodle_velocity.height >
    m.config.doodle_velocity_0.height);
}

TEST_CASE("block_off screen")
{
    Model m;
    m.get_block_test()[1].block_pos = {150, 1000};
    m.launch();
    m.on_frame(0.01);
    CHECK(m.get_block_test()[1].block_pos.y != 1000);
    CHECK(m.get_block_test().size() == 15);
    /*CHECK(m.get_block()[0].block_pos.y < m.config.scene_dims.height);
    CHECK(m.get_block()[1].block_pos.y < m.config.scene_dims.height);
    CHECK(m.get_block()[2].block_pos.y < m.config.scene_dims.height);
    m.on_frame(0.1);
    CHECK(m.get_block()[0].block_pos.y < m.config.scene_dims.height);
    CHECK(m.get_block()[1].block_pos.y < m.config.scene_dims.height);
    CHECK(m.get_block()[2].block_pos.y < m.config.scene_dims.height);
    m.get_block_test()[0].block_pos.y = 900;
    Position block1pos = m.get_block_test()[0].block_pos;
    m.get_block_test()[1].block_pos.y = 1000;
    Position block2pos = m.get_block_test()[1].block_pos;
    m.get_block_test()[2].block_pos.y = 1300;
    Position block3pos = m.get_block_test()[2].block_pos;

    Position block13pos = m.get_block_test()[13].block_pos;
    Position block14pos = m.get_block_test()[14].block_pos;
    Position block15pos = m.get_block_test()[15].block_pos;


    m.on_frame(0.1);
    m.on_frame(0.1);

    CHECK(block1pos.x != m.get_block_test()[0].block_pos.x);
    CHECK(block1pos.y != m.get_block_test()[0].block_pos.y);
    CHECK(block2pos.x != m.get_block_test()[1].block_pos.x);
    CHECK(block2pos.y != m.get_block_test()[1].block_pos.y);
    CHECK(block3pos.x != m.get_block_test()[2].block_pos.x);
    CHECK(block3pos.y != m.get_block_test()[2].block_pos.y);

    CHECK(block13pos.x != m.get_block_test()[13].block_pos.x);
    CHECK(block13pos.y != m.get_block_test()[13].block_pos.y);
    CHECK(block14pos.x != m.get_block_test()[14].block_pos.x);
    CHECK(block14pos.y != m.get_block_test()[14].block_pos.y);
    CHECK(block15pos.x != m.get_block_test()[15].block_pos.x);
    CHECK(block15pos.y != m.get_block_test()[15].block_pos.y);*/

}

TEST_CASE("obstacle game over")
{
    Model m;
    m.obstacle_.posn = {m.doodle().doodle_pos.x,
                        m.doodle().doodle_pos.y - 50};
    m.launch();
    m.on_frame(0.1);
    m.on_frame(0.1);
    CHECK(!m.get_doodle_test().live);
    CHECK((m.get_is_game_over()));
}