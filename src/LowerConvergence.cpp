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

#include <TCP/Service.hpp>
#include <TCP/LowerConvergence.hpp>
#include <TCP/UpperConvergence.hpp>

using namespace tcp;

STATIC_FACTORY_REGISTER_WITH_CREATOR(
	LowerConvergence,
	wns::ldk::FunctionalUnit,
	"tcp.lowerConvergence",
	wns::ldk::FUNConfigCreator
	);

LowerConvergence::LowerConvergence(wns::ldk::fun::FUN* fun, const wns::pyconfig::View& _config) :
	wns::ldk::CommandTypeSpecifier<>(fun),
	dataTransmissionService(NULL),
	config(_config),
    logger(config.get("logger")),
    tcpHeaderReader(NULL)
{
}

LowerConvergence::~LowerConvergence()
{
}

void
LowerConvergence::doSendData(const wns::ldk::CompoundPtr& compound)
{
    assure(compound, "doSendData called with an invalid compound.");

    assure(tcpHeaderReader, "No reader for the TCP Header available!");
    TCPCommand* tcpHeader = tcpHeaderReader->readCommand<TCPCommand>(
        compound->getCommandPool());

    getDataTransmissionService()->sendData(tcpHeader->peer.flowID.srcAddress,
                                           tcpHeader->peer.flowID.dstAddress,
                                           compound, this->protocolNumber);
}

void
LowerConvergence::doOnData(const wns::ldk::CompoundPtr& compound)
{
	assure(compound, "doOnData called with an invalid compound.");

    assure(tcpHeaderReader, "No reader for the TCP Header available!");
    TCPCommand* tcpHeader = tcpHeaderReader->readCommand<TCPCommand>(
        compound->getCommandPool());

    // swap the src/dst information of the flow ID
    wns::service::tl::FlowID tmpFlowID(
        tcpHeader->peer.flowID.dstAddress,
        tcpHeader->peer.flowID.dstPort,
        tcpHeader->peer.flowID.srcAddress,
        tcpHeader->peer.flowID.srcPort);
    tcpHeader->peer.flowID = tmpFlowID;

    MESSAGE_SINGLE(NORMAL, logger, "Incoming data for flow id: " << tcpHeader->peer.flowID);

	if (isAccepting(compound))
		getDeliverer()->getAcceptor(compound)->onData(compound);
}

void
LowerConvergence::onFUNCreated()
{
    tcpHeaderReader = getFUN()->getCommandReader("tcp.tcpHeader");
    assure(tcpHeaderReader, "No reader for the TCP Header available!");
}

void
LowerConvergence::setDataTransmissionService(wns::service::nl::Service* service)
{
	assure(!dataTransmissionService, "dataTransmissionService already set");
	dataTransmissionService = service;
}

wns::service::nl::Service*
LowerConvergence::getDataTransmissionService() const
{
	assure(dataTransmissionService, "dataTransmissionService not set");
	return dataTransmissionService;
}

bool
LowerConvergence::doIsAccepting(const wns::ldk::CompoundPtr& /*compound*/) const
{
	// always accepting since IP will not block
	return true;
}

void
LowerConvergence::doWakeup()
{
	getReceptor()->wakeup();
}

void
LowerConvergence::setProtocolNumber(wns::service::nl::protocolNumber _protocolNumber)
{
	this->protocolNumber = _protocolNumber;
}

void
LowerConvergence::setTLService(Service* _tlService)
{
	this->tlService = _tlService;
}

