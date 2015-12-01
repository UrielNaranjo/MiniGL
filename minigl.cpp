/**
 * Name: Uriel Naranjo
 * Email: unara001@ucr.edu
 * SID: 861103173
 *
 */

/*
 *
 * minigl.cpp
 * -------------------------------
 * Implement miniGL here.
 * Do not use any additional files
 */

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stack>
#include <math.h>
#include "minigl.h"
#include <iostream>

using namespace std;


vector<vector<MGLfloat> > modelMatrix; // stack for modelview mode
vector<vector<MGLfloat> > projMatrix; // stack for projection mode

MGLmatrix_mode mmode; // specify matrix mode
MGLpoly_mode pmode; // specify polygon mode

MGLpixel color; // stores current color

struct framebuffer{ 
    vector<MGLpixel> pixels; // color buffer for pixels
    vector<MGLfloat> zbuffer; // depth buffer for pixels
};

// stores given vertices with the current color
struct points{ 
    MGLfloat x;
    MGLfloat y;
    MGLfloat z;
    MGLfloat w;
    MGLpixel col;
};

MGLint nvert = 0; // specify the amount of vertices needed to make a primitive
vector<points> triangles, quads; // stores coordinate data
framebuffer MGL; // framebuffer for pixels


/**
 * Standard macro to report errors
 */
inline void MGL_ERROR(const char* description) {
    printf("%s\n", description);
    exit(1);
}

// sets pixel at (x,y) with color col
void set_pixel(int x, int y, MGLint width, MGLpixel col, MGLfloat z){
    size_t i = y * width + x;
    size_t size = MGL.pixels.size();

    if(z < MGL.zbuffer.at(i) && x < width && (i < size)){
        MGL.zbuffer.at(i) = z;
        MGL.pixels.at(i) = col;
    }
}

MGLpixel getColor(MGLpixel a, MGLpixel b, MGLpixel c, MGLfloat alp, MGLfloat bet, MGLfloat gam){
    MGLpixel ret = 0;
    MGLbyte r1,r2,r3,b1,b2,b3,g1,g2,g3, red, green, blue;

    r1 = MGL_GET_RED(a);
    r2 = MGL_GET_RED(b);
    r3 = MGL_GET_RED(c);

    b1 = MGL_GET_BLUE(a);
    b2 = MGL_GET_BLUE(b);
    b3 = MGL_GET_BLUE(c);

    g1 = MGL_GET_GREEN(a);
    g2 = MGL_GET_GREEN(b);
    g3 = MGL_GET_GREEN(c);

    red = (alp * r1) + (bet * r2) + (gam * r3);
    green = (alp * g1) + (bet * g2) + (gam * g3);
    blue = (alp * b1) + (bet * b2) + (gam * b3);

    MGL_SET_RED(ret, red);
    MGL_SET_GREEN(ret, green);
    MGL_SET_BLUE(ret, blue);

    return ret;
}

// applies current transformations to each vertex that is to be drawn
// does not do viewport transform as we do not know the width/height yet
void transform(float &x, float &y, float &z, float &w){
    MGLfloat tmp[4] = {x, y, z, w}; // vertex to transform
    MGLfloat result[4] = {0, 0, 0, 0};
    MGLfloat finalresult[4] = {0, 0, 0, 0};

    vector<MGLfloat> M1 = modelMatrix.at(modelMatrix.size() - 1); // modelview matrix
    vector<MGLfloat> M2 = projMatrix.at(projMatrix.size() - 1); // projection matrix


    // (projection * (modelview * vertex))

    for(size_t i = 0; i < 4; i++){ // modelview transformation
        for(size_t j = 0; j < 1; j++){
            MGLfloat temp = 0.0f;
            for(size_t k = 0; k < 4; k++){
                temp += M1[i * 4 + k] * tmp[k * 1 + j];
            }
            result[i] = temp;
        }
    }

    for(size_t i = 0; i < 4; i++){ // projection transformation
        for(size_t j = 0; j < 1; j++){
            MGLfloat temp = 0.0f;
            for(size_t k = 0; k < 4; k++){
                temp += M2[i * 4 + k] * result[k * 1 + j];
            }
            finalresult[i] = temp;
        }
    }
    
    //divide by w
    w = finalresult[3];

    x = finalresult[0] / w;
    y = finalresult[1] / w;
    z = finalresult[2] / w;

    // size_tviewport transform done in MGLReadPixels
    // since we do not know 
    // pixel height and width
}

// applies a viewport transform
void viewportTransform(MGLint width, MGLint height){
    MGLint nx = width;
    MGLint ny = height;
    MGLint a = nx/2;
    MGLint b = (nx-1)/2;
    MGLint c = ny/2;
    MGLint d = (ny -1)/2;

    MGLfloat result[4] = {0}; 
    MGLfloat tmp[4] = {0};

    MGLfloat viewport[16] = {
        a, 0, 0, b,
        0, c, 0, d,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    
    // modelview transform each point
    for(size_t t = 0; t < triangles.size(); t++){
        tmp[0] = triangles.at(t).x;
        tmp[1] = triangles.at(t).y;
        tmp[2] = 1;
        tmp[3] = 1;
        for(size_t i = 0; i < 4; i++){ 
            for(size_t j = 0; j < 1; j++){
                MGLfloat temp = 0.0f;
                for(size_t k = 0; k < 4; k++){
                   temp += viewport[i * 4 + k] * tmp[k * 1 + j];
                }
                result[i] = temp;
            }
        }
        triangles.at(t).x = result[0]; // update pixel points for x
        triangles.at(t).y = result[1]; // update pixel points for y
    }

    for(size_t t = 0; t < quads.size(); t++){
        tmp[0] = quads.at(t).x;
        tmp[1] = quads.at(t).y;
        tmp[2] = 1;
        tmp[3] = 1;
        for(size_t i = 0; i < 4; i++){ 
            for(size_t j = 0; j < 1; j++){
                MGLfloat temp = 0.0f;
                for(size_t k = 0; k < 4; k++){
                   temp += viewport[i * 4 + k] * tmp[k * 1 + j];
                }
                result[i] = temp;
            }
        }
        quads.at(t).x = result[0]; // update pixel points for x
        quads.at(t).y = result[1]; // update pixel points for y
    }
}

MGLfloat getMin(float x1, float x2, float x3){
    if(x1 < x2){
        swap(x1, x2);
    }

    if(x2 < x3){
        swap(x2, x3);
    }

    return x3;
}

MGLfloat getMax(float x1, float x2, float x3){
    if(x1 > x2){
        swap(x1, x2);
    }

    if(x2 > x3){
        swap(x2, x3);
    }

    return x3;
}

// rasterize triangles
void mglRasterize(points a, points b, points c, MGLint width){
   
    int xmin = static_cast<MGLpixel>(getMin(a.x, b.x, c.x) + 0.5);
    int xmax = static_cast<MGLpixel>(getMax(a.x, b.x, c.x) + 0.5);
    int ymin = static_cast<MGLpixel>(getMin(a.y, b.y, c.y) + 0.5);
    int ymax = static_cast<MGLpixel>(getMax(a.y, b.y, c.y) + 0.5);

    MGLfloat fabc = ((a.y - b.y)*c.x) + ((b.x - a.x)*c.y) + (a.x*b.y - b.x*a.y);
    MGLfloat facb = ((c.y - a.y)*b.x) + ((a.x - c.x)*b.y) + (c.x*a.y - a.x*c.y);
    MGLfloat fbca = ((b.y - c.y)*a.x) + ((c.x - b.x)*a.y) + (b.x*c.y - c.x*b.y);


    MGLfloat alpha, beta, gamma; // barycentric coordinates
    MGLfloat depth;

    MGLfloat fcar = ((c.y - a.y)*(-5)) + ((a.x - c.x)*(-5)) + (c.x*a.y - a.x*c.y);
    MGLfloat fabr = ((a.y - b.y)*(-5)) + ((b.x - a.x)*(-5)) + (a.x*b.y - b.x*a.y);
    MGLfloat fbcr = ((b.y - c.y)*(-5)) + ((c.x - b.x)*(-5)) + (b.x*c.y - c.x*b.y);

    MGLpixel pixelcolor = a.col;

    for(int x = xmin; x < xmax; x++){
        for(int y = ymin; y < ymax; y++){
            alpha = (((b.y - c.y)*x) + ((c.x - b.x)*y) + (b.x*c.y - c.x*b.y)) / (fbca);
            beta = (((c.y - a.y)*x) + ((a.x - c.x)*y) + (c.x*a.y - a.x*c.y)) / (facb);
            gamma = (((a.y - b.y)*x) + ((b.x - a.x)*y) + (a.x*b.y - b.x*a.y)) / (fabc);

            bool one = (0 < alpha);
            bool two = (0 < beta);
            bool three = (0 < gamma);
            bool four = (fbca*fbcr > 0);
            bool five = (facb*fcar > 0);
            bool six = (fabc*fabr > 0);

            if( (alpha >= 0) && (beta >=0) && (gamma >= 0)){
                if( (one || four ) && (two || five ) && (three || six ) ){
                    depth = (alpha * a.z) + ( beta * b.z) + (gamma * c.z);
                    pixelcolor = getColor(a.col, b.col, c.col, alpha, beta, gamma);
                    set_pixel(x,y,width, pixelcolor, depth);
                }
            }
        }
    }
}

/**
 * Read pixel data starting with the pixel at coordinates
 * (0, 0), up to (width,  height), into the array
 * pointed to by data.  The boundaries are lower-inclusive,
 * that is, a call with width = height = 1 would just read
 * the pixel at (0, 0).
 *
 * Rasterization and z-buffering should be performed when
 * this function is called, so that the data array is filled
 * with the actual pixel values that should be displayed on
 * the two-dimensional screen.
 */
void mglReadPixels(MGLsize width, MGLsize height, MGLpixel *data){

    // viewport transform to get correct pixel coordinates
    viewportTransform(width, height);

    // resize to size of window and initialize pixels to 0
    MGL.pixels.resize(width * height, 0); 
    // resize to size of window and initialize to furthest point
    MGL.zbuffer.resize(width * height, 1.0f); 

    for(size_t i = 0; i+2 < triangles.size(); i+=3){
        mglRasterize(triangles.at(i), triangles.at(i+1), triangles.at(i+2), width);
    }

    // quads are made by rasterizing two triangles
    for(size_t i = 0; i+3 < quads.size(); i+=4){
        mglRasterize(quads.at(i), quads.at(i+1), quads.at(i+2), width);
        mglRasterize(quads.at(i), quads.at(i+2), quads.at(i+3), width);
    }

    copy(MGL.pixels.begin(), MGL.pixels.end(), data);

}

/**
 * Start specifying the vertices for a group of primitives,
 * whose type is specified by the given mode.
 */
void mglBegin(MGLpoly_mode mode){

    switch(mode){
        case MGL_TRIANGLES:
            pmode = MGL_TRIANGLES;
            nvert = 3;
            break;

        case MGL_QUADS:
            pmode = MGL_QUADS;
            nvert = 4;
            break;

        default:
            MGL_ERROR("Invalid polymode");
            break;
    }

}

/**
 * Stop specifying the vertices for a group of primitives.
 */
void mglEnd() {


    //MGLpixel *pixels;

    // TRIANGLES
    if(pmode == MGL_TRIANGLES){
        
        int temp = triangles.size();

        if(temp % 3 == 1){ // one extra vertex not needed
            triangles.pop_back();
        }

        else if(temp % 3 == 2){ // two extra vertex not needed
            triangles.pop_back();    
            triangles.pop_back();
        }

    }

    // QUADS
    else if(pmode == MGL_QUADS){ 

        int temp = quads.size();

        if(temp % 4 == 1){
            quads.pop_back();
        }

        else if(temp % 4 == 2){
            quads.pop_back();
            quads.pop_back();
        }

        else if(temp % 4 == 3){
            quads.pop_back();
            quads.pop_back();
            quads.pop_back();
        }

    }

    else{
        MGL_ERROR("Unspecified polygon mode.");
    }

    nvert = 0; // unspecify primitives

}

/**
 * Specify a two-dimensional vertex; the x- and y-coordinates
 * are explicitly specified, while the z-coordinate is assumed
 * to be zero.  Must appear between calls to mglBegin() and
 * mglEnd().
 */
void mglVertex2(MGLfloat x, MGLfloat y){
    // if not called between being and end then error
    MGLfloat z = 0;
    MGLfloat w = 1;
    transform(x, y, z,w);
    points point = {x,y,z,w, color};
    if(pmode == MGL_TRIANGLES){
        triangles.push_back(point);
    }
    else{
        quads.push_back(point);
    }
}

/**
 * Specify a three-dimensional vertex.  Must appear between
 * calls to mglBegin() and mglEnd().
 */
void mglVertex3(MGLfloat x, MGLfloat y, MGLfloat z){
    MGLfloat w = 1;
    transform(x,y,z,w);
    points point = {x,y,z,w, color};
    if(pmode == MGL_TRIANGLES){
        triangles.push_back(point);
    }
    else{
        quads.push_back(point);
    }
}

/**
 * Set the current matrix mode (modelview or projection).
 */
void mglMatrixMode(MGLmatrix_mode mode){

    switch(mode){
        
        case MGL_MODELVIEW:
            mmode = MGL_MODELVIEW;
            break;

        case MGL_PROJECTION:
            mmode = MGL_PROJECTION;
            break;

        default:
            MGL_ERROR("Invalid Matrix mode.");
            break;

    }

}

/**
 * Push a copy of the current matrix onto the stack for the
 * current matrix mode.
 */
void mglPushMatrix(){

    if(mmode == MGL_MODELVIEW){
        int i = modelMatrix.size() - 1;
        vector<MGLfloat> temp = modelMatrix.at(i);
        modelMatrix.push_back(temp);
    }

    else if(mmode == MGL_PROJECTION){
        int i = projMatrix.size() - 1;
        vector<MGLfloat> temp = projMatrix.at(i);
        projMatrix.push_back(temp);
    }

    else{
        MGL_ERROR("Matrix mode not defined.");
    }
}

/**
 * Pop the top matrix from the stack for the current matrix
 * mode.
 */
void mglPopMatrix(){

    if(mmode == MGL_MODELVIEW){
        if(!modelMatrix.empty()){
            modelMatrix.pop_back();
        }
    }

    else if(mmode == MGL_PROJECTION){
        if(!projMatrix.empty()){
            projMatrix.pop_back();
        }
    }

    else{
        MGL_ERROR("Matrix mode not defined.");
    }
}

/**
 * Replace the current matrix with the identity.
 */
void mglLoadIdentity(){
    
    MGLfloat tmp[16]= {1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
                            0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f}; 

    vector<MGLfloat> I;
    for(size_t i = 0; i < 16; i++){
        I.push_back(tmp[i]);
    }

    if(mmode == MGL_MODELVIEW){
        if(!modelMatrix.empty()){
            modelMatrix.pop_back();
        }
        modelMatrix.push_back(I);
    }

    else if(mmode == MGL_PROJECTION){
        if(!projMatrix.empty()){
            projMatrix.pop_back();
        }
        projMatrix.push_back(I);
    }

    else{
        MGL_ERROR("Matrix mode no specified.");
    }

}

/**
 * Replace the current matrix with an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglLoadMatrix(const MGLfloat *matrix){
    
    vector<MGLfloat> temp;

    for(size_t i = 0; i < 16; i++){
        temp.push_back(matrix[i]);
    }

    if(mmode == MGL_MODELVIEW){
        if(!modelMatrix.empty()){
            modelMatrix.pop_back();
        }
        modelMatrix.push_back(temp);
    }

    else if(mmode == MGL_PROJECTION){
        if(!projMatrix.empty()){
            projMatrix.pop_back();
        }
        projMatrix.push_back(temp);
    }

    else{
        MGL_ERROR("Matrix mode no specified.");
    }


}

/**
 * Multiply the current matrix by an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglMultMatrix(const MGLfloat *matrix){

    vector<MGLfloat> M (16, 0.0f); // new matrix stored here

    vector<MGLfloat> M1(16); // will point to matrix on current matrix

    if(mmode == MGL_PROJECTION){

        if(projMatrix.empty()){
            MGL_ERROR("Matrix stack is empty."); 
        }
        
        int i = projMatrix.size() - 1;
        M1 = projMatrix.at(i);
    }

    else if(mmode == MGL_MODELVIEW){

        if(modelMatrix.empty()){
            MGL_ERROR("Matrix stack is empty");
        }
        
        int i = modelMatrix.size() - 1;
        M1 = modelMatrix.at(i);
    }

    size_t row = 4; // row length
    size_t col = 4; // col length

    for(size_t i = 0; i < row; i++){

        for(size_t j = 0; j < col; j++){
            M[i * row + j] = 0;
            for(size_t k = 0; k < col; k++){
                M[i * row + j] += M1[i * col + k] * matrix[k * col + j];
            }
        }

    }
    
    if(mmode == MGL_PROJECTION){
        projMatrix.pop_back();
        projMatrix.push_back(M);
    }

    else if(mmode == MGL_MODELVIEW){
        modelMatrix.pop_back();
        modelMatrix.push_back(M);
    }

}

/**
 * Multiply the current matrix by the translation matrix
 * for the translation vector given by (x, y, z).
 */
void mglTranslate(MGLfloat x, MGLfloat y, MGLfloat z){
    MGLfloat translate[16] = 
    {
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1
    };

    mglMultMatrix(translate);

}

/**
 * Multiply the current matrix by the rotation matrix
 * for a rotation of (angle) degrees about the vector
 * from the origin to the point (x, y, z).
 */
void mglRotate(MGLfloat angle, MGLfloat x, MGLfloat y, MGLfloat z){

    //convert angle to radians
    angle = angle * (M_PI / 180);

    MGLfloat costheta = cos(angle);
    MGLfloat sintheta = sin(angle);

    MGLfloat magnitude = sqrt((x*x) + (y*y) + (z*z));

    // normalize
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
    
    MGLfloat a = (x*x)*(1-costheta) + costheta;
    MGLfloat b = (x*y)*(1-costheta) - (z * sintheta);
    MGLfloat c = (x*z)*(1-costheta) + (y * sintheta);
    MGLfloat d = (y*x)*(1-costheta) + (z * sintheta);
    MGLfloat e = (y*y)*(1-costheta) + costheta;
    MGLfloat f = (y*z)*(1-costheta) - (x * sintheta);
    MGLfloat g = (x*z)*(1-costheta) - (y * sintheta);
    MGLfloat h = (y*z)*(1-costheta) + (x * sintheta);
    MGLfloat i = (z*z)*(1-costheta) + costheta;

    MGLfloat rotate[16] = 
    {
        a, b, c, 0,
        d, e, f, 0,
        g, h, i, 0,
        0, 0, 0, 1
    };

    mglMultMatrix(rotate);

}

/**
 * Multiply the current matrix by the scale matrix
 * for the given scale factors.
 */
void mglScale(MGLfloat x, MGLfloat y, MGLfloat z){
    MGLfloat scale[16] = 
    {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };

    mglMultMatrix(scale);
    
}

/**
 * Multiply the current matrix by the perspective matrix
 * with the given clipping plane coordinates.
 */
void mglFrustum(MGLfloat left, MGLfloat right, MGLfloat bottom,
                     MGLfloat top, MGLfloat near, MGLfloat far)
{
    MGLfloat A = (right + left) / (right - left);
    MGLfloat B = (top + bottom) / (top - bottom);
    MGLfloat C = -((far + near) / (far - near));
    MGLfloat D = -((2.0f*far*near) / (far - near));
    MGLfloat E = (2.0f*near) / (right - left);
    MGLfloat F = (2.0f*near) / (top - bottom);

    MGLfloat frust[16] = 
    {
        E, 0, A, 0,
        0, F, B, 0,
        0, 0, C, D,
        0, 0,-1, 0
    };

    mglMultMatrix(frust);

}

/**
 * Multiply the current matrix by the orthographic matrix
 * with the given clipping plane coordinates.
 */
void mglOrtho(MGLfloat left, MGLfloat right, MGLfloat bottom,
                MGLfloat top, MGLfloat near, MGLfloat far)
{
    MGLfloat x = -((right + left) / (right - left));
    MGLfloat y = -((top + bottom) / (top - bottom));
    MGLfloat z = -((far + near) / (far - near));

    MGLfloat w = (2) / (right - left);
    MGLfloat v = (2) / (top - bottom);
    MGLfloat t = (-2) / (far - near);

    MGLfloat ortho[16] =
    {
        w, 0, 0, x,
        0, v, 0, y,
        0, 0, t, z,
        0, 0, 0, 1
    };

    mglMultMatrix(ortho);

}

/**
 * Set the current color for drawn shapes.
 */
void mglColor(MGLbyte red, MGLbyte green, MGLbyte blue){
    MGLpixel temp = 0;
    MGL_SET_RED(temp, red);
    MGL_SET_GREEN(temp, green);
    MGL_SET_BLUE(temp, blue);
    color = temp;
}
