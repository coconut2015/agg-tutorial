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
        const agg::rgba8 redColor (0xff, 0, 0, 0xff);

        // clear the buffer with transparent white color
        rBase.clear(transparentWhiteColor);

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 scanline;

        ras.auto_close(false);

        // draw a red star
        {
            double star[] =
            {
                 50,  3,
                 20, 97,
                 95, 37,
                  5, 37,
                 80, 97,
                 50,  3     // close the polygon
            };

            SimplePath path (star, sizeof(star) / sizeof(double));

            ras.reset();
            ras.filling_rule(agg::fill_even_odd);
            ras.add_path(path);

            agg::render_scanlines_aa_solid(ras, scanline, rBase, redColor);
        }

        // now write the image out for visualization
        char fileName[1000] = { 0 };
        if (argc > 1)
        {
            sprintf (fileName, "%s/", argv[1]);
        }
        strcat(fileName, "tutorial_path_2.png");
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
