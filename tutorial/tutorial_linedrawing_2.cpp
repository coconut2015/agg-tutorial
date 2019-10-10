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
#include <agg_conv_curve.h>
#include <agg_conv_stroke.h>
#include <agg_pixfmt_rgba.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_renderer_base.h>
#include <agg_renderer_scanline.h>
#include <agg_scanline_p.h>

#include "path.h"
#include "writepng.h"

int
main (int argc, const char* argv[])
{
    try
    {
        const int imageWidth = 100;
        const int imageHeight = 100;

        typedef agg::pixfmt_rgba32  PixelFormat;
        typedef agg::renderer_base<PixelFormat> RendererBaseType;

        const int pixelSize = PixelFormat::pix_width;

        agg::rendering_buffer   renderBuffer;
        PixelFormat             pixFmt;
        RendererBaseType        rBase;

        // Allocate a 100 x 100 rgba buffer
        unsigned char *imageBuffer = new unsigned char[imageWidth * imageHeight * pixelSize];

        // Hook everything up
        renderBuffer.attach (imageBuffer, imageWidth, imageHeight, imageWidth * pixelSize);
        pixFmt.attach(renderBuffer);
        rBase.attach(pixFmt);

        const agg::rgba8 transparentWhiteColor (0xff, 0xff, 0xff, 0);
        const agg::rgba8 greenColor (0, 0xff, 0, 0xff);
        const agg::rgba8 redColor (0xff, 0, 0, 0xff);

        // clear the buffer with transparent white color
        rBase.clear(transparentWhiteColor);

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 scanline;

        ras.auto_close(false);

        CmdVertex dome[] =
        {
           { agg::path_cmd_move_to, 10, 90 },
           { agg::path_cmd_curve3,  50, 10 },
           { agg::path_cmd_curve3,  90, 90 },
        };

        CmdVertexPath path (dome, sizeof(dome) / sizeof(CmdVertex));

        // draw a green caret
        {
            // without conv_curve, path_cmd_curve3 are treated as
            // path_cmd_line_to
            //
            // so we will see a green caret
            agg::conv_stroke<CmdVertexPath> strokePath (path);

            double strokeWidth = 1.0;
            strokePath.width(strokeWidth);
            strokePath.line_cap(agg::square_cap);
            strokePath.line_join(agg::miter_join);
            strokePath.miter_limit(strokeWidth);

            ras.reset();
            ras.add_path(strokePath);

            agg::render_scanlines_aa_solid(ras, scanline, rBase, greenColor);
        }

        // draw a red dome
        {
            // generate a curved path
            agg::conv_curve<CmdVertexPath> curvePath (path);
            // and then use this curved path to generate the stroke path
            agg::conv_stroke<agg::conv_curve<CmdVertexPath>> strokePath (curvePath);

            double strokeWidth = 2.0;
            strokePath.width(strokeWidth);
            strokePath.line_cap(agg::square_cap);
            strokePath.line_join(agg::miter_join);
            strokePath.miter_limit(strokeWidth);

            ras.reset();
            ras.add_path(strokePath);

            agg::render_scanlines_aa_solid(ras, scanline, rBase, redColor);
        }

        // now write the image out for visualization
        char fileName[1000] = { 0 };
        if (argc > 1)
        {
            sprintf (fileName, "%s/", argv[1]);
        }
        strcat(fileName, "tutorial_linedrawing_2.png");
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
