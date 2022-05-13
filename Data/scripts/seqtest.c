//saving
int d0=datamat4(0.000f,0.000f,0.000f,0.0f, 0.000f,0.000f,10.000f,0.0f, 0.000f,1.000f,0.000f,0.0f, 0.0f,0.0f,0.0f,0.0f);
int n0=mat4(lookat,d0,641,177,"Look At");
int d1=datascalar(0.000f);
int n1=mat4(rotatey,d1,855,177,"Euler Y");
int n2=sequence(621,162,"default Sequence header");
bool sa2_0=seqadd(n2,n0);
bool sa2_1=seqadd(n2,n1);
//saved
