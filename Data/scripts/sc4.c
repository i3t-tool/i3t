//saving
int d0=datavec3(0.000f,0.000f,0.000f);
int n0=mat4(translate,d0,445,43,"Translation");
int n1=vec3oper(cross,96,60,"Vector3CrossVector3");
int d2=datavec3(0.000f,0.000f,0.000f);
int n2=vec3(d2,0,236,"-");
int n3=vec3oper(cross,1043,79,"Vector3CrossVector3");
int d4=datavec3(0.000f,0.000f,0.000f);
int n4=vec3(d4,751,67,"Vector 3");
int d5=datavec3(0.000f,0.000f,0.000f);
int n5=vec3(d5,-82,108,"Vector 3");
confnode(n5,4,full);
bool p1_0=plugnodes(n5,n1,0,0);
bool p1_1=plugnodes(n2,n1,0,1);
//saved
