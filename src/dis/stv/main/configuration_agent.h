/*
 * configuration_agent.h
 *
 *  Created on: Oct 1st, 2010
 *      Author: Hunter Davis
 */

#ifndef CONFIGURATIONL_AGENT_H_
#define CONFIGURATION_AGENT_H_

#include "repository_access.h"
#include "initial_agents.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>


class ConfigurationAgent
{
private:
	// agentName equates to the passed command line parameter argument string
	std::string agentName;

	// type of agent to use, ie command line options or interactive
	int agentType;

	// image size options for jpg output
	int imageWidth;
	int imageHeight;

	// scaling factors to use for spatial displacement
	// Default is unity
	int startWidth;
	int startHeight;
	
	// our background image name
	std::string backgroundImageName;

	// our filename for output file
	std::string fileName;
	int outputFileNumber;

	// our config file name
	std::string configFileName;

	// should we make many jpgs?
	int manyJpgs;

	// options for many jpgs
	int jpgStep;
	int jpgStart;
	int jpgStop;

	// should we output a jpg per revision?
	int revJpgs;

	// options for jpg per revison
	int revStep;
	int revStart;
	int revStop;


public:
	ConfigurationAgent();
	~ConfigurationAgent();
	void displayUsage();
	void parseCommandLine(int argc, char **argv);
	void parseConfigFile();
	std::string returnAgentName();
	std::string returnFileName();
	std::string returnBackgroundImageName();
	int returnOptionByName(std::string optionName);
	void setOptionByName(std::string optionName, std::string optionValue);
	RepositoryAccess* initializeRepositoryType();

};


#endif /*CONFIGURATION_AGENT_H_ */
