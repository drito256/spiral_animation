#version 430 core

layout(points) in;
layout(line_strip, max_vertices = 100) out;

uniform mat4 view;
uniform mat4 projection;

layout(std430, binding = 0) buffer ControlPoints {
    vec3 control_points[];
};

float factorial(int n) {
    float result = 1.0;
    for (int i = 1; i <= n; ++i) {
        result *= float(i);
    }
    return result;
}

vec3 calculateBezierPoint(int num_points, float t) {
    vec3 point = vec3(0.0);
    for (int i = 0; i < num_points; ++i) {
        float binomialCoeff = factorial(num_points - 1) / (factorial(i) * factorial(num_points - 1 - i));
        float bernsteinPoly = binomialCoeff * pow(t, i) * pow(1 - t, num_points - 1 - i);
        point += control_points[i] * bernsteinPoly;
    }
    return point;
}

void main() {
    int num_points = control_points.length(); // Get number of control points

    // Generate 100 points on the Bezier curve
    const int num_curve_points = 100;
    for (int i = 0; i < num_curve_points; ++i) {
        float t = float(i) / float(num_curve_points - 1);
        vec3 bezierPoint = calculateBezierPoint(num_points, t);
        gl_Position = projection * view * vec4(bezierPoint, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}

