#ifndef MARCO_X
#define MARCO_X

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#ifdef __cplusplus
extern "C"
{
#endif
	void CloseXWindows();
	void ShowLine(void *line, int line_len, int position);
	void SetupXWindows(int w, int h, int setup_color, char *display_name,
					   const char *window_title);

	void HXI(int *px, int *py, int *dim, int *done);

	int Black();
	int White();

#ifdef __cplusplus
}
#endif
#endif
