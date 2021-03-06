/*
 * surrogate_tree_node.h
 *
 *  Created on: Aug 13, 2010
 *      Author: programmer
 */

#ifndef SURROGATE_TREE_NODE_H_
#define SURROGATE_TREE_NODE_H_

#include <vector>
#include <boost/lexical_cast.hpp>
#include "./surrogate.h"
#include "../transform/property_transformer.h"
#include "../system/discursive_system.h"


namespace TreeNodeKey
{
	static const string DEPTH = "depth";
	static const string X = "x";
	static const string Y = "y";
	static const string SIZE = "size";
	static const string NAME = "name";
	static const string ROOT = "root";
	static const string SCOMX = "scomX";
	static const string SCOMY = "scomY";
	static const string SCOMWEIGHT = "scomWeight";
	static const string CREATION_TIME = "creation_time";
	static const string LENGTH = "length";
	static const string REVISIONCREATED = "revisioncreated";
	static const string DRAWNODE = "drawleaf";
	static const string ANGLE = "angle";
	static const string COLOR = "color";
};

using namespace std;

class SurrogateTreeNode : public Surrogate
{
private:
	int creationRevisionNumber;
public:
	SurrogateTreeNode();
	~SurrogateTreeNode();
	vector<SurrogateTreeNode*>* children;
	bool isFile;

	SurrogateTreeNode* getChildWithName();
	double findMin(string property);
	double findMax(string property);
	void scale(string property, double factor);
	void transform(string property, PropertyTransformer* transformer);
	void set(string key, string value);
	void set(string key, int value);
	std::string returnXml();
};

#endif /* SURROGATE_TREE_NODE_H_ */
