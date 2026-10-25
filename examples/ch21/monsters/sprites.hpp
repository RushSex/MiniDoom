// sprites.hpp — плоские картинки в трёхмерном мире
#pragma once

#include "canvas.hpp"
#include "image.hpp"

#include <span>
#include <vector>

namespace doom {

struct Sprite {
    double x = 0.0;               // где стоит на карте
    double y = 0.0;
    const Image* image = nullptr;
    double depth = 0.0;           // расстояние вдоль взгляда
};

struct Camera {
    double x = 0.0;               // где стоит игрок
    double y = 0.0;
    double angle = 0.0;
};

// zbuf — расстояние до стены в каждом столбце экрана.
void drawSprites(Canvas& canvas, std::vector<Sprite> sprites,
                 const Camera& cam, std::span<const double> zbuf);

} // namespace doom
