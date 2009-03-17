/******************************************************************************
 * TCP (WNS Module for the Transmission Control Protocol)                     *
 * __________________________________________________________________________ *
 *                                                                            *
 * Copyright (C) 2007                                                         *
 * Chair of Communication Networks (ComNets)                                  *
 * Kopernikusstr. 16, D-52074 Aachen, Germany                                 *
 * phone: ++49-241-80-27910 (phone), fax: ++49-241-80-22242                   *
 * email: wns@comnets.rwth-aachen.de                                          *
 * www: http://wns.comnets.rwth-aachen.de                                     *
 ******************************************************************************/


#ifndef TCP_TAHOECONGAVOID_HPP
#define TCP_TAHOECONGAVIOD_HPP

#include <WNS/logger/Logger.hpp>
#include <WNS/pyconfig/View.hpp>

#include <WNS/container/Registry.hpp>

#include <TCP/CongestionControlStrategy.hpp>


namespace tcp {

	class TahoeCongAvoid :
		virtual public CongestionAvoidanceStrategy
	{

	public:
		TahoeCongAvoid(const wns::pyconfig::View& _pyco);

		virtual
		~TahoeCongAvoid();

		/**
		 * @brief Implementation of the CongestionAvoidanceStrategy interface
		 */
		virtual void
		onSegmentLoss(segmentLoss _sl, uint32_t _ackNR);

		virtual void
		onRTTSample();

		virtual uint32_t
		getWindowSize();

		virtual simTimeType
		getRetransmissionTimeout();

		virtual void
		onSegmentAcknowledged();

		virtual void
		setWindowSize(uint32_t new_cwnd);

		virtual bool
		duplicateACKThresholdReached(uint32_t _ackNR);

		virtual void
		clearDuplicateACKCounter();

	private:
		std::string
		printReason(segmentLoss reason);

		wns::pyconfig::View pyco;

		/**
		 * @brief The size of the congestion window
		 */
		uint32_t cwnd;

		/**
		 * @brief Counter to increase congestion window
		 * cwnd linear
		 */
		uint32_t cwnd_cnt;

		/**
		 * @brief Tahoe's logger
		 */
		wns::logger::Logger logger;

		/**
		 * @brief Value for retransmissions in seconds
		 */
		simTimeType timeout;

		/**
		 * @brief The initial threshold of duplicate acks
		 */
		int ndup;

		/**
		 * @brief Registry for counting the number of duplicate acks of
		 * regarding the same acknowledgement number
		 */
		wns::container::Registry<uint32_t, int> countDuplicateACKs;
	};
} // namespace tcp


#endif // NOT defined TCP_TAHOECONGAVOID_HPP
