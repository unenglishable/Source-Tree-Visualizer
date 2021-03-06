/*
 * artist.h
 *
 *  Created on: Aug 12, 2010
 *      Author: Mark Christensen
 */

#ifndef ARTIST_H_
#define ARTIST_H_

#include "drawable_data.h"
#include <Magick++.h>
#include "../system/discursive_system.h"


using namespace Magick;

class Artist
{
public:
	virtual void draw(Image &image, DrawableData* dataset) = 0;
};

#endif /* ARTIST_H_ */
