/******************************************************************************
 * TCP (WNS Module for the Transmission Control Protocol)                     *
 * __________________________________________________________________________ *
 *                                                                            *
 * Copyright (C) 2006                                                         *
 * Chair of Communication Networks (ComNets)                                  *
 * Kopernikusstr. 16, D-52074 Aachen, Germany                                 *
 * phone: ++49-241-80-27910 (phone), fax: ++49-241-80-22242                   *
 * email: wns@comnets.rwth-aachen.de                                          *
 * www: http://wns.comnets.rwth-aachen.de                                     *
 ******************************************************************************/

#include <TCP/Module.hpp>

using namespace tcp;

STATIC_FACTORY_REGISTER_WITH_CREATOR(
	Module,
	wns::module::Base,
	"tcp",
	wns::PyConfigViewCreator);

Module::Module(const wns::pyconfig::View& pyco) :
	wns::module::Module<Module>(pyco),
	log("TCP", "Module")
{
}

Module::~Module()
{
}

void Module::configure()
{
}

void Module::startUp()
{
}

void Module::shutDown()
{
}
