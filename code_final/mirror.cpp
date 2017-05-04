#include "headers.h"
#include "vao.h"
#include "block.h"
#include "mirror.h"

float mirror_length = 1.0f;
float mirror_breadth = 0.08f;


Mirror::Mirror(float x, float y, float rotation)
{
    this->initialize(x, y, 0.0f, 0.01f, rotation);
    blocks.push_back(new Block());
    (blocks.back())->initialize(0.0f, 0.0f, 0.0f, 0.0f, mirror_length, mirror_breadth, 0.0f, 0.0f, 0.0f, 0.0f);
}

void Mirror::update()
{
    Sprite::update();
    rotation += 0.1f;
    //cout << vy << " " << y << endl;
    if(y>2.0f || y<-2.0f)
        vy *= -1;
}
