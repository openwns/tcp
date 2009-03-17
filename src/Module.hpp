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

#ifndef TCP_MODULE_HPP
#define TCP_MODULE_HPP

#include <WNS/module/Module.hpp>
#include <WNS/logger/Logger.hpp>

namespace tcp
{
	class Module :
		public wns::module::Module<Module>
	{
	public:
        explicit
		Module(const wns::pyconfig::View& pyco);

		virtual
		~Module();

		// Module interface
		virtual void
		configure();

		virtual void
		startUp();

		virtual void
		shutDown();

	private:
		wns::logger::Logger log;
	};
} // namespace tcp

#endif // NOT defined TCP_MODULE_HPP
