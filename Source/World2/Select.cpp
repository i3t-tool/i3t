#include "Select.h"
#include "Core/Input/InputManager.h"
#include "Component.h"
#include "World2.h"
bool Select::stencilRef[256] = {false};

int Select::registerStencil(){
    for (int i = 1; i < 255; i++){
        if (!Select::stencilRef[i]){
            //printf("reg m_stencil %d\n", i);
            Select::stencilRef[i] = true;
            return i;
        }
    }
    return -1;
}
bool Select::freeStencil(char stencil){
    if (Select::stencilRef[stencil]){
        //printf("free m_stencil %d\n", i);
        Select::stencilRef[stencil] = false;
        return true;
    }
    return false;
}
unsigned char Select::getStencilAt(int x, int y, int r, int filter){
    int w=r*2+1,h=r*2+1;x-=r;y-=r;
    float viewport[4];
    glGetFloatv(GL_VIEWPORT, viewport);
    if(x<(int)viewport[0]){w-=(int)viewport[0]-x;x=(int)viewport[0];}
    if(y<(int)viewport[1]){h-=(int)viewport[1]-y;y=(int)viewport[1];}
    if(x+w>(int)viewport[2]){w-=x+w-(int)viewport[2];}
    if(y+h>(int)viewport[3]){h-=y+h-(int)viewport[3];}
    if(w<=0||h<=0){return 0;}

    unsigned int*read=(unsigned int*)malloc(size_t(w * h*sizeof(unsigned int)));
    glReadPixels(x, y, w, h, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, read);
  
    unsigned char stencilnum = 0;
    int pos;
    float dist, closest = 1000000.0f, tmp;
    for (int i=0;i<h;i++){
        for (int j=0;j<w;j++){
            tmp=i-h*0.5f;
            dist=tmp*tmp;
            tmp=j-w*0.5f;
            dist+=tmp*tmp;
            pos=w*i+j;
            read[pos] = read[pos] & 255;
            if (dist < closest && Select::stencilRef[read[pos]] != NULL && read[pos] != filter){closest = dist;stencilnum = read[pos];}
        }
    }
    // printf("ret %d\n",stencilnum);
    free(read);
    /*const char* name;
    if (Select::stencilRef[stencilnum] != NULL) {
        name=Select::stencilRef[stencilnum]->getComponentType();
        printf("select %d,%s\n", stencilnum, name);
    }
    printf("select %d,-\n",stencilnum);*/
    return stencilnum;
}