#include "Renderer.h"
#include "../Select.h"
#include <iostream>
#include <typeinfo>

const char* Renderer::typeStatic = NULL;

Renderer::Renderer(unsigned int flags){
    Renderer::typeStatic = typeid(Renderer).name();
    this->type = Renderer::typeStatic;

    this->isTransparent = (flags & Renderer::IS_TRANSPARENT) != 0;
    this->useStencil = (flags & Renderer::USE_STENCIL) != 0;
    this->drawLines = (flags & Renderer::DRAW_LINES) != 0;
    this->stencil = 0;

    if (this->useStencil){this->stencil = Select::registerStencil((Component*)this);}
    // printf("is transparent %d, mask %d\n",this->isTransparent,);
}

// void Renderer::start(){}

void Renderer::render(glm::mat4* parent, bool renderTransparent){
    if (this->isTransparent == renderTransparent){
        glStencilMask(255 * this->useStencil);
        glStencilFunc(GL_ALWAYS, this->stencil, 255);
        int primbkp = owner->primitive;
        if (this->drawLines){owner->primitive = GL_LINES;owner->draw(*parent);}
        else{owner->primitive = GL_TRIANGLES;owner->draw(*parent);}
        owner->primitive = primbkp;
        glStencilMask(0);
    }
}
