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

#include <WNS/ldk/Compound.hpp>

#include <TCP/IPDataHandler.hpp>
#include <TCP/UpperConvergence.hpp>

using namespace tcp;

IPDataHandler::IPDataHandler() :
	lowerConvergence(NULL)
{
	friends.uc = NULL;
}

void
IPDataHandler::onData(
	wns::service::nl::Address /*_sourceIP*/,
	const wns::osi::PDUPtr& _pdu)
{
	assure(_pdu, "Invalid PDU (NULL)");


	lowerConvergence->onData(wns::dynamicCast<wns::ldk::Compound>(_pdu));
}

void
IPDataHandler::setLowerConvergence(wns::ldk::FunctionalUnit* _lowerConvergence)
{
	assure(!lowerConvergence, "LowerConvergence already set!");
	lowerConvergence = _lowerConvergence;
}

wns::ldk::FunctionalUnit*
IPDataHandler::getLowerConvergence() const
{
	assure(lowerConvergence, "LowerConvergence not set!");
	return lowerConvergence;
}
