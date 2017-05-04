#include "headers.h"
#include "vao.h"
#include "block.h"
#include "canon.h"

float canon_half_round_small_triangles_precision = 5.0f;
float canon_half_round_small_triagles_width = 0.1f;
float canon_half_round_radius = 0.40f;
float canon_funnel_width = 0.1f;
float canon_funnel_length = 0.80f;
float canon_funnel_rotation_unit = 1.0f;
float canon_move_speed = 0.05f;

Canon::Canon()
{
    this->funnel_rotation = 0.0f;
    this->initialize(min_x, 0.0f, 0.0f, 0.0f, 0.0f);

    float w = canon_half_round_small_triagles_width;
    int n = 180.0f / canon_half_round_small_triangles_precision;
    for(int i=0; i<n; i++)
    {
        triangles.push_back(new Triangle());
        (triangles.back())->initialize(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(canon_half_round_radius,w,0.0f), glm::vec3(canon_half_round_radius,-w,0.0f),
            0.0f, 0.0f, -90.0f + i*canon_half_round_small_triangles_precision, 0.0f, 0.0f, 1.0f);
    }

    blocks.push_back(new Block());
    (blocks.back())->initialize( 0.0f, -canon_funnel_width/2.0f, 0.0f, 0.0f, canon_funnel_length, canon_funnel_width, 0.0f, 0.0f, 0.0f, 1.0f);

}

void Canon::update_funnel_rotation(float degrees)
{
    funnel_rotation += degrees;
    while(funnel_rotation > 360.0f)
        rotation -= 360.0f;
    (blocks.back())->update_rotation(degrees);
}

void Canon::set_funnel_rotation(float r)
{
    funnel_rotation = r;
    (blocks.back())->set_rotation(r);
}

void Canon::update()
{
    Sprite::update();
    if(y + canon_half_round_radius > max_y) y = max_y - canon_half_round_radius;
    else if(y - canon_half_round_radius < min_y) y = min_y + canon_half_round_radius;
}

pair<pair<float, float>, pair<float, float> > Canon::get_bullet_start()
{
    float x_ = this->x + canon_funnel_length * cos(this->funnel_rotation * M_PI / 180.0f);
    float y_ = this->y + canon_funnel_length * sin(this->funnel_rotation * M_PI / 180.0f);
    float vx_ = /*this->vx +*/ bullet_speed * cos(this->funnel_rotation * M_PI / 180.0f);
    float vy_ = /*this->vy +*/ bullet_speed * sin(this->funnel_rotation * M_PI / 180.0f);
    return make_pair(make_pair(x_, y_), make_pair(vx_, vy_));
}

bool Canon::on_canon()
{
    double xx, yy;
    get_coords(&xx, &yy);

    if(sqrt((xx-x)*(xx-x) + (y-yy)*(y-yy)) <= canon_half_round_radius )
        return true;
    else
        return false;
}
bool Canon::on_funnel()
{
    double xx, yy;
    get_coords(&xx, &yy);
    double dist = sqrt((xx-x)*(xx-x) + (y-yy)*(y-yy));
    if(dist <= canon_funnel_length && dist >= canon_half_round_radius)
    {
        double theta = atan2(yy-y, xx-x) * 180.0f / M_PI;
        double delta = atan2(canon_funnel_width/2, canon_funnel_length) * 180.0f / M_PI;
        delta *= 2.0f;
        if(funnel_rotation + delta >= theta && funnel_rotation - delta <= theta)
            return true;
    }
    return false;
}
