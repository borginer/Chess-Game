#include "raylib.h"
#include <vector>
#include <cmath>
#include <functional>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <nlohmann/json.hpp>

#include "../engine/chess_game.hpp"
#include "../interface/graphics.hpp"

namespace wspp = websocketpp;
using std::vector;
using client = wspp::client<websocketpp::config::asio_client>;
using wspp::connection_hdl;
using msg_ptr = client::message_ptr;
using std::cout, std::cerr, std::endl;
using json = nlohmann::json;