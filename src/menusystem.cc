/** \file menusystem.cc
   A very naive menusystem for the various settings screens
*/
/*
   Copyright (C) 2003-2004  Mathias Broxvall

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "menusystem.h"

#include "font.h"
#include "glHelp.h"

#include <SDL2/SDL_mouse.h>

#define MAX_MENUS 256

typedef struct sSelectionArea {
  int x0, y0, x1, y1;
  int returnCode;
} SelectionArea;

int focusArea = -1;
int nSelectionAreas = 0;
SelectionArea selectionAreas[MAX_MENUS];

void clearSelectionAreas() { nSelectionAreas = 0; }
void addArea(int code, int x, int y, int w, int h) {
  int thisArea = nSelectionAreas++;
  SelectionArea *sarea = &selectionAreas[thisArea];
  sarea->x0 = x;
  sarea->x1 = x + w;
  sarea->y0 = y;
  sarea->y1 = y + h;
  sarea->returnCode = code;
}

void addText_Left(int code, int fontSize, int y0, const char *leftStr, int leftX) {
  if (nSelectionAreas >= MAX_MENUS) { error("too many menus active"); }
  int thisArea = nSelectionAreas++;
  const GLfloat *color = ((code && getSelectedArea() == code) || (thisArea == focusArea))
                             ? menuColorSelected
                             : menuColor;
  int width = Font::drawSimpleText(leftStr, leftX, y0, fontSize, color[0], color[1], color[2],
                                   color[3]);
  SelectionArea *sarea = &selectionAreas[thisArea];
  sarea->x0 = leftX;
  sarea->x1 = leftX + width;
  sarea->y0 = y0 - fontSize;
  sarea->y1 = y0 + fontSize;
  sarea->returnCode = code;
}
void addText_Right(int code, int fontSize, int y0, const char *rightStr, int rightX) {
  if (nSelectionAreas >= MAX_MENUS) { error("too many menus active"); }
  int thisArea = nSelectionAreas++;
  const GLfloat *color = ((code && getSelectedArea() == code) || (thisArea == focusArea))
                             ? menuColorSelected
                             : menuColor;
  int width = Font::drawRightSimpleText(rightStr, rightX, y0, fontSize, color[0], color[1],
                                        color[2], color[3]);
  SelectionArea *sarea = &selectionAreas[thisArea];
  sarea->x0 = rightX - width;
  sarea->x1 = rightX;
  sarea->y0 = y0 - fontSize;
  sarea->y1 = y0 + fontSize;
  sarea->returnCode = code;
}
void addText_Center(int code, int fontSize, int y0, const char *str, int cx) {
  if (nSelectionAreas >= MAX_MENUS) { error("too many menus active"); }
  int thisArea = nSelectionAreas++;
  const GLfloat *color = ((code && getSelectedArea() == code) || (thisArea == focusArea))
                             ? menuColorSelected
                             : menuColor;
  int width = Font::drawCenterSimpleText(str, cx, y0, fontSize, color[0], color[1], color[2],
                                         color[3]);
  SelectionArea *sarea = &selectionAreas[thisArea];
  sarea->x0 = cx - width / 2;
  sarea->x1 = cx + width / 2;
  sarea->y0 = y0 - fontSize;
  sarea->y1 = y0 + fontSize;
  sarea->returnCode = code;
}
void addText_LeftRight(int code, int fontSize, int y0, const char *leftStr, int leftX,
                       const char *rightStr, int rightX) {
  if (nSelectionAreas >= MAX_MENUS) { error("too many menus active"); }
  int thisArea = nSelectionAreas++;
  const GLfloat *color = ((code && getSelectedArea() == code) || (thisArea == focusArea))
                             ? menuColorSelected
                             : menuColor;
  Font::drawSimpleText(leftStr, leftX, y0, fontSize, color[0], color[1], color[2], color[3]);
  Font::drawRightSimpleText(rightStr, rightX, y0, fontSize, color[0], color[1], color[2],
                            color[3]);
  SelectionArea *sarea = &selectionAreas[thisArea];
  sarea->x0 = leftX;
  sarea->x1 = rightX;
  sarea->y0 = y0 - fontSize;
  sarea->y1 = y0 + fontSize;
  sarea->returnCode = code;
}

int getSelectedArea() {
  int mouseX, mouseY, i;
  SDL_GetMouseState(&mouseX, &mouseY);
  for (i = 0; i < nSelectionAreas; i++) {
    if (mouseX >= selectionAreas[i].x0 && mouseX <= selectionAreas[i].x1 &&
        mouseY >= selectionAreas[i].y0 && mouseY <= selectionAreas[i].y1)
      return selectionAreas[i].returnCode;
    if (focusArea == i) return selectionAreas[i].returnCode;
  }
  return 0;
}

int fontsize, screenBorder, top, menuSpacing;

void computeScreenSizes() {
  if (screenWidth <= 640) {
    fontsize = 18;
    screenBorder = 50;
    menuSpacing = fontsize + 10;
    top = 150;
  } else if (screenWidth == 800) {
    fontsize = 20;
    screenBorder = 75;
    menuSpacing = fontsize + 14;
    top = 180;
  } else if (screenWidth == 1024) {
    fontsize = 24;
    screenBorder = 100;
    menuSpacing = fontsize + 18;
    top = 200;
  } else if (screenWidth >= 1280) {
    fontsize = 28;
    screenBorder = 150;
    menuSpacing = fontsize + 20;
    top = 200;
  }
}

void menuItem_Left(int code, int row, const char *leftStr) {
  computeScreenSizes();
  addText_Left(code, fontsize, (row + 1) * menuSpacing + top, leftStr, screenBorder);
}
void menuItem_Center(int code, int row, const char *str) {
  computeScreenSizes();
  addText_Center(code, fontsize, (row + 1) * menuSpacing + top, str, screenWidth / 2);
}
void menuItem_LeftRight(int code, int row, int indent, const char *leftStr,
                        const char *rightStr) {
  computeScreenSizes();
  addText_LeftRight(code, fontsize, (row + 1) * menuSpacing + top, leftStr,
                    screenBorder + 3 * fontsize * indent / 2, rightStr,
                    screenWidth - screenBorder);
}

void clearKeyboardFocus() { focusArea = -1; }

void moveKeyboardFocus(int reverse) {
  for (int i = 0; i < nSelectionAreas + 1; i++) {
    focusArea += reverse ? -1 : 1;
    if (focusArea < -1) focusArea = nSelectionAreas - 1;
    if (focusArea >= nSelectionAreas) focusArea = -1;
    /* Skip over regions without a return code */
    if (focusArea < 0 || selectionAreas[focusArea].returnCode != 0) return;
  }
}
