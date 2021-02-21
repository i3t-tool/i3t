#include "Select.h"
#include "Component.h"
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
  unsigned char*read=(unsigned char*)malloc(w * h);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(x, y, w, h, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &read);

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
      if (dist < closest && Select::stencilRef[read[pos]] != NULL && read[pos] != filter){closest = dist;stencilnum = read[pos];}
    }
  }
  // printf("ret %d\n",stencilnum);
  free(read);
  return stencilnum;
}