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

#ifndef TCP_IPDATAHANDLER_HPP
#define TCP_IPDATAHANDLER_HPP

#include <WNS/service/nl/DataHandler.hpp>
#include <WNS/ldk/FunctionalUnit.hpp>

namespace tcp {

	class UpperCommand;

	/**
	 * @brief Lower end of TCP (towards IP)
	 */
	class IPDataHandler :
		virtual public wns::service::nl::DataHandler
	{
	public:
		explicit
		IPDataHandler();

		virtual void
		onData(
			wns::service::nl::Address _sourceIP,
			const wns::osi::PDUPtr& _pdu);

		void
		setLowerConvergence(wns::ldk::FunctionalUnit* _lowerConvergence);

		wns::ldk::FunctionalUnit*
		getLowerConvergence() const;

	private:
		/**
		 * @brief My friends. Currently depending on the UpperConvergence command.
		 */
		struct Friends{
			wns::ldk::FunctionalUnit* uc;
		} friends;

		wns::ldk::FunctionalUnit* lowerConvergence;
	};
} // namespace tcp


#endif // NOT defined TCP_IPDATAHANDLER_HPP
