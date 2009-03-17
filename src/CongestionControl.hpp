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


#ifndef TCP_CONGESTIONCONTROL_HPP
#define TCP_CONGESTIONCONTROL_HPP

#include <WNS/pyconfig/View.hpp>
#include <WNS/logger/Logger.hpp>

#include <TCP/CongestionControlStrategy.hpp>


namespace tcp { 

	class SlowStart;

	namespace tests {
		class CongestionControlTest;
		class CumulativeACKTest;
	} // namespace tests

	class CongestionControl :
		virtual public CongestionControlStrategy
	{	
		friend class tests::CongestionControlTest;
		friend class tests::CumulativeACKTest;

	public:
        explicit
		CongestionControl(const wns::pyconfig::View& _pyco);

		virtual
		~CongestionControl();

		/**
		 * @brief Implementation of the CongestionControlStrategy
		 * interface
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

		virtual bool
		duplicateACKThresholdReached(uint32_t _ackNR);

		virtual void
		clearDuplicateACKCounter();

		CongestionControlStrategy*
		getCongestionControlMode();

	private:
		std::string
		printMode(Mode _mode);

		wns::pyconfig::View pyco;

		/**
		 * @brief The current mode of congestion control
		 */
		Mode mode;

		/**
		 * @brief The SlowStart instance
		 */
		SlowStartStrategy* slowStart;

		/**
		 * @brief Currently active CongestionAvoidance algorithm
		 */
		CongestionAvoidanceStrategy* ca;

		/**
		 * @brief CongestionControl's logger
		 */
		wns::logger::Logger logger;

		virtual void
		setWindowSize(uint32_t new_cwnd);

		/**
		 * @brief Changing between SlowStart mode and
		 * CongestionAvoidance mode
		 */
		void
		setCongestionControlMode(Mode _mode);

		/**
		 * @brief Returns the SlowStart threshold value
		 */
		uint32_t
		getSlowStartThreshold();
	};
} // namespace tcp

#endif // NOT defined TCP_CONGESTIONCONTROL_HPP
