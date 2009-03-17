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

#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include <WNS/service/tl/Connection.hpp>
#include <WNS/service/tl/FlowID.hpp>

#include <TCP/FlowIDBuilder.hpp>

namespace tcp {

	class UpperConvergence;

	/**
	 * @brief The top most FU in a TCP FUN
	 */
	class Connection:
		virtual public wns::service::tl::Connection
	{
	public:
		Connection(const wns::service::tl::FlowID& _flowID,
			   UpperConvergence* _upperConvergence,
			   const wns::pyconfig::View& _config);

		// wns::service:.tl::Connection interface
		virtual void
		registerDataHandler(wns::service::tl::DataHandler *_dh);

		virtual void
		sendData(const wns::osi::PDUPtr &_pdu);

		wns::service::tl::FlowID
		getFlowID();

		/**
		 * @brief Deliver the retrieved data to the corresponding data
		 * handler
		 * @param[in] The data
		 */
		void
		onData(const wns::osi::PDUPtr& _pdu);

	private:
		/**
		 * @brief The connection's flow id
		 */
		wns::service::tl::FlowID flowID;

		/**
		 * @brief My logger
		 */
		wns::logger::Logger logger;

		/**
		 * @brief The DataHandler of the upper layer.
		 */
		wns::service::tl::DataHandler* dh;

		/**
		 * @brief The UpperConvergence of TCP's FUN
		 */
		UpperConvergence* upperConvergence;
	};
} // namespace tcp

#endif // NOT defined TCP_CONNECTION_HPP
