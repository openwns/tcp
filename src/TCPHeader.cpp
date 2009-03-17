/******************************************************************************
 * TCPModule  Internet Protocol Implementation                                *
 * __________________________________________________________________________ *
 *                                                                            *
 * Copyright (C) 2008                                                         *
 * Lehrstuhl fuer Kommunikationsnetze (ComNets)                               *
 * Kopernikusstr. 16, D-52074 Aachen, Germany                                 *
 * phone: ++49-241-80-27910 (phone), fax: ++49-241-80-22242                   *
 * email: wns@comnetsrwth-aachen.de, www: http://wns.comnets.rwth-aachen.de/  *
 ******************************************************************************/

#include <TCP/TCPHeader.hpp>

using namespace tcp;

STATIC_FACTORY_REGISTER_WITH_CREATOR(
    TCPHeader,
    wns::ldk::FunctionalUnit,
    "tcp.tcpHeader",
    wns::ldk::FUNConfigCreator);


TCPHeader::TCPHeader(wns::ldk::fun::FUN* fun, const wns::pyconfig::View& pyco):
    wns::ldk::fu::Plain<TCPHeader, TCPCommand>(fun),
    headerSize(pyco.get<Bit>("headerSize"))
{
}

wns::ldk::CommandPool*
TCPHeader::createReply(const wns::ldk::CommandPool* original) const
{
    TCPCommand* originalCommand = getCommand(original);

    wns::ldk::CommandPool* reply = getFUN()->getProxy()->createCommandPool();
    TCPCommand* replyCommand = activateCommand(reply);

    /**
	 * Just return the flow id getting from the ARQ FU.
	 * The source/destination addresses/ports are swaped
	 * in lowerConvergence (bottom FU of the TCP's FUN).
	 */
    replyCommand->peer.flowID = originalCommand->peer.flowID;

    return reply;
}

void
TCPHeader::calculateSizes(const wns::ldk::CommandPool* commandPool,
                         Bit& commandPoolSize, Bit& dataSize) const
{
    getFUN()->calculateSizes(commandPool, commandPoolSize, dataSize, this);

    // TCP header size is 20 byte
    commandPoolSize += 20*8;
}
