//saving
int d0=datascalar(0.000f);
int n0=scalar(d0,-390,-358,"Float");
int n1=scalaroper(mul,-278,-352,"float * float");
int d2=datascalar(180.000f);
int n2=scalar(d2,-278,-262,"Float");
int d3=datascalar(3.1426f);
int n3=scalar(d3,-390,-278,"Float");
int n4=scalaroper(div,-144,-342,"float / float");
confnode(n0,1,full);
confnode(n2,0,full);
confnode(n3,4,full);
bool p1_0=plugnodes(n0,n1,0,0);
bool p1_1=plugnodes(n3,n1,0,1);
bool p4_0=plugnodes(n1,n4,0,0);
bool p4_1=plugnodes(n2,n4,0,1);
//saved
