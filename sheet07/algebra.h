/****************************************************************
*																*
* C++ Vector and Matrix Algebra routines						*
* Author: Jean-Francois DOUE									*
* Version 3.1 --- October 1993									*
*																*
****************************************************************/
//
//	From "Graphics Gems IV / Edited by Paul S. Heckbert
//	Academic Press, 1994, ISBN 0-12-336156-9
//	"You are free to use and modify this code in any way 
//	you like." (p. xv)
//
//	Modified by J. Nagle, March 1997
//	-	All functions are inline.
//	-	All functions are const-correct.
//	-	All checking is via the standard "assert" macro.
//	-	Stream I/O is disabled for portability, but can be
//		re-enabled by defining ALGEBRA3IOSTREAMS.
//
//      Modified by M. Eisemann, October 2007
//      -       Made more similar to GLSL
//      -       operator * is now component-wise
//      -       dotproduct is now dot function
//      -       added namespace to circumvent min/max problems
//      -       added ivec2, ivec3 and ivec4
//
// Todo:
// - make vec2, vec3, vec4 work with ivec, e.g. vec2 = vec2 + ivec2
//	
#ifndef ALGEBRA3H
#define ALGEBRA3H

#include <stdlib.h>
#include <assert.h>

#ifdef WIN32
// Windows
#define _USE_MATH_DEFINES
#include <math.h>
#else
#include <cmath>
#endif



namespace alg {
  // this line defines a new type: pointer to a function which returns a
  // float and takes as argument a float
  typedef float (*V_FCT_PTR)(float);
  typedef int   (*VI_FCT_PTR)(int);
  
  // min-max macros
#define MIN(A,B) ((A) < (B) ? (A) : (B))
#define MAX(A,B) ((A) > (B) ? (A) : (B))
  
#undef min					// allow as function names
#undef max
  
  // error handling macro
#define ALGEBRA_ERROR(E) { assert(false); }
  
  class vec2;
  class vec3;
  class vec4;
  class mat3;
  class mat4;

  class ivec2;
  class ivec3;
  class ivec4;
  
  enum vxyzw{VX, VY, VZ, VW};		    // axes
  enum papbpcpd{PA, PB, PC, PD};		    // planes
  enum rgb{RED, GREEN, BLUE};	    // colors
  enum phong{KA, KD, KS, ES};		    // phong coefficients
  //
  //	PI
  //
  //const float M_PI_2 = (M_PI/2.0);				// PI/2
  const float M2_PI = (M_PI*2.0);				// PI*2
  
  
  /****************************************************************
   *																*
   *			    2D Vector										*
   *																*
   ****************************************************************/
  
  class vec2
  {
  protected:
    
    float n[2];
    
  public:
    
    // Constructors
    
    vec2();
    vec2(const float x, const float y);
    vec2(const float d);
    vec2(const vec2& v);			// copy constructor
    vec2(const vec3& v);			// cast v3 to v2
    vec2(const vec3& v, int dropAxis);	// cast v3 to v2
    
    // Assignment operators
    
    vec2& operator  = ( const vec2& v );	// assignment of a vec2
    vec2& operator += ( const vec2& v );	// incrementation by a vec2
    vec2& operator -= ( const vec2& v );	// decrementation by a vec2
    vec2& operator *= ( const vec2& v);         // componentwise multiplication by a vec2
    vec2& operator *= ( const float d );	// multiplication by a constant
    vec2& operator /= ( const float d );	// division by a constant
    vec2& operator /= ( const vec2& v );
    float& operator [] ( int i);		// indexing
    float operator [] ( int i) const;       // read-only indexing
    
    float* ptr();
    const float* ptr() const;

    // special functions
    
    float length() const;			// length of a vec2
    float length2() const;			// squared length of a vec2
    vec2& normalize() ;			// normalize a vec2 in place
    vec2& apply(V_FCT_PTR fct);		// apply a func. to each component
    
    // friends
    
    friend vec2 operator - (const vec2& v);	                    // -v1
    friend vec2 operator + (const vec2& a, const vec2& b);	    // v1 + v2
    friend vec2 operator - (const vec2& a, const vec2& b);	    // v1 - v2
    friend vec2 operator * (const vec2& a, const float d);	    // v1 * 3.0
    friend vec2 operator * (const float d, const vec2& a);	    // 3.0 * v1
    friend vec2 operator * (const mat3& a, const vec2& v);	    // M . v
    friend vec2 operator * (const vec2& v, const mat3& a);	    // v . M
    friend vec2 operator * (const vec2& a, const vec2& b);          // component-wise product
    friend vec2 operator / (const vec2& a, const float d);	    // v1 / 3.0
    friend vec2 operator / (const vec2& a, const vec2& b);	    // v1 / v2 componentwise
    friend vec3 operator ^ (const vec2& a, const vec2& b);	    // cross product
    friend int operator == (const vec2& a, const vec2& b);	    // v1 == v2 ?
    friend int operator != (const vec2& a, const vec2& b);	    // v1 != v2 ?
    
#ifdef ALGEBRA3IOSTREAMS
    friend ostream& operator << (ostream& s, const vec2& v);    // output to stream
    friend istream& operator >> (istream& s, vec2& v);	    // input from strm.
#endif //ALGEBRA3IOSTREAMS
    
    friend void  swap(vec2& a, vec2& b);			    // swap v1 & v2
    friend float min(const vec2& a);
    friend float max(const vec2& a);
    friend vec2  min(const vec2& a, const vec2& b);		    // min(v1, v2)
    friend vec2  max(const vec2& a, const vec2& b);		    // max(v1, v2)
    friend vec2  prod(const vec2& a, const vec2& b);		    // term by term *
    friend float dot(const vec2& a, const vec2& b);              // dot product
    
    // necessary friend declarations
    
    friend class vec3;
  };
  
  /****************************************************************
   *																*
   *			    3D Vector										*
   *																*
   ****************************************************************/
  
  class vec3
  {
  protected:
    
    float n[3];
    
  public:
    
    // Constructors
    
    vec3();
    vec3(const float x, const float y, const float z);
    vec3(const float d);
    vec3(const vec3& v);					// copy constructor
    vec3(const vec2& v);					// cast v2 to v3
    vec3(const vec2& v, float d);		                // cast v2 to v3
    vec3(const vec4& v);					// cast v4 to v3
    vec3(const vec4& v, int dropAxis);	                // cast v4 to v3
    
    // Assignment operators
    
    vec3& operator	= ( const vec3& v );	    // assignment of a vec3
    vec3& operator += ( const vec3& v );	    // incrementation by a vec3
    vec3& operator -= ( const vec3& v );	    // decrementation by a vec3
    vec3& operator *= ( const float d );	    // multiplication by a constant
    vec3& operator *= ( const vec3& v );	    // component-wise multiplication by a vec3
    vec3& operator /= ( const float d );	    // division by a constant
    vec3& operator /= ( const vec3& v );	    // compontent-wise division by a vec3
    float& operator [] ( int i);		    // indexing
    float operator[] (int i) const;		    // read-only indexing
    
    float* ptr();
    const float* ptr() const;
    
    
    // special functions
    
    float length() const;			    // length of a vec3
    float length2() const;			    // squared length of a vec3
    vec3& normalize();			    // normalize a vec3 in place
    vec3& apply(V_FCT_PTR fct);		    // apply a func. to each component
    
    // friends
    
    friend vec3 operator - (const vec3& v);						// -v1
    friend vec3 operator + (const vec3& a, const vec3& b);	    // v1 + v2
    friend vec3 operator - (const vec3& a, const vec3& b);	    // v1 - v2
    friend vec3 operator * (const vec3& a, const float d);	    // v1 * 3.0
    friend vec3 operator * (const float d, const vec3& a);	    // 3.0 * v1
    friend vec3 operator * (const mat4& a, const vec3& v);	    // M . v
    friend vec3 operator * (const vec3& v, const mat4& a);	    // v . M
    friend vec3 operator * (const vec3& a, const vec3& b);          // component-wise multiplication
    friend vec3 operator / (const vec3& a, const float d);	    // v1 / 3.0
    friend vec3 operator / (const vec3& a, const vec3& b);	    // v1 / v2 componentwise
    friend vec3 operator ^ (const vec3& a, const vec3& b);	    // cross product
    friend int operator == (const vec3& a, const vec3& b);	    // v1 == v2 ?
    friend int operator != (const vec3& a, const vec3& b);	    // v1 != v2 ?
    
#ifdef ALGEBRA3IOSTREAMS
    friend ostream& operator << (ostream& s, const vec3& v);    // output to stream
    friend istream& operator >> (istream& s, vec3& v);	    // input from strm.
#endif // ALGEBRA3IOSTREAMS
    
    friend void  swap(vec3& a, vec3& b);			    // swap v1 & v2
    friend vec3  min(const vec3& a, const vec3& b);		    // min(v1, v2)
    friend vec3  max(const vec3& a, const vec3& b);		    // max(v1, v2)
    friend vec3  prod(const vec3& a, const vec3& b);	    // term by term *
    friend float dot(const vec3& a, const vec3& b);             // dot product
    friend float angleBetween(const vec3& a, const vec3& b);    // angle between two vectors
    
    // necessary friend declarations
    
    friend class vec2;
    friend class vec4;
    friend class mat3;
    friend vec2 operator * (const mat3& a, const vec2& v);	// linear transform
    friend vec3 operator * (const mat3& a, const vec3& v);
    friend mat3 operator * (const mat3& a, const mat3& b);	// matrix 3 product
  };
  
  /****************************************************************
   *																*
   *			    4D Vector										*
   *																*
   ****************************************************************/
  
  class vec4
  {
  protected:
    
    float n[4];
    
  public:
    
    // Constructors
    
    vec4();
    vec4(const float x, const float y, const float z, const float w);
    vec4(const float d);
    vec4(const vec4& v);			    // copy constructor
    vec4(const vec3& v);			    // cast vec3 to vec4
    vec4(const vec3& v, const float d);	    // cast vec3 to vec4
    
    // Assignment operators
    
    vec4& operator	= ( const vec4& v );	    // assignment of a vec4
    vec4& operator += ( const vec4& v );	    // incrementation by a vec4
    vec4& operator -= ( const vec4& v );	    // decrementation by a vec4
    vec4& operator *= ( const float d );	    // multiplication by a constant
    vec4& operator *= ( const vec4& v );	    // componentwise multiplication by a vec4
    vec4& operator /= ( const float d );	    // division by a constant
    vec4& operator /= ( const vec4& v );	    // componentwise division by a vec4
    float& operator [] ( int i);    	    // indexing
    float operator[] (int i) const;	    // read-only indexing
    
    float* ptr();
    const float* ptr() const;
    
    // special functions
    
    float length() const;			// length of a vec4
    float length2() const;			// squared length of a vec4
    vec4& normalize();			// normalize a vec4 in place
    vec4& apply(V_FCT_PTR fct);		// apply a func. to each component
    
    // friends
    
    friend vec4 operator - (const vec4& v);		    // -v1
    friend vec4 operator + (const vec4& a, const vec4& b);	    // v1 + v2
    friend vec4 operator - (const vec4& a, const vec4& b);	    // v1 - v2
    friend vec4 operator * (const vec4& a, const float d);	    // v1 * 3.0
    friend vec4 operator * (const float d, const vec4& a);	    // 3.0 * v1
    friend vec4 operator * (const mat4& a, const vec4& v);	    // M . v
    friend vec4 operator * (const vec4& v, const mat4& a);	    // v . M
    friend vec4 operator * (const vec4& a, const vec4& b);     // component-wise multiplication
    friend vec4 operator / (const vec4& a, const float d);	    // v1 / 3.0
    friend vec4 operator / (const vec4& a, const vec4& b);	    // v1 / v2 componentwise
    friend int operator == (const vec4& a, const vec4& b);	    // v1 == v2 ?
    friend int operator != (const vec4& a, const vec4& b);	    // v1 != v2 ?
    
#ifdef ALGEBRA3IOSTREAMS
    friend ostream& operator << (ostream& s, const vec4& v);	// output to stream
    friend istream& operator >> (istream& s, vec4& v);	    // input from strm.
#endif //  ALGEBRA3IOSTREAMS
    
    friend void  swap(vec4& a, vec4& b);			    // swap v1 & v2
    friend vec4  min(const vec4& a, const vec4& b);	    // min(v1, v2)
    friend vec4  max(const vec4& a, const vec4& b);	    // max(v1, v2)
    friend vec4  prod(const vec4& a, const vec4& b);           // term by term *
    friend float dot(const vec4& a, const vec4& b);            // dot product
    
    // necessary friend declarations
    
    friend class vec3;
    friend class mat4;
    friend vec3 operator * (const mat4& a, const vec3& v);	// linear transform
    friend mat4 operator * (const mat4& a, const mat4& b);	// matrix 4 product
  };
  
  /****************************************************************
   *								*
   *			   3x3 Matrix                           *
   *                                                               *
   ****************************************************************/
  
  class mat3
  {
  protected:
    
    vec3 v[3];
    
  public:
    
    // Constructors
    
    mat3();
    mat3(const vec3& v0, const vec3& v1, const vec3& v2);
    mat3(const float d);
    mat3(const mat3& m);
    
    // Assignment operators
    
    mat3& operator	= ( const mat3& m );	    // assignment of a mat3
    mat3& operator += ( const mat3& m );	    // incrementation by a mat3
    mat3& operator -= ( const mat3& m );	    // decrementation by a mat3
    mat3& operator *= ( const float d );	    // multiplication by a constant
    mat3& operator /= ( const float d );	    // division by a constant
    vec3& operator [] ( int i);		    // indexing
    const vec3& operator [] ( int i) const;	    // read-only indexing
    
    // special functions
    
    mat3 transpose() const;			    // transpose
    mat3 inverse() const;			    // inverse
    mat3& apply(V_FCT_PTR fct);		    // apply a func. to each element
    
    // friends
    
    friend mat3 operator - (const mat3& a);			    // -m1
    friend mat3 operator + (const mat3& a, const mat3& b);	    // m1 + m2
    friend mat3 operator - (const mat3& a, const mat3& b);	    // m1 - m2
    friend mat3 operator * (const mat3& a, const mat3& b);	    // m1 * m2
    friend mat3 operator * (const mat3& a, const float d);	    // m1 * 3.0
    friend mat3 operator * (const float d, const mat3& a);	    // 3.0 * m1
    friend mat3 operator / (const mat3& a, const float d);	    // m1 / 3.0
    friend int operator == (const mat3& a, const mat3& b);	    // m1 == m2 ?
    friend int operator != (const mat3& a, const mat3& b);	    // m1 != m2 ?
    
#ifdef ALGEBRA3IOSTREAMS
    friend ostream& operator << (ostream& s, const mat3& m);    // output to stream
    friend istream& operator >> (istream& s, mat3& m);	    // input from strm.
#endif //  ALGEBRA3IOSTREAMS
    
    friend void swap(mat3& a, mat3& b);			    // swap m1 & m2
    
    // necessary friend declarations
    
    friend vec3 operator * (const mat3& a, const vec3& v);	    // linear transform
    friend vec2 operator * (const mat3& a, const vec2& v);	    // linear transform
  };
  
  /****************************************************************
   *								*
   *			   4x4 Matrix				*
   *								*
   ****************************************************************/
  
  class mat4
  {
  protected:
    
    vec4 v[4];
    
  public:
    
    // Constructors
    
    mat4();
    mat4(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3);
    mat4(const float d);
    mat4(const float* d);
    mat4(const mat4& m);
    
    void getValues(float* d);
    float* ptr();
    const float* ptr() const;
    
    
    // Assignment operators
    
    mat4& operator	= ( const mat4& m );	    // assignment of a mat4
    mat4& operator += ( const mat4& m );	    // incrementation by a mat4
    mat4& operator -= ( const mat4& m );	    // decrementation by a mat4
    mat4& operator *= ( const float d );	    // multiplication by a constant
    mat4& operator /= ( const float d );	    // division by a constant
    vec4& operator [] ( int i);		    // indexing
    const vec4& operator [] ( int i) const;	    // read-only indexing
    
    // special functions
    
    mat4 transpose() const;			    // transpose
    mat4 inverse() const;			    // inverse
    mat4& apply(V_FCT_PTR fct);		    // apply a func. to each element
    
    // friends
    
    friend mat4 operator - (const mat4& a);			    // -m1
    friend mat4 operator + (const mat4& a, const mat4& b);	    // m1 + m2
    friend mat4 operator - (const mat4& a, const mat4& b);	    // m1 - m2
    friend mat4 operator * (const mat4& a, const mat4& b);	    // m1 * m2
    friend mat4 operator * (const mat4& a, const float d);	    // m1 * 4.0
    friend mat4 operator * (const float d, const mat4& a);	    // 4.0 * m1
    friend mat4 operator / (const mat4& a, const float d);	    // m1 / 3.0
    friend int operator == (const mat4& a, const mat4& b);	    // m1 == m2 ?
    friend int operator != (const mat4& a, const mat4& b);	    // m1 != m2 ?
    
#ifdef ALGEBRA3IOSTREAMS
    friend ostream& operator << (ostream& s, const mat4& m);    // output to stream
    friend istream& operator >> (istream& s, mat4& m);	    // input from strm.
#endif //  ALGEBRA3IOSTREAMS
    
    friend void swap(mat4& a, mat4& b);			    // swap m1 & m2
    
    // necessary friend declarations
    
    friend vec4 operator * (const mat4& a, const vec4& v);	    // linear transform
    friend vec3 operator * (const mat4& a, const vec3& v);	    // linear transform
  };


/****************************************************************
   *								*
   *			    2D Integer Vector			*
   *								*
   ****************************************************************/
  
  class ivec2
  {
  protected:
    
    int n[2];
    
  public:
    
    // Constructors
    
    ivec2();
    ivec2(const int x, const int y);
    ivec2(const int d);
    ivec2(const ivec2& v);			// copy constructor
    ivec2(const ivec3& v);			// cast v3 to v2
    ivec2(const ivec3& v, int dropAxis);	// cast v3 to v2
    
    // Assignment operators
    
    ivec2& operator  = ( const ivec2& v );	// assignment of a ivec2
    ivec2& operator += ( const ivec2& v );	// incrementation by a ivec2
    ivec2& operator -= ( const ivec2& v );	// decrementation by a ivec2
    ivec2& operator *= ( const ivec2& v);       // componentwise multiplication by a vec2
    ivec2& operator *= ( const int d );	        // multiplication by a constant
    ivec2& operator /= ( const int d );	        // division by a constant
    ivec2& operator /= ( const ivec2& v );
    int& operator [] ( int i);		        // indexing
    int operator [] ( int i) const;             // read-only indexing
    

    int* ptr();
    const int* ptr() const;

    // special functions
    float length() const;			// length of a vec2
    float length2() const;			// squared length of a vec2
    ivec2& apply(VI_FCT_PTR fct);		// apply a func. to each component
    
    // friends
    
    friend ivec2 operator - (const ivec2& v);	                    // -v1
    friend ivec2 operator + (const ivec2& a, const ivec2& b);	    // v1 + v2
    friend ivec2 operator - (const ivec2& a, const ivec2& b);	    // v1 - v2
    friend ivec2 operator * (const ivec2& a, const int d);	    // v1 * 3.0
    friend ivec2 operator * (const int d, const ivec2& b);	    // 3.0 * v1
    friend ivec2 operator * (const ivec2& a, const ivec2& b);       // component-wise product
    friend ivec2 operator / (const ivec2& a, const int d);	    // v1 / 3.0
    friend ivec2 operator / (const ivec2& a, const ivec2& b);	    // v1 / v2 componentwise
    friend int operator == (const ivec2& a, const ivec2& b);	    // v1 == v2 ?
    friend int operator != (const ivec2& a, const ivec2& b);	    // v1 != v2 ?
    
#ifdef ALGEBRA3IOSTREAMS
    friend ostream& operator << (ostream& s, const ivec2& v);    // output to stream
    friend istream& operator >> (istream& s, vec2& v);	    // input from strm.
#endif //ALGEBRA3IOSTREAMS
    
    friend void  swap(ivec2& a, ivec2& b);			    // swap v1 & v2
    friend int   min(const ivec2& a);
    friend int   max(const ivec2& a);
    friend ivec2 min(const ivec2& a, const ivec2& b);		    // min(v1, v2)
    friend ivec2 max(const ivec2& a, const ivec2& b);		    // max(v1, v2)
    friend ivec2 prod(const ivec2& a, const ivec2& b);		    // term by term *
    friend int   dot(const ivec2& a, const ivec2& b);              // dot product
    
    // necessary friend declarations
    
    friend class ivec3;
  };


 /****************************************************************
   *																*
   *			    3D Integer Vector										*
   *																*
   ****************************************************************/
  
  class ivec3
  {
  protected:
    
    int n[3];
    
  public:
    
    // Constructors
    
    ivec3();
    ivec3(const int x, const int y, const int z);
    ivec3(const int d);
    ivec3(const ivec3& v);					// copy constructor
    ivec3(const ivec2& v);					// cast v2 to v3
    ivec3(const ivec2& v, int d);		                // cast v2 to v3
    ivec3(const ivec4& v);					// cast v4 to v3
    ivec3(const ivec4& v, int dropAxis);	                // cast v4 to v3
    
    // Assignment operators
    
    ivec3& operator  = ( const ivec3& v );	    // assignment of a vec3
    ivec3& operator += ( const ivec3& v );	    // incrementation by a vec3
    ivec3& operator -= ( const ivec3& v );	    // decrementation by a vec3
    ivec3& operator *= ( const int d ); 	    // multiplication by a constant
    ivec3& operator *= ( const ivec3& v );	    // component-wise multiplication by a vec3
    ivec3& operator /= ( const int d ); 	    // division by a constant
    ivec3& operator /= ( const ivec3& v );	    // compontent-wise division by a vec3
    int& operator [] ( int i);	         	    // indexing
    int operator[] (int i) const;		    // read-only indexing
    
    int* ptr();
    const int* ptr() const;
    
    
    // special functions
    
    float length() const;			    // length of a vec3
    float length2() const;			    // squared length of a vec3
    ivec3& apply(VI_FCT_PTR fct);		    // apply a func. to each component
    
    // friends
    
    friend ivec3 operator - (const ivec3& v);						// -v1
    friend ivec3 operator + (const ivec3& a, const ivec3& b);	    // v1 + v2
    friend ivec3 operator - (const ivec3& a, const ivec3& b);	    // v1 - v2
    friend ivec3 operator * (const ivec3& a, const int d);	    // v1 * 3.0
    friend ivec3 operator * (const int d, const ivec3& a);	    // 3.0 * v1
    friend ivec3 operator * (const ivec3& a, const ivec3& b);          // component-wise multiplication
    friend ivec3 operator / (const ivec3& a, const int d);	    // v1 / 3.0
    friend ivec3 operator / (const ivec3& a, const ivec3& b);	    // v1 / v2 componentwise
    friend ivec3 operator ^ (const ivec3& a, const ivec3& b);	    // cross product
    friend int operator == (const ivec3& a, const ivec3& b);	    // v1 == v2 ?
    friend int operator != (const ivec3& a, const ivec3& b);	    // v1 != v2 ?
    
#ifdef ALGEBRA3IOSTREAMS
    friend ostream& operator << (ostream& s, const ivec3& v);    // output to stream
    friend istream& operator >> (istream& s, ivec3& v);	    // input from strm.
#endif // ALGEBRA3IOSTREAMS
    
    friend void  swap(ivec3& a, ivec3& b);			    // swap v1 & v2
    friend ivec3 min(const ivec3& a, const ivec3& b);		    // min(v1, v2)
    friend ivec3 max(const ivec3& a, const ivec3& b);		    // max(v1, v2)
    friend ivec3 prod(const ivec3& a, const ivec3& b); 	    // term by term *
    friend int   dot(const ivec3& a, const ivec3& b);                 // dot product
    
    // necessary friend declarations
    
    friend class ivec2;
    friend class ivec4;
  };



 /****************************************************************
   *																*
   *			    4D Integer Vector										*
   *																*
   ****************************************************************/
  
  class ivec4
  {
  protected:
    
    int n[4];
    
  public:
    
    // Constructors
    
    ivec4();
    ivec4(const int x, const int y, const int z, const int w);
    ivec4(const int d);
    ivec4(const ivec4& v);			    // copy constructor
    ivec4(const ivec3& v);			    // cast vec3 to vec4
    ivec4(const ivec3& v, const int d);	    // cast vec3 to vec4
    
    // Assignment operators
    
    ivec4& operator  = ( const ivec4& v );	    // assignment of a vec4
    ivec4& operator += ( const ivec4& v );	    // incrementation by a vec4
    ivec4& operator -= ( const ivec4& v );	    // decrementation by a vec4
    ivec4& operator *= ( const int d ); 	    // multiplication by a constant
    ivec4& operator *= ( const ivec4& v );	    // componentwise multiplication by a vec4
    ivec4& operator /= ( const int d ); 	    // division by a constant
    ivec4& operator /= ( const ivec4& v );	    // componentwise division by a vec4
    int& operator [] ( int i);    	    // indexing
    int operator[] (int i) const;	    // read-only indexing
    
    int* ptr();
    const int* ptr() const;
    
    // special functions
    
    float length() const;			// length of a vec4
    float length2() const;			// squared length of a vec4
    ivec4& apply(VI_FCT_PTR fct);		// apply a func. to each component
    
    // friends
    
    friend ivec4 operator - (const ivec4& v);		    // -v1
    friend ivec4 operator + (const ivec4& a, const ivec4& b);	    // v1 + v2
    friend ivec4 operator - (const ivec4& a, const ivec4& b);	    // v1 - v2
    friend ivec4 operator * (const ivec4& a, const int d);	    // v1 * 3.0
    friend ivec4 operator * (const int d, const ivec4& a);	    // 3.0 * v1
    friend ivec4 operator * (const ivec4& a, const ivec4& b);     // component-wise multiplication
    friend ivec4 operator / (const ivec4& a, const int d);	    // v1 / 3.0
    friend ivec4 operator / (const ivec4& a, const ivec4& b);	    // v1 / v2 componentwise
    friend int operator == (const ivec4& a, const ivec4& b);	    // v1 == v2 ?
    friend int operator != (const ivec4& a, const ivec4& b);	    // v1 != v2 ?
    
#ifdef ALGEBRA3IOSTREAMS
    friend ostream& operator << (ostream& s, const ivec4& v);	// output to stream
    friend istream& operator >> (istream& s, ivec4& v);	    // input from strm.
#endif //  ALGEBRA3IOSTREAMS
    
    friend void  swap(ivec4& a, ivec4& b);			    // swap v1 & v2
    friend ivec4 min(const ivec4& a, const ivec4& b);	    // min(v1, v2)
    friend ivec4 max(const ivec4& a, const ivec4& b);	    // max(v1, v2)
    friend ivec4 prod(const ivec4& a, const ivec4& b);           // term by term *
    friend int   dot(const ivec4& a, const ivec4& b);            // dot product
    
    // necessary friend declarations
    
    friend class ivec3;
  };


  
  /****************************************************************
   *								*
   *	       2D functions and 3D functions			*
   *								*
   ****************************************************************/
  
  mat3 identity2D();						// identity 2D
  mat3 translation2D(const vec2& v);				// translation 2D
  mat3 rotation2D(const vec2& Center, const float angleDeg);	// rotation 2D
  mat3 scaling2D(const vec2& scaleVector);		        // scaling 2D
  mat4 identity3D();						// identity 3D
  mat4 translation3D(const vec3& v);				// translation 3D
  mat4 rotation3D(vec3 Axis, const float angleDeg);               // rotation 3D
  mat4 scaling3D(const vec3& scaleVector);		        // scaling 3D
  mat4 perspective3D(const float d);			        // perspective 3D
  
  //
  //	Implementation
  //
  
  /****************************************************************
   *							        *
   *		    vec2 Member functions			*
   *								*
   ****************************************************************/
  
  // CONSTRUCTORS
  
  inline vec2::vec2() {}
  
  inline vec2::vec2(const float x, const float y)
  { n[VX] = x; n[VY] = y; }
  
  inline vec2::vec2(const float d)
  { n[VX] = n[VY] = d; }
  
  inline vec2::vec2(const vec2& v)
  { n[VX] = v.n[VX]; n[VY] = v.n[VY]; }
  
  inline vec2::vec2(const vec3& v) // it is up to caller to avoid divide-by-zero
  { n[VX] = v.n[VX]/v.n[VZ]; n[VY] = v.n[VY]/v.n[VZ]; };
  
  inline vec2::vec2(const vec3& v, int dropAxis) {
    switch (dropAxis) {
    case VX: n[VX] = v.n[VY]; n[VY] = v.n[VZ]; break;
    case VY: n[VX] = v.n[VX]; n[VY] = v.n[VZ]; break;
    default: n[VX] = v.n[VX]; n[VY] = v.n[VY]; break;
    }
  }
  
  
  // ASSIGNMENT OPERATORS
  
  inline vec2& vec2::operator = (const vec2& v)
    { n[VX] = v.n[VX]; n[VY] = v.n[VY]; return *this; }
  
  inline vec2& vec2::operator += ( const vec2& v )
  { n[VX] += v.n[VX]; n[VY] += v.n[VY]; return *this; }
  
  inline vec2& vec2::operator -= ( const vec2& v )
  { n[VX] -= v.n[VX]; n[VY] -= v.n[VY]; return *this; }

  inline vec2& vec2::operator *= ( const vec2& v )
  { n[VX] *= v.n[VX]; n[VY] *= v.n[VY]; return *this; }
  
  inline vec2& vec2::operator *= ( const float d )
  { n[VX] *= d; n[VY] *= d; return *this; }
  
  inline vec2& vec2::operator /= ( const float d )
  { float d_inv = 1./d; n[VX] *= d_inv; n[VY] *= d_inv; return *this; }

  inline vec2& vec2::operator /= ( const vec2& b ) // up to the user to prevent division by 0
  { n[VX] /= b[VX]; n[VY] /= b[VY]; return *this; }
  
  inline float& vec2::operator [] ( int i) {
    assert(!(i < VX || i > VY));		// subscript check
    return n[i];
  }
  
  inline float vec2::operator [] ( int i) const {
    assert(!(i < VX || i > VY));
    return n[i];
  }

  inline float* vec2::ptr(){ return n; }
  inline const float* vec2::ptr() const { return n; }
  
  
  // SPECIAL FUNCTIONS
  
  inline float vec2::length() const
  { return sqrt(length2()); }
  
  inline float vec2::length2() const
  { return n[VX]*n[VX] + n[VY]*n[VY]; }
  
  inline vec2& vec2::normalize() // it is up to caller to avoid divide-by-zero
  { *this /= length(); return *this; }
  
  inline vec2& vec2::apply(V_FCT_PTR fct)
  { n[VX] = (*fct)(n[VX]); n[VY] = (*fct)(n[VY]); return *this; }
  
  
  // FRIENDS
  
  inline vec2 operator - (const vec2& a)
  { return vec2(-a.n[VX],-a.n[VY]); }
  
  inline vec2 operator + (const vec2& a, const vec2& b)
  { return vec2(a.n[VX]+ b.n[VX], a.n[VY] + b.n[VY]); }
  
  inline vec2 operator - (const vec2& a, const vec2& b)
  { return vec2(a.n[VX]-b.n[VX], a.n[VY]-b.n[VY]); }
  
  inline vec2 operator * (const vec2& a, const float d)
    { return vec2(d*a.n[VX], d*a.n[VY]); }
  
  inline vec2 operator * (const float d, const vec2& a)
    { return a*d; }
  
  inline vec2 operator * (const mat3& a, const vec2& v) {
    vec3 av;
    
    av.n[VX] = a.v[0].n[VX]*v.n[VX] + a.v[0].n[VY]*v.n[VY] + a.v[0].n[VZ];
    av.n[VY] = a.v[1].n[VX]*v.n[VX] + a.v[1].n[VY]*v.n[VY] + a.v[1].n[VZ];
    av.n[VZ] = a.v[2].n[VX]*v.n[VX] + a.v[2].n[VY]*v.n[VY] + a.v[2].n[VZ];
    return av;
  }
  
  inline vec2 operator * (const vec2& v, const mat3& a) 
    { return a.transpose() * v; }
  
  inline vec2 operator * (const vec2& a, const vec2& b)
    { return vec2(a.n[VX]*b.n[VX], a.n[VY]*b.n[VY]); }
  
  inline vec2 operator / (const vec2& a, const float d)
  { float d_inv = 1./d; return vec2(a.n[VX]*d_inv, a.n[VY]*d_inv); }

  inline vec2 operator / (const vec2& a, const vec2& b) // up to the user to prevent division by 0
  { return vec2(a.n[VX] / b.n[VX], a.n[VY] / b.n[VY]); }
  
  inline vec3 operator ^ (const vec2& a, const vec2& b)
  { return vec3(0.0, 0.0, a.n[VX] * b.n[VY] - b.n[VX] * a.n[VY]); }
  
  inline int operator == (const vec2& a, const vec2& b)
  { return (a.n[VX] == b.n[VX]) && (a.n[VY] == b.n[VY]); }
  
  inline int operator != (const vec2& a, const vec2& b)
  { return !(a == b); }
  
#ifdef ALGEBRA3IOSTREAMS
  inline ostream& operator << (ostream& s, const vec2& v)
  { return s << "| " << v.n[VX] << ' ' << v.n[VY] << " |"; }
  inline istream& operator >> (istream& s, vec2& v) {
    vec2	v_tmp;
    char	c = ' ';
    
    while (isspace(c))
      s >> c;
    // The vectors can be formatted either as x y or | x y |
    if (c == '|') {
      s >> v_tmp[VX] >> v_tmp[VY];
      while (s >> c && isspace(c)) ;
      if (c != '|')
	s.set(_bad);
    }
    else {
      s.putback(c);
      s >> v_tmp[VX] >> v_tmp[VY];
    }
    if (s)
      v = v_tmp;
    return s;
  }
#endif // ALGEBRA3IOSTREAMS
  
  inline void swap(vec2& a, vec2& b)
  { vec2 tmp(a); a = b; b = tmp; }
  
  inline float min(const vec2&a)
  { return MIN(a.n[VX], a.n[VY]); }
  
  inline float max(const vec2&a)
  { return MAX(a.n[VX], a.n[VY]); }
  
  inline vec2 min(const vec2& a, const vec2& b)
  { return vec2(MIN(a.n[VX], b.n[VX]), MIN(a.n[VY], b.n[VY])); }
  
  inline vec2 max(const vec2& a, const vec2& b)
  { return vec2(MAX(a.n[VX], b.n[VX]), MAX(a.n[VY], b.n[VY])); }
  
  inline vec2 prod(const vec2& a, const vec2& b)
  { return vec2(a.n[VX] * b.n[VX], a.n[VY] * b.n[VY]); }
  
  inline float dot(const vec2& a, const vec2& b)
  { return (a.n[VX]*b.n[VX] + a.n[VY]*b.n[VY]); }
  
  /****************************************************************
   *																*
   *		    vec3 Member functions								*
   *																*
   ****************************************************************/
  
  // CONSTRUCTORS
  
  inline vec3::vec3() {}
  
  inline vec3::vec3(const float x, const float y, const float z)
  { n[VX] = x; n[VY] = y; n[VZ] = z; }
  
  inline vec3::vec3(const float d)
  { n[VX] = n[VY] = n[VZ] = d; }
  
  inline vec3::vec3(const vec3& v)
  { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VZ]; }
  
  inline vec3::vec3(const vec2& v)
  { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = 1.0; }
  
  inline vec3::vec3(const vec2& v, float d)
  { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = d; }
  
  inline vec3::vec3(const vec4& v) // it is up to caller to avoid divide-by-zero
  { n[VX] = v.n[VX] / v.n[VW]; n[VY] = v.n[VY] / v.n[VW];
    n[VZ] = v.n[VZ] / v.n[VW]; }
  
  inline vec3::vec3(const vec4& v, int dropAxis) {
    switch (dropAxis) {
    case VX: n[VX] = v.n[VY]; n[VY] = v.n[VZ]; n[VZ] = v.n[VW]; break;
    case VY: n[VX] = v.n[VX]; n[VY] = v.n[VZ]; n[VZ] = v.n[VW]; break;
    case VZ: n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VW]; break;
    default: n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VZ]; break;
    }
  }
  
  
  // ASSIGNMENT OPERATORS
  
  inline vec3& vec3::operator = (const vec3& v)
    { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VZ]; return *this; }
  
  inline vec3& vec3::operator += ( const vec3& v )
  { n[VX] += v.n[VX]; n[VY] += v.n[VY]; n[VZ] += v.n[VZ]; return *this; }
  
  inline vec3& vec3::operator -= ( const vec3& v )
  { n[VX] -= v.n[VX]; n[VY] -= v.n[VY]; n[VZ] -= v.n[VZ]; return *this; }
  
  inline vec3& vec3::operator *= ( const float d )
  { n[VX] *= d; n[VY] *= d; n[VZ] *= d; return *this; }

  inline vec3& vec3::operator *= ( const vec3& v )
  { n[VX] *= v.n[VX]; n[VY] *= v.n[VY]; n[VZ] *= v.n[VZ]; return *this; }
  
  inline vec3& vec3::operator /= ( const float d )
  { float d_inv = 1./d; n[VX] *= d_inv; n[VY] *= d_inv; n[VZ] *= d_inv;
    return *this; }

  inline vec3& vec3::operator /= ( const vec3& v ) // it is up to caller to avoid divide-by-zero
  { n[VX] /= v.n[VX]; n[VY] /= v.n[VY]; n[VZ] /= v.n[VZ];
    return *this; }
  
  inline float& vec3::operator [] ( int i) {
    assert(! (i < VX || i > VZ));
    return n[i];
  }
  
  inline float vec3::operator [] ( int i) const {
    assert(! (i < VX || i > VZ));
    return n[i];
  }
  
  
  inline float* vec3::ptr(){ return n; }
  inline const float* vec3::ptr() const { return n; }
  
  // SPECIAL FUNCTIONS
  
  inline float vec3::length() const
  {  return sqrt(length2()); }
  
  inline float vec3::length2() const
  {  return n[VX]*n[VX] + n[VY]*n[VY] + n[VZ]*n[VZ]; }
  
  inline vec3& vec3::normalize() // it is up to caller to avoid divide-by-zero
  { *this /= length(); return *this; }
  
  inline vec3& vec3::apply(V_FCT_PTR fct)
  { n[VX] = (*fct)(n[VX]); n[VY] = (*fct)(n[VY]); n[VZ] = (*fct)(n[VZ]);
    return *this; }
  
  
  // FRIENDS
  
  inline vec3 operator - (const vec3& a)
  {  return vec3(-a.n[VX],-a.n[VY],-a.n[VZ]); }
  
  inline vec3 operator + (const vec3& a, const vec3& b)
  { return vec3(a.n[VX]+ b.n[VX], a.n[VY] + b.n[VY], a.n[VZ] + b.n[VZ]); }
  
  inline vec3 operator - (const vec3& a, const vec3& b)
  { return vec3(a.n[VX]-b.n[VX], a.n[VY]-b.n[VY], a.n[VZ]-b.n[VZ]); }
  
  inline vec3 operator * (const vec3& a, const float d)
    { return vec3(d*a.n[VX], d*a.n[VY], d*a.n[VZ]); }
  
  inline vec3 operator * (const float d, const vec3& a)
    { return a*d; }
  
  inline vec3 operator * (const mat3& a, const vec3& v) {
#define ROWCOL(i) a.v[i].n[0]*v.n[VX] + a.v[i].n[1]*v.n[VY]	\
      + a.v[i].n[2]*v.n[VZ]
    return vec3(ROWCOL(0), ROWCOL(1), ROWCOL(2));
#undef ROWCOL // (i)
  }
  
  inline vec3 operator * (const mat4& a, const vec3& v)
    { return a * vec4(v); }
  
  inline vec3 operator * (const vec3& v, const mat4& a)
    { return a.transpose() * v; }
  
  inline vec3 operator * (const vec3& a, const vec3& b)
    { return vec3(a.n[VX]*b.n[VX], a.n[VY]*b.n[VY], a.n[VZ]*b.n[VZ]); }
  
  inline vec3 operator / (const vec3& a, const float d)
  { float d_inv = 1./d; return vec3(a.n[VX]*d_inv, a.n[VY]*d_inv,
				    a.n[VZ]*d_inv); }

  inline vec3 operator / (const vec3& a, const vec3& b) // it is up to the caller to prevent division by zero
  { return vec3(a.n[VX] / b.n[VX], a.n[VY] / b.n[VY],
		a.n[VZ] / b.n[VZ]); }
  
  inline vec3 operator ^ (const vec3& a, const vec3& b) {
    return vec3(a.n[VY]*b.n[VZ] - a.n[VZ]*b.n[VY],
		a.n[VZ]*b.n[VX] - a.n[VX]*b.n[VZ],
		a.n[VX]*b.n[VY] - a.n[VY]*b.n[VX]);
  }
  
  inline int operator == (const vec3& a, const vec3& b)
  { return (a.n[VX] == b.n[VX]) && (a.n[VY] == b.n[VY]) && (a.n[VZ] == b.n[VZ]);
  }
  
  inline int operator != (const vec3& a, const vec3& b)
  { return !(a == b); }
  
#ifdef ALGEBRA3IOSTREAMS
  inline ostream& operator << (ostream& s, const vec3& v)
  { return s << "| " << v.n[VX] << ' ' << v.n[VY] << ' ' << v.n[VZ] << " |"; }
  
  inline istream& operator >> (istream& s, vec3& v) {
    vec3	v_tmp;
    char	c = ' ';
    
    while (isspace(c))
      s >> c;
    // The vectors can be formatted either as x y z or | x y z |
    if (c == '|') {
      s >> v_tmp[VX] >> v_tmp[VY] >> v_tmp[VZ];
      while (s >> c && isspace(c)) ;
      if (c != '|')
	s.set(_bad);
    }
    else {
      s.putback(c);
      s >> v_tmp[VX] >> v_tmp[VY] >> v_tmp[VZ];
    }
    if (s)
      v = v_tmp;
    return s;
  }
#endif // ALGEBRA3IOSTREAMS
  
  inline void swap(vec3& a, vec3& b)
  { vec3 tmp(a); a = b; b = tmp; }
  
  inline vec3 min(const vec3& a, const vec3& b)
  { return vec3(MIN(a.n[VX], b.n[VX]), MIN(a.n[VY], b.n[VY]), MIN(a.n[VZ],
								  b.n[VZ])); }
  
  inline vec3 max(const vec3& a, const vec3& b)
  { return vec3(MAX(a.n[VX], b.n[VX]), MAX(a.n[VY], b.n[VY]), MAX(a.n[VZ],
								  b.n[VZ])); }
  
  inline vec3 prod(const vec3& a, const vec3& b)
  { return vec3(a.n[VX] * b.n[VX], a.n[VY] * b.n[VY], a.n[VZ] * b.n[VZ]); }
  
  inline float dot(const vec3& a, const vec3& b)
  { return (a.n[VX]*b.n[VX] + a.n[VY]*b.n[VY] + a.n[VZ]*b.n[VZ]); }
  
  inline float angleBetween(const vec3& a, const vec3& b)
  {
    vec3 cross = a ^ b;
    const float l = cross.length();
    return atan2f(l, dot(a, b));
  }
  
  
  /****************************************************************
   *																*
   *		    vec4 Member functions								*
   *																*
   ****************************************************************/
  
  // CONSTRUCTORS
  
  inline vec4::vec4() {}
  
  inline vec4::vec4(const float x, const float y, const float z, const float w)
  { n[VX] = x; n[VY] = y; n[VZ] = z; n[VW] = w; }
  
  inline vec4::vec4(const float d)
  {  n[VX] = n[VY] = n[VZ] = n[VW] = d; }
  
  inline vec4::vec4(const vec4& v)
  { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VZ]; n[VW] = v.n[VW]; }
  
  inline vec4::vec4(const vec3& v)
  { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VZ]; n[VW] = 1.0; }
  
  inline vec4::vec4(const vec3& v, const float d)
  { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VZ];  n[VW] = d; }
  
  
  // ASSIGNMENT OPERATORS
  
  inline vec4& vec4::operator = (const vec4& v)
    { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VZ]; n[VW] = v.n[VW];
      return *this; }
  
  inline vec4& vec4::operator += ( const vec4& v )
  { n[VX] += v.n[VX]; n[VY] += v.n[VY]; n[VZ] += v.n[VZ]; n[VW] += v.n[VW];
    return *this; }
  
  inline vec4& vec4::operator -= ( const vec4& v )
  { n[VX] -= v.n[VX]; n[VY] -= v.n[VY]; n[VZ] -= v.n[VZ]; n[VW] -= v.n[VW];
    return *this; }
  
  inline vec4& vec4::operator *= ( const float d )
  { n[VX] *= d; n[VY] *= d; n[VZ] *= d; n[VW] *= d; return *this; }

  inline vec4& vec4::operator *= ( const vec4& v )
  { n[VX] *= v.n[VX]; n[VY] *= v.n[VY]; n[VZ] *= v.n[VZ]; n[VW] *= v.n[VW]; return *this; }
  
  inline vec4& vec4::operator /= ( const float d )
  { float d_inv = 1./d; n[VX] *= d_inv; n[VY] *= d_inv; n[VZ] *= d_inv;
    n[VW] *= d_inv; return *this; }

  inline vec4& vec4::operator /= ( const vec4& v ) // it is up to the caller to prevent division by zero
  { n[VX] /= v.n[VX]; n[VY] /= v.n[VY]; n[VZ] /= v.n[VZ];
    n[VW] /= v.n[VW]; return *this; }
  
  inline float& vec4::operator [] ( int i) {
    assert(! (i < VX || i > VW));
    return n[i];
  }
  
  inline float vec4::operator [] ( int i) const {
    assert(! (i < VX || i > VW));
    return n[i];
  }
  
  inline float* vec4::ptr(){ return n; }
  inline const float* vec4::ptr() const { return n; }
  
  // SPECIAL FUNCTIONS
  
  inline float vec4::length() const
  { return sqrt(length2()); }
  
  inline float vec4::length2() const
  { return n[VX]*n[VX] + n[VY]*n[VY] + n[VZ]*n[VZ] + n[VW]*n[VW]; }
  
  inline vec4& vec4::normalize() // it is up to caller to avoid divide-by-zero
  { *this /= length(); return *this; }
  
  inline vec4& vec4::apply(V_FCT_PTR fct)
  { n[VX] = (*fct)(n[VX]); n[VY] = (*fct)(n[VY]); n[VZ] = (*fct)(n[VZ]);
    n[VW] = (*fct)(n[VW]); return *this; }
  
  
  // FRIENDS
  
  inline vec4 operator - (const vec4& a)
  { return vec4(-a.n[VX],-a.n[VY],-a.n[VZ],-a.n[VW]); }
  
  inline vec4 operator + (const vec4& a, const vec4& b)
  { return vec4(a.n[VX] + b.n[VX], a.n[VY] + b.n[VY], a.n[VZ] + b.n[VZ],
		a.n[VW] + b.n[VW]); }
  
  inline vec4 operator - (const vec4& a, const vec4& b)
  {  return vec4(a.n[VX] - b.n[VX], a.n[VY] - b.n[VY], a.n[VZ] - b.n[VZ],
		 a.n[VW] - b.n[VW]); }
  
  inline vec4 operator * (const vec4& a, const float d)
    { return vec4(d*a.n[VX], d*a.n[VY], d*a.n[VZ], d*a.n[VW] ); }
  
  inline vec4 operator * (const float d, const vec4& a)
    { return a*d; }
  
  inline vec4 operator * (const mat4& a, const vec4& v) {
#define ROWCOL(i) a.v[i].n[0]*v.n[VX] + a.v[i].n[1]*v.n[VY]	\
      + a.v[i].n[2]*v.n[VZ] + a.v[i].n[3]*v.n[VW]
    return vec4(ROWCOL(0), ROWCOL(1), ROWCOL(2), ROWCOL(3));
#undef ROWCOL // (i)
  }
  
  inline vec4 operator * (const vec4& v, const mat4& a)
    { return a.transpose() * v; }
  
  inline vec4 operator * (const vec4& a, const vec4& b)
    { return vec4(a.n[VX]*b.n[VX], a.n[VY]*b.n[VY], a.n[VZ]*b.n[VZ],
		  a.n[VW]*b.n[VW]); }
  
  inline vec4 operator / (const vec4& a, const float d)
  { float d_inv = 1./d; return vec4(a.n[VX]*d_inv, a.n[VY]*d_inv, a.n[VZ]*d_inv,
				    a.n[VW]*d_inv); }

  inline vec4 operator / (const vec4& a, const vec4& b)
  { return vec4(a.n[VX] / b.n[VX], a.n[VY] / b.n[VY], a.n[VZ] / b.n[VZ],
		a.n[VW] / b.n[VW]); }
  
  inline int operator == (const vec4& a, const vec4& b)
  { return (a.n[VX] == b.n[VX]) && (a.n[VY] == b.n[VY]) && (a.n[VZ] == b.n[VZ])
    && (a.n[VW] == b.n[VW]); }
  
  inline int operator != (const vec4& a, const vec4& b)
  { return !(a == b); }
  
#ifdef ALGEBRA3IOSTREAMS
  inline ostream& operator << (ostream& s, const vec4& v)
  { return s << "| " << v.n[VX] << ' ' << v.n[VY] << ' ' << v.n[VZ] << ' '
	     << v.n[VW] << " |"; }
  
  inline stream& operator >> (istream& s, vec4& v) {
    vec4	v_tmp;
    char	c = ' ';
    
    while (isspace(c))
      s >> c;
    // The vectors can be formatted either as x y z w or | x y z w |
    if (c == '|') {
      s >> v_tmp[VX] >> v_tmp[VY] >> v_tmp[VZ] >> v_tmp[VW];
      while (s >> c && isspace(c)) ;
      if (c != '|')
	s.set(_bad);
    }
    else {
      s.putback(c);
      s >> v_tmp[VX] >> v_tmp[VY] >> v_tmp[VZ] >> v_tmp[VW];
    }
    if (s)
      v = v_tmp;
    return s;
  }
#endif // ALGEBRA3IOSTREAMS
  
  inline void swap(vec4& a, vec4& b)
  { vec4 tmp(a); a = b; b = tmp; }
  
  inline vec4 min(const vec4& a, const vec4& b)
  { return vec4(MIN(a.n[VX], b.n[VX]), MIN(a.n[VY], b.n[VY]), MIN(a.n[VZ],
								  b.n[VZ]), MIN(a.n[VW], b.n[VW])); }
  
  inline vec4 max(const vec4& a, const vec4& b)
  { return vec4(MAX(a.n[VX], b.n[VX]), MAX(a.n[VY], b.n[VY]), MAX(a.n[VZ],
								  b.n[VZ]), MAX(a.n[VW], b.n[VW])); }
  
  inline vec4 prod(const vec4& a, const vec4& b)
  { return vec4(a.n[VX] * b.n[VX], a.n[VY] * b.n[VY], a.n[VZ] * b.n[VZ],
		a.n[VW] * b.n[VW]); }
  
  inline float dot(const vec4& a, const vec4& b)
  { return (a.n[VX]*b.n[VX] + a.n[VY]*b.n[VY] + a.n[VZ]*b.n[VZ] +
	    a.n[VW]*b.n[VW]); }
  
  /****************************************************************
   *																*
   *		    mat3 member functions								*
   *																*
   ****************************************************************/
  
  // CONSTRUCTORS
  
  inline mat3::mat3() {}
  
  inline mat3::mat3(const vec3& v0, const vec3& v1, const vec3& v2)
  { v[0] = v0; v[1] = v1; v[2] = v2; }
  
  inline mat3::mat3(const float d)
  { v[0] = v[1] = v[2] = vec3(d); }
  
  inline mat3::mat3(const mat3& m)
  { v[0] = m.v[0]; v[1] = m.v[1]; v[2] = m.v[2]; }
  
  
  // ASSIGNMENT OPERATORS
  
  inline mat3& mat3::operator = ( const mat3& m )
    { v[0] = m.v[0]; v[1] = m.v[1]; v[2] = m.v[2]; return *this; }
  
  inline mat3& mat3::operator += ( const mat3& m )
  { v[0] += m.v[0]; v[1] += m.v[1]; v[2] += m.v[2]; return *this; }
  
  inline mat3& mat3::operator -= ( const mat3& m )
  { v[0] -= m.v[0]; v[1] -= m.v[1]; v[2] -= m.v[2]; return *this; }
  
  inline mat3& mat3::operator *= ( const float d )
  { v[0] *= d; v[1] *= d; v[2] *= d; return *this; }
  
  inline mat3& mat3::operator /= ( const float d )
  { v[0] /= d; v[1] /= d; v[2] /= d; return *this; }
  
  inline vec3& mat3::operator [] ( int i) {
    assert(! (i < VX || i > VZ));
    return v[i];
  }
  
  inline const vec3& mat3::operator [] ( int i) const {
    assert(!(i < VX || i > VZ));
    return v[i];
  }
  
  // SPECIAL FUNCTIONS
  
  inline mat3 mat3::transpose() const {
    return mat3(vec3(v[0][0], v[1][0], v[2][0]),
		vec3(v[0][1], v[1][1], v[2][1]),
		vec3(v[0][2], v[1][2], v[2][2]));
  }
  
  inline mat3 mat3::inverse()	const    // Gauss-Jordan elimination with partial pivoting
    {
      mat3 a(*this),	    // As a evolves from original mat into identity
	b(identity2D());   // b evolves from identity into inverse(a)
      int	 i, j, i1;
      
      // Loop over cols of a from left to right, eliminating above and below diag
      for (j=0; j<3; j++) {   // Find largest pivot in column j among rows j..2
	i1 = j;		    // Row with largest pivot candidate
	for (i=j+1; i<3; i++)
	  if (fabs(a.v[i].n[j]) > fabs(a.v[i1].n[j]))
	    i1 = i;
	
	// Swap rows i1 and j in a and b to put pivot on diagonal
	swap(a.v[i1], a.v[j]);
	swap(b.v[i1], b.v[j]);
	
	// Scale row j to have a unit diagonal
	if (a.v[j].n[j]==0.)
	  ALGEBRA_ERROR("mat3::inverse: singular matrix; can't invert\n")
	    b.v[j] /= a.v[j].n[j];
	a.v[j] /= a.v[j].n[j];
	
	// Eliminate off-diagonal elems in col j of a, doing identical ops to b
	for (i=0; i<3; i++)
	  if (i!=j) {
	    b.v[i] -= a.v[i].n[j]*b.v[j];
	    a.v[i] -= a.v[i].n[j]*a.v[j];
	  }
      }
      return b;
    }
  
  inline mat3& mat3::apply(V_FCT_PTR fct) {
    v[VX].apply(fct);
    v[VY].apply(fct);
    v[VZ].apply(fct);
    return *this;
  }
  
  
  // FRIENDS
  
  inline mat3 operator - (const mat3& a)
  { return mat3(-a.v[0], -a.v[1], -a.v[2]); }
  
  inline mat3 operator + (const mat3& a, const mat3& b)
  { return mat3(a.v[0] + b.v[0], a.v[1] + b.v[1], a.v[2] + b.v[2]); }
  
  inline mat3 operator - (const mat3& a, const mat3& b)
  { return mat3(a.v[0] - b.v[0], a.v[1] - b.v[1], a.v[2] - b.v[2]); }
  
  inline mat3 operator * (const mat3& a, const mat3& b) {
#define ROWCOL(i, j)							\
    a.v[i].n[0]*b.v[0][j] + a.v[i].n[1]*b.v[1][j] + a.v[i].n[2]*b.v[2][j]
    return mat3(vec3(ROWCOL(0,0), ROWCOL(0,1), ROWCOL(0,2)),
		vec3(ROWCOL(1,0), ROWCOL(1,1), ROWCOL(1,2)),
		vec3(ROWCOL(2,0), ROWCOL(2,1), ROWCOL(2,2)));
#undef ROWCOL // (i, j)
  }
  
  inline mat3 operator * (const mat3& a, const float d)
    { return mat3(a.v[0] * d, a.v[1] * d, a.v[2] * d); }
  
  inline mat3 operator * (const float d, const mat3& a)
    { return a*d; }
  
  inline mat3 operator / (const mat3& a, const float d)
  { return mat3(a.v[0] / d, a.v[1] / d, a.v[2] / d); }
  
  inline int operator == (const mat3& a, const mat3& b)
  { return (a.v[0] == b.v[0]) && (a.v[1] == b.v[1]) && (a.v[2] == b.v[2]); }
  
  inline int operator != (const mat3& a, const mat3& b)
  { return !(a == b); }
  
#ifdef ALGEBRA3IOSTREAMS
  inline ostream& operator << (ostream& s, const mat3& m)
  { return s << m.v[VX] << '\n' << m.v[VY] << '\n' << m.v[VZ]; }
  
  inline stream& operator >> (istream& s, mat3& m) {
    mat3    m_tmp;
    
    s >> m_tmp[VX] >> m_tmp[VY] >> m_tmp[VZ];
    if (s)
      m = m_tmp;
    return s;
  }
#endif // ALGEBRA3IOSTREAMS
  
  inline void swap(mat3& a, mat3& b)
  { mat3 tmp(a); a = b; b = tmp; }
  
  
  /****************************************************************
   *																*
   *		    mat4 member functions								*
   *																*
   ****************************************************************/
  
  // CONSTRUCTORS
  
  inline mat4::mat4() {}
  
  inline mat4::mat4(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3)
  { v[0] = v0; v[1] = v1; v[2] = v2; v[3] = v3; }
  
  inline mat4::mat4(const float d)
  { v[0] = v[1] = v[2] = v[3] = vec4(d); }
  
  inline mat4::mat4(const float* d)
  {
    for(int i=0;i<4;i++)
      {
	for(int j=0;j<4;j++)
	  {
	    v[i][j]=d[i*4+j];
	  }	
      }
  }
  
  inline void mat4::getValues(float* d)
  {
    for(int i=0;i<4;i++)
      {
	for(int j=0;j<4;j++)
	  {
	    d[i*4+j]=v[i][j];
	  }	
      }
  }
  
  inline float* mat4::ptr()
  {
    return v[0].ptr();
  }
  
  inline const float* mat4::ptr() const
  {
    return v[0].ptr();
  }
  
  inline mat4::mat4(const mat4& m)
  { v[0] = m.v[0]; v[1] = m.v[1]; v[2] = m.v[2]; v[3] = m.v[3]; }
  
  // ASSIGNMENT OPERATORS
  
  inline mat4& mat4::operator = ( const mat4& m )
    { v[0] = m.v[0]; v[1] = m.v[1]; v[2] = m.v[2]; v[3] = m.v[3];
      return *this; }
  
  inline mat4& mat4::operator += ( const mat4& m )
  { v[0] += m.v[0]; v[1] += m.v[1]; v[2] += m.v[2]; v[3] += m.v[3];
    return *this; }
  
  inline mat4& mat4::operator -= ( const mat4& m )
  { v[0] -= m.v[0]; v[1] -= m.v[1]; v[2] -= m.v[2]; v[3] -= m.v[3];
    return *this; }
  
  inline mat4& mat4::operator *= ( const float d )
  { v[0] *= d; v[1] *= d; v[2] *= d; v[3] *= d; return *this; }
  
  inline mat4& mat4::operator /= ( const float d )
  { v[0] /= d; v[1] /= d; v[2] /= d; v[3] /= d; return *this; }
  
  inline vec4& mat4::operator [] ( int i) {
    assert(! (i < VX || i > VW));
    return v[i];
  }
  
  inline const vec4& mat4::operator [] ( int i) const {
    assert(! (i < VX || i > VW));
    return v[i];
  }
  
  // SPECIAL FUNCTIONS;
  
  inline mat4 mat4::transpose() const{
    return mat4(vec4(v[0][0], v[1][0], v[2][0], v[3][0]),
		vec4(v[0][1], v[1][1], v[2][1], v[3][1]),
		vec4(v[0][2], v[1][2], v[2][2], v[3][2]),
		vec4(v[0][3], v[1][3], v[2][3], v[3][3]));
  }
  
  inline mat4 mat4::inverse()	const    // Gauss-Jordan elimination with partial pivoting
    {
      mat4 a(*this),	    // As a evolves from original mat into identity
	b(identity3D());   // b evolves from identity into inverse(a)
      int i, j, i1;
      
      // Loop over cols of a from left to right, eliminating above and below diag
      for (j=0; j<4; j++) {   // Find largest pivot in column j among rows j..3
	i1 = j;		    // Row with largest pivot candidate
	for (i=j+1; i<4; i++)
	  if (fabs(a.v[i].n[j]) > fabs(a.v[i1].n[j]))
	    i1 = i;
	
	// Swap rows i1 and j in a and b to put pivot on diagonal
	swap(a.v[i1], a.v[j]);
	swap(b.v[i1], b.v[j]);
	
	// Scale row j to have a unit diagonal
	if (a.v[j].n[j]==0.)
	  ALGEBRA_ERROR("mat4::inverse: singular matrix; can't invert\n");
	b.v[j] /= a.v[j].n[j];
	a.v[j] /= a.v[j].n[j];
	
	// Eliminate off-diagonal elems in col j of a, doing identical ops to b
	for (i=0; i<4; i++)
	  if (i!=j) {
	    b.v[i] -= a.v[i].n[j]*b.v[j];
	    a.v[i] -= a.v[i].n[j]*a.v[j];
	  }
      }
      return b;
    }
  
  inline mat4& mat4::apply(V_FCT_PTR fct)
  { v[VX].apply(fct); v[VY].apply(fct); v[VZ].apply(fct); v[VW].apply(fct);
    return *this; }
  
  
  // FRIENDS
  
  inline mat4 operator - (const mat4& a)
  { return mat4(-a.v[0], -a.v[1], -a.v[2], -a.v[3]); }
  
  inline mat4 operator + (const mat4& a, const mat4& b)
  { return mat4(a.v[0] + b.v[0], a.v[1] + b.v[1], a.v[2] + b.v[2],
		a.v[3] + b.v[3]);
  }
  
  inline mat4 operator - (const mat4& a, const mat4& b)
  { return mat4(a.v[0] - b.v[0], a.v[1] - b.v[1], a.v[2] - b.v[2], a.v[3] - b.v[3]); }
  
  inline mat4 operator * (const mat4& a, const mat4& b) {
#define ROWCOL(i, j) a.v[i].n[0]*b.v[0][j] + a.v[i].n[1]*b.v[1][j] +	\
      a.v[i].n[2]*b.v[2][j] + a.v[i].n[3]*b.v[3][j]
    return mat4(
		vec4(ROWCOL(0,0), ROWCOL(0,1), ROWCOL(0,2), ROWCOL(0,3)),
		vec4(ROWCOL(1,0), ROWCOL(1,1), ROWCOL(1,2), ROWCOL(1,3)),
		vec4(ROWCOL(2,0), ROWCOL(2,1), ROWCOL(2,2), ROWCOL(2,3)),
		vec4(ROWCOL(3,0), ROWCOL(3,1), ROWCOL(3,2), ROWCOL(3,3))
		);
#undef ROWCOL
  }
  
  inline mat4 operator * (const mat4& a, const float d)
    { return mat4(a.v[0] * d, a.v[1] * d, a.v[2] * d, a.v[3] * d); }
  
  inline mat4 operator * (const float d, const mat4& a)
    { return a*d; }
  
  inline mat4 operator / (const mat4& a, const float d)
  { return mat4(a.v[0] / d, a.v[1] / d, a.v[2] / d, a.v[3] / d); }
  
  inline int operator == (const mat4& a, const mat4& b)
  { return ((a.v[0] == b.v[0]) && (a.v[1] == b.v[1]) && (a.v[2] == b.v[2]) &&
	    (a.v[3] == b.v[3])); }
  
  inline int operator != (const mat4& a, const mat4& b)
  { return !(a == b); }
  
#ifdef ALGEBRA3IOSTREAMS
  inline ostream& operator << (ostream& s, const mat4& m)
  { return s << m.v[VX] << '\n' << m.v[VY] << '\n' << m.v[VZ] << '\n' << m.v[VW]; }
  
  inline istream& operator >> (istream& s, mat4& m)
  {
    mat4    m_tmp;
    
    s >> m_tmp[VX] >> m_tmp[VY] >> m_tmp[VZ] >> m_tmp[VW];
    if (s)
      m = m_tmp;
    return s;
  }
#endif // ALGEBRA3IOSTREAMS
  
  inline void swap(mat4& a, mat4& b)
  { mat4 tmp(a); a = b; b = tmp; }
  
  
  /****************************************************************
   *																*
   *	       2D functions and 3D functions						*
   *																*
   ****************************************************************/
  
  inline mat3 identity2D()
  {   return mat3(vec3(1.0, 0.0, 0.0),
		  vec3(0.0, 1.0, 0.0),
		  vec3(0.0, 0.0, 1.0)); }
  
  inline mat3 translation2D(const vec2& v)
  {   return mat3(vec3(1.0, 0.0, v[VX]),
		  vec3(0.0, 1.0, v[VY]),
		  vec3(0.0, 0.0, 1.0)); }
  
  inline mat3 rotation2D(const vec2& Center, const float angleDeg) {
    float  angleRad = angleDeg * M_PI / 180.0,
      c = cos(angleRad),
      s = sin(angleRad);
    
    return mat3(vec3(c, -s, Center[VX] * (1.0-c) + Center[VY] * s),
		vec3(s, c, Center[VY] * (1.0-c) - Center[VX] * s),
		vec3(0.0, 0.0, 1.0));
  }
  
  inline mat3 scaling2D(const vec2& scaleVector)
  {   return mat3(vec3(scaleVector[VX], 0.0, 0.0),
		  vec3(0.0, scaleVector[VY], 0.0),
		  vec3(0.0, 0.0, 1.0)); }
  
  inline mat4 identity3D()
  {   return mat4(vec4(1.0, 0.0, 0.0, 0.0),
		  vec4(0.0, 1.0, 0.0, 0.0),
		  vec4(0.0, 0.0, 1.0, 0.0),
		  vec4(0.0, 0.0, 0.0, 1.0)); }
  
  inline mat4 translation3D(const vec3& v)
  {   return mat4(vec4(1.0, 0.0, 0.0, v[VX]),
		  vec4(0.0, 1.0, 0.0, v[VY]),
		  vec4(0.0, 0.0, 1.0, v[VZ]),
		  vec4(0.0, 0.0, 0.0, 1.0)); }
  
  inline mat4 rotation3D(vec3 Axis, const float angleDeg) {
    float  angleRad = angleDeg * M_PI / 180.0,
      c = cos(angleRad),
      s = sin(angleRad),
      t = 1.0 - c;
    
    Axis.normalize();
    return mat4(vec4(t * Axis[VX] * Axis[VX] + c,
		     t * Axis[VX] * Axis[VY] - s * Axis[VZ],
		     t * Axis[VX] * Axis[VZ] + s * Axis[VY],
		     0.0),
		vec4(t * Axis[VX] * Axis[VY] + s * Axis[VZ],
		     t * Axis[VY] * Axis[VY] + c,
		     t * Axis[VY] * Axis[VZ] - s * Axis[VX],
		     0.0),
		vec4(t * Axis[VX] * Axis[VZ] - s * Axis[VY],
		     t * Axis[VY] * Axis[VZ] + s * Axis[VX],
		     t * Axis[VZ] * Axis[VZ] + c,
		     0.0),
		vec4(0.0, 0.0, 0.0, 1.0));
  }
  
  inline mat4 scaling3D(const vec3& scaleVector)
  {   return mat4(vec4(scaleVector[VX], 0.0, 0.0, 0.0),
		  vec4(0.0, scaleVector[VY], 0.0, 0.0),
		  vec4(0.0, 0.0, scaleVector[VZ], 0.0),
		  vec4(0.0, 0.0, 0.0, 1.0)); }
  
  inline mat4 perspective3D(const float d)
  {   return mat4(vec4(1.0, 0.0, 0.0, 0.0),
		  vec4(0.0, 1.0, 0.0, 0.0),
		  vec4(0.0, 0.0, 1.0, 0.0),
		  vec4(0.0, 0.0, 1.0/d, 0.0)); }
  



/****************************************************************
   *							        *
   *		    ivec2 Member functions			*
   *								*
   ****************************************************************/
  
  // CONSTRUCTORS
  
  inline ivec2::ivec2() {}
  
  inline ivec2::ivec2(const int x, const int y)
  { n[VX] = x; n[VY] = y; }
  
  inline ivec2::ivec2(const int d)
  { n[VX] = n[VY] = d; }
  
  inline ivec2::ivec2(const ivec2& v)
  { n[VX] = v.n[VX]; n[VY] = v.n[VY]; }
  
  inline ivec2::ivec2(const ivec3& v) // it is up to caller to avoid divide-by-zero
  { n[VX] = v.n[VX]/v.n[VZ]; n[VY] = v.n[VY]/v.n[VZ]; };
  
  inline ivec2::ivec2(const ivec3& v, int dropAxis) {
    switch (dropAxis) {
    case VX: n[VX] = v.n[VY]; n[VY] = v.n[VZ]; break;
    case VY: n[VX] = v.n[VX]; n[VY] = v.n[VZ]; break;
    default: n[VX] = v.n[VX]; n[VY] = v.n[VY]; break;
    }
  }
  
  
  // ASSIGNMENT OPERATORS
  
  inline ivec2& ivec2::operator = (const ivec2& v)
    { n[VX] = v.n[VX]; n[VY] = v.n[VY]; return *this; }
  
  inline ivec2& ivec2::operator += ( const ivec2& v )
  { n[VX] += v.n[VX]; n[VY] += v.n[VY]; return *this; }
  
  inline ivec2& ivec2::operator -= ( const ivec2& v )
  { n[VX] -= v.n[VX]; n[VY] -= v.n[VY]; return *this; }

  inline ivec2& ivec2::operator *= ( const ivec2& v )
  { n[VX] *= v.n[VX]; n[VY] *= v.n[VY]; return *this; }
  
  inline ivec2& ivec2::operator *= ( const int d )
  { n[VX] *= d; n[VY] *= d; return *this; }
  
  inline ivec2& ivec2::operator /= ( const int d )
  { n[VX] /= d; n[VY] /= d; return *this; }

  inline ivec2& ivec2::operator /= ( const ivec2& b ) // up to the user to prevent division by 0
  { n[VX] /= b[VX]; n[VY] /= b[VY]; return *this; }
  
  inline int& ivec2::operator [] ( int i) {
    assert(!(i < VX || i > VY));		// subscript check
    return n[i];
  }
  
  inline int ivec2::operator [] ( int i) const {
    assert(!(i < VX || i > VY));
    return n[i];
  }


  inline int* ivec2::ptr(){ return n; }
  inline const int* ivec2::ptr() const { return n; }
  
  
  // SPECIAL FUNCTIONS
  
  inline float ivec2::length() const
  { return sqrt(length2()); }
  
  inline float ivec2::length2() const
  { return static_cast<float>(n[VX]*n[VX] + n[VY]*n[VY]); }
  
  inline ivec2& ivec2::apply(VI_FCT_PTR fct)
  { n[VX] = (*fct)(n[VX]); n[VY] = (*fct)(n[VY]); return *this; }
  
  
  // FRIENDS
  
  inline ivec2 operator - (const ivec2& a)
  { return ivec2(-a.n[VX],-a.n[VY]); }
  
  inline ivec2 operator + (const ivec2& a, const ivec2& b)
  { return ivec2(a.n[VX]+ b.n[VX], a.n[VY] + b.n[VY]); }
  
  inline ivec2 operator - (const ivec2& a, const ivec2& b)
  { return ivec2(a.n[VX]-b.n[VX], a.n[VY]-b.n[VY]); }
  
  inline ivec2 operator * (const ivec2& a, const int d)
    { return ivec2(d*a.n[VX], d*a.n[VY]); }
  
  inline ivec2 operator * (const int d, const ivec2& a)
    { return a*d; }
  
  inline ivec2 operator * (const ivec2& a, const ivec2& b)
    { return ivec2(a.n[VX]*b.n[VX], a.n[VY]*b.n[VY]); }
  
  inline ivec2 operator / (const ivec2& a, const int d)
  { return ivec2(a.n[VX]/d, a.n[VY]/d); }

inline ivec2 operator / (const ivec2& a, const ivec2& b) // up to the user to prevent division by 0
  { return ivec2(a.n[VX] / b.n[VX], a.n[VY] / b.n[VY]); }
  
  inline int operator == (const ivec2& a, const ivec2& b)
  { return (a.n[VX] == b.n[VX]) && (a.n[VY] == b.n[VY]); }
  
  inline int operator != (const ivec2& a, const ivec2& b)
  { return !(a == b); }
  
#ifdef ALGEBRA3IOSTREAMS
  inline ostream& operator << (ostream& s, const ivec2& v)
  { return s << "| " << v.n[VX] << ' ' << v.n[VY] << " |"; }
  inline istream& operator >> (istream& s, ivec2& v) {
    vec2	v_tmp;
    char	c = ' ';
    
    while (isspace(c))
      s >> c;
    // The vectors can be formatted either as x y or | x y |
    if (c == '|') {
      s >> v_tmp[VX] >> v_tmp[VY];
      while (s >> c && isspace(c)) ;
      if (c != '|')
	s.set(_bad);
    }
    else {
      s.putback(c);
      s >> v_tmp[VX] >> v_tmp[VY];
    }
    if (s)
      v = v_tmp;
    return s;
  }
#endif // ALGEBRA3IOSTREAMS
  
  inline void swap(ivec2& a, ivec2& b)
  { ivec2 tmp(a); a = b; b = tmp; }
  
  inline int min(const ivec2&a)
  { return MIN(a.n[VX], a.n[VY]); }
  
  inline int max(const ivec2&a)
  { return MAX(a.n[VX], a.n[VY]); }
  
  inline ivec2 min(const ivec2& a, const ivec2& b)
  { return ivec2(MIN(a.n[VX], b.n[VX]), MIN(a.n[VY], b.n[VY])); }
  
  inline ivec2 max(const ivec2& a, const ivec2& b)
  { return ivec2(MAX(a.n[VX], b.n[VX]), MAX(a.n[VY], b.n[VY])); }
  
  inline ivec2 prod(const ivec2& a, const ivec2& b)
  { return ivec2(a.n[VX] * b.n[VX], a.n[VY] * b.n[VY]); }
  
  inline int dot(const ivec2& a, const ivec2& b)
  { return (a.n[VX]*b.n[VX] + a.n[VY]*b.n[VY]); }



/****************************************************************
   *																*
   *		    ivec3 Member functions								*
   *																*
   ****************************************************************/
  
  // CONSTRUCTORS
  
  inline ivec3::ivec3() {}
  
  inline ivec3::ivec3(const int x, const int y, const int z)
  { n[VX] = x; n[VY] = y; n[VZ] = z; }
  
  inline ivec3::ivec3(const int d)
  { n[VX] = n[VY] = n[VZ] = d; }
  
  inline ivec3::ivec3(const ivec3& v)
  { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VZ]; }
  
  inline ivec3::ivec3(const ivec2& v)
  { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = 1; }
  
  inline ivec3::ivec3(const ivec2& v, int d)
  { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = d; }
  
  inline ivec3::ivec3(const ivec4& v) // it is up to caller to avoid divide-by-zero
  { n[VX] = v.n[VX] / v.n[VW]; n[VY] = v.n[VY] / v.n[VW];
    n[VZ] = v.n[VZ] / v.n[VW]; }
  
  inline ivec3::ivec3(const ivec4& v, int dropAxis) {
    switch (dropAxis) {
    case VX: n[VX] = v.n[VY]; n[VY] = v.n[VZ]; n[VZ] = v.n[VW]; break;
    case VY: n[VX] = v.n[VX]; n[VY] = v.n[VZ]; n[VZ] = v.n[VW]; break;
    case VZ: n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VW]; break;
    default: n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VZ]; break;
    }
  }
  
  
  // ASSIGNMENT OPERATORS
  
  inline ivec3& ivec3::operator = (const ivec3& v)
    { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VZ]; return *this; }
  
  inline ivec3& ivec3::operator += ( const ivec3& v )
  { n[VX] += v.n[VX]; n[VY] += v.n[VY]; n[VZ] += v.n[VZ]; return *this; }
  
  inline ivec3& ivec3::operator -= ( const ivec3& v )
  { n[VX] -= v.n[VX]; n[VY] -= v.n[VY]; n[VZ] -= v.n[VZ]; return *this; }
  
  inline ivec3& ivec3::operator *= ( const int d )
  { n[VX] *= d; n[VY] *= d; n[VZ] *= d; return *this; }

  inline ivec3& ivec3::operator *= ( const ivec3& v )
  { n[VX] *= v.n[VX]; n[VY] *= v.n[VY]; n[VZ] *= v.n[VZ]; return *this; }
  
  inline ivec3& ivec3::operator /= ( const int d )
  { n[VX] /= d; n[VY] /= d; n[VZ] /= d;
    return *this; }

  inline ivec3& ivec3::operator /= ( const ivec3& v ) // it is up to caller to avoid divide-by-zero
  { n[VX] /= v.n[VX]; n[VY] /= v.n[VY]; n[VZ] /= v.n[VZ];
    return *this; }
  
  inline int& ivec3::operator [] ( int i) {
    assert(! (i < VX || i > VZ));
    return n[i];
  }
  
  inline int ivec3::operator [] ( int i) const {
    assert(! (i < VX || i > VZ));
    return n[i];
  }
  
  
  inline int* ivec3::ptr(){ return n; }
  inline const int* ivec3::ptr() const { return n; }
  
  // SPECIAL FUNCTIONS
  
  inline float ivec3::length() const
  {  return sqrt(length2()); }
  
  inline float ivec3::length2() const
  {  return static_cast<float>(n[VX]*n[VX] + n[VY]*n[VY] + n[VZ]*n[VZ]); }
  
  inline ivec3& ivec3::apply(VI_FCT_PTR fct)
  { n[VX] = (*fct)(n[VX]); n[VY] = (*fct)(n[VY]); n[VZ] = (*fct)(n[VZ]);
    return *this; }
  
  
  // FRIENDS
  
  inline ivec3 operator - (const ivec3& a)
  {  return ivec3(-a.n[VX],-a.n[VY],-a.n[VZ]); }
  
  inline ivec3 operator + (const ivec3& a, const ivec3& b)
  { return ivec3(a.n[VX]+ b.n[VX], a.n[VY] + b.n[VY], a.n[VZ] + b.n[VZ]); }
  
  inline ivec3 operator - (const ivec3& a, const ivec3& b)
  { return ivec3(a.n[VX]-b.n[VX], a.n[VY]-b.n[VY], a.n[VZ]-b.n[VZ]); }
  
  inline ivec3 operator * (const ivec3& a, const int d)
    { return ivec3(d*a.n[VX], d*a.n[VY], d*a.n[VZ]); }
  
  inline ivec3 operator * (const int d, const ivec3& a)
    { return a*d; }
  
  inline ivec3 operator * (const ivec3& a, const ivec3& b)
    { return ivec3(a.n[VX]*b.n[VX], a.n[VY]*b.n[VY], a.n[VZ]*b.n[VZ]); }
  
  inline ivec3 operator / (const ivec3& a, const int d)
  { return ivec3(a.n[VX]/d, a.n[VY]/d, a.n[VZ]/d); }

  inline ivec3 operator / (const ivec3& a, const ivec3& b) // it is up to the caller to prevent division by zero
  { return ivec3(a.n[VX] / b.n[VX], a.n[VY] / b.n[VY],
		a.n[VZ] / b.n[VZ]); }
  
  inline ivec3 operator ^ (const ivec3& a, const ivec3& b) {
    return ivec3(a.n[VY]*b.n[VZ] - a.n[VZ]*b.n[VY],
		 a.n[VZ]*b.n[VX] - a.n[VX]*b.n[VZ],
		 a.n[VX]*b.n[VY] - a.n[VY]*b.n[VX]);
  }
  
  inline int operator == (const ivec3& a, const ivec3& b)
  { return (a.n[VX] == b.n[VX]) && (a.n[VY] == b.n[VY]) && (a.n[VZ] == b.n[VZ]);
  }
  
  inline int operator != (const ivec3& a, const ivec3& b)
  { return !(a == b); }
  
#ifdef ALGEBRA3IOSTREAMS
  inline ostream& operator << (ostream& s, const ivec3& v)
  { return s << "| " << v.n[VX] << ' ' << v.n[VY] << ' ' << v.n[VZ] << " |"; }
  
  inline istream& operator >> (istream& s, ivec3& v) {
    vec3	v_tmp;
    char	c = ' ';
    
    while (isspace(c))
      s >> c;
    // The vectors can be formatted either as x y z or | x y z |
    if (c == '|') {
      s >> v_tmp[VX] >> v_tmp[VY] >> v_tmp[VZ];
      while (s >> c && isspace(c)) ;
      if (c != '|')
	s.set(_bad);
    }
    else {
      s.putback(c);
      s >> v_tmp[VX] >> v_tmp[VY] >> v_tmp[VZ];
    }
    if (s)
      v = v_tmp;
    return s;
  }
#endif // ALGEBRA3IOSTREAMS
  
  inline void swap(ivec3& a, ivec3& b)
  { ivec3 tmp(a); a = b; b = tmp; }
  
  inline ivec3 min(const ivec3& a, const ivec3& b)
  { return ivec3(MIN(a.n[VX], b.n[VX]), MIN(a.n[VY], b.n[VY]), MIN(a.n[VZ],
								  b.n[VZ])); }
  
  inline ivec3 max(const ivec3& a, const ivec3& b)
  { return ivec3(MAX(a.n[VX], b.n[VX]), MAX(a.n[VY], b.n[VY]), MAX(a.n[VZ],
								  b.n[VZ])); }
  
  inline ivec3 prod(const ivec3& a, const ivec3& b)
  { return ivec3(a.n[VX] * b.n[VX], a.n[VY] * b.n[VY], a.n[VZ] * b.n[VZ]); }
  
  inline int dot(const ivec3& a, const ivec3& b)
  { return (a.n[VX]*b.n[VX] + a.n[VY]*b.n[VY] + a.n[VZ]*b.n[VZ]); }



/****************************************************************
   *																*
   *		    ivec4 Member functions								*
   *																*
   ****************************************************************/
  
  // CONSTRUCTORS
  
  inline ivec4::ivec4() {}
  
  inline ivec4::ivec4(const int x, const int y, const int z, const int w)
  { n[VX] = x; n[VY] = y; n[VZ] = z; n[VW] = w; }
  
  inline ivec4::ivec4(const int d)
  {  n[VX] = n[VY] = n[VZ] = n[VW] = d; }
  
  inline ivec4::ivec4(const ivec4& v)
  { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VZ]; n[VW] = v.n[VW]; }
  
  inline ivec4::ivec4(const ivec3& v)
  { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VZ]; n[VW] = 1; }
  
  inline ivec4::ivec4(const ivec3& v, const int d)
  { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VZ];  n[VW] = d; }
  
  
  // ASSIGNMENT OPERATORS
  
  inline ivec4& ivec4::operator = (const ivec4& v)
    { n[VX] = v.n[VX]; n[VY] = v.n[VY]; n[VZ] = v.n[VZ]; n[VW] = v.n[VW];
      return *this; }
  
  inline ivec4& ivec4::operator += ( const ivec4& v )
  { n[VX] += v.n[VX]; n[VY] += v.n[VY]; n[VZ] += v.n[VZ]; n[VW] += v.n[VW];
    return *this; }
  
  inline ivec4& ivec4::operator -= ( const ivec4& v )
  { n[VX] -= v.n[VX]; n[VY] -= v.n[VY]; n[VZ] -= v.n[VZ]; n[VW] -= v.n[VW];
    return *this; }
  
  inline ivec4& ivec4::operator *= ( const int d )
  { n[VX] *= d; n[VY] *= d; n[VZ] *= d; n[VW] *= d; return *this; }

  inline ivec4& ivec4::operator *= ( const ivec4& v )
  { n[VX] *= v.n[VX]; n[VY] *= v.n[VY]; n[VZ] *= v.n[VZ]; n[VW] *= v.n[VW]; return *this; }
  
  inline ivec4& ivec4::operator /= ( const int d )
  { n[VX] /= d; n[VY] /= d; n[VZ] /= d; n[VW] /= d; return *this; }

  inline ivec4& ivec4::operator /= ( const ivec4& v ) // it is up to the caller to prevent division by zero
  { n[VX] /= v.n[VX]; n[VY] /= v.n[VY]; n[VZ] /= v.n[VZ];
    n[VW] /= v.n[VW]; return *this; }
  
  inline int& ivec4::operator [] ( int i) {
    assert(! (i < VX || i > VW));
    return n[i];
  }
  
  inline int ivec4::operator [] ( int i) const {
    assert(! (i < VX || i > VW));
    return n[i];
  }
  
  inline int* ivec4::ptr(){ return n; }
  inline const int* ivec4::ptr() const { return n; }
  
  // SPECIAL FUNCTIONS
  
  inline float ivec4::length() const
  { return sqrt(length2()); }
  
  inline float ivec4::length2() const
  { return static_cast<float>(n[VX]*n[VX] + n[VY]*n[VY] + n[VZ]*n[VZ] + n[VW]*n[VW]); }
  
  inline ivec4& ivec4::apply(VI_FCT_PTR fct)
  { n[VX] = (*fct)(n[VX]); n[VY] = (*fct)(n[VY]); n[VZ] = (*fct)(n[VZ]);
    n[VW] = (*fct)(n[VW]); return *this; }
  
  
  // FRIENDS
  
  inline ivec4 operator - (const ivec4& a)
  { return ivec4(-a.n[VX],-a.n[VY],-a.n[VZ],-a.n[VW]); }
  
  inline ivec4 operator + (const ivec4& a, const ivec4& b)
  { return ivec4(a.n[VX] + b.n[VX], a.n[VY] + b.n[VY], a.n[VZ] + b.n[VZ],
		a.n[VW] + b.n[VW]); }
  
  inline ivec4 operator - (const ivec4& a, const ivec4& b)
  {  return ivec4(a.n[VX] - b.n[VX], a.n[VY] - b.n[VY], a.n[VZ] - b.n[VZ],
		  a.n[VW] - b.n[VW]); }
  
  inline ivec4 operator * (const ivec4& a, const int d)
    { return ivec4(d*a.n[VX], d*a.n[VY], d*a.n[VZ], d*a.n[VW] ); }
  
  inline ivec4 operator * (const int d, const ivec4& a)
    { return a*d; }
  
  inline ivec4 operator * (const ivec4& a, const ivec4& b)
    { return ivec4(a.n[VX]*b.n[VX], a.n[VY]*b.n[VY], a.n[VZ]*b.n[VZ],
		   a.n[VW]*b.n[VW]); }
  
  inline ivec4 operator / (const ivec4& a, const int d)
  { return ivec4(a.n[VX]/d, a.n[VY]/d, a.n[VZ]/d, a.n[VW]/d); }

  inline ivec4 operator / (const ivec4& a, const ivec4& b)
  { return ivec4(a.n[VX] / b.n[VX], a.n[VY] / b.n[VY], a.n[VZ] / b.n[VZ],
		a.n[VW] / b.n[VW]); }
  
  inline int operator == (const ivec4& a, const ivec4& b)
  { return (a.n[VX] == b.n[VX]) && (a.n[VY] == b.n[VY]) && (a.n[VZ] == b.n[VZ])
    && (a.n[VW] == b.n[VW]); }
  
  inline int operator != (const ivec4& a, const ivec4& b)
  { return !(a == b); }
  
#ifdef ALGEBRA3IOSTREAMS
  inline ostream& operator << (ostream& s, const ivec4& v)
  { return s << "| " << v.n[VX] << ' ' << v.n[VY] << ' ' << v.n[VZ] << ' '
	     << v.n[VW] << " |"; }
  
  inline stream& operator >> (istream& s, ivec4& v) {
    vec4	v_tmp;
    char	c = ' ';
    
    while (isspace(c))
      s >> c;
    // The vectors can be formatted either as x y z w or | x y z w |
    if (c == '|') {
      s >> v_tmp[VX] >> v_tmp[VY] >> v_tmp[VZ] >> v_tmp[VW];
      while (s >> c && isspace(c)) ;
      if (c != '|')
	s.set(_bad);
    }
    else {
      s.putback(c);
      s >> v_tmp[VX] >> v_tmp[VY] >> v_tmp[VZ] >> v_tmp[VW];
    }
    if (s)
      v = v_tmp;
    return s;
  }
#endif // ALGEBRA3IOSTREAMS
  
  inline void swap(ivec4& a, ivec4& b)
  { ivec4 tmp(a); a = b; b = tmp; }
  
  inline ivec4 min(const ivec4& a, const ivec4& b)
  { return ivec4(MIN(a.n[VX], b.n[VX]), MIN(a.n[VY], b.n[VY]), MIN(a.n[VZ],
								   b.n[VZ]), MIN(a.n[VW], b.n[VW])); }
  
  inline ivec4 max(const ivec4& a, const ivec4& b)
  { return ivec4(MAX(a.n[VX], b.n[VX]), MAX(a.n[VY], b.n[VY]), MAX(a.n[VZ],
								   b.n[VZ]), MAX(a.n[VW], b.n[VW])); }
  
  inline ivec4 prod(const ivec4& a, const ivec4& b)
  { return ivec4(a.n[VX] * b.n[VX], a.n[VY] * b.n[VY], a.n[VZ] * b.n[VZ],
		 a.n[VW] * b.n[VW]); }
  
  inline int dot(const ivec4& a, const ivec4& b)
  { return (a.n[VX]*b.n[VX] + a.n[VY]*b.n[VY] + a.n[VZ]*b.n[VZ] +
	    a.n[VW]*b.n[VW]); }  


};
#endif // ALGEBRA3H
