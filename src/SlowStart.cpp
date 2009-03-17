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


#include <TCP/SlowStart.hpp>


using namespace tcp;

STATIC_FACTORY_REGISTER_WITH_CREATOR(
	SlowStart,
	SlowStartStrategy,
	"tcp.SlowStart",
	wns::PyConfigViewCreator
	);


SlowStart::SlowStart(const wns::pyconfig::View& _pyco):
	pyco(_pyco),
	cwnd(pyco.get<int>("congestionWindow")),
	ssthresh(pyco.get<uint32_t>("ssthresh")),
	logger(pyco.get("logger")),
	timeout(pyco.get<simTimeType>("retransmissionTimeout"))
{
}


SlowStart::~SlowStart()
{
}


void
SlowStart::onSegmentLoss(segmentLoss reason, uint32_t /*_ackNR*/)
{
	MESSAGE_SINGLE(NORMAL, logger, "onSegmentLoss called. Reason: " << printReason(reason));

	switch(reason)
	{
	case TIMEOUT:
		reCalcThreshold();
		setWindowSize(1);
		break;

	case DUPLICATE_ACK:
		reCalcThreshold();
		setWindowSize(cwnd/2);
		break;

	default:
		break;
	}
}


void
SlowStart::onRTTSample()
{
}


uint32_t
SlowStart::getWindowSize()
{
	return cwnd;
}


simTimeType
SlowStart::getRetransmissionTimeout()
{
	MESSAGE_SINGLE(NORMAL, logger, "Timeout for retransmission: " << timeout);
	return this->timeout;
}


void
SlowStart::onSegmentAcknowledged()
{
	cwnd++;
	MESSAGE_SINGLE(NORMAL, logger, "Segment acknowledged. Window size: " << cwnd);
}


void
SlowStart::setWindowSize(uint32_t new_cwnd)
{
	MESSAGE_SINGLE(NORMAL, logger, "Setting window size: " << new_cwnd << "(old value: " << cwnd << ")");
	cwnd = new_cwnd;
}


bool
SlowStart::duplicateACKThresholdReached(uint32_t /*_ackNR*/)
{
	// threshold for fast retransmit; implemented in tahoe congestion avoidance
	assure(false, "SlowStart::duplicateACKThresholdReached should never be called!");
	return false;
}


void
SlowStart::clearDuplicateACKCounter()
{// nothing to do
}


uint32_t
SlowStart::getSlowStartThreshold()
{
	MESSAGE_SINGLE(NORMAL, logger, "SlowStart threshold: " << ssthresh);
	return this->ssthresh;
}


void
SlowStart::reCalcThreshold()
{
	uint32_t prior_ssthresh __attribute__ ((unused)) = ssthresh;
	this->ssthresh = max(getWindowSize()>>1, 2);
	MESSAGE_SINGLE(NORMAL, logger, "Recalculate SlowStart threshold: " << ssthresh << "(old value: " << prior_ssthresh << ").");
}


std::string
SlowStart::printReason(segmentLoss reason)
{
	switch(reason){
	case TIMEOUT:
		return "TIMEOUT";
		break;
	case DUPLICATE_ACK:
		return "DUPLICATE_ACK";
		break;
	default:
		assure(false, "Unknown segment loss!");
		return "";
		break;
	}
}


uint32_t
SlowStart::max(uint32_t x, uint32_t y)
{
	return (x<y) ? y : x;
}
