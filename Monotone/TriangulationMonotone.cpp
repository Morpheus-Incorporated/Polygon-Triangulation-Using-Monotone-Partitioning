#EJ Added Required Header Files..

#include <windows.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <GL/glut.h>
#include <algorithm>
#include <stack>

using namespace std;

#EJ Created a Class Object Point..
class Point{
public:
    float x,y;
};

#EJ Created a Class Object Vertex..
class Vertex{
public:
    int index;
    Point P;
    int type;
};

/*Vectors for managing Polygon Vertices*/
vector <Vertex> poly;
vector <Vertex> eventQ;

void init(){

    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,640,0,480);

}

void plotPt(Point pt){

    glColor3f(1.0,0.0,0.0);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f(pt.x,pt.y);
    glEnd();
    glFlush();
}

void plotPoly(){

    glColor3f(0,1,0);
    glPointSize(5);
    glBegin(GL_LINE_LOOP);
        for(int i=0;i<poly.size();i++){
            glVertex2f(poly[i].P.x,poly[i].P.y);
        }
    glEnd();
    glFlush();
}

void plotLine(Point pt,Point pt1){

    glColor3f(1,0,0);
    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2f(pt.x,pt.y);
        glVertex2f(pt1.x,pt1.y);
    glEnd();
    glFlush();

}

float areaofTriangle(Point a,Point b,Point c){
    return((b.x-a.x)*(c.y-a.y)-(c.x-a.x)*(b.y-a.y));
}

bool Left(Point a,Point b,Point c){
    return areaofTriangle(a,b,c) > 0;
}

bool LeftOn(Point a,Point b,Point c){
    return areaofTriangle(a,b,c) >= 0;
}

bool Collinear(Point a,Point b,Point c){
    return areaofTriangle(a,b,c) == 0;
}

bool Xor(bool x,bool y){
    return !x ^ !y;
}

bool Intersection(Point a,Point b,Point c,Point d){
    if(Collinear(a,b,c)||Collinear(a,b,d)||Collinear(c,d,a)||Collinear(c,d,b))
        return false;
    return Xor(Left(a,b,c),Left(a,b,d)) && Xor(Left(c,d,a),Left(c,d,b));

}

bool Between(Point a,Point b,Point c){
    if(!Collinear(a,b,c))
        return false;
    if(a.x!=b.x){
        return(((a.x<=c.x)&&(c.x<=b.x))||((a.x>=c.x)&&(c.x>=b.x)));
    }
    else
        return(((a.y<=c.y)&&(c.y<=b.y))||((a.y>=c.y)&&(c.y>=b.y)));
}

bool Intersect(Point a,Point b,Point c,Point d){
    if(Intersection(a,b,c,d)){
        return true;
    }
    else if(Between(a,b,c)||Between(a,b,d)||Between(c,d,a)||Between(c,d,b))
        return true;
    else{
        return false;
    }
}

bool isDiagonal(Vertex a,Vertex b){
    for(int i=0;i<poly.size()-1;i++){
        if((poly[i].index!=a.index)&& (poly[i+1].index!=a.index)&&(poly[i].index!=b.index)&& (poly[i+1].index!=b.index)&&Intersect(a.P,b.P,poly[i].P,poly[i+1].P)){
            return false;
        }
    }
    if((poly[poly.size()-1].index!=a.index)&& (poly[0].index!=a.index)&&(poly[poly.size()-1].index!=b.index)&& (poly[0].index!=b.index)&&Intersect(a.P,b.P,poly[poly.size()-1].P,poly[0].P)){
            return false;
        }
    return true;
}

bool InCone(Vertex a,Vertex b){
    if(LeftOn(a.P,poly[a.index].P,poly[a.index-2].P)){
        return(Left(a.P,b.P,poly[a.index-2].P) && Left(b.P,a.P,poly[a.index].P));
    }
    return(!(Left(a.P,b.P,poly[a.index].P) && Left(b.P,a.P,poly[a.index-2].P)));
}

bool Diagonal(Vertex a, Vertex b){
    return(InCone(a,b) && InCone(b,a) && isDiagonal(a,b));
}

bool comp(Vertex v1,Vertex v2){
    if(v1.P.y==v2.P.y)
        return v1.P.x<v2.P.x;
    else
        return v1.P.y>v2.P.y;
}

bool compx(pair<int,int> a,pair<int,int> b){
    return(a.second < b.second);
}

bool right(Vertex cur,Vertex prev){
    if(cur.P.y < prev.P.y)
        return true;
    else if(cur.P.y == prev.P.y){
        if(cur.P.x < prev.P.x)
            return false;
    }
    return false;
}

void Triangulate(){
    int n = poly.size();
    eventQ=poly;
    sort(eventQ.begin(),eventQ.end(),comp);
    cout<<"Event queue:\n";
    cout<<"Xcor Ycor index\n";
    for(int i=0;i<n;i++){
        cout<<eventQ[i].P.x<<" "<<eventQ[i].P.y<<" "<<eventQ[i].index<<"\n";
    }
    int frontQ=0;
    stack <int> s;
    cout<<"\nPush segment"<<eventQ[0].index;
    cout<<"\nPush segment"<<eventQ[1].index;
    s.push(eventQ[0].index);
    s.push(eventQ[1].index);
    frontQ=2;
    while(frontQ!=n){
        int val=right(poly[s.top()-1],poly[s.top()-2])^ right(poly[eventQ[frontQ].index-1],poly[eventQ[frontQ].index-2]);
        if(val==1){
            while(!s.empty()){
                int ind=s.top();
                int qind=eventQ[frontQ].index,prev1,next1;
                    if(qind==1) prev1=poly.size();
                    else prev1=qind-1;
                    if(qind==poly.size())next1=1;
                    else next1=qind+1;
                    if(ind!=next1 && ind!=prev1){
                        cout<<"\nDifferent chain: Insert diagonal between "<<s.top()<<" and "<<eventQ[frontQ].index;
                        plotLine(poly[s.top()-1].P,poly[eventQ[frontQ].index-1].P);
                    }
                cout<<"\nPop out segment"<<s.top();
                s.pop();
            }
            cout<<"\nPush segment"<<eventQ[frontQ-1].index;
            s.push(eventQ[frontQ-1].index);
            cout<<"\nPush segment"<<eventQ[frontQ].index;
            s.push(eventQ[frontQ].index);
        }
        else{
            int ch=1;
            int pind;
            while(ch==1){
                int ind=s.top();
                int c=eventQ[frontQ].index;
                if(Diagonal(poly[ind-1],poly[c-1])){
                    int qind=eventQ[frontQ].index,prev1,next1;
                    if(qind==1) prev1=poly.size();
                    else prev1=qind-1;
                    if(qind==poly.size())next1=1;
                    else next1=qind+1;
                    if(ind!=next1 && ind!=prev1){
                    cout<<"\nSame chain: Insert diagonal between "<<s.top()<<" and "<<eventQ[frontQ].index;
                    plotLine(poly[s.top()-1].P,poly[eventQ[frontQ].index-1].P);
                    pind=s.top();
                    cout<<"\nPop out segment"<<pind;
                    s.pop();
                    }
                }
                else{

                    pind=s.top();
                    s.pop();
                    ch=0;
                }
            }
            cout<<"\nPush segment"<<pind;
            s.push(pind);
            s.push(eventQ[frontQ].index);
        }
        frontQ++;

    }
    cout<<"\nPolygon is triangulated";
}

static void display(void){
    glFlush();
}

int winHeight=480;
void mousePlot(int button,int state,int x,int y){
    static int countv=0;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        y=winHeight-y;
        Point pt;
        pt.x=x;
        pt.y=y;
        plotPt(pt);
        Vertex v;
        v.P=pt;
        v.index=countv+1;
        poly.push_back(v);
        countv++;
    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
            plotPoly();
            Triangulate();
    }
}

int main(int argc,char **argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(640,480);
    glutCreateWindow("Triangulation");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mousePlot);
    glutMainLoop();
}
