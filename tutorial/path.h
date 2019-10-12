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
#ifndef TUTORIAL_PATH_H
#define TUTORIAL_PATH_H

#include <agg_basics.h>
#include <agg_trans_affine.h>

/**
 * A very simple path that wraps around an array of doubles.
 */
class SimplePath
{
public:
    SimplePath(const double* points, int size)
    : m_points (points),
      m_size (size),
      m_pos (0)
    {
    }

    /**
     * Rewind getting the path vertices.
     *
     * @param[in]  path
     *             an optional value to indicate which path to be rewinded.
     */
    void rewind (int path = 0)
    {
        m_pos = 0;
    }

    /**
     * Get the next vertex.
     *
     * @param[out] x
     *             the vertex x coordinate.
     * @param[out] y
     *             the vertex y coordinate.
     * @return     the coordinate type.  See agg::path_commands_e.
     *             agg::path_cmd_stop indicates no more points available.
     */
    unsigned vertex(double* x, double* y)
    {
        if (m_pos < m_size)
        {
            *x = m_points[m_pos++];
            *y = m_points[m_pos++];
            return (m_pos == 2 ? agg::path_cmd_move_to : agg::path_cmd_line_to);
        }
        return agg::path_cmd_stop;
    }

private:
    const double *m_points;
    int           m_size;
    int           m_pos;
};

/**
 * A vertex with command info
 */
struct CmdVertex
{
    agg::path_commands_e    cmd;
    double  x;
    double  y;
};

/**
 * A very simple path that wraps around an array of CmdVertex.
 */
class CmdVertexPath
{
public:
    CmdVertexPath(const CmdVertex* vertices, int size)
    : m_vertices (vertices),
      m_size (size),
      m_pos (0)
    {
    }

    /**
     * Rewind getting the path vertices.
     *
     * @param[in]  path
     *             an optional value to indicate which path to be rewinded.
     */
    void rewind (int path = 0)
    {
        m_pos = 0;
    }

    /**
     * Get the next vertex.
     *
     * @param[out] x
     *             the vertex x coordinate.
     * @param[out] y
     *             the vertex y coordinate.
     * @return     the coordinate type.  See agg::path_commands_e.
     *             agg::path_cmd_stop indicates no more points available.
     */
    unsigned vertex(double* x, double* y)
    {
        if (m_pos < m_size)
        {
            *x = m_vertices[m_pos].x;
            *y = m_vertices[m_pos].y;
            agg::path_commands_e cmd = m_vertices[m_pos].cmd;

            ++m_pos;
            return cmd;
        }
        return agg::path_cmd_stop;
    }

    int size () const { return m_size; }

private:
    const CmdVertex *m_vertices;
    int m_size;
    int m_pos;
};

/**
 * A wrapper that calculates the new path point based on the transformation
 * matrix.
 */
template<class Path>
class TransformPath
{
public:
    TransformPath(Path& path, const agg::trans_affine& matrix)
    : m_path (path),
      m_matrix (matrix)
    {
    }

    void rewind (int path = 0)
    {
        m_path.rewind (path);
    }

    unsigned vertex(double* x, double* y)
    {
        unsigned cmd = m_path.vertex (x, y);
        if (cmd != agg::path_cmd_stop)
        {
            m_matrix.transform(x, y);
        }
        return cmd;
    }

private:
    Path    m_path;
    agg::trans_affine   m_matrix;
};


#endif  /* TUTORIAL_PATH_H */
