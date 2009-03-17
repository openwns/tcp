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


#ifndef TCP_DATAHANDLER_HPP
#define TCP_DATAHANDLER_HPP


#include <WNS/service/tl/DataHandler.hpp>

namespace tcp { 

	class DataHandler:
		virtual public wns::service::tl::DataHandler {
	public:
		DataHandler();

		virtual
		~DataHandler();
	};
} // namespace tcp

#endif // NOT defined TCP_DATAHANDLER_HPP
