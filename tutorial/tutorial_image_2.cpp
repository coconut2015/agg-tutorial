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
#include <agg_ellipse.h>
#include <agg_image_accessors.h>
#include <agg_path_storage.h>
#include <agg_pixfmt_rgba.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_renderer_base.h>
#include <agg_renderer_scanline.h>
#include <agg_scanline_p.h>
#include <agg_span_allocator.h>
#include <agg_span_image_filter_rgba.h>
#include <agg_span_interpolator_linear.h>

#include "path.h"
#include "writepng.h"

typedef agg::pixfmt_rgba32  PixelFormat;
typedef agg::renderer_base<PixelFormat> RendererBaseType;

template<class Path>
void
drawImage(RendererBaseType& dst, RendererBaseType& src, const agg::trans_affine& matrix, Path& path)
{
    typedef agg::span_interpolator_linear<agg::trans_affine> InterpolatorType;
    typedef agg::image_accessor_clip<PixelFormat> ImageAccessorType;
    typedef agg::span_image_filter_rgba_nn<ImageAccessorType, InterpolatorType> SpanGeneratorType;
    typedef agg::span_allocator<agg::rgba8> SpanAllocatorType;

    const agg::rgba8 transparentWhiteColor (0xff, 0xff, 0xff, 0);

    double srcWidth = src.width();
    double srcHeight = src.height();

    // from the matrix, compute the invert matrix.  It will
    // be the lookup of src pixels from the dst location.
    agg::trans_affine invertMatrix (matrix);
    invertMatrix.invert();

    InterpolatorType interpolator (invertMatrix);
    ImageAccessorType ia (src.ren(), transparentWhiteColor);
    SpanGeneratorType sg (ia, interpolator);

    agg::rasterizer_scanline_aa<> ras;
    agg::scanline_p8 scanline;
    SpanAllocatorType sa;

    ras.reset();
    ras.add_path(path);
    agg::render_scanlines_aa(ras, scanline, dst, sa, sg);
}

int
main (int argc, const char* argv[])
{
    try
    {
        const int iconWidth = 200;
        const int iconHeight = 200;
        const int imageWidth = 800;
        const int imageHeight = 200;

        const agg::rgba8 transparentWhiteColor (0xff, 0xff, 0xff, 0);
        const agg::rgba8 whiteColor (0xff, 0xff, 0xff, 0xff);

        agg::rasterizer_scanline_aa<> ras;
        ras.auto_close(false);

        agg::scanline_p8 scanline;

        const int pixelSize = PixelFormat::pix_width;

        agg::rendering_buffer   iconRenderBuffer;
        PixelFormat             iconPixFmt;
        RendererBaseType        iconRBase;

        const double PI = 3.14159265358979;
        // Allocate a 50 x 50 rgba buffer
        unsigned char *iconImageBuffer = new unsigned char[iconWidth * iconHeight * pixelSize];

        /////////////////////////////////////////////////////
        // Draw a simple color wheel icon
        /////////////////////////////////////////////////////
        {
            iconRenderBuffer.attach (iconImageBuffer, iconWidth, iconHeight, iconWidth * pixelSize);
            iconPixFmt.attach(iconRenderBuffer);
            iconRBase.attach(iconPixFmt);

            iconRBase.clear(transparentWhiteColor);

            // draw a big white circle
            {
                agg::ellipse circle (100, 100, 95, 95);
                ras.reset();
                ras.add_path(circle);
                agg::render_scanlines_aa_solid(ras, scanline, iconRBase, whiteColor);
            }

            // draw a small color wheel
            double triangle[] =
            {
                 0,   0,
               -20, -90,
                20, -90,
                 0,   0
            };
            agg::rgba8 colors[] =
            {
               { 0xfe, 0xfe, 0x33, 0xff },
               { 0xfb, 0x99, 0x02, 0xff },
               { 0xfa, 0xbc, 0x02, 0xff },
               { 0xfe, 0x27, 0x12, 0xff },
               { 0xfd, 0x53, 0x08, 0xff },
               { 0x86, 0x01, 0xaf, 0xff },
               { 0xa7, 0x19, 0x4b, 0xff },
               { 0x02, 0x47, 0xfe, 0xff },
               { 0x3d, 0x01, 0xa4, 0xff },
               { 0x66, 0xb0, 0x32, 0xff },
               { 0x03, 0x91, 0xce, 0xff },
               { 0xd0, 0xea, 0x2b, 0xff }
            };
            SimplePath path (triangle, sizeof(triangle) / sizeof(double));

            int numRotations = sizeof(colors) / sizeof(agg::rgba8);
            for (int i = 0; i < numRotations; ++i)
            {
                agg::trans_affine matrix;
                matrix.rotate( PI * 2 * i / numRotations);
                matrix.translate(100, 100);
                TransformPath<SimplePath> tp (path, matrix);

                ras.reset();
                ras.add_path(tp);

                agg::render_scanlines_aa_solid(ras, scanline, iconRBase, colors[i]);
            }

            // draw a small white circle
            {
                agg::ellipse innerCircle (100, 100, 40, 40);
                ras.reset();
                ras.add_path(innerCircle);
                agg::render_scanlines_aa_solid(ras, scanline, iconRBase, whiteColor);
            }

            // now write the icon out for visualization
            char fileName[1000] = { 0 };
            if (argc > 1)
            {
                sprintf (fileName, "%s/", argv[1]);
            }
            strcat(fileName, "tutorial_image_icon.png");
            writePng<RendererBaseType> (fileName, iconRBase);
        }

        agg::rendering_buffer   renderBuffer;
        PixelFormat             pixFmt;
        RendererBaseType        rBase;

        // Allocate a 100 x 100 rgba buffer
        unsigned char *imageBuffer = new unsigned char[imageWidth * imageHeight * pixelSize];

        // Hook everything up
        renderBuffer.attach (imageBuffer, imageWidth, imageHeight, imageWidth * pixelSize);
        pixFmt.attach(renderBuffer);
        rBase.attach(pixFmt);

        const agg::rgba8 greenColor (0, 0xff, 0, 0xff);

        // clear the buffer with green color
        rBase.clear(greenColor);

        /////////////////////////////////////////////////////
        // Stamp the image icon
        /////////////////////////////////////////////////////
        {
            double rect[] =
            {
                  0,   0,
                  0, 200,
                200, 200,
                200,   0,
                  0,   0
            };
            // draw the image as is at left
            {
                SimplePath rectPath (rect, sizeof(rect) / sizeof(double));
                agg::trans_affine matrix;
                drawImage (rBase, iconRBase, matrix, rectPath);
            }

            // create a donut shape
            agg::path_storage path;
            {
                agg::ellipse outerRing (100, 100, 100, 100);
                path.concat_path(outerRing);
                path.arrange_orientations(0, agg::path_flags_cw);

                agg::path_storage tmpPath;
                agg::ellipse innerRing (100, 100, 35, 35);
                tmpPath.concat_path(innerRing);
                tmpPath.arrange_orientations(0, agg::path_flags_ccw);
                path.concat_path(tmpPath);
            }

            // draw the icon image with a hole in the middle
            {
                agg::trans_affine matrix;
                matrix.translate(200, 0);
                TransformPath<agg::path_storage> tp (path, matrix);
                drawImage (rBase, iconRBase, matrix, tp);
            }

            // draw a sheared donut
            {
                double shx = -2.0;
                agg::trans_affine matrix(1.0, 0.0, shx, 1.0, 0.0, 0.0);
                matrix.translate(700, 0);
                TransformPath<agg::path_storage> tp (path, matrix);
                drawImage (rBase, iconRBase, matrix, tp);
            }
        }

        // now write the image out for visualization
        char fileName[1000] = { 0 };
        if (argc > 1)
        {
            sprintf (fileName, "%s/", argv[1]);
        }
        strcat(fileName, "tutorial_image_2.png");
        writePng<RendererBaseType> (fileName, rBase);

        delete iconImageBuffer;
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
