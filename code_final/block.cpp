#include "headers.h"
#include "vao.h"
#include "block.h"

// Default Values
float default_block_width = 0.25f;
float default_block_height = 0.50f;
float default_block_red = 0.75f;
float default_block_green = 0.75f;
float default_block_blue = 0.75f;

float allowed_x_out = 0.3f;
float allowed_y_out = 0.3f;


float bucket_height = 0.40f;
float bucket_width = 0.60f;
float bucket_speed = 0.07f;
int red = 1;
int green = 2;
int blue = 3;
int black = 4;

void Triangle::initialize(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3,
    float vx, float vy, float rotation,
    float r, float g, float b)
{
    this->x = x;
    this->y = y;
    this->r = r;
    this->g = g;
    this->b = b;
    vertices[0] = v2 - v1;
    vertices[1] = v3 - v1;

    this->rotation = rotation;
    this->vx = vx;
    this->vy = vy;
    create_block();
}

void Triangle::create_block()
{
    GLfloat vertex_buffer_data [] =
    {
        0, 0, 0,
        vertices[0][0], vertices[0][1], vertices[0][2],
        vertices[1][0], vertices[1][1], vertices[1][2]
    };

    GLfloat color_buffer_data [] =
    {
        r, g, b,
        r, g, b,
        r, g, b
    };
    vao = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color_buffer_data, GL_FILL);
}

void Triangle::draw(glm::mat4 M)
{
    glm::mat4 VP = Matrices.projection * Matrices.view;

    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translateTriagle = glm::translate (glm::vec3(x, y, 0.0f)); // glTranslatef
    glm::mat4 rotateTriagle = glm::rotate((float)(rotation*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
    M = M * translateTriagle * rotateTriagle;

    Matrices.model = M;
    glm::mat4 MVP = VP * Matrices.model;

    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(vao);
}

glm::mat4 Triangle::get_transform(glm::mat4 M)
{
    glm::mat4 translate = glm::translate (glm::vec3(x, y, 0.0f));
    glm::mat4 rotate = glm::rotate((float)(rotation*M_PI/180.0f), glm::vec3(0,0,1));
    glm::mat4 transform = translate * rotate;
    return transform * M;
}

void Triangle::update()
{
    x += vx;
    y += vy;
}

glm::vec3 Triangle::get_vertex(int i) {return vertices[i];}



void Block::initialize(float x, float y, float vx, float vy, float width, float height, float rotation,
    float r = default_block_red,
    float g = default_block_green,
    float b = default_block_blue)
{
    this->x = x;
    this->y = y;
    this->r = r;
    this->g = g;
    this->b = b;

    this->width = width;
    this->height = height;

    this->rotation = rotation;
    this->vx = vx;
    this->vy = vy;
    create_block();
}

void Block::create_block()
{
    GLfloat vertex_buffer_data [] =
    {
        0, 0, 0,              //     .   .
        width, 0, 0,          //
        width, height, 0,     //         .
        0, 0, 0,              //     .
        width, height, 0,     //
        0, height, 0          //     .   .
    };

    GLfloat color_buffer_data [] =
    {
        r, g, b,
        r, g, b,
        r, g, b,
        r, g, b,
        r, g, b,
        r, g, b
    };

    vao = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}

void Block::draw(glm::mat4 M)
{
    glm::mat4 VP = Matrices.projection * Matrices.view;

    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translateBlock = glm::translate (glm::vec3(x, y, 0.0f)); // glTranslatef
    glm::mat4 rotateBlock = glm::rotate((float)(rotation*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
    M = M * translateBlock * rotateBlock;

    Matrices.model = M;
    glm::mat4 MVP = VP * Matrices.model;

    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(vao);
}

glm::mat4 Block::get_transform(glm::mat4 M)
{
    glm::mat4 translate = glm::translate (glm::vec3(x, y, 0.0f));
    glm::mat4 rotate = glm::rotate((float)(rotation*M_PI/180.0f), glm::vec3(0,0,1));
    glm::mat4 transform = translate * rotate;
    return transform * M;
}

void Block::update()
{
    if(x + width < min_x || x > max_x || y < min_y || y + height > max_y)
    {
        return;
    }
    x += vx;
    y += vy;
}
void Block::update_rotation(float degrees)
{
    rotation += degrees;
    while(rotation > 360.0f)
        rotation -= 360.0f;
}
float Block::get_x() {return x;}
float Block::get_y() {return y;}
float Block::get_width() {return width;}
float Block::get_height() {return height;}
float Block::get_rotation() {return rotation;}
float Block::set_rotation(float rotation) {this->rotation = rotation;}








void Sprite::initialize(float x, float y, float vx, float vy, float rotation)
{
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->rotation = rotation;
}

glm::mat4 Sprite::get_transform(glm::mat4 M)
{
    glm::mat4 translate = glm::translate (glm::vec3(x, y, 0.0f));
    glm::mat4 rotate = glm::rotate((float)(rotation*M_PI/180.0f), glm::vec3(0,0,1));
    glm::mat4 transform = translate * rotate;
    return transform * M;
}

void Sprite::draw(glm::mat4 M)
{
    glm::mat4 translate = glm::translate (glm::vec3(x, y, 0.0f));
    glm::mat4 rotate = glm::rotate((float)(rotation*M_PI/180.0f), glm::vec3(0,0,1));
    glm::mat4 transform = translate * rotate;

    for(int i=0; i<(int)triangles.size(); i++)
        triangles[i]->draw(transform * M);

    for(int i=0; i<(int)blocks.size(); i++)
        blocks[i]->draw(transform * M);
}
void Sprite::update()
{
    x += vx;
    y += vy;
}
bool Sprite::is_out()
{
    if(x + allowed_x_out < min_x || x - allowed_x_out > max_x || y + allowed_y_out < min_y || y - allowed_y_out > max_y)
        return true;
    else
        return false;
}
float Sprite::get_x() {return x;}
float Sprite::get_y() {return y;}
void Sprite::set_x(float xx) {x = xx;}
void Sprite::set_y(float yy) {y = yy;}
float Sprite::get_vx() {return vx;}
float Sprite::get_vy() {return vy;}
void Sprite::set_vx(float dvx) {vx = dvx;}
void Sprite::set_vy(float dvy) {vy = dvy;}
float Sprite::get_rotation() {return rotation;}
float Sprite::set_rotation(float rotation) {this->rotation = rotation;}
