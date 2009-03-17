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


#ifndef TCP_CONGESTIONCONTROLSTRATEGY_HPP
#define TCP_CONGESTIONCONTROLSTRATEGY_HPP


#include <WNS/StaticFactory.hpp>
#include <WNS/PyConfigViewCreator.hpp>

namespace tcp { 

	namespace tests {
		class CongestionControlTest;
		class CumulativeACKTest;
	} // namespace tests

	class CongestionControlStrategy
	{	
		friend class CongestionControl;
		friend class tests::CongestionControlTest;
		friend class tests::CumulativeACKTest;

	public:
		/**
		 * @brief Reasons for a segement loss
		 */
		enum segmentLoss {TIMEOUT, DUPLICATE_ACK};

		virtual
		~CongestionControlStrategy() {}

		virtual void
		onSegmentLoss(segmentLoss _sl, uint32_t _ackNR) = 0;

		virtual void
		onRTTSample() = 0;

		virtual uint32_t
		getWindowSize() = 0;

		virtual simTimeType
		getRetransmissionTimeout() = 0;

		virtual void
		onSegmentAcknowledged() = 0;

		virtual bool
		duplicateACKThresholdReached(uint32_t _ackNR) = 0;

		virtual void
		clearDuplicateACKCounter() = 0;

	protected:
		virtual void
		setWindowSize(uint32_t new_cwnd) = 0;

		/**
		 * @brief CongestionControl is either in SlowStart mode
		 * or CongestionAvoidance mode
		 */
		enum Mode {SLOWSTART, CONGESTION_AVOIDANCE};
	};

	typedef CongestionControlStrategy SlowStartStrategy;
	typedef CongestionControlStrategy CongestionAvoidanceStrategy;

	typedef wns::PyConfigViewCreator<SlowStartStrategy> SlowStartCreator;
	typedef wns::StaticFactory<SlowStartCreator> SlowStartFactory;

	typedef wns::PyConfigViewCreator<CongestionAvoidanceStrategy> CongestionAvoidanceCreator;
	typedef wns::StaticFactory<CongestionAvoidanceCreator> CongestionAvoidanceFactory;

} // namespace tcp

#endif // NOT defined TCP_CONGESTIONCONTROLSTRATEGY_HPP
