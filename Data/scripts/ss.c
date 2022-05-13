//saving
int d0=datamat4(1.000f,0.000f,0.000f,0.000f, 0.000f,1.000f,0.000f,0.000f, 0.000f,0.000f,1.000f,0.000f, 0.000f,0.000f,0.000f,1.000f);
int n0=mat4(free,d0,569,78,"Free");
int d1=datavec3(0.000f,0.000f,0.000f);
int n1=mat4(translate,d1,715,78,"Translation");
int n2=sequence(549,63,"default Sequence header");
bool sa2_0=seqadd(n2,n0);
bool sa2_1=seqadd(n2,n1);
//saved
