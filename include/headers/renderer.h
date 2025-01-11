#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "../glad/glad.h"

#include "object.h"
#include "shader.h"
#include "bezier.h"
#include "bspline.h"

class Renderer{
	public:
		Renderer();
		void addObject(Object* object);
        void addCurve(const Bezier curve);
        void addBSpline(BSpline* spline);
		void renderScene(std::vector<Shader> &shaders);

	private:
		std::vector<Object*> objects;
		std::vector<BSpline*> bsplines;
        std::vector<Bezier> curves;
};

#endif
