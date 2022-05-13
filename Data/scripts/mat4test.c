//saving
int d0=datamat4(1.000f,0.000f,0.000f,0.000f, 0.000f,1.140f,0.240f,0.000f, 0.000f,0.000f,0.600f,0.000f, 0.300f,0.000f,0.000f,1.000f);
int n0=mat4(free,d0,978,154,"Free");
int d1=datavec3(0.060f,0.220f,0.000f);
int n1=mat4(translate,d1,816,160,"Translation");
int d2=datascalar(0.000f);
int n2=mat4(rotatex,d2,1136,160,"Euler X");
int d3=datascalar(0.000f);
int n3=mat4(rotatey,d3,1456,160,"Euler Y");
int d4=datascalar(0.000f);
int n4=mat4(rotatez,d4,1296,160,"Euler Z");
int d5=datascalar(70.000f);
int n5=mat4(axisangle,d5,1616,160,"AxisAngle");
int d6=datavec3(0.910f,0.910f,0.910f);
int n6=mat4(scale,d6,1455,294,"Scale");
int d7=datavec3(1.140f,1.140f,1.140f);
int n7=mat4(scale,d7,1296,304,"Scale");
int d8=datamat4(-3.667f,4.191f,0.0f,0.0f, 2.411f,-3.651f,0.0f,0.0f, 8.254f,2.168f,0.0f,0.0f, 0.000f,0.000f,0.0f,0.0f);
int n8=mat4(frustrum,d8,432,288,"Frustum");
int d9=datamat4(-11.670f,1.689f,0.0f,0.0f, 5.000f,-5.000f,0.0f,0.0f, 1.000f,10.000f,0.0f,0.0f, 0.000f,0.000f,0.0f,0.0f);
int n9=mat4(ortho,d9,833,342,"Ortho");
int d10=datamat4(0.000f,0.000f,0.0f,0.0f, 0.000f,0.000f,0.0f,0.0f, 1.767f,3.689f,0.0f,0.0f, 42.174f,2.657f,0.0f,0.0f);
int n10=mat4(perspective,d10,1072,288,"Perspective");
confnode(n8,4,full);
//saved
