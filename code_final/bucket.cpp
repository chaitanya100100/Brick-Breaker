#include "headers.h"
#include "vao.h"
#include "block.h"
#include "brick.h"
#include "bucket.h"


Bucket::Bucket(int color)
{
    this->color = color;
    this->collection = 0;

    this->initialize(0.0f, min_y, 0.0f, 0.0f, 0.0f);

    blocks.push_back(new Block());
    if(color == red)
        (blocks.back())->initialize( 0.0f, 0.0f, 0.0f, 0.0f, bucket_width, bucket_height, 0.0f, 1.0f, 0.0f, 0.0f);
    else if(color == green)
        (blocks.back())->initialize( 0.0f, 0.0f, 0.0f, 0.0f, bucket_width, bucket_height, 0.0f, 0.0f, 1.0f, 0.0f);
    else
    {
        cout << "Color not specified" << endl;
        exit(EXIT_FAILURE);
    }
}

bool Bucket::has_collided(Brick * that)
{
    for(vector<Block*>::iterator i=(this->blocks).begin(); i!=(this->blocks).end(); i++)
        for(vector<Block*>::iterator j=(that->blocks).begin(); j!=(that->blocks).end(); j++)
            if(::has_collided(*i, make_pair(this->get_x(), this->get_y()), *j, make_pair(that->get_x(), that->get_y())))
                return true;
    return false;
}
void Bucket::update()
{
    Sprite::update();

    if(x + blocks[0]->get_width() > max_x) x = max_x - blocks[0]->get_width();
    else if(x < min_x) x = min_x;
}

int Bucket::get_color()
{
    return color;
}

void Bucket::update_collection(int x)
{
    collection += x;
}

int Bucket::get_collection()
{
    return collection;
}
