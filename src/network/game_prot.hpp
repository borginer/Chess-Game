#include <nlohmann/json.hpp>
#include "../engine/chess_game.hpp"

#include <string>

using json = nlohmann::json;

namespace game_prot {
    enum action_type {
        act_create_room = 0,
        act_create_room_resp,
        act_join_room,
        act_join_room_resp,
        act_do_move,
        act_do_move_resp,
        act_leave_room,
    };

    struct do_move_info {
        Square from; Square to;
        PieceColor player_c;
        int player_id;
    };

    struct do_move_resp_info {
        Square from; Square to;
        move_result res;
    };

    struct create_room_info {
        string room_name;
        PieceColor player_c;
    };

    struct create_room_resp_info {
        int player_id;
    };

    struct join_room_info {
        string room_name;
    };

    struct join_room_resp_info {
        PieceColor player_c;
        int player_id;
    };

    json do_move_json(Square from, Square to,
        PieceColor player_c, int player_id);
    do_move_info parse_do_move(json j);

    json do_move_resp_json(Square from, Square to, move_result res);
    do_move_resp_info parse_do_move_resp(json j);

    json create_room_json(string name, PieceColor player_c);
    create_room_info parse_create_room(json j);

    json create_room_resp_json(int player_id);
    create_room_resp_info parse_create_room_resp(json j);

    json join_room_json(string name);
    join_room_info parse_join_room(json j);

    json join_room_resp_json(PieceColor player_c, int player_id);
    join_room_resp_info parse_join_room_resp(json j);

}

