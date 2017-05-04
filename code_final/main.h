extern struct GLMatrices Matrices;
extern GLuint programID;
extern float triangle_rot_dir;
extern float rectangle_rot_dir;
extern bool triangle_rot_status;
extern bool rectangle_rot_status;
extern VAO *triangle, *rectangle;
extern float camera_rotation_angle;
extern float rectangle_rotation;
extern float triangle_rotation;


extern const char GameHeading[10];
extern int window_width;
extern int window_height;
extern float max_x;
extern float min_x;
extern float max_y;
extern float min_y;
extern float hor_shift;
extern float ver_shift;


extern int selected;

extern set<Brick*> a;
extern Canon * c;
extern set<Bullet*> b;
extern vector<Bucket*> d;

extern glm::mat4 world_model_mat;


void draw ();
void initGL (GLFWwindow* window, int width, int height);
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
static void error_callback(int error, const char* description);
void quit(GLFWwindow *window);
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL);
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL);
void draw3DObject (struct VAO* vao);
GLFWwindow* initGLFW (int width, int height);
