#ifndef BEZIER_H
#define BEZIER_H

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include <vector>
#include <string>
#include "../glad/glad.h"
#include <iostream>

class Bezier{
    public:
        Bezier(std::vector<glm::vec3> control_points);
        Bezier();
        unsigned int getPolyVAO();
        unsigned int getAproxVAO();
        unsigned int getInterVAO();
        int getControlPointNum();
        int getAproxPointNum();
        void setSegmentNum(int segment_num);
        int getSegmentNum();
        std::vector<glm::vec3> getBezierVertices();
        void setupBezier(std::vector<glm::vec3> control_points);

    private:
        unsigned int vbo_poly, vao_poly;
        unsigned int vbo_aprox, vao_aprox;
        unsigned int vbo_inter, vao_inter;
        std::vector<glm::vec3> control_points;
        std::vector<glm::vec3> bezier_vertices;
        int segment_num = 100;

        std::vector<glm::vec3> calculateBezierCurve(const std::vector<glm::vec3>& control_points);
        glm::vec3 calculateBezierPoint(const std::vector<glm::vec3>& control_points, float t);
        std::vector<glm::vec3> calculateNewControlPoints(glm::vec3 p0, glm::vec3 p1,
                                                         glm::vec3 p2, glm::vec3 p3);
};

#endif
