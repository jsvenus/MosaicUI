/*
---------------------------------------------------------------------------------
 
                                               oooo
                                               `888
                oooo d8b  .ooooo.  oooo    ooo  888  oooo  oooo
                `888""8P d88' `88b  `88b..8P'   888  `888  `888
                 888     888   888    Y888'     888   888   888
                 888     888   888  .o8"'88b    888   888   888
                d888b    `Y8bod8P' o88'   888o o888o  `V88V"V8P'
 
                                                  www.roxlu.com
                                             www.apollomedia.nl
                                          www.twitter.com/roxlu
 
---------------------------------------------------------------------------------
*/

#ifndef TOPSHOP_H
#define TOPSHOP_H

#include <mosaic/Mosaic.h>
#include <grid/Grid.h>
#include <topshop/ImageCollector.h>
#include <topshop/ImageProcessor.h>
#include <tracking/Tracking.h>

#define USE_GRID 1

namespace top {
  
  class TopShop {
  public:
    TopShop();
    ~TopShop();
    int init();
    int shutdown();
    void update();
    void draw();

  public:
    mos::Mosaic mosaic;

#if USE_GRID
    grid::Grid left_grid;
    grid::Grid right_grid;
#endif

    ImageCollector img_collector;                       /* watches a directory for new files */
    //track::Tracking tracking;                         /* responsible for the interactive/tracking part of the mosaic. */
  };

} /* namespace top */

#endif