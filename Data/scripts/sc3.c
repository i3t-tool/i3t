//saving
int d0=datavec3(0.000f,0.000f,0.000f);
int n0=vec3(d0,-82,108,"Vector 3");
int n1=vec3oper(cross,96,60,"Vector3CrossVector3");
int d2=datavec3(0.000f,0.000f,0.000f);
int n2=mat4(translate,d2,445,43,"Translation");
confnode(n0,4,full);
bool p1_0=plugnodes(n0,n1,0,0);
//saved
