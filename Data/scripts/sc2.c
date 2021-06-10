//saving
int d0=datavec3(0.919f,0.919f,0.919f);
int n0=mat4(scale,d0,3732,-1585,"Scale");
int d1=datascalar(3.000f);
int n1=scalar(d1,3275,-1377,"Free Float");
int n2=vec4oper(norm,2704,-1269,"NormVector");
int d3=datavec4(0.000f,6.000f,3.000f,1.000f);
int n3=vec4(d3,2408,-1273,"Free");
int d4=datavec3(1.530f,4.000f,0.990f);
int n4=mat4(translate,d4,2454,-1601,"Translate");
int n5=mat4oper(inverse,3020,-1637,"Inverse");
int n6=mat4oper(transpose,3024,-1493,"Transpose");
int n7=mat4oper(determinant,3291,-1457,"Determinant");
int d8=datavec3(7.000f,7.000f,7.000f);
int n8=mat4(scale,d8,2416,-1433,"Scale");
int n9=mat4oper(rotatex,3388,-1253,"EulerX");
int n10=mat4oper(free,2384,-1189,"Free");
int n11=mat4oper(rotatey,3616,-1301,"EulerY");
int n12=mat4oper(rotatey,3840,-1345,"EulerZ");
int d13=datavec3(0.079f,0.000f,-0.229f);
int n13=mat4(translate,d13,3508,-1569,"Translation");
int n14=sequence(2885,-1205,"default Sequence header");
int n15=sequence(2645,-1205,"default Sequence header");
int n16=mat4oper(mul,2764,-1505,"Mat*mat");
int n17=mat4oper(free,2764,-1637,"Matrix2Matrix");
int n18=mat4oper(perspective,4380,-1253,"Perspective");
int n19=mat4oper(frustrum,4570,-1577,"Frustum");
int n20=mat4oper(ortho,4320,-1580,"Ortho");
int n21=mat4oper(lookat,4092,-1605,"LookAt");
int n22=mat4oper(axisangle,4092,-1461,"Rotate");
int n23=mat4oper(add,3280,-1633,"MatrixAddMatrix");
int n24=vec3oper(dot,2985,-1929,"Vector3DotVector3");
int n25=vec3oper(add,2535,-1812,"Vector3AddVector3");
int n26=vec3oper(sub,2537,-1701,"Vector3SubVector3");
int n27=vec3oper(cross,2537,-1926,"Vector3CrossVector3");
int n28=vec3oper(vecmulfloat,2985,-1817,"Vector3MulFloat");
int n29=vec3oper(norm,2537,-2069,"NormalizeVector3");
int n30=vec4oper(mix,2745,-2067,"MixVector3");
int d31=datavec3(3.000f,5.000f,0.000f);
int n31=vec3(d31,2537,-1993,"Vector3");
int n32=vec3oper(show,2745,-1909,"ShowVector3");
int n33=vec3oper(length,2985,-2005,"Vector3Length");
int n34=vec4oper(dot,3752,-2029,"WorkspaceVectorDotVector");
int n35=vec4oper(add,3524,-1949,"VectorAddVector");
int n36=vec4oper(sub,3524,-1825,"VectorSubVector");
int d37=datavec4(0.000f,0.000f,0.000f,0.000f);
int n37=vec4(d37,3522,-2025,"Vector4");
int n38=vec4oper(vecmulfloat,3524,-1713,"VectorMulFloat");
int n39=vec4oper(norm,3748,-1905,"NormVector");
int n40=vec4oper(mix,3748,-1758,"MixVector");
confnode(n2,4,full);
bool p2_0=plugnodes(n3,n2,0,0);
bool p5_0=plugnodes(n17,n5,0,0);
bool p6_0=plugnodes(n16,n6,0,0);
bool p7_0=plugnodes(n6,n7,0,0);
bool p9_0=plugnodes(n1,n9,0,0);
bool p11_0=plugnodes(n1,n11,0,0);
bool p12_0=plugnodes(n1,n12,0,0);
bool p14_0=plugnodes(n15,n14,0,0);
bool p14_1=plugnodes(n15,n14,1,1);
bool p15_1=plugnodes(n10,n15,0,1);
bool p16_0=plugnodes(n4,n16,0,0);
bool p16_1=plugnodes(n8,n16,0,1);
bool p17_0=plugnodes(n4,n17,0,0);
bool p23_0=plugnodes(n5,n23,0,0);
bool p23_1=plugnodes(n6,n23,0,1);
//saved
