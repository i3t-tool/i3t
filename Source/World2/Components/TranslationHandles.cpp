#include "TranslationHandles.h"
#include <typeinfo>

const char* TranslationHandles::typeStatic = NULL;

TranslationHandles::TranslationHandles() {
    TranslationHandles::typeStatic = typeid(TranslationHandles).name();
    this->type = TranslationHandles::typeStatic;

}

void TranslationHandles::start(){

}

void TranslationHandles::render(glm::mat4* parent, bool renderTransparent) {

}

void TranslationHandles::update() {

}
