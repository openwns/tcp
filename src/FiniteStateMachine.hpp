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

#ifndef TCP_FINITESTATEMACHINE_HPP
#define TCP_FINITESTATEMACHINE_HPP

#include <WNS/logger/Logger.hpp>

#include <TCP/ConnectionControl.hpp>
#include <TCP/ThreeWayHandshake.hpp>


namespace tcp {

	class FiniteStateMachine :
		public ConnectionControlInterface
	{
	public:
		/**
		 * @brief The Implementation of TCP's finite state machine
		 * @param[in] Global variables shared by all states of the fsm
		 */
		FiniteStateMachine(const ConnectionControlInterface::VariablesType& v, wns::logger::Logger _logger);

		/**
		 * @brief Definition of all signlas defined in
		 * ConnectionControl to perform the state transitions of the finite
		 * state machine.
		 */
		void
		syn();

		void
		syn_ack();

		void
		ack();

		void
		fin();

		void
		rst();

		void
		fin_ack();

		tcp::ConnectionVariables&
		ConnectionVariables()
		{
			return getVariables();
		}

		/**
		 * @brief My logger.
		 */
		wns::logger::Logger logger;

	private:
		/**
		 * @brief Copy constructor
		 * Disallow the copy constructor
		 */
		FiniteStateMachine(FiniteStateMachine& _fsm) : ConnectionControlInterface(_fsm.getVariables()){}
	};
} // namespace tcp

#endif // NOT defined TCP_FINITESTATEMACHINE_HPP
