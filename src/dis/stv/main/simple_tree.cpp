/*
 * simple_tree.cpp
 *
 *  Created on: Aug 12, 2010
 *      Author: programmer
 */

#include "../system/discursive_system.h"
#include "./configuration_agent.h"
#include "./repository_access.h"
#include <Magick++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../dec/decorator_factory.h"
#include "../gen/digitizer_factory.h"
#include "../transform/transform_factory.h"
#include "../model/surrogate_tree_node.h"
#include "../dec/spatial_displacement.h"
#include "../draw/scanline_artist.h"
#include "../draw/trapezoid_artist.h"
#include "../gen/space_colonizer.h"
#include "../transform/transformer.h"


using namespace Magick;
DiscursiveDebugAgent debugAgent;

int main(int argc, char **argv)
{
	// initialize libmagick
	InitializeMagick(*argv);

	// initialize our timing class
	DiscursiveTime timerAgent("Entire SourceTreeVis Program");

	// initialize our configuration agent
	ConfigurationAgent configAgent;

	// parse our config file and also pre-check command line
	configAgent.parseConfigFile();

	// parse our command line
	configAgent.parseCommandLine(argc,argv);

	// store our XML
	std::string megaXmlString;

	// Tree Generation pipeline
	// create an abstract method for repository access
	RepositoryAccess* git = configAgent.initializeRepositoryType();

    // set our repository single or manyjpg options
	git->snapshotJpgs = configAgent.returnOptionByName("manyJpgs");
	git->jpgIndex = configAgent.returnOptionByName("outputFileNumber");

	git->fileName = configAgent.returnFileName();
	git->startWidth = (double)configAgent.returnOptionByName("startWidth");
	git->startHeight = (double)configAgent.returnOptionByName("startHeight");
	git->imageWidth = configAgent.returnOptionByName("imageWidth");
	git->imageHeight = configAgent.returnOptionByName("imageHeight");
	git->backgroundImageName = configAgent.returnBackgroundImageName();
	// set our scaling factor from our received or default options
	git->scaleWidth = git->imageWidth / git->startWidth;
	git->scaleHeight = git->imageHeight / git->startHeight;
	git->globalInserts = 0;
	git->localInserts = 0;
	git->insertTarget = 0;
	git->revJpgs = configAgent.returnOptionByName("revJpgs");
	git->globalRevs = 0;
	git->localRevs = 0;
	git->revTarget = 0;
	git->currentTreeSize = 0;
	git->logGenerated = 0;

	// retrieve our source tree
	DiscursiveDebugPrint("default","Retrieving From Server, Please Wait...\n");
	timerAgent.Tic("Initial Retrieve");
	git->source = git->retrieve();
	DiscursiveDebugPrint("time","Server Retrieval Took:\n");
	timerAgent.Toc("Initial Retrieve");

	// output tree info
	DiscursiveDebugPrint("default","Source tree name is %s\n", git->source->data[TreeNodeKey::NAME].c_str());
	DiscursiveDebugPrint("default","Source tree has %d revisions which require %d tree inserts\n",git->globalRevs,git->globalInserts);

	// read in background image if exists
	// create a background image variable for lookup in loop
	bool useBackgroundImage = 0;
	timerAgent.Tic("Background Retrieve and scale");
	Image backgroundImage;
	if(git->backgroundImageName != "")
	{
		backgroundImage.read(git->backgroundImageName);
		// scale image
		backgroundImage.scale(Geometry(git->startWidth,git->startHeight));
		useBackgroundImage = 1;
	}
	timerAgent.Toc("Background Retrieve and scale");

	delete(git->source);
	git->source = NULL;

	// create loop step variables generically for any jpg looping type
	int loopStart = 0;
	int loopStop = 1;
	int loopStep = 1;
	int executeLoopType = 1;

	// check for our custom loop types
	if(configAgent.returnOptionByName("manyJpgs"))
	{
		loopStart = configAgent.returnOptionByName("jpgStart");
		loopStop = configAgent.returnOptionByName("jpgStop");
		loopStep = configAgent.returnOptionByName("jpgStep");
		executeLoopType = 2;
	}
	else if(configAgent.returnOptionByName("revJpgs"))
	{
		loopStart = configAgent.returnOptionByName("revStart");
		loopStop = configAgent.returnOptionByName("revStop");
		loopStep = configAgent.returnOptionByName("revStep");
		executeLoopType = 3;
	}

	// check for too many global inserts or revisions
	if((executeLoopType == 2) && (loopStop >= git->globalInserts))
	{
		loopStop = git->globalInserts-1;
	}
	DiscursiveDebugPrint("default","Total revs in repo = %d and loopstop = %d\n",git->globalRevs,loopStop);
	if((executeLoopType == 3) && (loopStop >= git->globalRevs))
	{
		loopStop = git->globalRevs-1;
	}

	// loop over a bunch of increasingly large trees for debug or revision tree generation
	// use our user-set parameters to define our step
	for(int i = loopStart; i<loopStop;i+= loopStep)
	{

		// reset our insert variables
		if(executeLoopType == 2)
		{
			git->localInserts = 0;
			git->insertTarget = i;
		}
		else if(executeLoopType == 3)
		{
			git->localRevs = 0;
			git->revTarget = i;
		}

		// retrieve our source tree
		git->source = git->retrieve();


		// Decorate surrogate tree nodes with locations
		DiscursiveDebugPrint("default","Decorating surrogate trees %d out of %d step value %d\n",i,loopStop,loopStep);
		timerAgent.Tic("Decorating surrogate trees");
		int decoratorType = DecoratorFactory::SPATIAL_DISPLACEMENT_LEAF_CLUSTERING;  //SPATIAL_DISPLACEMENT_NAIVE;
		Decorator* decorator = DecoratorFactory::getInstance(decoratorType, 0);
		decorator->decorate(git->source);
		timerAgent.Toc("Decorating surrogate trees");

		// print an xml file of our last surrogate tree
		timerAgent.Tic("Creating XML");
		megaXmlString = git->source->returnXml();
		timerAgent.Toc("Creating XML");

		// Transform coordinates
		DiscursivePrint("Transforming coordinates to create %d x %d image\n",git->startWidth, git->startHeight);
		timerAgent.Tic("Transforming tree");
		int transformerType = TransformFactory::IMAGE_MAGICK_TRANSFORMER;
//		printf("Calling transform with (%d, %d, %p, %d, %d)\n",transformerType,3,git->source,git->startWidth, (int)(0.95*git->startHeight));
		TransformFactory::transform(transformerType,3,git->source,git->startWidth, (int)(0.96*git->startHeight));
		timerAgent.PrintToc("Transforming tree");

		// Digitize decorated surrogate tree into line segment tree
		DiscursiveDebugPrint("default","Digitizing decorated surrogate trees into line segment trees %d out of %d step value %d\n",i,loopStop,loopStep);
		timerAgent.Tic("Digitizing decorated trees into line segments");
		int segmentLength = 1;
		int digitizerType = DigitizerFactory::SIMPLE_TRAPEZOIDER;
		Digitizer* digitizer = DigitizerFactory::getInstance(digitizerType,0);
		DrawableData* lines = digitizer->digitize(git->source);
		timerAgent.Toc("Digitizing decorated trees into line segments");

		// Draw tree
		DiscursiveDebugPrint("default","Drawing Tree %d out of %d step value %d\n",i,loopStop,loopStep);
		timerAgent.Tic("Drawing Tree with artist.draw");
		
		// instantiate canvas and draw lines
		Image canvas(Geometry(git->startWidth,git->startHeight),"white");
		if(useBackgroundImage == 1)
		{
			timerAgent.Tic("Background Image to Canvas Memory Copy");
			canvas = backgroundImage;
			timerAgent.Toc("Background Image to Canvas Memory Copy");
		}
		TrapezoidArtist artist;
		artist.draw(canvas, lines);
		timerAgent.Toc("Drawing Tree with artist.draw");

		// Transform image
		DiscursivePrint("Transforming image of size %d x %d to create %d x %d image\n",git->startWidth,git->startHeight,git->imageWidth, git->imageHeight);
		timerAgent.Tic("Transforming image");
		transformerType = TransformFactory::IMAGE_RESIZE_TRANSFORMER;
		TransformFactory::transform(transformerType,3,&canvas,git->imageWidth,git->imageHeight);
		timerAgent.PrintToc("Transforming image");

		// actually generate a tree
		timerAgent.Tic("actually generating image from canvas");
		git->WriteJPGFromCanvas(&canvas);
		timerAgent.Toc("actually generating image from canvas");

		delete digitizer;
		delete decorator;

		DiscursiveDebugPrint("default","\n");
	}

	// print the latest XML
	DiscursiveDebugPrint("xml","%s\n",megaXmlString.c_str());

	// finish the timing on our entire program
	timerAgent.Toc("Entire SourceTreeVis Program");
	timerAgent.PrintRunningTotals();


	return 0;
}

//template TransformFactory<ImageMagickTransformer>;
//template TransformFactory<ImageResizeTransformer>;
