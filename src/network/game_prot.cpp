#include "game_prot.hpp"

using namespace game_prot;

json game_prot::do_move_json(Square from, Square to,
    PieceColor player_c, int player_id) {
    json j;

    j["type"] = action_type::act_do_move;
    j["from.x"] = from.x;
    j["from.y"] = from.y;
    j["to.x"] = to.x;
    j["to.y"] = to.y;
    j["player_color"] = player_c;
    j["player_id"] = player_id;

    return j;
}

do_move_info game_prot::parse_do_move(json j) {
    do_move_info info;
    
    info.from = { j["from.x"], j["from.y"] };
    info.to = { j["to.x"], j["to.y"] };
    info.player_c = j["player_color"];
    info.player_id = j["player_id"];

    return info;
}

json game_prot::do_move_resp_json(Square from, Square to, move_result res) {
    json j;

    j["type"] = action_type::act_do_move_resp;
    j["from.x"] = from.x;
    j["from.y"] = from.y;
    j["to.x"] = to.x;
    j["to.y"] = to.y;
    j["move_result"] = res;

    return j;
}

do_move_resp_info game_prot::parse_do_move_resp(json j) {
    do_move_resp_info info;
    
    info.from = { j["from.x"], j["from.y"] };
    info.to = { j["to.x"], j["to.y"] };
    info.res = j["move_result"];

    return info;
}

json game_prot::create_room_json(string name, PieceColor player_c) {
    json j;

    j["type"] = action_type::act_create_room;
    j["player_color"] = player_c;
    j["room_name"] = name;

    return j;
}

create_room_info game_prot::parse_create_room(json j) {
    create_room_info info;
    
    info.room_name = j["room_name"];
    info.player_c = j["player_color"];

    return info;    
}

json game_prot::create_room_resp_json(int player_id) {
    json j;

    j["type"] = action_type::act_create_room_resp;
    j["player_id"] = player_id;

    return j;
}

create_room_resp_info game_prot::parse_create_room_resp(json j) {
    create_room_resp_info info;
    
    info.player_id = j["player_id"];

    return info;    
}

json game_prot::join_room_json(string name) {
    json j;

    j["type"] = action_type::act_join_room;
    j["room_name"] = name;

    return j;
}

join_room_info game_prot::parse_join_room(json j) {
    join_room_info info;
    
    info.room_name = j["room_name"];

    return info;    
}

json game_prot::join_room_resp_json(PieceColor player_c, int player_id) {
    json j;

    j["type"] = action_type::act_join_room_resp;
    j["player_color"] = player_c;
    j["player_id"] = player_id;

    return j;
}

join_room_resp_info game_prot::parse_join_room_resp(json j) {
    join_room_resp_info info;
    
    info.player_c = j["player_color"];
    info.player_id = j["player_id"];

    return info;    
}