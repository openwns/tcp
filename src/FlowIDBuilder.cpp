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

#include <TCP/FlowIDBuilder.hpp>
#include <TCP/UpperConvergence.hpp>

#include <WNS/ldk/FlowSeparator.hpp>


using namespace tcp;

STATIC_FACTORY_REGISTER_WITH_CREATOR(
	FlowIDBuilder,
	wns::ldk::KeyBuilder,
	"tcp.FlowIDBuilder",
	wns::ldk::FUNConfigCreator);

FlowIDBuilder::FlowIDBuilder(wns::ldk::fun::FUN* _fun,
			     const wns::pyconfig::View& _config) :
	config(_config),
    fun(_fun),
    tcpHeaderReader(NULL)
{
}

FlowIDKey::FlowIDKey(const wns::service::tl::FlowID& flowID):
    flowID_(flowID)
{
}

bool
FlowIDKey::operator<(const Key& _other) const
{
    assureType(&_other, const FlowIDKey*);
    const FlowIDKey* other = static_cast<const FlowIDKey*>(&_other);

    return flowID_ < other->flowID_;
}

std::string
FlowIDKey::str() const
{
    return flowID_.doToString();
}

void
FlowIDBuilder::onFUNCreated()
{
    tcpHeaderReader = fun->getCommandReader("tcp.tcpHeader");
    assure(tcpHeaderReader, "No reader for the TCP Header available!");
}

wns::ldk::ConstKeyPtr
FlowIDBuilder::operator()(const wns::ldk::CompoundPtr& _compound, int /*_direction*/) const
{
    assure(tcpHeaderReader, "No reader for the TCP Header available!");
    TCPCommand* tcpHeader = tcpHeaderReader->readCommand<TCPCommand>(
        _compound->getCommandPool());

    wns::ldk::ConstKeyPtr key = wns::ldk::ConstKeyPtr(new FlowIDKey(tcpHeader->peer.flowID));

    return key;
}
