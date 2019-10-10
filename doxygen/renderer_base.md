Renderer Base {#rendererbase}
=============================

AGG uses `agg::renderer_base` to handle most of operations.  It contains all
the information needed regarding the image metadata and buffering.

You will need the following parts before creating a agg::renderer_base.

* a raw image byte buffer.
* a agg::rendering_buffer that wraps around the byte buffer with width, height, and x-stride information.
* pixel format which specifies the pixel color format.


```cpp
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
```

Example
-------

See tutorial_rendererbase.cpp for an example of constructing an RGBA renderer_base. 
It also shows how to do direct pixel manipulations using renderer_base.  In this
example, a simple green border is drawn.  The output is saved to an PNG.

\image tutorial_rendererbase.png
