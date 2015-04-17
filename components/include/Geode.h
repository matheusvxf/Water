#pragma once

#include "DataType.h"
#include "Node.h"
#include "Object.h"

class Geode :
    public Node,
    public Object
{
protected:
    BOOLEAN is_visible;
public:
    Geode();
    virtual ~Geode();

    virtual void draw(const Matrix4& C);
    virtual Matrix4 get_model_view() const;

	virtual void render() = 0;

    static UINT32 counter;
    static UINT32 hiden;
};

