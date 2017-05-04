extern float block_area_per;
extern float brick_speed;
extern float min_x;
extern float max_x;
extern float max_y;
extern float min_y;

extern int red;
extern int blue;
extern int green;
extern int black;

#ifndef BRICK
#define BRICK
class Brick : public Sprite
{

    private:
        int color;
    public:
        Brick();
        int get_color();
};
#endif
