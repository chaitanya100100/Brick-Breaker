extern Canon * c;
extern set<Brick*> a;
extern set<Bullet*> b;
extern float time_now;
extern float last_bullet_time;
extern bool mypause;
extern bool keep_firing;
extern int canon_move;
extern int funnel_rotation;
extern int bucket_move[2];
extern float zoom_factor;
extern float ortho;
extern float min_brick_speed;
extern float max_brick_speed;
extern float brick_speed;
extern float brick_speed_unit;
extern int selected;

extern float hor_shift;
extern float ver_shift;

extern float pan_unit;

void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods);
void keyboardChar (GLFWwindow* window, unsigned int key);
void mouseButton (GLFWwindow* window, int button, int action, int mods);
void reshapeWindow (GLFWwindow* window, int width, int height);
void quit(GLFWwindow *window);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void scroll(GLFWwindow* window, double xpos, double ypos);
