#include "../include/headers/renderer.h"

void Renderer::addObject(Object* object){
	objects.push_back(object);
}

void Renderer::addCurve(const Bezier curve){
    curves.clear();
    curves.push_back(curve);
}

void Renderer::addBSpline(BSpline* spline){
    bsplines.push_back(spline);
}

void Renderer::renderScene(std::vector<Shader> &shaders) {
    	for (int i = 0; i < objects.size(); i++) {
            Shader current_shader = shaders.at(i);
            current_shader.use();
    	    current_shader.setMat4("model", objects[i]->getModelMatrix());

        	for (int j = 0; j < objects[i]->getMeshes().size(); j++) {
            		Mesh mesh = objects[i]->getMeshes()[j];
            		glBindVertexArray(mesh.getVAO());  

            		glDrawElements(GL_TRIANGLES, mesh.getIndices().size(),
				       GL_UNSIGNED_INT, 0);
            		glBindVertexArray(0); // Unbind VAO
            }
        }

        //...crtanje beziera

       // if(curves.size()>0){
            //crtanje poligona
            /*shaders.at(2).use();
            shaders.at(2).setVec3("color", glm::vec3(1,0,0));
            glBindVertexArray(curves[0].getPolyVAO());
            glDrawArrays(GL_LINE_LOOP,0, curves[0].getControlPointNum());
            glBindVertexArray(0);
            
            //crtanje aproximacijske
            shaders.at(2).use();
            shaders.at(2).setVec3("color", glm::vec3(0,1,0));
            glBindVertexArray(curves[0].getAproxVAO());
            glDrawArrays(GL_LINE_STRIP,0, curves[0].getAproxPointNum() );
            glBindVertexArray(0);
            
            //crtanje interpolacijske
            if(curves[0].getControlPointNum() > 3){
                shaders.at(2).setVec3("color", glm::vec3(0,0,1));
                glBindVertexArray(curves[0].getInterVAO());
                glDrawArrays(GL_LINE_STRIP,0, curves[0].getAproxPointNum() );
                glBindVertexArray(0);
            }*/

        //}
        // cubic bsplines
        if(bsplines.size() > 0){
            shaders.at(1).use();
            shaders.at(1).setVec3("color", glm::vec3(0,1,1));
            glBindVertexArray(bsplines[0]->getVAO());
            glDrawArrays(GL_LINE_STRIP, 0 , bsplines[0]->getSplinePointNum());
            glBindVertexArray(0);
        }


        glUseProgram(0); // reset shader
}


Renderer::Renderer(){
}
