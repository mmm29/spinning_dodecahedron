#pragma once

#include <list>
#include <memory>

#include "object.h"
#include "math/matrix.h"

class World {
public:
    World();

    void AddObject(const std::shared_ptr<Object> &object);

    const std::list<std::shared_ptr<Object>> &ListObjects() const;

    Matrix4 GetWorldMatrix() const;

private:
    Matrix4 world_matrix_;

    std::list<std::shared_ptr<Object>> objects_;
};