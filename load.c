//jjhjhj
printf("loading\n");

int d0=datamat4(0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f);
int d1=datascalar(0.5f);
int d2=datavec3(1.5f,1.5f,1.5f);
int d3=datavec3(1.5f,0.5f,9.5f);
int d4=datavec4(1.5f,0.5f,9.5f,5.0f);

//int n0=mat4(free,d0,150,230);
int n1=mat4(free,d0,100,200);
int n2=mat4(scale,d2,-720,300);
int n3=mat4(translate,d3,600,300);
int n4=normvec4(-1,100,480);

bool p0=plugnodes(n2,n1,0,0);

printf("loaded\n");