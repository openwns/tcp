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

#include <fstream>
#include <iomanip>

#include <WNS/pyconfig/View.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>

#include <TCP/CongestionControl.hpp>

#include <cppunit/extensions/HelperMacros.h>


namespace tcp { namespace tests {
	class CongestionControlTest :
		public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(CongestionControlTest);
		CPPUNIT_TEST(init);
		CPPUNIT_TEST(slowStartMode);
		CPPUNIT_TEST(congestionAvoidanceMode);
		CPPUNIT_TEST(changeModes);
		CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void init();
		void slowStartMode();
		void congestionAvoidanceMode();
		void changeModes();
		void tearDown();

	private:
		std::stringstream ss;
		CongestionControl* cc;
		wns::pyconfig::Parser all;

		std::fstream f;
	};

} // tests
} // tcp


using namespace tcp::tests;

CPPUNIT_TEST_SUITE_REGISTRATION(CongestionControlTest);


void
CongestionControlTest::setUp()
{
	ss << "import tcp.TCP\n"
	   << "congContr = tcp.TCP.CongestionControl()\n"
	   << "congContr.slowStart.ssthresh = 65537\n"; // value in line 162 accordingly!

	all.loadString(ss.str());

	wns::pyconfig::View config(all, "congContr");

	cc = new CongestionControl(config);
}


void
CongestionControlTest::init()
{
	CPPUNIT_ASSERT(cc->getCongestionControlMode() != NULL);
}


void
CongestionControlTest::slowStartMode()
{
	f.open("SlowStartMode.txt", std::ios::out);

	cc->setCongestionControlMode(cc->SLOWSTART);
	CPPUNIT_ASSERT_EQUAL(cc->slowStart, cc->getCongestionControlMode());

	f << "SlowStart mode entered!" << std::endl;
	f << "Slow start threshold: " << cc->getSlowStartThreshold() << std::endl << std::endl;
	f << std::setw(15) << "prev_cwnd:" << std::setw(13) << "cwnd" << std::endl;

	uint32_t prev_cwnd;

	while (cc->slowStart->getWindowSize() <= cc->getSlowStartThreshold()) {
		prev_cwnd = cc->slowStart->getWindowSize();

		for(uint32_t i=1; i<= prev_cwnd; i++){
			cc->slowStart->onSegmentAcknowledged();
		}
		f << std::setw(12) << prev_cwnd << std::setw(15) << cc->slowStart->getWindowSize() << std::endl;

		// double the size of previous window size?
		CPPUNIT_ASSERT_EQUAL(prev_cwnd*2, cc->getWindowSize());
	}

	// still in "safe area"
	cc->onSegmentAcknowledged();
	// now in "unsafe area"
	CPPUNIT_ASSERT_EQUAL(cc->ca, cc->getCongestionControlMode());

	f.close();
}


void
CongestionControlTest::congestionAvoidanceMode()
{
	f.open("CA-Tahoe.txt", std::ios::out);

	f << "CongestionAvoidance::TahoeCongAvoid mode entered!" << std::endl << std::endl;
	f << std::setw(15) << "prev_cwnd:" << std::setw(13) << "cwnd" << std::endl;

	cc->setCongestionControlMode(cc->CONGESTION_AVOIDANCE);
	CPPUNIT_ASSERT_EQUAL(cc->ca, cc->getCongestionControlMode());

	uint32_t prev_cwnd;

	while (cc->getWindowSize() <= 2048) {
		prev_cwnd = cc->getWindowSize();
		for(uint32_t i=0; i<=prev_cwnd; i++){
			CPPUNIT_ASSERT_EQUAL(prev_cwnd, cc->getWindowSize());
			// assure congestion avoidance mode by calling the
			// method of the ca instance
			cc->ca->onSegmentAcknowledged();
		}
		f << std::setw(12) << prev_cwnd << std::setw(15) << cc->getWindowSize() << std::endl;

		CPPUNIT_ASSERT_EQUAL(prev_cwnd+1, cc->getWindowSize());
	}

	f.close();
}


void
CongestionControlTest::changeModes()
{
	/**
	 * Cover all possible transition handled by TCP congestion control
	 * mechanisms:
	 *   1) SlowStart -> SlowStart (if congestion occurs in slow start mode)
	 *   2) SlowStart -> CongAvoid (if threshold is reached in slow start
	 *                              mode)
	 *   3) CongAvoid -> SlowStart (if congestion occurs in congestion
	 *                              avoidance mode)
	 */

	// No. 1:
	cc->setCongestionControlMode(cc->SLOWSTART);
	CPPUNIT_ASSERT_EQUAL(uint32_t(65537), cc->getSlowStartThreshold()); // ssthresh according to pyconfig
	CPPUNIT_ASSERT_EQUAL(cc->slowStart, cc->getCongestionControlMode());

	// assure "safe area"
	cc->slowStart->setWindowSize(cc->getSlowStartThreshold() >> 4); // arbitrary value
							     // >> 0 and <<
							     // ssthresh
	// remember window size
	uint32_t windowSize = cc->getWindowSize();
	CPPUNIT_ASSERT(windowSize <= cc->getSlowStartThreshold());
	cc->onSegmentLoss(cc->TIMEOUT, uint32_t(0));
	CPPUNIT_ASSERT_EQUAL(cc->slowStart, cc->getCongestionControlMode());
	CPPUNIT_ASSERT_EQUAL(windowSize / 2, cc->getSlowStartThreshold());
	CPPUNIT_ASSERT_EQUAL(uint32_t(1), cc->getWindowSize());


	// No. 2:
	cc->setCongestionControlMode(cc->SLOWSTART);
	CPPUNIT_ASSERT_EQUAL(cc->slowStart, cc->getCongestionControlMode());

	cc->slowStart->setWindowSize(cc->getSlowStartThreshold());
	// still in "safe mode"
	cc->onSegmentAcknowledged();
	CPPUNIT_ASSERT_EQUAL(cc->getWindowSize(), cc->getSlowStartThreshold()+1);
	CPPUNIT_ASSERT_EQUAL(cc->getCongestionControlMode(), cc->slowStart);

	// change mode with next ack
	uint32_t tmp_ssthresh = cc->getSlowStartThreshold();
	cc->onSegmentAcknowledged();
	CPPUNIT_ASSERT_EQUAL(cc->ca, cc->getCongestionControlMode());
	CPPUNIT_ASSERT_EQUAL(tmp_ssthresh+1, cc->getWindowSize());


	// No. 3:
	// a) congestion = timeout
	// reset congestion window
	cc->setCongestionControlMode(cc->CONGESTION_AVOIDANCE);
	cc->ca->setWindowSize(uint32_t(16384));

	CPPUNIT_ASSERT_EQUAL(cc->ca, cc->getCongestionControlMode());

	cc->onSegmentLoss(cc->TIMEOUT, uint32_t(0));

	CPPUNIT_ASSERT_EQUAL(uint32_t(1), cc->getWindowSize());
	CPPUNIT_ASSERT_EQUAL(cc->slowStart, cc->getCongestionControlMode());

	// b) congestion = 3 ACKs of same number in a row
	cc->setCongestionControlMode(cc->CONGESTION_AVOIDANCE);
	cc->ca->setWindowSize(uint32_t(16384));

	CPPUNIT_ASSERT_EQUAL(cc->ca, cc->getCongestionControlMode());

	cc->onSegmentLoss(cc->TIMEOUT, uint32_t(0));
	cc->onSegmentLoss(cc->TIMEOUT, uint32_t(0));
	cc->onSegmentLoss(cc->TIMEOUT, uint32_t(0));

	CPPUNIT_ASSERT_EQUAL(uint32_t(1), cc->getWindowSize());
	CPPUNIT_ASSERT_EQUAL(cc->slowStart, cc->getCongestionControlMode());
}


void
CongestionControlTest::tearDown()
{
	delete cc;
}





