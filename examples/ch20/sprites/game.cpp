// game.cpp — ходьба, двери, предметы
#include "game.hpp"

#include "art.hpp"
#include "raycast.hpp"
#include "sprites.hpp"
#include "topview.hpp"
#include "view3d.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <utility>

namespace doom {

namespace {
constexpr double kWalk = 3.0;      // клеток в секунду
constexpr double kRun = 5.5;
constexpr double kTurn = 2.5;      // радиан в секунду — стрелками
constexpr double kMouse = 0.0025;  // радиан на единицу мыши
constexpr double kRadius = 0.25;   // половина ширины игрока
constexpr double kReach = 2.0;     // до какой двери дотянемся
constexpr double kDoorSpeed = 2.0; // открывается за 0,5 с
constexpr double kDoorWait = 3.0;  // столько стоит открытой
constexpr double kPickup = 0.5;    // с какого расстояния подбираем
constexpr double kMessage = 2.0;   // сколько секунд висит сообщение
} // namespace

Game::Game(Map map) : map_(std::move(map)) {
    const Cell start = map_.find('P').value_or(Cell{1, 1});
    x_ = start.x + 0.5;            // встаём в центр клетки
    y_ = start.y + 0.5;
    // Буквы h и a на карте — предметы. Кладём их в центры клеток.
    for (int cy = 0; cy < map_.height(); ++cy) {
        for (int cx = 0; cx < map_.width(); ++cx) {
            const char c = map_.at(cx, cy);
            if (c == 'h' || c == 'a') {
                const auto kind = c == 'h' ? ItemKind::Medkit
                                           : ItemKind::Clip;
                items_.push_back({kind, cx + 0.5, cy + 0.5});
            }
        }
    }
}

void Game::update(const Input& in, double dt) {
    if (in.map) {
        showMap_ = !showMap_;
    }
    if (in.use) {
        useDoor();
    }
    updateDoors(dt);
    pickUpItems();
    messageTime_ = std::max(0.0, messageTime_ - dt);
    if (messageTime_ == 0.0) {
        message_.clear();
    }
    const double turn = (in.turnRight ? 1.0 : 0.0) -
                        (in.turnLeft ? 1.0 : 0.0);
    angle_ += turn * kTurn * dt + in.mouseDX * kMouse;
    angle_ = std::remainder(angle_, 2.0 * std::numbers::pi);

    const double fwd = (in.forward ? 1.0 : 0.0) -
                       (in.back ? 1.0 : 0.0);
    const double side = (in.strafeRight ? 1.0 : 0.0) -
                        (in.strafeLeft ? 1.0 : 0.0);
    const double dirX = std::cos(angle_);       // куда смотрим
    const double dirY = std::sin(angle_);
    // Вперёд — вдоль (dirX, dirY), вправо — вдоль (-dirY, dirX).
    const double mx = dirX * fwd - dirY * side;
    const double my = dirY * fwd + dirX * side;
    const double len = std::hypot(mx, my);
    if (len > 0.0) {
        const double step = (in.run ? kRun : kWalk) * dt / len;
        tryMove(mx * step, my * step);
    }
}

bool Game::blocked(double x, double y) const {
    // Игрок — квадратик 2r × 2r. Проверяем все клетки под ним.
    const int x0 = static_cast<int>(std::floor(x - kRadius));
    const int x1 = static_cast<int>(std::floor(x + kRadius));
    const int y0 = static_cast<int>(std::floor(y - kRadius));
    const int y1 = static_cast<int>(std::floor(y + kRadius));
    for (int cy = y0; cy <= y1; ++cy) {
        for (int cx = x0; cx <= x1; ++cx) {
            if (map_.isSolid(cx, cy)) {
                return true;
            }
        }
    }
    return false;
}

bool Game::touches(int cx, int cy) const {
    // Задевает ли квадратик игрока клетку (cx, cy)?
    const auto cell = [](double v) {
        return static_cast<int>(std::floor(v));
    };
    return cell(x_ - kRadius) <= cx && cx <= cell(x_ + kRadius) &&
           cell(y_ - kRadius) <= cy && cy <= cell(y_ + kRadius);
}

void Game::tryMove(double dx, double dy) {
    // По осям по очереди: упёрлись по x — по y всё равно едем.
    if (!blocked(x_ + dx, y_)) {
        x_ += dx;
    }
    if (!blocked(x_, y_ + dy)) {
        y_ += dy;
    }
}

void Game::useDoor() {
    // Луч прямо по взгляду: если первой стоит дверь и она близко —
    // открываем.
    const RayHit hit =
        castRay(map_, x_, y_, std::cos(angle_), std::sin(angle_));
    Door* door = map_.door(hit.cellX, hit.cellY);
    if (door != nullptr && hit.dist < kReach) {
        door->move = 1;
    }
}

void Game::updateDoors(double dt) {
    for (int cy = 0; cy < map_.height(); ++cy) {
        for (int cx = 0; cx < map_.width(); ++cx) {
            Door* d = map_.door(cx, cy);
            if (d == nullptr) {
                continue;
            }
            if (d->move > 0) {                       // открывается
                d->open = std::min(1.0, d->open + kDoorSpeed * dt);
                if (d->open >= 1.0) {
                    d->move = 0;
                    d->wait = kDoorWait;
                }
            } else if (d->move < 0) {                // закрывается
                if (touches(cx, cy)) {
                    d->move = 1;                     // не прищемить!
                    continue;
                }
                d->open = std::max(0.0, d->open - kDoorSpeed * dt);
                if (d->open <= 0.0) {
                    d->move = 0;
                }
            } else if (d->open >= 1.0) {          // стоит открытой
                d->wait -= dt;
                if (d->wait <= 0.0 && !touches(cx, cy)) {
                    d->move = -1;
                }
            }
        }
    }
}

void Game::pickUpItems() {
    for (Item& item : items_) {
        const double dx = item.x - x_;
        const double dy = item.y - y_;
        if (item.taken || dx * dx + dy * dy > kPickup * kPickup) {
            continue;
        }
        if (item.kind == ItemKind::Medkit) {
            if (health_ >= 100) {
                continue;           // здоров — аптечка лежит дальше
            }
            health_ = std::min(100, health_ + 25);
            message_ = "PICKED UP A MEDIKIT";
        } else {
            ammo_ += 10;
            message_ = "PICKED UP A CLIP";
        }
        item.taken = true;
        messageTime_ = kMessage;
    }
}

void Game::render(Canvas& canvas) const {
    if (showMap_) {
        drawTopView(canvas, map_, x_, y_, angle_);
        return;
    }
    const auto w = static_cast<std::size_t>(canvas.width());
    std::vector<double> zbuf(w);    // расстояние до стены в столбце
    drawView(canvas, map_, x_, y_, angle_, zbuf);
    std::vector<Sprite> sprites;
    for (const Item& item : items_) {
        if (!item.taken) {
            const Image* img = &itemImage(item.kind);
            sprites.push_back({item.x, item.y, img});
        }
    }
    drawSprites(canvas, std::move(sprites), {x_, y_, angle_}, zbuf);
}

} // namespace doom
