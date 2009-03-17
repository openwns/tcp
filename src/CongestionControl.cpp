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
#include <TCP/CongestionControl.hpp>


using namespace tcp;


CongestionControl::CongestionControl(const wns::pyconfig::View& _pyco) :
	pyco(_pyco),
	mode(SLOWSTART),
	slowStart(NULL),
	ca(NULL),
	logger(pyco.get("logger"))
{
	slowStart = SlowStartFactory::creator("tcp.SlowStart")->create(pyco.getView("slowStart"));
	ca = CongestionAvoidanceFactory::creator("tcp.TahoeCongAvoid")->create(pyco.getView("tahoeCA"));
}

CongestionControl::~CongestionControl()
{
	delete slowStart;
	delete ca;
}

void
CongestionControl::onSegmentLoss(segmentLoss reason, uint32_t _ackNR)
{
	/**
	 * Remember current window size
	 */
	uint32_t curr_cwnd;

	switch(reason)
	{
	case TIMEOUT:
		/**
		 * @brief A timeout always leads to SlowStart mode
		 */
		getCongestionControlMode()->onSegmentLoss(reason, _ackNR);
		curr_cwnd = getWindowSize();

		this->setCongestionControlMode(SLOWSTART);

		getCongestionControlMode()->setWindowSize(curr_cwnd);

		break;

	case DUPLICATE_ACK:

		getCongestionControlMode()->onSegmentLoss(reason, _ackNR);
		curr_cwnd = getWindowSize();

		if(duplicateACKThresholdReached(_ackNR))
		{
			setCongestionControlMode(SLOWSTART);

			// clear counter for duplicate acks
			ca->clearDuplicateACKCounter();
			
			getCongestionControlMode()->setWindowSize(curr_cwnd);
		}

		break;

	default:
		break;
	}
}

void
CongestionControl::onRTTSample()
{
	this->getCongestionControlMode()->onRTTSample();
}

uint32_t
CongestionControl::getWindowSize()
{
	return getCongestionControlMode()->getWindowSize();
}

simTimeType
CongestionControl::getRetransmissionTimeout()
{
	return getCongestionControlMode()->getRetransmissionTimeout();
}

void
CongestionControl::onSegmentAcknowledged()
{
	uint32_t curr_cwnd = getWindowSize();

	switch(mode)
	{
	case(SLOWSTART):
		if (curr_cwnd > dynamic_cast<tcp::SlowStart*>(slowStart)->getSlowStartThreshold())
		{
			setCongestionControlMode(CONGESTION_AVOIDANCE);
			getCongestionControlMode()->setWindowSize(curr_cwnd);
		}
		break;

	case(CONGESTION_AVOIDANCE):
		// do nothing
		break;

	default:
		break;
	}

	this->getCongestionControlMode()->onSegmentAcknowledged();
}

CongestionControlStrategy*
CongestionControl::getCongestionControlMode()
{
	switch(this->mode)
	{
	case SLOWSTART:
		return slowStart;
		break;
	case CONGESTION_AVOIDANCE:
		return ca;
		break;
	default:
		assure(false, "Unknown congestion control mode!");
		return NULL;
		break;
	}
}

void
CongestionControl::setCongestionControlMode(Mode _mode)
{
	if (this->mode == _mode)
	{
		MESSAGE_SINGLE(NORMAL, logger, "Still in mode " << printMode(_mode));
	}
	else
	{
		MESSAGE_SINGLE(NORMAL, logger, "Leaving mode " << printMode(mode));
		this->mode = _mode;
		MESSAGE_SINGLE(NORMAL, logger, "Entering mode " << printMode(_mode));
	}
}

void
CongestionControl::setWindowSize(uint32_t /*new_cwnd*/)
{
	assure(false, "CongestionControl: Not allowed to set window size!");
}


uint32_t
CongestionControl::getSlowStartThreshold()
{
	return dynamic_cast<tcp::SlowStart*>(slowStart)->getSlowStartThreshold();
}


bool
CongestionControl::duplicateACKThresholdReached(uint32_t _ackNR)
{
	// forward this function to the CongestionAvoidance entity
	return ca->duplicateACKThresholdReached(_ackNR);
}


void
CongestionControl::clearDuplicateACKCounter()
{
	getCongestionControlMode()->clearDuplicateACKCounter();
}


std::string
CongestionControl::printMode(Mode _mode)
{
	switch(_mode)
	{
	case(SLOWSTART):
		return "SlowStart";
		break;

	case(CONGESTION_AVOIDANCE):
		return "CongestionAvoidance";
		break;

	default:
		assure(false, "Wrong type of mode.");
		return "";
		break;
	}
}
