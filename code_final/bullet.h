extern float bullet_radius;
extern float bullet_small_triangles_width;
extern float bullet_small_triangles_precision;
extern float bullet_rotation_unit;
extern float bullet_speed;
extern float last_bullet_time;
extern float min_time_between_bullets;
class Mirror;
#ifndef BULLET
#define BULLET
class Bullet : public Sprite
{
    private:
        float last_hit;
    public:
        Bullet();
        Bullet(float x, float y, float vx, float vy);
        void update();
        bool has_collided(Brick * that);
        bool has_collided(Mirror * that);
        void change_dir(Mirror * m);
};
#endif

bool has_collided(Block * & a, pair<float, float> ao, Block * & b, pair<float, float> bo);
bool has_collided(Block * & a, pair<float, float> ao, Triangle * & b, pair<float, float> bo);
bool has_collided(Triangle * & b, pair<float, float> bo, Block * & a, pair<float, float> ao);
bool has_collided(Triangle * & b, pair<float, float> bo, Block * & a, pair<float, float> ao, float r);
bool has_collided(Block * & a, pair<float, float> ao, Triangle * & b, pair<float, float> bo, float r);
