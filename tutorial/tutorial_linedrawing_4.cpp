/*
 * Copyright (c) 2019 Heng Yuan
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
#include <agg_conv_stroke.h>
#include <agg_pixfmt_rgba.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_renderer_base.h>
#include <agg_renderer_scanline.h>
#include <agg_scanline_p.h>

#include "path.h"
#include "writepng.h"

typedef agg::pixfmt_rgba32  PixelFormat;
typedef agg::renderer_base<PixelFormat> RendererBaseType;

void
drawLine (RendererBaseType& rBase, double x1, double y1, double x2, double y2, double lineWidth, agg::rgba8 color)
{
    agg::rasterizer_scanline_aa<> ras;
    agg::scanline_p8 scanline;

    ras.auto_close(false);

    double linePoints[] = { x1, y1, x2, y2 };
    SimplePath path (linePoints, sizeof(linePoints) / sizeof(double));
    agg::conv_stroke<SimplePath> strokePath (path);

    strokePath.width(lineWidth);
    strokePath.line_cap(agg::butt_cap);
    strokePath.line_join(agg::miter_join);
    strokePath.miter_limit(lineWidth);

    ras.reset();
    ras.add_path(strokePath);

    agg::render_scanlines_aa_solid(ras, scanline, rBase, color);
}

int
main (int argc, const char* argv[])
{
    try
    {
        const int imageWidth = 100;
        const int imageHeight = 25;

        const int pixelSize = PixelFormat::pix_width;

        agg::rendering_buffer   renderBuffer;
        PixelFormat             pixFmt;
        RendererBaseType        rBase;

        unsigned char *imageBuffer = new unsigned char[imageWidth * imageHeight * pixelSize];

        // Hook everything up
        renderBuffer.attach (imageBuffer, imageWidth, imageHeight, imageWidth * pixelSize);
        pixFmt.attach(renderBuffer);
        rBase.attach(pixFmt);

        const agg::rgba8 whiteColor (0xff, 0xff, 0xff, 0xff);
        const agg::rgba8 blackColor (0, 0, 0, 0xff);

        // clear the buffer with transparent white color
        rBase.clear(whiteColor);

        // draw a bunch horizontal lines
        drawLine (rBase, 0, 5, 100, 5, 0.5, blackColor);
        drawLine (rBase, 0, 8.1, 100, 8.1, 0.5, blackColor);
        drawLine (rBase, 0, 11.3, 100, 11.3, 0.5, blackColor);
        drawLine (rBase, 0, 14.5, 100, 14.5, 0.5, blackColor);
        drawLine (rBase, 0, 17.7, 100, 17.7, 0.5, blackColor);

        // now write the image out for visualization
        char fileName[1000] = { 0 };
        if (argc > 1)
        {
            sprintf (fileName, "%s/", argv[1]);
        }
        strcat(fileName, "tutorial_linedrawing_4.png");
        writePng<RendererBaseType> (fileName, rBase);

        delete imageBuffer;
    }
    catch (TutorialException& ex)
    {
        printf ("%s\n", ex.getMessage());
        return 1;
    }
    catch (...)
    {
        printf ("Unknown exception detected.\n");
        return 1;
    }

    return 0;
}
