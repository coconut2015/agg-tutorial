Validating Values {#validate}
=============================

If AGG is to be used to draw images from user provided data,
some precautions need to be made, since AGG itself does not
validate the values passed to it.

Validation here refers to making sure various values are finite,
and in some cases within certain value ranges, or clip boundaries.
NaN or INF values can cause problems in a lot of calculations.
If such precautions are not done, one could encounter infinite
loops, very large memory allocations etc.

From my experience, the following steps need to be taken.

1. Validate all paths (VertexSource) points.
2. Validate agg::trans_affine values.
3. Create a wrapper class that does what agg::conv_transform
   does and validate the transformed values.
4. Override new and delete operators that validate the size
   request to avoid allocating excessive amount of memory.
