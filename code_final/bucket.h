
extern int red;
extern int blue;
extern int green;
extern int black;
extern float bucket_height;
extern float bucket_width;
extern float bucket_speed;

extern int px, wid;
extern float width;
extern float ortho;

#ifndef BUCKET
#define BUCKET
class Bucket : public Sprite
{
    private:
        int color;
        int collection;
    public:
        Bucket(int color);
        bool has_collided(Brick * that);
        void update();
        int get_color();
        void update_collection(int x);
        int get_collection();
};
#endif

bool has_collided(Block * & a, pair<float, float> ao, Block * & b, pair<float, float> bo);
