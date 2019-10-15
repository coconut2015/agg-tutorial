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
#include <agg_gamma_functions.h>
#include <agg_gamma_lut.h>
#include <agg_gradient_lut.h>
#include <agg_pixfmt_rgba.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_renderer_base.h>
#include <agg_renderer_scanline.h>
#include <agg_scanline_p.h>
#include <agg_span_allocator.h>
#include <agg_span_interpolator_linear.h>
#include <agg_span_gradient.h>

#include "path.h"
#include "writepng.h"

typedef agg::pixfmt_rgba32  PixelFormat;
typedef agg::renderer_base<PixelFormat> RendererBaseType;

template<class GradientFunctionType>
void
drawGradient (GradientFunctionType& gradientFunction,
              RendererBaseType& rBase,
              SimplePath& path,
              const agg::trans_affine& matrix)
{
    typedef agg::gradient_linear_color<agg::rgba8> ColorFunctionType;

    typedef agg::span_interpolator_linear<agg::trans_affine> InterpolatorType;
    typedef agg::span_allocator<agg::rgba8> SpanAllocatorType;
    typedef agg::span_gradient<agg::rgba8,
                               InterpolatorType,
                               GradientFunctionType,
                               ColorFunctionType> SpanGradientType;

    agg::trans_affine invertMatrix (matrix);
    invertMatrix.invert();
    InterpolatorType interpolator (invertMatrix);

    ColorFunctionType colorFunction (agg::rgba8 (0, 0, 0, 0xff),
                                     agg::rgba8 (0xff, 0xff, 0xff, 0xff),
                                     100);

    SpanGradientType sg (interpolator, gradientFunction, colorFunction, 0.0, 50.0);
    SpanAllocatorType sa;

    agg::rasterizer_scanline_aa<> ras;
    agg::scanline_p8 scanline;

    ras.auto_close(false);
    ras.reset ();
    TransformPath<SimplePath> tp (path, matrix);
    ras.add_path(tp);
    agg::render_scanlines_aa(ras, scanline, rBase, sa, sg);
}

int
main (int argc, const char* argv[])
{
    try
    {
        const int imageWidth = 600;
        const int imageHeight = 300;

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

        const agg::rgba8 transparentwhiteColor (0xff, 0xff, 0xff, 0);
        const agg::rgba8 blueColor (0, 0, 0xff, 0xff);

        // clear the buffer with transparent white color
        rBase.clear(transparentwhiteColor);

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 scanline;

        ras.auto_close(false);

        double rect[] =
        {
            0.0, 0.0,
            100.0, 0.0,
            100.0, 100.0,
            0.0, 100.0,
            0.0, 0.0
        };
        SimplePath path (rect, sizeof(rect) / sizeof(double));

        //////////////////////////////////////////////////////////////////
        // basic gradient functions
        //////////////////////////////////////////////////////////////////
        double x = 0;
        double y = 0;

        // x gradient
        {
            agg::gradient_x gradientFunction;
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gradientFunction,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // y gradient
        {
            agg::gradient_y gradientFunction;
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gradientFunction,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // diamond gradient
        {
            agg::gradient_diamond gradientFunction;
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gradientFunction,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // xy gradient
        {
            agg::gradient_xy gradientFunction;
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gradientFunction,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // sqrt xy gradient
        {
            agg::gradient_sqrt_xy gradientFunction;
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gradientFunction,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // conic gradient
        {
            agg::gradient_conic gradientFunction;
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gradientFunction,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        //////////////////////////////////////////////////////////////////
        // repeat adapter gradient functions
        //////////////////////////////////////////////////////////////////
        x = 0;
        y = 100;

        // x gradient
        {
            agg::gradient_x gradientFunction;
            agg::gradient_repeat_adaptor<agg::gradient_x> gf2 (gradientFunction);
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gf2,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // y gradient
        {
            agg::gradient_y gradientFunction;
            agg::gradient_repeat_adaptor<agg::gradient_y> gf2 (gradientFunction);
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gf2,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // diamond gradient
        {
            agg::gradient_diamond gradientFunction;
            agg::gradient_repeat_adaptor<agg::gradient_diamond> gf2 (gradientFunction);
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gf2,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // xy gradient
        {
            agg::gradient_xy gradientFunction;
            agg::gradient_repeat_adaptor<agg::gradient_xy> gf2 (gradientFunction);
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gf2,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // sqrt xy gradient
        {
            agg::gradient_sqrt_xy gradientFunction;
            agg::gradient_repeat_adaptor<agg::gradient_sqrt_xy> gf2 (gradientFunction);
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gf2,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // conic gradient
        {
            agg::gradient_conic gradientFunction;
            agg::gradient_repeat_adaptor<agg::gradient_conic> gf2 (gradientFunction);
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gf2,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        //////////////////////////////////////////////////////////////////
        // reflective adapter gradient functions
        //////////////////////////////////////////////////////////////////
        x = 0;
        y += 100;

        // x gradient
        {
            agg::gradient_x gradientFunction;
            agg::gradient_reflect_adaptor<agg::gradient_x> gf2 (gradientFunction);
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gf2,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // y gradient
        {
            agg::gradient_y gradientFunction;
            agg::gradient_reflect_adaptor<agg::gradient_y> gf2 (gradientFunction);
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gf2,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // diamond gradient
        {
            agg::gradient_diamond gradientFunction;
            agg::gradient_reflect_adaptor<agg::gradient_diamond> gf2 (gradientFunction);
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gf2,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // xy gradient
        {
            agg::gradient_xy gradientFunction;
            agg::gradient_reflect_adaptor<agg::gradient_xy> gf2 (gradientFunction);
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gf2,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // sqrt xy gradient
        {
            agg::gradient_sqrt_xy gradientFunction;
            agg::gradient_reflect_adaptor<agg::gradient_sqrt_xy> gf2 (gradientFunction);
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gf2,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // conic gradient
        {
            agg::gradient_conic gradientFunction;
            agg::gradient_reflect_adaptor<agg::gradient_conic> gf2 (gradientFunction);
            agg::trans_affine matrix;
            matrix.translate(x, y);
            drawGradient (gf2,
                          rBase,
                          path,
                          matrix);
            x += 100;
        }

        // now write the image out for visualization
        char fileName[1000] = { 0 };
        if (argc > 1)
        {
            sprintf (fileName, "%s/", argv[1]);
        }
        strcat(fileName, "tutorial_gradient_1.png");
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
