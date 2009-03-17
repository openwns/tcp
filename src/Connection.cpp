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

#include <TCP/Connection.hpp>
#include <TCP/DataHandler.hpp>
#include <TCP/UpperConvergence.hpp>


using namespace tcp;


Connection::Connection(const wns::service::tl::FlowID& _flowID,
		       UpperConvergence* _upperConvergence,
		       const wns::pyconfig::View& _config) :
	flowID(_flowID),
	logger(_config.get("logger")),
	upperConvergence(_upperConvergence)
{
}

void
Connection::registerDataHandler(wns::service::tl::DataHandler* _dh)
{
	assure(_dh, "No valid instance of DataHandler");
	dh = _dh;
	MESSAGE_SINGLE(NORMAL, logger, "dataHandler set for flowID: " << flowID);
}

void
Connection::sendData(const wns::osi::PDUPtr& sdu)
{
	MESSAGE_SINGLE(NORMAL, logger, sdu->getLengthInBits()/8 << " bytes received from upper layer.");

	assure(upperConvergence, "UpperConvergence of TCP's FUN not set!");

	upperConvergence->sendData(this->flowID, sdu);
}

wns::service::tl::FlowID
Connection::getFlowID()
{
	return this->flowID;
}


void
Connection::onData(const wns::osi::PDUPtr& _pdu)
{
	MESSAGE_SINGLE(NORMAL, logger, "onData for flowID: " << flowID);
	assure(dh, "No DataHandler set!");
	dh->onData(_pdu);
}
