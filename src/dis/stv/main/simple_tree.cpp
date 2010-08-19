/*
 * simple_tree.cpp
 *
 *  Created on: Aug 12, 2010
 *      Author: programmer
 */
#include <Magick++.h>
#include "../model/surrogate_tree_node.h"
#include "../io/git_repository_access.h"
#include "../io/github_repository_access.h"
#include "../io/svn_remote_repository_access.h"
#include "../dec/spatial_displacement.h"
#include "../draw/scanline_artist.h"
#include "../gen/space_colonizer.h"

using namespace Magick;

int main(int argc, char **argv)
{
	// Tree image size parameters
	int WIDTH = 500;
	int HEIGHT = 500;

	// Tree Generation pipeline

	// Retrieve tree from GIT and create surrogate tree
	// for now, ask the user if local files or github project
	cout << "\n1==local\n2==gritProject\n3==any github project";
	cout << "\n4==http://hkit.googlecode.com/svn/trunk/\n5==any SVN url\n";
	int answer = 0 * argc;
	cin >> answer;

	RepositoryAccess* git;
	if(answer == 1)
	{
		git = new GitRepositoryAccess("~/Projects/source_tree_vis");
	}
	else if(answer == 2)
	{
		git = new GitHubRepositoryAccess("mojombo","grit");
	}
	else if(answer == 3)
	{
		// allow user to pick github user name/project
		char userNameBuffer[255];
		char projectNameBuffer[255];
		cout << "\nEnter the github user name\n";
		cin >> userNameBuffer;
		cout << "\nEnter the github project name\n";
		cin >> projectNameBuffer;

		git = new GitHubRepositoryAccess(userNameBuffer,projectNameBuffer);
	}
	else if(answer == 4)
	{
		git = new SvnRemoteRepositoryAccess(" http://hkit.googlecode.com/svn/trunk/");
	}
	else if(answer == 5)
	{
		char svnRepoBuffer[255];
		cout << "\nEnter a SVN url like http://hkit.googlecode.com/svn/trunk/\n";
		cin >> svnRepoBuffer;
		git = new SvnRemoteRepositoryAccess(svnRepoBuffer);
	}

	SurrogateTreeNode* source = git->retrieve();
	printf("Source tree name is %s\n", source->data["name"].c_str());

	// Decorate surrogate tree nodes with locations
	SpatialDisplacement* disp = new SpatialDisplacement(WIDTH,HEIGHT);
	disp->decorate(source);

	// Digitize decorated surrogate tree into line segment tree
	SpaceColonizer* digitizer = new SpaceColonizer(2);
	DrawableData* lines = digitizer->digitize(source);

	// Transform

	// Draw tree
	InitializeMagick(*argv);
	Image canvas(Geometry(WIDTH,HEIGHT),"white");
	ScanlineArtist* artist = new ScanlineArtist();
	artist->draw(canvas, lines);

	try
	{
		// Write the image to a file
		canvas.write( "out/tree.jpg" );
	}
	catch( Exception &error_ )
	{
		cout << "Caught exception: " << error_.what() << endl;
		return 1;
	}

	return 0;
}
