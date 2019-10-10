/*
 * Copyright (c) 2018 Heng Yuan
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TUTORIAL_WRITE_PNG_H
#define TUTORIAL_WRITE_PNG_H

#include "exception.h"

#include <cstdio>
#include <png.h>    // requires libpng on Linux

#include <agg_color_rgba.h>

template<class T>
int
getPngColorType () { throw TutorialException ("Cannot determine color type"); }

template<>
int
getPngColorType<agg::rgba8> () { return PNG_COLOR_TYPE_RGBA; }

/**
 * This is a simple utility function that saves AGG generated image to
 * a PNG file.
 *
 * @param   fileName
 *          the output file name.
 * @param   rBase
 *          the AGG renderer base.
 */
template<class RendererBaseType>
void
writePng (const char* fileName, const RendererBaseType& rBase)
{
    FILE*       outFile;
    png_structp pngPtr;
    png_infop   infoPtr;

    outFile = fopen (fileName, "wb");
    if (outFile == nullptr)
    {
        throw TutorialException ("Unable to write to %s", fileName);
    }

    pngPtr = png_create_write_struct (PNG_LIBPNG_VER_STRING,
                                      nullptr,
                                      nullptr,
                                      nullptr);
    ASSERT(pngPtr != nullptr);

    infoPtr = png_create_info_struct(pngPtr);
    ASSERT(infoPtr != nullptr);

    if (setjmp(png_jmpbuf(pngPtr)))
    {
        ASSERT(false);
    }

    png_init_io(pngPtr, outFile);

    int pngColorType = getPngColorType<typename RendererBaseType::color_type> ();
    const int width = rBase.width ();
    const int height = rBase.height ();
    const int valueSize = sizeof(typename RendererBaseType::color_type::value_type);

    png_set_IHDR(pngPtr,
                 infoPtr,
                 width,
                 height,
                 valueSize * 8,
                 getPngColorType<typename RendererBaseType::color_type> (),
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    png_write_info(pngPtr, infoPtr);

    png_bytep *rows = new png_bytep[height];
    for (int y = 0; y < height; ++y)
    {
        rows[y] = (png_bytep)rBase.ren ().pix_ptr (0, y);
    }

    png_write_image(pngPtr, rows);
    delete rows;

    png_write_end(pngPtr, NULL);
    fclose(outFile);
    png_destroy_write_struct(&pngPtr, &infoPtr);
}

#endif // TUTORIAL_WRITE_PNG_H
