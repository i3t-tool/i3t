//saving
int n0=mat4oper(add,3868,-1617,"MatrixAddMatrix");
int d1=datavec3(7.000f,7.000f,7.000f);
int n1=mat4(scale,d1,2416,-1433,"Scale");
int d2=datascalar(3.000f);
int n2=scalar(d2,3275,-1377,"Free Float");
int n3=vec4oper(norm,2704,-1269,"NormVector");
int d4=datavec4(0.000f,6.000f,3.000f,1.000f);
int n4=vec4(d4,2408,-1273,"Free");
int d5=datavec3(1.530f,4.000f,0.990f);
int n5=mat4(translate,d5,2454,-1601,"Translate");
int n6=mat4oper(inverse,3020,-1637,"Inverse");
int n7=mat4oper(transpose,3024,-1493,"Transpose");
int n8=mat4oper(determinant,3264,-1477,"Determinant");
int d9=datavec3(0.000f,0.000f,0.000f);
int n9=mat4(translate,d9,3412,-1633,"Translation");
int n10=mat4oper(rotatex,3388,-1253,"EulerX");
int n11=mat4oper(matrix,2384,-1189,"Free");
int n12=mat4oper(rotatey,3616,-1301,"EulerY");
int n13=mat4oper(rotatey,3840,-1345,"EulerZ");
int d14=datavec3(1.000f,1.000f,1.000f);
int n14=mat4(scale,d14,3636,-1585,"Scale");
int n15=sequence(2885,-1205,"default Sequence header");
int n16=sequence(2645,-1205,"default Sequence header");
int n17=mat4oper(mul,2764,-1505,"Mat*mat");
int n18=mat4oper(matrix,2764,-1637,"Matrix2Matrix");
confnode(n3,4,full);
confnode(n5,2,setvalues);
bool p0_0=plugnodes(n9,n0,0,0);
bool p0_1=plugnodes(n14,n0,0,1);
bool p3_0=plugnodes(n4,n3,0,0);
bool p6_0=plugnodes(n18,n6,0,0);
bool p7_0=plugnodes(n17,n7,0,0);
bool p8_0=plugnodes(n7,n8,0,0);
bool p10_0=plugnodes(n2,n10,0,0);
bool p12_0=plugnodes(n2,n12,0,0);
bool p13_0=plugnodes(n2,n13,0,0);
bool p15_0=plugnodes(n16,n15,0,0);
bool p15_1=plugnodes(n16,n15,1,1);
bool p16_1=plugnodes(n11,n16,0,1);
bool p17_0=plugnodes(n5,n17,0,0);
bool p17_1=plugnodes(n1,n17,0,1);
bool p18_0=plugnodes(n5,n18,0,0);
//saved
