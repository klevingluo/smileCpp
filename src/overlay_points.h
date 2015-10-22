// Copyright (C) 2014  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.

#include "./dlib/image_processing/full_object_detection.h"
#include "./dlib/gui_widgets.h"
#include <vector>

namespace dlib
{
    inline std::vector<image_window::overlay_circle> overlay_points (
        const std::vector<full_object_detection>& dets,
        const rgb_pixel color = rgb_pixel(0,255,0)
    )
    {
        std::vector<image_window::overlay_circle> circles;
        const full_object_detection& d = dets[0];
        for (unsigned long i = 0; i < dets.size(); ++i) {
          circles.push_back(image_window::overlay_circle(d.part(i), 2, color));
        }
        return circles;
    }

// ----------------------------------------------------------------------------------------

    inline std::vector<image_window::overlay_circle> overlay_points (
        const full_object_detection& det,
        const rgb_pixel color = rgb_pixel(0,255,0)
    )
    {
        std::vector<full_object_detection> dets;
        dets.push_back(det);
        return overlay_points(dets, color);
    }

// ----------------------------------------------------------------------------------------

}
