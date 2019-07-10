
extern float default_block_width;
extern float default_block_height;
extern float default_block_red;
extern float default_block_green;
extern float default_block_blue;

extern float allowed_x_out;
extern float allowed_y_out;


extern float min_x;
extern float max_x;
extern float max_y;
extern float min_y;

extern struct GLMatrices Matrices;

struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode);
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode);
void draw3DObject (struct VAO* vao);


/*
    Triangle is defined by 3 points. One is (0, 0, 0).
    Triangle's position in the world is defined by the first vertex.
*/
#ifndef TRIANGLE
#define TRIANGLE

class Triangle
{
    private:
        glm::vec3 vertices[2];
        VAO * vao;               // vao object
        float x, y;              // co-ordinates
        float vx, vy;            // velocity
        float rotation;          // rotation - anticlockwise
        float r, g, b;           // rgb color

    public:
        void initialize(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3,
            float vx, float vy, float rotation,
            float r, float g, float b);
        void create_block();
        void draw(glm::mat4 M);
        void update();
        glm::vec3 get_vertex(int i);
        glm::mat4 get_transform(glm::mat4 M);
};
#endif


#ifndef BLOCK
#define BLOCK
class Block
{
    private:
        VAO * vao;               // vao object
        float x, y;              // co-ordinates
        float vx, vy;            // velocity
        float width, height;     // size parameters
        float rotation;          // rotation - anticlockwise
        float r, g, b;           // rgb color

    public:
        void initialize(float x, float y, float vx, float vy, float width, float height, float rotation,
            float r, float g, float b);
        void create_block();
        void draw(glm::mat4 M);
        void update();
        void update_rotation(float degrees);
        float get_x();
        float get_y();
        float get_width();
        float get_height();
        float get_rotation();
        float set_rotation(float rotation);
        glm::mat4 get_transform(glm::mat4 M);
};
#endif


#ifndef SPRITE
#define SPRITE
class Sprite
{
    public:
        float x, y;
        float vx, vy;
        float rotation;
        vector<Block*> blocks;
        vector<Triangle*> triangles;
    public:
        void initialize(float x, float y, float vx, float vy, float rotation);
        void draw(glm::mat4 M);
        void update();
        bool is_out();
        float get_x();
        float get_y();
        void set_x(float x);
        void set_y(float y);
        float get_vx();
        float get_vy();
        void set_vx(float dvx);
        void set_vy(float dvy);
        float get_rotation();
        float set_rotation(float rotation);
        glm::mat4 get_transform(glm::mat4 M);
};
#endif
