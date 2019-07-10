extern float mirror_length;
extern float mirror_breadth;

#ifndef MIRROR
#define MIRROR
class Mirror : public Sprite
{
    /*
    private:
    */
    public:
        Mirror();
        Mirror(float x, float y, float rotation);
        void update();
};
#endif
