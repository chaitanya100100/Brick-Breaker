extern float canon_half_round_small_triangles_precision;
extern float canon_half_round_small_triagles_width;
extern float canon_half_round_radius;
extern float canon_funnel_width;
extern float canon_funnel_length;
extern float canon_funnel_rotation_unit;
extern float canon_move_speed;

extern float bullet_speed;

extern GLFWwindow* window;

#ifndef CANON
#define CANON
class Canon : public Sprite
{
    private:
        float funnel_rotation;

    public:
        Canon();
        void update_funnel_rotation(float degrees);
        void set_funnel_rotation(float r);
        pair<pair<float, float>, pair<float, float> > get_bullet_start();
        void update();
        bool on_canon();
        bool on_funnel();
};
#endif

void get_coords(double * x, double * y);
