#include "headers.h"
#include "vao.h"
#include "block.h"
#include "brick.h"
#include "mirror.h"
#include "bullet.h"

float bullet_radius = 0.08f;
float bullet_small_triangles_width = 0.03f;
float bullet_small_triangles_precision = 120.0f;
float bullet_rotation_unit = 10.0f;
float bullet_speed = 0.15f;
float last_bullet_time;
float min_time_between_bullets = 0.4f;


Bullet::Bullet()
{
    this->initialize(0.0f, 0.0f, 0.01f, 0.01f, 0.0f);

    float w = bullet_small_triangles_width;
    float r = bullet_radius;

    int n = 360.0f / bullet_small_triangles_precision;
    for(int i=0; i<n; i++)
    {
        triangles.push_back(new Triangle());
        /*(triangles.back())->initialize(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(r,w,0.0f), glm::vec3(r,-w,0.0f),
            0.0f, 0.0f, -90.0f + i*bullet_small_triangles_precision, 1.0f, 1.0f, 1.0f);*/
        (triangles.back())->initialize(glm::vec3(0.0f,w,0.0f), glm::vec3(0.0f,-w,0.0f), glm::vec3(r,0.0f,0.0f),
            0.0f, 0.0f, -90.0f + i*bullet_small_triangles_precision, 1.0f, 1.0f, 1.0f);
    }

}

Bullet::Bullet(float x, float y, float vx, float vy)
{
    this->initialize(x, y, vx, vy, 0.0f);

    float w = bullet_small_triangles_width;
    float r = bullet_radius;

    int n = 360.0f / bullet_small_triangles_precision;
    for(int i=0; i<n; i++)
    {
        triangles.push_back(new Triangle());
        (triangles.back())->initialize(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(r,w,0.0f), glm::vec3(r,-w,0.0f),
            0.0f, 0.0f, -90.0f + i*bullet_small_triangles_precision, 1.0f, 1.0f, 1.0f);
    }
    this->last_hit = glfwGetTime();
}

void Bullet::update()
{
    Sprite::update();

    rotation += bullet_rotation_unit;
    while(rotation > 360.0f)
        rotation -= 360.0f;
}


bool Bullet::has_collided(Brick * that)
{

    //for(vector<Triangle*>::iterator i=(this->triangles).begin(); i!=(this->triangles).end(); i++)
    //    for(vector<Triangle*>::iterator j=(that->triangles).begin(); j!=(that->triangles).end(); j++)
    //        if(::has_collided(*i, make_pair(this->get_x(), this->get_y()), *j, make_pair(that->get_x(), that->get_y())))
    //            return true;

    for(vector<Block*>::iterator i=(this->blocks).begin(); i!=(this->blocks).end(); i++)
        for(vector<Block*>::iterator j=(that->blocks).begin(); j!=(that->blocks).end(); j++)
            if(::has_collided(*i, make_pair(this->get_x(), this->get_y()), *j, make_pair(that->get_x(), that->get_y())))
                return true;

    for(vector<Block*>::iterator i=(this->blocks).begin(); i!=(this->blocks).end(); i++)
        for(vector<Triangle*>::iterator j=(that->triangles).begin(); j!=(that->triangles).end(); j++)
            if(::has_collided(*i, make_pair(this->get_x(), this->get_y()), *j, make_pair(that->get_x(), that->get_y())))
                return true;

    for(vector<Triangle*>::iterator i=(this->triangles).begin(); i!=(this->triangles).end(); i++)
        for(vector<Block*>::iterator j=(that->blocks).begin(); j!=(that->blocks).end(); j++)
            if(::has_collided(*i, make_pair(this->get_x(), this->get_y()), *j, make_pair(that->get_x(), that->get_y())))
                return true;

    return false;
}

bool Bullet::has_collided(Mirror * that)
{

    for(vector<Triangle*>::iterator i=(this->triangles).begin(); i!=(this->triangles).end(); i++)
        for(vector<Block*>::iterator j=(that->blocks).begin(); j!=(that->blocks).end(); j++)
            if(::has_collided(*i, make_pair(this->get_x(), this->get_y()), *j, make_pair(that->get_x(), that->get_y()), that->get_rotation()))
            {
                float time_now = glfwGetTime();
                if( time_now - this->last_hit > 2.0f/32)
                {
                    this->last_hit = time_now;
                    return true;
                }
                else
                    ;
            }
    return false;
}

void Bullet::change_dir(Mirror * m)
{
    float vxx = this->get_vx();
    float vyy = this->get_vy();
    float theta = m->get_rotation();

    float phi = atan2(vyy, vxx)*180.0f/M_PI;
    float ang = 2.0f*theta - phi;
    //cout << theta << " " << phi << " " << ang << endl;
    float speed = sqrt(vxx*vxx + vyy*vyy);
    this->set_vx(speed * cos(ang * M_PI / 180.0f));
    this->set_vy(speed * sin(ang * M_PI / 180.0f));

}
