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

#ifndef TCP_FLOWIDBUILDER_HPP
#define TCP_FLOWIDBUILDER_HPP

#include <TCP/UpperConvergence.hpp>

#include <WNS/service/nl/Address.hpp>
#include <WNS/service/tl/Service.hpp>

#include <WNS/ldk/FunctionalUnit.hpp>
#include <WNS/ldk/Compound.hpp>
#include <WNS/ldk/fun/FUN.hpp>
#include <WNS/ldk/Key.hpp>


#include <sstream>

namespace tcp {

    class FlowIDKey :
        public wns::ldk::Key
    {
    public:
        FlowIDKey(const wns::service::tl::FlowID&);

        virtual bool
        operator<(const Key& other) const;

        virtual std::string
        str() const;

        virtual
        ~FlowIDKey()
        {}

        wns::service::tl::FlowID flowID_;
    };

    typedef wns::SmartPtr<FlowIDKey> FlowIDKeyPtr;
    typedef wns::SmartPtr<const FlowIDKey> ConstFlowIDKeyPtr;

	class FlowIDBuilder :
		public wns::ldk::KeyBuilder
	{
	public:
		FlowIDBuilder(
			wns::ldk::fun::FUN* _fun,
			const wns::pyconfig::View& _config);

		virtual
		~FlowIDBuilder(){}

		virtual
		void onFUNCreated();

		virtual wns::ldk::ConstKeyPtr
		operator () (const wns::ldk::CompoundPtr& /*compound*/, int /*direction*/) const;

	private:
		wns::pyconfig::View config;

		const wns::ldk::fun::FUN* fun;

        wns::ldk::CommandReaderInterface* tcpHeaderReader;

	};

} // namespace tcp

#endif // NOT defined TCP_FLOWID_HPP
