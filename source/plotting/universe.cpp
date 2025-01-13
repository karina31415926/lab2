#include "plotting/plotter.h"

void Plotter::add_bodies_to_image(Universe& universe){
    // fill bitmap

    for(auto position : universe.positions){
        double position_x = position[0];
        double position_y = position[1];

        if(!plot_bounding_box.contains(position)){
            // body not within the plotted box
            continue;
        }

        // plot pixel
        mark_position(position, 255, 255, 255);
    }
}



