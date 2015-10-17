#ifndef __Window__
#define __Window__

#include "Types.h"

void Window_CreateOpenGLContext(struct Window *window, const GLchar *name);
void Window_SetOpenGLFlags();
void Window_ClearBuffers();
void Window_SwapBuffers();
void Window_Destroy();

#endif