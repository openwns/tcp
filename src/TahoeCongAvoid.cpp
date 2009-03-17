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

#include <TCP/TahoeCongAvoid.hpp>


using namespace tcp;

STATIC_FACTORY_REGISTER_WITH_CREATOR(
	TahoeCongAvoid,
	CongestionAvoidanceStrategy,
	"tcp.TahoeCongAvoid",
	wns::PyConfigViewCreator
	);


TahoeCongAvoid::TahoeCongAvoid(const wns::pyconfig::View& _pyco):
	pyco(_pyco),
	cwnd(pyco.get<uint32_t>("congestionWindow")),
	cwnd_cnt(pyco.get<uint32_t>("congestionWindowCounter")),
	logger(pyco.get("logger")),
	timeout(pyco.get<simTimeType>("retransmissionTimeout")),
	ndup(pyco.get<int>("numberDupACKs")),
	countDuplicateACKs()
{
	countDuplicateACKs.clear();
}

TahoeCongAvoid::~TahoeCongAvoid()
{
	countDuplicateACKs.clear();
}

void
TahoeCongAvoid::onSegmentLoss(segmentLoss reason, uint32_t _ackNR)
{
	MESSAGE_SINGLE(NORMAL, logger, "onSegmentLoss called. Reason: " << printReason(reason));

	switch(reason)
	{
	case(TIMEOUT):
		countDuplicateACKs.clear();
		setWindowSize(1);

		break;

	case(DUPLICATE_ACK):
		if(!countDuplicateACKs.knows(_ackNR))
		{
			// increase if ndup duplicate acks occure
			// _in a row_
			countDuplicateACKs.clear();
			// 1st dup ack is 2nd ack of same nr in a row
			countDuplicateACKs.insert(_ackNR, 2);
		}
		else
		{
			int tmp = countDuplicateACKs.find(_ackNR);
			if(tmp < ndup)
			{
				tmp++;
				countDuplicateACKs.update(_ackNR, tmp);
			}
			else
			{
				uint32_t curr_cwnd = getWindowSize();
				setWindowSize(curr_cwnd/2);
			}
		}

		break;

	default:
		break;
	}
}


void
TahoeCongAvoid::onRTTSample()
{
}

uint32_t
TahoeCongAvoid::getWindowSize()
{
	return cwnd;
}

simTimeType
TahoeCongAvoid::getRetransmissionTimeout()
{
	MESSAGE_SINGLE(NORMAL, logger, "Retransmission timeout: " << timeout);
	return this->timeout;
}

void
TahoeCongAvoid::onSegmentAcknowledged()
{
	countDuplicateACKs.clear();

	if (cwnd_cnt >= cwnd)
	{
		cwnd++;
		cwnd_cnt = 0;
	}
	else
	{
		cwnd_cnt++;
	}
	MESSAGE_SINGLE(NORMAL, logger, "Segment acknowledged. Window size: " << cwnd);
}

void
TahoeCongAvoid::setWindowSize(uint32_t new_cwnd)
{
	MESSAGE_SINGLE(NORMAL, logger, "Setting window size: " << new_cwnd << "(old value: " << cwnd << ")");
	cwnd = new_cwnd;
}


std::string
TahoeCongAvoid::printReason(segmentLoss reason)
{
	switch(reason)
	{
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


bool
TahoeCongAvoid::duplicateACKThresholdReached(uint32_t _ackNR)
{
	if(countDuplicateACKs.knows(_ackNR))
		return countDuplicateACKs.find(_ackNR) == ndup;
	else
		return false;
}


void
TahoeCongAvoid::clearDuplicateACKCounter()
{
	countDuplicateACKs.clear();
}
