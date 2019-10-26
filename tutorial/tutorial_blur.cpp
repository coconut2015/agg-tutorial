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
#include <cstring>
#include <agg_color_conv.h>
#include <agg_conv_stroke.h>
#include <agg_conv_transform.h>
#include <agg_ellipse.h>
#include <agg_pixfmt_rgba.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_renderer_base.h>
#include <agg_renderer_scanline.h>
#include <agg_scanline_p.h>
#include <agg_span_allocator.h>
#include <agg_span_interpolator_linear.h>
#include <agg_span_gradient.h>
#include <agg_blur.h>

#include "path.h"
#include "writepng.h"

typedef agg::pixfmt_rgba32  PixelFormat;
typedef agg::renderer_base<PixelFormat> RendererBaseType;

template<class SrcPixelFormat>
void
drawIcon(SrcPixelFormat& pixFmt, int x, int y, int width, int height)
{
    agg::rendering_buffer   renBuffer;
    SrcPixelFormat dstPixFmt (renBuffer);
    dstPixFmt.attach (pixFmt, x, y , x + width - 1, y + height - 1);
    agg::renderer_base<SrcPixelFormat>  rBase (dstPixFmt);

    agg::rasterizer_scanline_aa<> ras;
    ras.auto_close(false);
    agg::scanline_p8 scanline;

    // draw a purple circle
    {
        agg::ellipse circle (100, 100, 70, 70);

        agg::conv_stroke<agg::ellipse> strokePath (circle);

        double strokeWidth = 10.0;
        strokePath.width(strokeWidth);
        strokePath.line_cap(agg::butt_cap);
        strokePath.line_join(agg::miter_join);
        strokePath.miter_limit(strokeWidth);

        ras.reset();
        ras.add_path(strokePath);
        const agg::rgba8 purpleColor (0xee, 0, 0xcc, 0xcc);
        agg::render_scanlines_aa_solid(ras, scanline, rBase, purpleColor);
    }

    // draw a gradient button
    {
        typedef agg::gradient_linear_color<agg::rgba8> ColorFunctionType;
        typedef agg::gradient_x GradientFunctionType;

        typedef agg::span_interpolator_linear<agg::trans_affine> InterpolatorType;
        typedef agg::span_allocator<agg::rgba8> SpanAllocatorType;
        typedef agg::span_gradient<agg::rgba8,
                                   InterpolatorType,
                                   GradientFunctionType,
                                   ColorFunctionType> SpanGradientType;

        agg::ellipse circle (30, 30, 30, 30);

        GradientFunctionType gradientFunction;
        agg::trans_affine matrix;
        matrix.translate (70, 70);

        agg::trans_affine invertMatrix (matrix);
        invertMatrix.invert();
        InterpolatorType interpolator (invertMatrix);

        ColorFunctionType colorFunction (agg::rgba8 (0xff, 0xd7, 0, 0xff),
                                         agg::rgba8 (0xff, 0, 0, 0xff),
                                         60);

        SpanGradientType sg (interpolator, gradientFunction, colorFunction, 0.0, 60.0);
        SpanAllocatorType sa;

        agg::conv_transform<agg::ellipse> tp(circle, matrix);
        ras.reset ();
        ras.add_path(tp);

        agg::render_scanlines_aa(ras, scanline, rBase, sa, sg);
    }
}

template<class PixFormat, class BlurType>
void
blurImage(PixFormat& pixFmt, int x, int y, int width, int height, BlurType& blur, double radius)
{
    agg::rendering_buffer   renBuffer;
    PixFormat subPixFmt (renBuffer);
    subPixFmt.attach (pixFmt, x, y , x + width - 1, y + height - 1);
    blur.blur (subPixFmt, radius);
}

int
main (int argc, const char* argv[])
{
    try
    {
        const int imageWidth = 800;
        const int imageHeight = 200;

        agg::rasterizer_scanline_aa<> ras;
        ras.auto_close(false);

        agg::scanline_p8 scanline;

        const int pixelSize = PixelFormat::pix_width;

        const double PI = 3.14159265358979;

        unsigned char *imageBuffer = new unsigned char[imageWidth * imageHeight * pixelSize];
        agg::rendering_buffer   renderBuffer (imageBuffer, imageWidth, imageHeight, imageWidth * pixelSize);
        PixelFormat             pixFmt (renderBuffer);
        RendererBaseType        rBase (pixFmt);

        const agg::rgba8 whiteColor (0xff, 0xff, 0xff, 0xff);
        rBase.clear(whiteColor);

        {
            drawIcon (pixFmt, 0, 0, 200, 200);

            {
                drawIcon (pixFmt, 200, 0, 200, 200);
                agg::recursive_blur<agg::rgba8, agg::recursive_blur_calc_rgba<> > blur;
                blurImage (pixFmt, 200, 0, 200, 200, blur, 10);
            }

            {
                drawIcon (pixFmt, 400, 0, 200, 200);
                agg::stack_blur<agg::rgba8, agg::stack_blur_calc_rgba<> > blur;
                blurImage (pixFmt, 400, 0, 200, 200, blur, 10);
            }

            {
                drawIcon (pixFmt, 600 + 5, 0 + 5, 200, 200);
                agg::recursive_blur<agg::rgba8, agg::recursive_blur_calc_rgba<> > blur;
                blurImage (pixFmt, 600 + 5, 0 + 5, 200, 200, blur, 10);

                drawIcon (pixFmt, 600, 0, 200, 200);
            }
        }

        // now write the image out for visualization
        char fileName[1000] = { 0 };
        if (argc > 1)
        {
            sprintf (fileName, "%s/", argv[1]);
        }
        strcat(fileName, "tutorial_blur.png");
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
