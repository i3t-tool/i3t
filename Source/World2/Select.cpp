#include "Select.h"
#include "Component.h"
#include "World2.h"
#include "../Core/InputController.h"
Component* Select::stencilRef[256] = {NULL};

int Select::registerStencil(Component* owner){
  for (int i = 1; i < 255; i++){
    if (Select::stencilRef[i] == NULL){
      printf("reg stencil %d\n", i);
      Select::stencilRef[i] = owner;
      return i;
    }
  }
  return -1;
}
bool Select::freeStencil(Component* owner){
  for (int i = 1; i < 255; i++){
    if (Select::stencilRef[i] == owner){
      printf("free stencil %d\n", i);
      Select::stencilRef[i] = NULL;
      return true;
    }
  }
  return false;
}
unsigned char Select::getStencilAt(int x, int y, int r, int filter){
  int w=r*2+1,h=r*2+1;x-=r;y-=r;
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
  return stencilnum;
}