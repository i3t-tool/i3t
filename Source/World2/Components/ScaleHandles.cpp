#include "ScaleHandles.h"
#include <typeinfo>

const char* ScaleHandles::typeStatic = NULL;

ScaleHandles::ScaleHandles() {
    ScaleHandles::typeStatic = typeid(ScaleHandles).name();
    this->type = ScaleHandles::typeStatic;

}

void ScaleHandles::start() {

}

void ScaleHandles::render(glm::mat4* parent, bool renderTransparent) {

}

void ScaleHandles::update() {

}
