/******************************************************************************
 * TCPModule   Internet Protocol Implementation                               *
 * __________________________________________________________________________ *
 *                                                                            *
 * Copyright (C) 2008                                                         *
 * Lehrstuhl fuer Kommunikationsnetze (ComNets)                               *
 * Kopernikusstr. 16, D-52074 Aachen, Germany                                 *
 * phone: ++49-241-80-27910 (phone), fax: ++49-241-80-22242                   *
 * email: wns@comnetsrwth-aachen.de, www: http://wns.comnets.rwth-aachen.de/  *
 ******************************************************************************/

#ifndef TCP_TCPHEADER_HPP
#define TCP_TCPHEADER_HPP

#include <WNS/service/tl/TCPHeader.hpp>
#include <WNS/ldk/Forwarding.hpp>
#include <WNS/ldk/fu/Plain.hpp>

namespace tcp {

    class TCPCommand:
        public wns::ldk::Command,
        public wns::service::tl::ITCPHeader
    {
     public:
         TCPCommand()
             {
                 peer.urg = false;
                 peer.ack = false;
                 peer.psh = false;
                 peer.rst = false;
                 peer.syn = false;
                 peer.fin = false;
             }

         struct {} local;
         struct {
             wns::service::tl::FlowID flowID;
             bool urg;
             bool ack;
             bool psh;
             bool rst;
             bool syn;
             bool fin;
         } peer;
         struct {} magic;

        const wns::service::tl::FlowID& getFlowID() const { return peer.flowID; }

        wns::service::tl::FlowID& getFlowID() { return peer.flowID; }

        const bool& getUrgentFlag() const { return peer.urg; }

        bool& getUrgentFlag() { return peer.urg; }

        const bool& getAckFlag() const { return peer.ack; }

        bool& getAckFlag() { return peer.ack; }

        const bool& getPushFlag() const { return peer.psh; }

        bool& getPushFlag() { return peer.psh; }

        const bool& getResetFlag() const { return peer.rst; }

        bool& getResetFlag() { return peer.rst; }

        const bool& getSynFlag() const { return peer.syn; }

        bool& getSynFlag() { return peer.syn; }

        const bool& getFinFlag() const { return peer.fin; }

        bool& getFinFlag() { return peer.fin; }
     };

    /**
     * @brief Dummy Functional Unit that only provides the TCP Header
     *
     */
    class TCPHeader :
        public wns::ldk::Forwarding<TCPHeader>,
        public wns::ldk::fu::Plain<TCPHeader, TCPCommand>
    {
    public:
        TCPHeader(wns::ldk::fun::FUN* fun, const wns::pyconfig::View& config);

        virtual ~TCPHeader() {}

        wns::ldk::CommandPool*
        createReply(const wns::ldk::CommandPool* original) const;

        virtual void
        calculateSizes(const wns::ldk::CommandPool* commandPool, Bit& commandPoolSize, Bit& dataSize) const;

    private:
        /**
		 * @brief Allow for changing the IP-header size from the default
		 *        20*8b
         */
        Bit headerSize;



    };
} // tcp


#endif // TCP_TCPHEADER_HPP
