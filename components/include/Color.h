#pragma once

#define ORANGE  1.0, 0.5, 0.0, 1.0
#define RED     1.0, 0.0, 0.0, 1.0
#define BLUE    0.0, 0.0, 1.0, 1.0

class Color
{
protected:
    float color[4];
public:
    Color();
    Color(float r, float g, float b);
    Color(float r, float g, float b, float a);
    ~Color();

    inline float r();
    inline float g();
    inline float b();
    inline float a();
    inline float r(float);
    inline float g(float);
    inline float b(float);
    inline float a(float);

    inline Color& operator=(const Color &color);
    inline Color& apply_attenuation(float attenuation);
    inline float *get_pointer();
};

inline Color::Color() : Color(1.0, 1.0, 1.0) {}

inline Color::Color(float r, float g, float b) : Color(r, g, b, 1.0) {}

inline Color::Color(float r, float g, float b, float a)
{
    this->r(r);
    this->g(g);
    this->b(b);
    this->a(a);   
}

inline Color::~Color() {}

inline float Color::r()
{
    return color[0];
}

inline float Color::g()
{
    return color[1];
}

inline float Color::b()
{
    return color[2];
}

inline float Color::a()
{
    return color[3];
}

inline float Color::r(float r)
{
    return color[0] = r;
}

inline float Color::g(float g)
{
    return color[1] = g;
}

inline float Color::b(float b)
{
    return color[2] = b;
}

inline float Color::a(float a)
{
    return color[3] = a;
}

inline Color& Color::operator=(const Color &color)
{
    for (register UINT8 i = 0; i < 4; ++i)
        this->color[i] = color.color[i];
    return (*this);
}

inline Color& Color::apply_attenuation(float attenuation)
{
    for (register UINT8 i = 0; i < 3; ++i)
    {
        color[i] *= attenuation;
    }
    return *this;
}

inline float *Color::get_pointer()
{
    return color;
}