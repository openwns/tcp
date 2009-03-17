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

#ifndef TCP_CONNECTIONHANDLER_HPP
#define TCP_CONNECTIONHANDLER_HPP

#include <WNS/service/tl/ConnectionHandler.hpp>

namespace tcp {

	class ConnectionHandler:
		virtual public wns::service::tl::ConnectionHandler{
	public:
		ConnectionHandler();
		~ConnectionHandler() {};

	};
} // namespace tcp

#endif // NOT defined TCP_CONNECTIONHANDLER_HPP
