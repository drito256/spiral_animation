#ifndef B_SPLINE
#define B_SPLINE

#include <vector>
#include "../glm/glm.hpp"
#include "../glad/glad.h"

class BSpline{
    private:
        unsigned int vbo;
        unsigned int vao;
        std::vector<glm::vec3> control_points = {
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(.0f, 10.0f, 5.0f),
                    glm::vec3(10.0f, 10.0f, 10.0f),
                    glm::vec3(10.0f, 0.0f, 15.0f),

                    glm::vec3(0.0f, 0.0f, 20.0f),
                    glm::vec3(0.0f, 10.0f, 25.0f),
                    glm::vec3(10.0f, 10.0f, 30.0f),
                    glm::vec3(10.0f, 0.0f, 35.0f),

                    glm::vec3(0.0f, 0.0f, 40.0f),
                    glm::vec3(0.0f, 10.0f, 45.0f),
                    glm::vec3(10.0f, 10.0f, 50.0f),
                    glm::vec3(10.0f, 0.0f, 55.0f)

            };
        std::vector<glm::vec3> bspline_points;

        float B0(float u) { return (1 - u) * (1 - u) * (1 - u) / 6.0f; }
        float B1(float u) { return (3 * u * u * u - 6 * u * u + 4) / 6.0f; }
        float B2(float u) { return (-3 * u * u * u + 3 * u * u + 3 * u + 1) / 6.0f; }
        float B3(float u) { return u * u * u / 6.0f; }

        glm::vec3 evaluateSpline(const std::vector<glm::vec3>& controlPoints,int i, float u) {
            return controlPoints[i] * B0(u) +
                   controlPoints[i+1] * B1(u) +
                   controlPoints[i+2] * B2(u) +
                   controlPoints[i+3] * B3(u);
        }

   public:
    BSpline(){
        
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        
        for(int j=0;j<= control_points.size()-4;j++){
            for(int i=0;i<100;i++){
                float u = (float) i / 100;  // u == t u lv
                glm::vec3 point = evaluateSpline(control_points, j, u);
                bspline_points.push_back(point);
            }
        }

        glBufferData(GL_ARRAY_BUFFER, bspline_points.size() * sizeof(float) * 3,
        &this->bspline_points[0],GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
 
        glBindVertexArray(0);
    }
    //BSpline() = default;

    int getVAO(){
        return this->vao;
    }

    int getSplinePointNum(){
        return bspline_points.size();
    }

    std::vector<glm::vec3> getVertices(){
        return this->bspline_points;
    }



};



#endif
