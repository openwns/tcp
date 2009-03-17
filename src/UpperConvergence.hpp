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

#ifndef TCP_UPPERCONVERGENCE_HPP
#define TCP_UPPERCONVERGENCE_HPP

#include <TCP/TCPHeader.hpp>

#include <WNS/container/Registry.hpp>

#include <WNS/service/tl/FlowID.hpp>

#include <WNS/ldk/Command.hpp>
#include <WNS/ldk/CommandTypeSpecifier.hpp>
#include <WNS/ldk/HasConnector.hpp>
#include <WNS/ldk/HasReceptor.hpp>
#include <WNS/ldk/HasDeliverer.hpp>
#include <WNS/ldk/Forwarding.hpp>
#include <WNS/ldk/Command.hpp>


namespace tcp {

	class Service;

	/**
	 * @brief Interface to TCP's FUN
	 */
	class UpperConvergence :
		public wns::ldk::CommandTypeSpecifier<>,
		public wns::ldk::HasReceptor<>,
		public wns::ldk::HasConnector<>,
		public wns::ldk::HasDeliverer<>,
		public wns::ldk::Forwarding<UpperConvergence>,
		public wns::Cloneable<UpperConvergence>
	{

	public:
		/**
		 * @brief Adapting the service provided by a TCP component
		 * to the FUN
		 */
		UpperConvergence(wns::ldk::fun::FUN* _fun,
				 const wns::pyconfig::View& _pyco);

		virtual
		~UpperConvergence();

        virtual void
        onFUNCreated();

		virtual void
		sendData(const wns::service::tl::FlowID& _flowID, const wns::osi::PDUPtr& sdu);

		virtual void
		processIncoming(const wns::ldk::CompoundPtr& _compound);

		void
		setTLService(Service* _tlService);

	private:
		wns::pyconfig::View pyco;

		wns::logger::Logger logger;

		Service* tlService;

        TCPHeader* tcpHeaderFU;

        wns::ldk::CommandReaderInterface* tcpHeaderReader;
        
	};

} // namespace tcp

#endif // NOT defined TCP_UPPERCONVERGENCE_HPP
