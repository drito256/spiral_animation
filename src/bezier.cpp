#include "../include/headers/bezier.h"

void Bezier::setupBezier(std::vector<glm::vec3> control_points){
    //kalkulacija verticesa i pospremanje u buffere
    this->control_points = control_points;

    //poligon
    glGenVertexArrays(1, &vao_poly);
    glGenBuffers(1, &vbo_poly);

    glBindVertexArray(vao_poly);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_poly);

    glBufferData(GL_ARRAY_BUFFER, control_points.size() * sizeof(float) * 3, &this->control_points[0], 
    GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    glBindVertexArray(0);
    
    //aproksimacijska
    this->bezier_vertices = calculateBezierCurve(control_points);
    //this->bezier_vertices = control_points;
    glGenVertexArrays(1, &vao_aprox);
    glGenBuffers(1, &vbo_aprox);

    glBindVertexArray(vao_aprox);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_aprox);

    glBufferData(GL_ARRAY_BUFFER, bezier_vertices.size() * sizeof(float) * 3, 
    &this->bezier_vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glBindVertexArray(0);

    //interpolacijska samo za 4 zadnje tocke
    if(control_points.size()>3){
        this->bezier_vertices = calculateBezierCurve(calculateNewControlPoints(
        control_points[control_points.size() - 4],
        control_points[control_points.size() - 3],
        control_points[control_points.size() - 2],
        control_points[control_points.size() - 1]
        ));

        glGenVertexArrays(1, &vao_inter);
        glGenBuffers(1, &vbo_inter);

        glBindVertexArray(vao_inter);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_inter);

        glBufferData(GL_ARRAY_BUFFER, bezier_vertices.size() * sizeof(float) * 3, 
        &this->bezier_vertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
        glBindVertexArray(0);
    }



}
std::vector<glm::vec3> Bezier::getBezierVertices(){
    return this->bezier_vertices;
}

int factorial(int n){
    int fact = 1;
    for(int i=2;i<=n;i++){
        fact *=i;
    }
    return fact;
}

glm::vec3 Bezier::calculateBezierPoint(const std::vector<glm::vec3>& controlPoints, float t) {
    int n = controlPoints.size() - 1; // Number of control points minus 1
    glm::vec3 point(0.0f);

    for (int i = 0; i <= n; i++) {
        float blend = (float)factorial(n) / (float)(factorial(i) * factorial(n - i)); // Bernstein polynomial

        blend *= pow(1.0f - t, n - i) * pow(t, i);

        // Add the contribution of this control point to the interpolated point
        point += controlPoints[i] * blend;
    }

    return point;
}

std::vector<glm::vec3> Bezier::calculateNewControlPoints(glm::vec3 p0, glm::vec3 p1, 
                                                glm::vec3 p2, glm::vec3 p3){
    
    glm::mat4 mat(18.f/18.f,0,0,0,
                  -33.f/18.f, 54.f/18.f, -27.f/18.f, 6.f/18.f,
                  21.f/18.f, -81.f/18.f, 81.f/18.f, -21.f/18.f,
                  -6.f/18.f, 27.f/18.f, -54.f/18.f, 33.f/18.f);

    glm::mat4 points;
    points[0] = glm::vec4(p0,0.0f);
    points[1] = glm::vec4(p1,0.0f);
    points[2] = glm::vec4(p2,0.0f);
    points[3] = glm::vec4(p3,0.0f);

    glm::mat4 res =  points * mat;

    std::vector<glm::vec3> vec;
    vec.push_back(p0);
    vec.push_back(glm::vec3(res[1].x, res[1].y,res[1].z));
    vec.push_back(glm::vec3(res[2].x, res[2].y,res[2].z));
    vec.push_back(p3);
    
    return vec;
}

std::vector<glm::vec3> Bezier::calculateBezierCurve(const std::vector<glm::vec3>& controlPoints) {
    std::vector<glm::vec3> curvePoints;

    for (int i = 0; i <= segment_num; i++) {
        float t = static_cast<float>(i) / static_cast<float>(segment_num);
        glm::vec3 point = calculateBezierPoint(controlPoints, t);
        curvePoints.push_back(point);
    }

    return curvePoints;
}


Bezier::Bezier(){
}

Bezier::Bezier(std::vector<glm::vec3> control_points){
    setupBezier(control_points);
}


void Bezier::setSegmentNum(int segment_num){
    this->segment_num = segment_num;
}

int Bezier::getControlPointNum(){
    return control_points.size();
}

int Bezier::getAproxPointNum(){
    return bezier_vertices.size();
}

int Bezier::getSegmentNum(){
    return this->segment_num;
}

unsigned int Bezier::getPolyVAO(){
    return this->vao_poly;
}

unsigned int Bezier::getAproxVAO(){
    return this->vao_aprox;
}

unsigned int Bezier::getInterVAO(){
    return this->vao_inter;
}
