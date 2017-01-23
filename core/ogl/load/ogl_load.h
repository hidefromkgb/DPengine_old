#ifndef OGL_LOAD_H
#define OGL_LOAD_H

/** ogl_load: A header-only library that makes your app OpenGL 2.0 capable.
    Original author: hidefromkgb (hidefromkgb@gmail.com)
    _________________________________________________________________________

    This is free and unencumbered software released into the public domain.

    Anyone is free to copy, modify, publish, use, compile, sell, or
    distribute this software, either in source code form or as a compiled
    binary, for any purpose, commercial or non-commercial, and by any means.

    In jurisdictions that recognize copyright laws, the author or authors
    of this software dedicate any and all copyright interest in the
    software to the public domain. We make this dedication for the benefit
    of the public at large and to the detriment of our heirs and
    successors. We intend this dedication to be an overt act of
    relinquishment in perpetuity of all present and future rights to this
    software under copyright law.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
    _________________________________________________________________________
**/

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>



#ifndef OGL_TEXTURE_ERROR
#define OGL_TEXTURE_ERROR(s) printf("%s!\n", (s))
#endif
#ifndef OGL_SHADER_ERROR
#define OGL_SHADER_ERROR(p, s) printf("%s error!\n%s\n\n",             \
                                     (p)? "Shader" : "Shader program", \
                                     (s)? (s) : "[NO LOG RETURNED]")
#endif
#ifndef OGL_LOAD_ERROR
#define OGL_LOAD_ERROR(p, s) _OGL_FailDispatcher((GLvoid**)&p, (GLchar*)(s))
#endif

#ifdef _WIN32
    typedef char GLchar;
    #include <GL/gl.h>
    #include <windows.h>
    #define GL_COMPILE_STATUS                 0x8B81
    #define GL_LINK_STATUS                    0x8B82
    #define GL_VALIDATE_STATUS                0x8B83
    #define GL_INFO_LOG_LENGTH                0x8B84
    #define GL_FRAGMENT_SHADER                0x8B30
    #define GL_VERTEX_SHADER                  0x8B31
    #define GL_READ_ONLY                      0x88B8
    #define GL_PIXEL_PACK_BUFFER              0x88EB
    #define GL_READ_FRAMEBUFFER               0x8CA8
    #define GL_DRAW_FRAMEBUFFER               0x8CA9
    #define GL_FRAMEBUFFER                    0x8D40
    #define GL_RENDERBUFFER                   0x8D41
    #define GL_COLOR_ATTACHMENT0              0x8CE0
    #define GL_TEXTURE_SWIZZLE_R              0x8E42
    #define GL_TEXTURE_SWIZZLE_G              0x8E43
    #define GL_TEXTURE_SWIZZLE_B              0x8E44
    #define GL_TEXTURE_SWIZZLE_A              0x8E45
    #define GL_TEXTURE0                       0x84C0
    #define GL_TEXTURE_CUBE_MAP               0x8513
    #define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
    #define GL_TEXTURE_DEPTH                  0x8071
    #define GL_TEXTURE_WRAP_R                 0x8072
    #define GL_TEXTURE_3D                     0x806F
    #define GL_TEXTURE_2D_ARRAY               0x8C1A
    #define GL_ARRAY_BUFFER                   0x8892
    #define GL_ELEMENT_ARRAY_BUFFER           0x8893
    #define GL_DEPTH_COMPONENT16              0x81A5
    #define GL_DEPTH_COMPONENT24              0x81A6
    #define GL_DEPTH_COMPONENT32              0x81A7
    #define GL_DEPTH_ATTACHMENT               0x8D00
    #define GL_STREAM_READ                    0x88E1
    #define GL_STREAM_DRAW                    0x88E0
    #define GL_STATIC_DRAW                    0x88E4
    #define GL_DYNAMIC_DRAW                   0x88E8
    #define GL_CLAMP_TO_EDGE                  0x812F
    #define GL_R8                             0x8229
    #define GL_RG8                            0x822B
    #define GL_RED                            0x1903
    #define GL_RED_INTEGER                    0x8D94
    #define GL_RG                             0x8227
    #define GL_RG_INTEGER                     0x8228
    #define GL_RGB_INTEGER                    0x8D98
    #define GL_RGBA_INTEGER                   0x8D99
    #define GL_BGR_INTEGER                    0x8D9A
    #define GL_BGRA_INTEGER                   0x8D9B
    #define GL_BGR                            0x80E0
    #define GL_BGRA                           0x80E1
    #define GL_R32F                           0x822E
    #define GL_RG32F                          0x8230
    #define GL_RGB32F                         0x8815
    #define GL_RGBA32F                        0x8814
    #define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
    #define _OGL_GET_FUNCTION(s) wglGetProcAddress(s)
#elif __APPLE__
    #include <OpenGL/gl.h>
    #include <dlfcn.h>
    #define APIENTRY
    #define GL_RGBA32F                        0x8814
    #define GL_TEXTURE_2D_ARRAY               0x8C1A
    #define _OGL_GET_FUNCTION(s) dlsym(RTLD_DEFAULT, s)
#else
    #include <GL/gl.h>
    #include <GL/glx.h>
    #define _OGL_GET_FUNCTION(s) glXGetProcAddress((GLubyte*)(s))
#endif



#define _OGL_L(c, ...) \
_OGL_L4(c,1,0,,,,,,,,,,,,,##__VA_ARGS__) _OGL_L4(c,0,1,,,,,,,,,##__VA_ARGS__) \
_OGL_L4(c,0,2,,,,,        ##__VA_ARGS__) _OGL_L4(c,0,3,        ##__VA_ARGS__)

#define _OGL_L4(c, f, n, ...) \
_OGL_L3(c,f,n##0,,,,__VA_ARGS__) _OGL_L3(c,0,n##1,,,__VA_ARGS__) \
_OGL_L3(c,0,n##2,,  __VA_ARGS__) _OGL_L3(c,0,n##3,  __VA_ARGS__)

#define _OGL_L3(...) _OGL_L2(__VA_ARGS__, \
1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,  0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, )

#define _OGL_L2(c, f, \
n00,n01,n02,n03, n04,n05,n06,n07, n08,n09,n0A,n0B, n0C,n0D,n0E,n0F, \
a00,a01,a02,a03, a04,a05,a06,a07, a08,a09,a0A,a0B, a0C,a0D,a0E,a0F, \
s, ...) _OGL_L##s(c, f, n00, a00)

#define _OGL_L1(c, f, n, a) c##f(n, a)
#define _OGL_L0(c, f, n, a)

#define _OGL_P1(n, a)   a _##n
#define _OGL_P0(n, a) , _OGL_P1(n, a)
#define _OGL_A1(n, a)   _OGL_P1(n,  )
#define _OGL_A0(n, a)   _OGL_P0(n,  )

#define _OGL_F(retn, name, ...) __attribute__((unused))    \
static retn name(_OGL_L(_OGL_P, ##__VA_ARGS__)) {          \
    static APIENTRY retn (*func)(__VA_ARGS__) = 0;         \
    if (!(func || (func = (APIENTRY retn (*)(__VA_ARGS__)) \
                           _OGL_MainLoader(#name)))) {     \
        OGL_LOAD_ERROR(func, #name);                       \
        if (!func) {                                       \
            retn (*fail)() = (void*)_OGL_Stub;             \
            return fail();                                 \
        }                                                  \
    }                                                      \
    return func(_OGL_L(_OGL_A, ##__VA_ARGS__));            \
}

static APIENTRY GLvoid _OGL_glGenerateMipmap(GLenum);

__attribute__((unused))
static GLvoid _OGL_FailDispatcher(GLvoid **func, GLchar *name) {
    printf("ERROR: can`t load '%s*'!\n", name);
    if (!strcmp(name, "glGenerateMipmap"))
        *func = _OGL_glGenerateMipmap;
}

__attribute__((unused))
static GLvoid *_OGL_Stub() {
    return (GLvoid*)0;
}

__attribute__((unused))
static GLvoid *_OGL_MainLoader(GLchar *name) {
    GLchar *suff[] = {"", "ARB", "EXT", "NV", "AMD", "ATI"}, temp[128], *indx;
    GLvoid *retn = 0;
    GLint iter = -1;

    *temp = 0;
    strcat(temp, name);
    indx = &temp[strlen(temp)];
    while (!retn && (++iter < sizeof(suff) / sizeof(*suff))) {
        *indx = 0;
        strcat(indx, suff[iter]);
        retn = (GLvoid*)_OGL_GET_FUNCTION(temp);
    }
    return retn;
}

#ifndef __APPLE__
_OGL_F(GLvoid, glUniform1iv, GLint, GLsizei, GLint*);
_OGL_F(GLvoid, glUniform1fv, GLint, GLsizei, GLfloat*);
_OGL_F(GLvoid, glUniform2iv, GLint, GLsizei, GLint*);
_OGL_F(GLvoid, glUniform2fv, GLint, GLsizei, GLfloat*);
_OGL_F(GLvoid, glUniform3iv, GLint, GLsizei, GLint*);
_OGL_F(GLvoid, glUniform3fv, GLint, GLsizei, GLfloat*);
_OGL_F(GLvoid, glUniform4iv, GLint, GLsizei, GLint*);
_OGL_F(GLvoid, glUniform4fv, GLint, GLsizei, GLfloat*);
_OGL_F(GLvoid, glUniformMatrix4fv, GLint, GLsizei, GLboolean, GLfloat*);
_OGL_F(GLuint, glCreateProgram);
_OGL_F(GLvoid, glDeleteProgram, GLuint);
_OGL_F(GLvoid, glValidateProgram, GLuint);
_OGL_F(GLvoid, glLinkProgram, GLuint);
_OGL_F(GLvoid, glUseProgram, GLuint);
_OGL_F(GLvoid, glGetProgramInfoLog, GLuint, GLint, GLint*, GLchar*);
_OGL_F(GLvoid, glGetShaderInfoLog, GLuint, GLint, GLint*, GLchar*);
_OGL_F(GLvoid, glGetProgramiv, GLuint, GLenum, GLint*);
_OGL_F(GLvoid, glGetShaderiv, GLuint, GLenum, GLint*);
_OGL_F(GLuint, glCreateShader, GLenum);
_OGL_F(GLvoid, glDeleteShader, GLuint);
_OGL_F(GLvoid, glAttachShader, GLuint, GLuint);
_OGL_F(GLvoid, glShaderSource, GLuint, GLuint, const GLchar**, GLint*);
_OGL_F(GLvoid, glCompileShader, GLuint);
_OGL_F(GLint,  glGetAttribLocation, GLuint, GLchar*);
_OGL_F(GLint,  glGetUniformLocation, GLuint, GLchar*);
_OGL_F(GLvoid, glEnableVertexAttribArray, GLint);
_OGL_F(GLvoid, glDisableVertexAttribArray, GLint);
_OGL_F(GLvoid, glVertexAttribPointer, GLuint, GLint, GLenum,
                                      GLboolean, GLsizei, GLvoid*);
#ifdef _WIN32
_OGL_F(GLvoid, glActiveTexture, GLenum);
_OGL_F(GLvoid, glTexImage3D, GLenum, GLint, GLenum, GLsizei, GLsizei,
                             GLsizei, GLint, GLenum, GLenum, GLvoid*);
_OGL_F(GLvoid, glTexSubImage3D, GLenum, GLint, GLint, GLint, GLint, GLsizei,
                                GLsizei, GLsizei, GLenum, GLenum, GLvoid*);
#endif /** _WIN32 **/
_OGL_F(GLvoid*,glMapBuffer, GLenum, GLenum);
_OGL_F(GLvoid, glUnmapBuffer, GLenum);
_OGL_F(GLvoid, glGenBuffers, GLsizei, GLuint*);
_OGL_F(GLvoid, glBindBuffer, GLenum, GLuint);
_OGL_F(GLvoid, glBufferData, GLenum, GLsizei, GLvoid*, GLenum);
_OGL_F(GLvoid, glBufferSubData, GLenum, GLint, GLsizei, GLvoid*);
_OGL_F(GLvoid, glDeleteBuffers, GLsizei, GLuint*);
/** these require GL_(ARB|EXT)_framebuffer_object that is not in OGL 2.0,
    but still, they are extremely useful, so try to load them anyway! **/
_OGL_F(GLvoid, glGenFramebuffers, GLsizei, GLuint*);
_OGL_F(GLvoid, glGenRenderbuffers, GLsizei, GLuint*);
_OGL_F(GLvoid, glDeleteFramebuffers, GLsizei, GLuint*);
_OGL_F(GLvoid, glDeleteRenderbuffers, GLsizei, GLuint*);
_OGL_F(GLvoid, glBindFramebuffer, GLenum, GLuint);
_OGL_F(GLvoid, glBindRenderbuffer, GLenum, GLuint);
_OGL_F(GLvoid, glRenderbufferStorage, GLenum, GLenum, GLsizei, GLsizei);
_OGL_F(GLvoid, glFramebufferRenderbuffer, GLenum, GLenum, GLenum, GLuint);
_OGL_F(GLvoid, glFramebufferTexture2D, GLenum, GLenum, GLenum, GLuint, GLint);
_OGL_F(GLvoid, glGenerateMipmap, GLenum);
/** end of GL_(ARB|EXT)_framebuffer_object **/
#endif /** !__APPLE__ **/

#undef _OGL_L
#undef _OGL_L4
#undef _OGL_L3
#undef _OGL_L2
#undef _OGL_L1
#undef _OGL_L0
#undef _OGL_P1
#undef _OGL_P0
#undef _OGL_A1
#undef _OGL_A0
#undef _OGL_F

__attribute__((unused))
static APIENTRY GLvoid _OGL_glGenerateMipmap(GLenum what) {
    GLint fltr;

    glGetTexParameteriv(what, GL_TEXTURE_MIN_FILTER, &fltr);
    switch (fltr) {
        case GL_LINEAR_MIPMAP_LINEAR:
        case GL_LINEAR_MIPMAP_NEAREST:
            glTexParameteri(what, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            break;
        case GL_NEAREST_MIPMAP_LINEAR:
        case GL_NEAREST_MIPMAP_NEAREST:
            glTexParameteri(what, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            break;
    }
}



/** types of texture binding modes **/
#define OGL_TEX_NSET 0 /** do not bind, just return OGL_FTEX **/
#define OGL_TEX_DFLT 1 /** do a regular bind **/
#define OGL_TEX_FRMB 2 /** do a framebuffer bind **/

/** types of uniforms **/
#define OGL_UNI_T1IV 0
#define OGL_UNI_T1FV 1

#define OGL_UNI_T2IV 2
#define OGL_UNI_T2FV 3

#define OGL_UNI_T3IV 4
#define OGL_UNI_T3FV 5

#define OGL_UNI_T4IV 6
#define OGL_UNI_T4FV 7

#define OGL_UNI_TMFV 8

/** this flag is to indicate that the uniform value is immediate **/
#define OGL_UNI_IIII 0x8000

/** immediate types **/
#define OGL_UNI_T1II (OGL_UNI_T1IV | OGL_UNI_IIII)
#define OGL_UNI_T1FI (OGL_UNI_T1FV | OGL_UNI_IIII)

#define OGL_UNI_T2II (OGL_UNI_T2IV | OGL_UNI_IIII)
#define OGL_UNI_T2FI (OGL_UNI_T2FV | OGL_UNI_IIII)

#define OGL_UNI_T3II (OGL_UNI_T3IV | OGL_UNI_IIII)
#define OGL_UNI_T3FI (OGL_UNI_T3FV | OGL_UNI_IIII)

#define OGL_UNI_T4II (OGL_UNI_T4IV | OGL_UNI_IIII)
#define OGL_UNI_T4FI (OGL_UNI_T4FV | OGL_UNI_IIII)

#define OGL_UNI_TMFI (OGL_UNI_TMFV | OGL_UNI_IIII)



typedef struct {
    GLenum type, draw;
    GLuint indx, cdat;
    GLvoid *pdat;
    GLchar *name;
} OGL_UNIF;

typedef struct OGL_FTEX OGL_FTEX;
#define _OGL_FTEX struct {         \
    GLenum trgt, type, mode;       \
    GLuint xdim, ydim, zdim, indx; \
    GLvoid *orig;                  \
}

typedef struct OGL_FVBO OGL_FVBO;
#define _OGL_FVBO struct { \
    GLenum      elem;      \
    GLuint      cind;      \
    GLuint      ctex;      \
    _OGL_FTEX **ptex;      \
    GLuint      catr;      \
    GLuint     *pbuf;      \
    struct {               \
        GLint   aloc;      \
        GLint   ecnt;      \
        GLenum  elem;      \
    } *patr;               \
    GLuint      cshd;      \
    struct {               \
        GLuint    prog;    \
        GLuint    cuni;    \
        OGL_UNIF *puni;    \
    } *pshd;               \
}



__attribute__((unused))
static GLint _OGL_EnumShader(GLuint prog, GLboolean shad, GLenum parm) {
    GLint stat, size = 0;
    GLchar *buff = 0;

    if (shad) {
        glGetShaderiv(prog, parm, &stat);
        if (stat != GL_TRUE) {
            glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &size);
            if (size)
                glGetShaderInfoLog(prog, size, &size,
                                   buff = calloc(1, size + 1));
            OGL_SHADER_ERROR(1, buff);
        }
    }
    else {
        glGetProgramiv(prog, parm, &stat);
        if (stat != GL_TRUE) {
            glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &size);
            if (size)
                glGetProgramInfoLog(prog, size, &size,
                                    buff = calloc(1, size + 1));
            OGL_SHADER_ERROR(0, buff);
        }
    }
    free(buff);
    return stat;
}



__attribute__((unused))
static GLboolean _OGL_MakeShader(const GLchar *fstr,
                                 GLuint prog, GLenum type) {
    GLint slen, shad;

    if (!fstr || !(shad = glCreateShader(type))) {
        OGL_SHADER_ERROR(1, "Shader allocation failed");
        glDeleteProgram(prog);
        return GL_FALSE;
    }
    slen = strlen(fstr);
    glShaderSource(shad, 1, &fstr, &slen);
    glCompileShader(shad); /** may emit a ';' to STDOUT, this is normal **/

    if (_OGL_EnumShader(shad, GL_TRUE, GL_COMPILE_STATUS) != GL_TRUE) {
        glDeleteShader(shad);
        glDeleteProgram(prog);
        return GL_FALSE;
    }
    glAttachShader(prog, shad);
    glDeleteShader(shad);
    return GL_TRUE;
}



__attribute__((unused))
static OGL_FTEX **OGL_BindTex(OGL_FVBO *vobj, GLuint bind, GLuint mode) {
    typedef _OGL_FTEX __OGL_FTEX;
    typedef _OGL_FVBO __OGL_FVBO;

    GLuint ktex;
    GLvoid *temp;
    __OGL_FTEX *ftex;
    __OGL_FVBO *vtex;

    if (!(vtex = (__OGL_FVBO*)vobj))
        return 0;
    if (vtex->ptex[ktex = bind] && !vtex->ptex[ktex]->trgt
                                &&  vtex->ptex[ktex]->orig) {
        do {
            if (!(ftex = (__OGL_FTEX*)vtex->ptex[ktex])) {
                OGL_TEXTURE_ERROR("Invalid texture pointer");
                return 0;
            }
            if ((OGL_FVBO*)(vtex = (__OGL_FVBO*)ftex->orig) == vobj) {
                OGL_TEXTURE_ERROR("Texture circular cross-ref");
                return 0;
            }
            ktex = ftex->indx;
        } while (vtex);
        temp = (vtex = (__OGL_FVBO*)vobj)->ptex[ktex = bind]->orig;
        memmove(vtex->ptex[ktex], ftex, sizeof(*ftex));
        vtex->ptex[ktex]->orig = temp;
    }
    if (mode == OGL_TEX_FRMB)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, vtex->ptex[ktex]->indx, 0);
    else if (mode == OGL_TEX_DFLT) {
        glActiveTexture(GL_TEXTURE0 + bind);
        glBindTexture(vtex->ptex[ktex]->trgt, vtex->ptex[ktex]->indx);
    }
    return (OGL_FTEX**)&vtex->ptex[ktex];
}



__attribute__((unused))
static GLvoid OGL_EnumTex(OGL_FTEX *ftex,
                          GLenum *trgt, GLenum *type, GLenum *mode,
                          GLuint *xdim, GLuint *ydim, GLuint *zdim) {
    if (ftex) {
        if (trgt)
           *trgt = ((_OGL_FTEX*)ftex)->trgt;
        if (type)
           *type = ((_OGL_FTEX*)ftex)->type;
        if (mode)
           *mode = ((_OGL_FTEX*)ftex)->mode;
        if (xdim)
           *xdim = ((_OGL_FTEX*)ftex)->xdim;
        if (ydim)
           *ydim = ((_OGL_FTEX*)ftex)->ydim;
        if (zdim)
           *zdim = ((_OGL_FTEX*)ftex)->zdim;
    }
}



__attribute__((unused))
static GLvoid OGL_FreeTex(OGL_FTEX **ftex) {
    if (*ftex && !((_OGL_FTEX*)*ftex)->orig) {
        glBindTexture(((_OGL_FTEX*)*ftex)->trgt, 0);
        glDeleteTextures(1, &((_OGL_FTEX*)*ftex)->indx);
    }
    free(*ftex);
    *ftex = 0;
}



__attribute__((unused))
static GLvoid OGL_LinkTex(OGL_FVBO *dvbo, GLuint dind,
                          OGL_FVBO *svbo, GLuint sind) {
    OGL_FTEX **ftex = OGL_BindTex(dvbo, dind, OGL_TEX_NSET);

    if (ftex) {
        OGL_FreeTex(ftex);
        *ftex = calloc(1, sizeof(_OGL_FTEX));
        ((_OGL_FTEX*)*ftex)->orig = svbo;
        ((_OGL_FTEX*)*ftex)->indx = sind;
    }
}



__attribute__((unused))
static OGL_FTEX *OGL_MakeTex(GLuint xdim, GLuint ydim, GLuint zdim,
                             GLenum trgt, GLenum wrap, GLint tmag, GLint tmin,
                             GLenum type, GLenum frmt, GLenum mode,
                             GLvoid *data) {
    GLuint iter, iend;
    _OGL_FTEX *retn;

    if (!trgt)
        return 0;
    retn = calloc(1, sizeof(*retn));
    retn->trgt = trgt;
    retn->type = type;
    retn->mode = mode;
    retn->xdim = xdim;
    retn->ydim = ydim;
    retn->zdim = zdim;
    glGenTextures(1, &retn->indx);
    glBindTexture(retn->trgt, retn->indx);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (retn->trgt != GL_TEXTURE_CUBE_MAP)
        iend = iter = retn->trgt;
    else {
        iend = 6 + (iter = GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        glTexParameteri(retn->trgt, GL_TEXTURE_WRAP_R, wrap);
    }
    while (glGetError() != GL_NO_ERROR);
    if ((retn->trgt != GL_TEXTURE_3D)
    &&  (retn->trgt != GL_TEXTURE_2D_ARRAY))
        for (; iter <= iend; iter++)
            glTexImage2D(iter, 0, frmt, retn->xdim, retn->ydim,
                         0, retn->mode, retn->type, data);
    else {
        glTexParameteri(retn->trgt, GL_TEXTURE_WRAP_R, wrap);
        glTexImage3D(retn->trgt, 0, frmt, retn->xdim, retn->ydim,
                     retn->zdim, 0, retn->mode, retn->type, data);
    }
    if (glGetError() != GL_NO_ERROR)
        OGL_FreeTex((OGL_FTEX**)&retn);
    else {
        glTexParameteri(retn->trgt, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(retn->trgt, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(retn->trgt, GL_TEXTURE_MAG_FILTER, tmag);
        glTexParameteri(retn->trgt, GL_TEXTURE_MIN_FILTER, tmin);
        if ((tmin == GL_NEAREST_MIPMAP_NEAREST)
        ||  (tmin == GL_NEAREST_MIPMAP_LINEAR)
        ||  (tmin == GL_LINEAR_MIPMAP_NEAREST)
        ||  (tmin == GL_LINEAR_MIPMAP_LINEAR))
            glGenerateMipmap(retn->trgt);
    }
    glBindTexture(trgt, 0);
    return (OGL_FTEX*)retn;
}



__attribute__((unused))
static GLenum OGL_LoadTex(OGL_FTEX *ftex,
                          GLint  xpos, GLint  ypos, GLint  zpos,
                          GLuint xdim, GLuint ydim, GLuint zdim,
                          GLvoid   *data) {
    typedef _OGL_FTEX __OGL_FTEX;

    __OGL_FTEX *retn = (__OGL_FTEX*)ftex;

    glBindTexture(retn->trgt, retn->indx);
    while (glGetError() != GL_NO_ERROR);
    switch (retn->trgt) {
        case GL_TEXTURE_2D:
            glTexSubImage2D(retn->trgt, 0, xpos, ypos,
                            xdim, ydim, retn->mode, retn->type, data);
            break;

        case GL_TEXTURE_3D:
        case GL_TEXTURE_2D_ARRAY:
            glTexSubImage3D(retn->trgt, 0, xpos, ypos, zpos,
                            xdim, ydim, zdim, retn->mode, retn->type, data);
            break;
    }
    return glGetError();
}



#define OGL_MakeVBO(ctex, elem, catr, patr, cuni, puni, cshd, pshd, ...) \
       _OGL_MakeVBO(ctex, elem, catr, patr, cuni, puni, cshd, pshd,      \
                    ##__VA_ARGS__, (GLchar*)0)
__attribute__((unused))
static OGL_FVBO *_OGL_MakeVBO(GLuint ctex, GLenum elem,
                              GLuint catr, OGL_UNIF *patr,
                              GLuint cuni, OGL_UNIF *puni,
                              GLuint cshd, GLchar *pshd[], ...) {
    GLint iter, indx, ctmp, step;
    GLchar **shdr, *curv, *curp;
    _OGL_FVBO *retn;
    va_list list;

    retn = calloc(1, sizeof(*retn));
    if (pshd && cshd && (~cshd & 1)) {
        shdr = calloc(cshd, sizeof(*pshd));
        for (iter = 0; iter < cshd; iter++)
            if (pshd[iter] == (GLchar*)-1)
                shdr[iter] = pshd[iter];
            else {
                indx = strlen(pshd[iter]);
                va_start(list, pshd);
                while ((curp = va_arg(list, GLchar*)))
                    indx += strlen(curp);
                va_end(list);
                curp = calloc(sizeof(*curp), indx + 2);
                va_start(list, pshd);
                vsnprintf(curp, indx + 1, pshd[iter], list);
                shdr[iter] = curp;
                va_end(list);
            }
        curv = curp = 0;
        retn->pshd = calloc(retn->cshd = cshd >> 1, sizeof(*retn->pshd));
        for (iter = 0; iter < retn->cshd; iter++) {
            if (shdr[iter * 2 + 0] != (GLchar*)-1)
                curv = shdr[iter * 2 + 0];
            if (shdr[iter * 2 + 1] != (GLchar*)-1)
                curp = shdr[iter * 2 + 1];
            retn->pshd[iter].prog = glCreateProgram();
            if (_OGL_MakeShader(curp, retn->pshd[iter].prog,
                                GL_FRAGMENT_SHADER)
            &&  _OGL_MakeShader(curv, retn->pshd[iter].prog,
                                GL_VERTEX_SHADER)) {
                glLinkProgram(retn->pshd[iter].prog);
                if (_OGL_EnumShader(retn->pshd[iter].prog, GL_FALSE,
                                    GL_LINK_STATUS) == GL_TRUE) {
                    glUseProgram(retn->pshd[iter].prog);
                    retn->pshd[iter].cuni = 0;
                    for (ctmp = 0; ctmp < cuni; ctmp++) {
                        indx = glGetUniformLocation(retn->pshd[iter].prog,
                                                    puni[ctmp].name);
                        if (indx != -1)
                            retn->pshd[iter].cuni++;
                    }
                    retn->pshd[iter].puni =
                        (cuni)? calloc(retn->pshd[iter].cuni,
                                       sizeof(*retn->pshd[iter].puni)) : 0;
                    for (step = ctmp = 0; ctmp < cuni; ctmp++) {
                        indx = glGetUniformLocation(retn->pshd[iter].prog,
                                                    puni[ctmp].name);
                        if (indx != -1) {
                            retn->pshd[iter].puni[step] = puni[ctmp];
                            retn->pshd[iter].puni[step++].indx = indx;
                        }
                    }
                    glUseProgram(0);
                    continue;
                }
            }
            /** Only executed if it`s impossible to build the shader **/
            glUseProgram(0);
            glDeleteProgram(retn->pshd[iter].prog);
            retn->pshd[iter].prog = 0;
        }
        for (iter = 0; iter < cshd; iter++)
            if (shdr[iter] != (GLchar*)-1)
                free(shdr[iter]);
        free(shdr);
    }
    if (ctex) {
        retn->ctex = ctex;
        retn->ptex = calloc(retn->ctex, sizeof(*retn->ptex));
    }
    retn->elem = elem;
    retn->catr = catr;
    glGenBuffers(catr, retn->pbuf = calloc(catr, sizeof(*retn->pbuf)));
    retn->patr = calloc(catr, sizeof(*retn->patr));

    retn->cind = patr[0].cdat / sizeof(GLuint);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, retn->pbuf[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 patr[0].cdat, patr[0].pdat, patr[0].draw);

    for (iter = 1; iter < catr; iter++) {
        glBindBuffer(GL_ARRAY_BUFFER, retn->pbuf[iter]);
        glBufferData(GL_ARRAY_BUFFER,
                     patr[iter].cdat, patr[iter].pdat, patr[iter].draw);
    }
    for (indx = 0; indx < retn->cshd; indx++) {
        if (!retn->pshd[indx].prog)
            continue;
        glUseProgram(retn->pshd[indx].prog);
        for (iter = 1; iter < catr; iter++) {
            retn->patr[iter].aloc =
                (patr[iter].name)? glGetAttribLocation(retn->pshd[indx].prog,
                                                       patr[iter].name) : -1;
            switch (patr[iter].type) {
                case OGL_UNI_T1IV: case OGL_UNI_T1FV:
                    retn->patr[iter].ecnt = 1; break;
                case OGL_UNI_T2IV: case OGL_UNI_T2FV:
                    retn->patr[iter].ecnt = 2; break;
                case OGL_UNI_T3IV: case OGL_UNI_T3FV:
                    retn->patr[iter].ecnt = 3; break;
                case OGL_UNI_T4IV: case OGL_UNI_T4FV:
                    retn->patr[iter].ecnt = 4; break;
            }
            switch (patr[iter].type) {
                case OGL_UNI_T1IV: case OGL_UNI_T2IV:
                case OGL_UNI_T3IV: case OGL_UNI_T4IV:
                    retn->patr[iter].elem = GL_INT;   break;
                case OGL_UNI_T1FV: case OGL_UNI_T2FV:
                case OGL_UNI_T3FV: case OGL_UNI_T4FV:
                    retn->patr[iter].elem = GL_FLOAT; break;
            }
        }
        glUseProgram(0);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return (OGL_FVBO*)retn;
}



__attribute__((unused))
static GLvoid OGL_DrawVBO(OGL_FVBO *draw, GLuint shad) {
    typedef _OGL_FVBO __OGL_FVBO;

    __OGL_FVBO *vobj = (__OGL_FVBO*)draw;
    OGL_UNIF *unif;
    GLenum iter;

    if ((shad < vobj->cshd) && vobj->pshd[shad].prog) {
        glUseProgram(vobj->pshd[shad].prog);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vobj->pbuf[0]);
        for (iter = 1; iter < vobj->catr; iter++) {
            if (vobj->patr[iter].aloc == -1)
                continue;
            glBindBuffer(GL_ARRAY_BUFFER, vobj->pbuf[iter]);
            glVertexAttribPointer(vobj->patr[iter].aloc, vobj->patr[iter].ecnt,
                                  vobj->patr[iter].elem, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(vobj->patr[iter].aloc);
        }
        for (iter = 0; iter < vobj->ctex; iter++)
            OGL_BindTex((OGL_FVBO*)vobj, iter, OGL_TEX_DFLT);

        for (iter = 0; iter < vobj->pshd[shad].cuni; iter++)
            switch ((unif = &vobj->pshd[shad].puni[iter])->type
                          & ~OGL_UNI_IIII) {
                case OGL_UNI_TMFV:
                    glUniformMatrix4fv(unif->indx, 1, GL_TRUE,
                                      (unif->type & OGL_UNI_IIII)?
                                      (GLfloat*)unif->pdat :
                                     *(GLfloat**)unif->pdat);
                    continue;

                case OGL_UNI_T1IV:
                    glUniform1iv(unif->indx, 1, (unif->type & OGL_UNI_IIII)?
                                (GLint*)&unif->pdat : (GLint*)unif->pdat);
                    break;

                case OGL_UNI_T1FV:
                    glUniform1fv(unif->indx, 1, (unif->type & OGL_UNI_IIII)?
                                (GLfloat*)&unif->pdat : (GLfloat*)unif->pdat);
                    break;

                case OGL_UNI_T2IV:
                    glUniform2iv(unif->indx, 1, (unif->type & OGL_UNI_IIII)?
                                (GLint*)&unif->pdat : (GLint*)unif->pdat);
                    break;

                case OGL_UNI_T2FV:
                    glUniform2fv(unif->indx, 1, (unif->type & OGL_UNI_IIII)?
                                (GLfloat*)&unif->pdat : (GLfloat*)unif->pdat);
                    break;

                case OGL_UNI_T3IV:
                    glUniform3iv(unif->indx, 1, (unif->type & OGL_UNI_IIII)?
                                (GLint*)&unif->pdat : (GLint*)unif->pdat);
                    break;

                case OGL_UNI_T3FV:
                    glUniform3fv(unif->indx, 1, (unif->type & OGL_UNI_IIII)?
                                (GLfloat*)&unif->pdat : (GLfloat*)unif->pdat);
                    break;

                case OGL_UNI_T4IV:
                    glUniform4iv(unif->indx, 1, (unif->type & OGL_UNI_IIII)?
                                (GLint*)&unif->pdat : (GLint*)unif->pdat);
                    break;

                case OGL_UNI_T4FV:
                    glUniform4fv(unif->indx, 1, (unif->type & OGL_UNI_IIII)?
                                (GLfloat*)&unif->pdat : (GLfloat*)unif->pdat);
                    break;

                default: continue;
            }
        glDrawElements(vobj->elem, vobj->cind, GL_UNSIGNED_INT, 0);
        for (iter = 1; iter < vobj->catr; iter++)
            if (vobj->patr[iter].aloc != -1)
                glDisableVertexAttribArray(vobj->patr[iter].aloc);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glUseProgram(0);
    }
}



__attribute__((unused))
static GLvoid OGL_FreeVBO(OGL_FVBO **retn) {
    typedef _OGL_FVBO __OGL_FVBO;

    __OGL_FVBO **vobj = (__OGL_FVBO**)retn;

    if (vobj && *vobj) {
        while ((*vobj)->cshd)
            if ((*vobj)->pshd[--(*vobj)->cshd].prog) {
                glDeleteProgram((*vobj)->pshd[(*vobj)->cshd].prog);
                free((*vobj)->pshd[(*vobj)->cshd].puni);
            }
        glDeleteBuffers((*vobj)->catr, (*vobj)->pbuf);
        free((*vobj)->pbuf);
        free((*vobj)->patr);
        free((*vobj)->pshd);
        if ((*vobj)->ctex) {
            while ((*vobj)->ctex)
                OGL_FreeTex((OGL_FTEX**)&(*vobj)->ptex[--(*vobj)->ctex]);
            free((*vobj)->ptex);
        }
        free(*vobj);
        *vobj = 0;
    }
}

#undef _OGL_FVBO
#undef _OGL_FTEX

#ifdef __cplusplus
}
#endif

#endif /** OGL_LOAD_H **/
