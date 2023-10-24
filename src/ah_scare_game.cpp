#include "ah_scare_game.h"

#include "bn_core.h"
#include "bn_keypad.h"

#include "bn_regular_bg_ptr.h"

#include "mj/mj_game_list.h"

#include "bn_sprite_animate_actions.h"
#include "bn_sprite_actions.h"
#include "bn_regular_bg_items_ah_background.h"
#include "bn_regular_bg_items_ah_background_dark.h"
#include "bn_regular_bg_items_tmg_press_a.h"
#include "bn_regular_bg_items_tmg_you_lose.h"
#include "bn_regular_bg_items_tmg_you_win.h"
#include "bn_regular_bg_items_tree.h"

#include "bn_sprite_items_ah_girl.h"

MJ_GAME_LIST_ADD(ah_scare::test_game)

namespace ah_scare
{

test_game::test_game(int completed_games, const mj::game_data& data) :
    _bg(bn::regular_bg_items::ah_background.create_bg((256 - 240) / 2, (256 - 160) / 2)),
    _character_sprite(bn::sprite_items::ah_girl.create_sprite(0, 0)),
    _action(bn::create_sprite_animate_action_forever(
                    _character_sprite, 8, bn::sprite_items::ah_girl.tiles_item(), 0, 1, 2, 3))
{
    constexpr int frames_diff = maximum_frames - minimum_frames;
    constexpr int maximum_speed_completed_games = 30;

    completed_games = bn::min(completed_games, maximum_speed_completed_games);

    int frames_reduction = (frames_diff * completed_games) / maximum_speed_completed_games;
    _total_frames = maximum_frames - frames_reduction;
    _total_frames -= data.random.get_int(60);
    _total_frames = bn::clamp(_total_frames, minimum_frames, maximum_frames);

    bn::point character_map_position(16, 16);
    _character_map_position = character_map_position;
    _character_sprite.set_position(60, _character_map_position.y()); // 0, 0
}

void test_game::fade_in([[maybe_unused]] const mj::game_data& data)
{
}

mj::game_result test_game::play(const mj::game_data& data)
{
    mj::game_result result;
    result.exit = data.pending_frames == 0;

    if (!_is_dark) {
        int off = data.random.get_int(100);
        if (off > 98) {
            _is_dark = true;
        }
    }
    
    if (_is_dark) {
        _bg.set_item(bn::regular_bg_items::ah_background_dark);
    } else {
        _bg.set_item(bn::regular_bg_items::ah_background);
    }

    if(! _victory && ! _defeat)
    {
        if (_is_dark) {
            bn::fixed character_sprite_x =  _character_sprite.x() + 0.75;
            if (character_sprite_x > 100) {
                character_sprite_x = 100;
            }
            _character_sprite.set_position(character_sprite_x, 0);
            _character_sprite.set_horizontal_flip(true);
        } else {
            bn::fixed character_sprite_x = _character_sprite.x() - 0.5;
            _character_sprite.set_position(character_sprite_x, 0);
            _character_sprite.set_horizontal_flip(false);
        }

        if (_character_sprite.x() < -30) {
            result.remove_title = true;
            _victory = true;
        }
        
        _action.update();
        // if(bn::keypad::a_pressed() && data.pending_frames > 60 && data.pending_frames < 180)
        // {
        //     _bg.set_item(bn::regular_bg_items::tmg_you_win);
        //     result.remove_title = true;
        //     _victory = true;
        // }
        // else if(! bn::keypad::start_pressed() && bn::keypad::a_pressed() && (data.pending_frames < 60 || data.pending_frames > 180))
        // {
        //     _bg.set_item(bn::regular_bg_items::tmg_you_lose);
        //     result.remove_title = true;
        //     _defeat = true;
        // }

        if(bn::keypad::r_pressed())
        {
            _is_dark = false;
            _bg.set_item(bn::regular_bg_items::ah_background);
        }
    }
    else
    {
        if(_show_result_frames)
        {
            --_show_result_frames;
        }
        else
        {
            result.exit = true;
        }
    }

    return result;
}

void test_game::fade_out([[maybe_unused]] const mj::game_data& data)
{
}

}
