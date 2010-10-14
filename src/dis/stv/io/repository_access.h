/*
 * repository_access.h
 *
 *  Created on: Aug 12, 2010
 *      Author: programmer
 */

#ifndef REPOSITORY_ACCESS_H_
#define REPOSITORY_ACCESS_H_

#include "../model/surrogate_tree_node.h"
#include "../system/discursive_system.h"

#include <Magick++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using namespace Magick;



class RepositoryAccess
{
private:
	std::vector<std::string> filterKeyStore;
	std::vector<std::string> inverseFilterKeyStore;
	void ParseFilterKeywords(std::string filterKeywords);
	void ParseInverseFilterKeywords(std::string filterKeywords);
protected:
	int repoType;
	std::string repoLog;

public:
	int snapshotJpgs;
	long globalInserts;
	long localInserts;
	long insertTarget;
	long revJpgs;
	long globalRevs;
	long localRevs;
	long revTarget;
	long jpgIndex;
	int logGenerated;
	int startWidth;
	int startHeight;
	int imageWidth;
	int imageHeight;
	int scaleWidth;
	int scaleHeight;
	int drawFilteredLeaves;

	long currentTreeSize;
	std::string fileName;
	std::string backgroundImageName;
	SurrogateTreeNode* source;
	int WriteJPGFromCanvas(Image* canvas);
	virtual SurrogateTreeNode* retrieve() = 0;
	void AddFilterKeywords(std::string filterKeywords);
	void AddInverseFilterKeywords(std::string filterKeywords);
	int DoAnyFilterKeywordsMatch(std::string filterKeywords);
	int DoAnyInverseFilterKeywordsMatch(std::string filterKeywords);
	int DoesThisStringContainFilterKeywords(std::string textualData);
	void InsertByPathName(SurrogateTreeNode* tree, string pathname, long time, int drawSubLeafs);
};

#endif /* REPOSITORY_ACCESS_H_ */
