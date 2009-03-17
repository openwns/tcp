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

#include <TCP/FiniteStateMachine.hpp>


using namespace tcp;

FiniteStateMachine::FiniteStateMachine(const ConnectionControlInterface::VariablesType& _v, wns::logger::Logger _logger) :
	ConnectionControlInterface(_v),
	logger(_logger)
{
	changeState(createState<Closed>());
	MESSAGE_SINGLE(NORMAL, logger, "FSM initialized to state " + getStateName());
}

void
FiniteStateMachine::syn()
{
	MESSAGE_SINGLE(NORMAL, logger, "Sending SYN.");
	changeState(getState()->syn());
}

void
FiniteStateMachine::syn_ack()
{
	MESSAGE_SINGLE(NORMAL, logger, "Sending SYN_ACK.");
	changeState(getState()->syn_ack());
}

void
FiniteStateMachine::ack()
{
	MESSAGE_SINGLE(NORMAL, logger, "Sending ACK.");
	changeState(getState()->ack());
}

void
FiniteStateMachine::fin()
{
	MESSAGE_SINGLE(NORMAL, logger, "Sending FIN.");
	changeState(getState()->fin());
}

void
FiniteStateMachine::rst()
{
	MESSAGE_SINGLE(NORMAL, logger, "Sending RST.");
	changeState(getState()->rst());
}

void
FiniteStateMachine::fin_ack()
{
	MESSAGE_SINGLE(NORMAL, logger, "Sending FIN_ACK.");
	changeState(getState()->fin_ack());
}
