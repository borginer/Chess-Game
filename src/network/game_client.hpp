#include "raylib.h"
#include <vector>
#include <cmath>
#include <functional>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <nlohmann/json.hpp>

#include "../engine/chess_game.hpp"
#include "../interface/graphics.hpp"

#include "game_prot.hpp"