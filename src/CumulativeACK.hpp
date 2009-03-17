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


#ifndef TCP_CUMULATIVEACK_HPP
#define TCP_CUMULATIVEACK_HPP


#include <WNS/ldk/CommandTypeSpecifier.hpp>
#include <WNS/ldk/HasConnector.hpp>
#include <WNS/ldk/HasReceptor.hpp>
#include <WNS/ldk/HasDeliverer.hpp>
#include <WNS/ldk/FunctionalUnit.hpp>

#include <WNS/probe/bus/ContextCollector.hpp>

#include <WNS/events/CanTimeout.hpp>

#include <WNS/container/Registry.hpp>

#include <WNS/logger/Logger.hpp>
#include <WNS/pyconfig/View.hpp>

#include <TCP/CongestionControl.hpp>

namespace tcp {

	namespace tests {  
		class CumulativeACKTest;
	} // namespace tests

	class CongestionControl;

	class CumulativeACKCommand : 
		public wns::ldk::Command
	{
	public:
		CumulativeACKCommand()
		{
			peer.type = I;

			peer.advertisedWindowSize = 0;
			peer.sequenceNumber = 0;
			peer.ACKNumber = 0;
		}

		typedef enum {I, ACK} FrameType; //todo: add PiggyBack

		struct {} local;

		struct {
			/**
			 * @brief Determine wether the sent out segement is a
			 * data compound or a control compound
			 */
			FrameType type;

			/**
			 * @brief The receiver's capacity being able to consume TCP segments
			 */
			uint32_t advertisedWindowSize;

			/**
			 * @brief The sender's sequence number
			 */
			uint32_t sequenceNumber;

			/**
			 * @brief The receiver's acknowledgement number
			 * This number indicates the sender the next segment
			 * expected to received. All segments up to this
			 * number - 1 are acknowledged implicitly.
			 */
			uint32_t ACKNumber;
		} peer;

		struct {} magic;
	};

	class CumulativeACK :
		public virtual wns::ldk::FunctionalUnit,
		public wns::ldk::CommandTypeSpecifier<CumulativeACKCommand>,
		public wns::ldk::HasReceptor<>,
		public wns::ldk::HasConnector<>,
		public wns::ldk::HasDeliverer<>,
		public wns::Cloneable<CumulativeACK>
	{
	private:
		/**
		 * @brief An element for a compound
		 */
		class Timeout :
			public wns::events::CanTimeout
		{
		public:
			Timeout();
			Timeout(CumulativeACK* _parent, uint32_t seqNr, simTimeType timer):
				CanTimeout(),
				sequenceNumber(seqNr),
				parent(_parent),
				logger("TCP", "CumulativeACK")
			{
				assure(_parent, "Invalid CumulativeACK instance.");
				this->setTimeout(timer);
			};

			virtual ~Timeout()
			{
				parent = NULL;
			}

			void onTimeout()
			{
				parent->ccStrategy->onSegmentLoss(CongestionControl::TIMEOUT, sequenceNumber);

				MESSAGE_BEGIN(NORMAL, logger, m, parent->getFUN()->getName());
				m << "\tTimeout of compund SequenceNr: " << sequenceNumber;
				MESSAGE_END();
				parent->ccStrategy->clearDuplicateACKCounter();

				parent->retransmitData(sequenceNumber);
			}

		private:
			uint32_t sequenceNumber;

			CumulativeACK* parent;

			wns::logger::Logger logger;
		};

		/**
		 * @brief Container for the time out events
		 */
		typedef wns::container::Registry<uint32_t, Timeout*, wns::container::registry::DeleteOnErase> Timeouts;

		/**
		 * @brief Container for the compounds
		 */
		typedef wns::container::Registry<uint32_t, wns::ldk::CompoundPtr, wns::container::registry::NoneOnErase> Compounds;


	public:
		CumulativeACK(wns::ldk::fun::FUN* _fun, const wns::pyconfig::View& _pyco);

		/**
		 * @brief Component's destructor
		 */
		virtual
		~CumulativeACK();

		/**
		 * @brief Implementation of the CompoundHandlerInterface
		 */
		virtual bool
		doIsAccepting(const wns::ldk::CompoundPtr& _compound) const;

		virtual void
		onFUNCreated();

		virtual void
		doOnData(const wns::ldk::CompoundPtr& _compound);

		virtual void
		doSendData(const wns::ldk::CompoundPtr& _compound);

		/**
		 * @brief Return the number of compounds being allowed to be
		 * sent
		 */
		uint32_t
		sendCredit() const;

		simTimeType
		getRetransmissionTimeout() { return ccStrategy->getRetransmissionTimeout();}

	private:
		void
		doWakeup();

		void
		retransmitData(const uint32_t seqNr);

		uint32_t
		min(const uint32_t x, const uint32_t y) const;

        void
        updateTCPHeader(const wns::ldk::CompoundPtr& _compound);
        
		const wns::pyconfig::View& pyco;

		wns::logger::Logger logger;

		wns::ldk::fun::FUN* fun;

		/**
		 * @brief The congestion control instance which guaranties slow
		 * start behaviour and is recalculating window size from the
		 * sender's point of view
		 */
		CongestionControl* ccStrategy;

		/**
		 * @brief The receiver's buffer size being capable of processing
		 * incoming TCP segments
		 */
		uint32_t advertisedWindowSize;

		/**
		 * @brief The sequence number of the next segment to be sent
		 */
		uint32_t sequenceNR;

		/**
		 * @brief The sequence number of the next segment to be received/acknowledged
		 */
		uint32_t ackNR;

		/**
		 * @brief The last successfully received acknowledgment number.
		 */
		uint32_t lastACKInOrder;

		/**
		 * @brief Time between retransmitting a TCP segment given in seconds
		 */
		simTimeType retransmissionTimeout;

		/**
		 * @brief Container for received compounds
		 */
		Compounds receivingCompounds;
		
		/**
		 * @brief All the compounds that have been sent but not acknowledged yet
		 */
		Compounds sendingCompounds;

		/**
		 * @brief Timeout events of compounds sent out
		 */
		Timeouts timeoutSendingCompounds;

		/**
		 * @brief Acknowledgement compound of the last in order received
		 * TCP segement
		 */
		wns::ldk::CompoundPtr ackCompound;
		wns::ldk::CompoundPtr copiedACKCompound;

        /**
         * @brief Reader for the TCP Header
         */
        wns::ldk::CommandReaderInterface* tcpHeaderReader;
        
		uint32_t probeNumber;

		uint32_t instanceID;

		bool probingEnabled;

        wns::probe::bus::ContextCollectorPtr windowSizeContextCollector;
        wns::probe::bus::ContextCollectorPtr sendCreditContextCollector;
	};
} // namespace tcp

#endif // NOT defined TCP_CUMULATIVEACK_HPP

