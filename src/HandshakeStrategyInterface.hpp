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

#ifndef TCP_HANDSHAKESTRATEGYINTERFACE_HPP
#define TCP_HANDSHAKESTRATEGYINTERFACE_HPP

#include <TCP/Service.hpp>

#include <WNS/service/tl/FlowID.hpp>


namespace tcp {

	/**
	 * @brief Interface for implementing different strategies for connection establishment
	 */
	class HandshakeStrategyInterface
	{
	public:
		virtual ~HandshakeStrategyInterface() {}

		virtual void
		activeOpen(const wns::service::tl::FlowID&) = 0;

		virtual void
		passiveOpen(const wns::service::tl::FlowID&) = 0;

		virtual void
		closeConnection(const wns::service::tl::FlowID&) = 0;

		virtual void
		registerStrategyHandler(HandshakeStrategyHandlerInterface* _strategyHandler) = 0;

	};
} // namespace tcp

#endif // NOT defined TCP_HANDSHAKESTRATEGYINTERFACE_HPP
