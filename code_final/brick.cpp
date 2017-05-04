#include "headers.h"
#include "vao.h"
#include "block.h"
#include "brick.h"


// Area percentage in which bricks are allowed
float brick_area_per = 80.0f;
float brick_speed = -0.015f;
float min_brick_speed = -0.005f;
float max_brick_speed = -0.027f;
float brick_speed_unit = 0.002f;

Brick::Brick()
{
    float left_ = (max_x - min_x) * (1.0f - brick_area_per/100.0f);
    float range = max_x - min_x - left_;
    float x = (double)rand() / RAND_MAX;
    x = x * range;
    x = x + min_x + left_;
    float y = max_y;
    int l = int(glfwGetTime())%3;
    float r, g, b;

    if(l==0) r = 1.0f, g = 0.0f, b = 0.0f, this->color = red;
    else if(l==1) r = 0.0f, g = 1.0f, b = 0.0f, this->color = green;
    else if(l==2) r = 0.0f, g = 0.0f, b = 0.0f, this->color = black;
    this->initialize(x, y, 0.0f, brick_speed, 0.0f);

    blocks.push_back(new Block());
    (blocks.back())->initialize(0.0f, 0.0f, 0.0f, 0.0f, default_block_width, default_block_height, 0.0f, r, g, b);
}

int Brick::get_color()
{
    return color;
}
