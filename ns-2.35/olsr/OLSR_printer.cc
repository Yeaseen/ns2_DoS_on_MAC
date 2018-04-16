/***************************************************************************
 *   Copyright (C) 2004 by Francisco J. Ros                                *
 *   fjrm@dif.um.es                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

///
/// \file	OLSR_printer.cc
/// \brief	Printing functions used for debugging and tracing are implemented in this file.
///

#include <olsr/OLSR_printer.h>

/********** Messages and packets printing functions **********/

///
/// \brief Prints a given common header into a given file.
/// \warning This function is actually not used.
/// \param out	File where the common header is going to be written.
/// \param ch	Common header to be written.
///
void
OLSR_printer::print_cmn_hdr(FILE* out, struct hdr_cmn* ch) {
	char *error, *direction;
	
	if (ch->error())
		error = "yes";
	else
		error = "no";
		
	if (ch->direction() == hdr_cmn::DOWN)
		direction = "DOWN";
	else if (ch->direction() == hdr_cmn::UP)
		direction = "UP";
	else if (ch->direction() == hdr_cmn::NONE)
		direction = "NONE";
	else
		direction = "UNKNOWN (likely a bug!)";
	
	// We could include this if we were interested in printing address type
	/*if (ch->addr_type() == NS_AF_NONE)
		addr_type = "NS_AF_NONE";
	else if (ch->addr_type() == NS_AF_ILINK)
		addr_type = "NS_AF_ILINK";
	else if (ch->addr_type() == NS_AF_INET)
		addr_type = "NS_AF_INET";
	else
		addr_type = "UNKNOWN (possibly a bug!)";*/
	
	fprintf(out,
		"  COMMON HEADER\n\tptype = %s\n\tuid = %d\n\tsize = %d\n\terror = %s\n\tdirection = %s\n\n",
		packet_info.name(ch->ptype()),
		ch->uid(),
		ch->size(),
		error,
		direction);
}

///
/// \brief Prints a given IP header into a given file.
/// \warning This function is actually not used.
/// \param out	File where the IP header is going to be written.
/// \param ih	IP header to be written.
///
void
OLSR_printer::print_ip_hdr(FILE* out, struct hdr_ip* ih) {
	fprintf(out,
		"  IP HEADER\n\tsrc_addr = %d\n\tsrc_port = %d\n\tdest_addr = %d\n\tdest_port = %d\n\tttl = %d\n\n",
		ih->saddr(),
		ih->sport(),
		ih->daddr(),
		ih->dport(),
		ih->ttl());
}

///
/// \brief Prints a given OLSR packet into a given file.
/// \warning This function is actually not used.
/// \param out	File where the %OLSR packet is going to be written.
/// \param pkt	%OLSR packet to be written.
///
void
OLSR_printer::print_olsr_pkt(FILE* out, OLSR_pkt* pkt) {
		
	fprintf(out,
		"  OLSR PACKET\n\tlength = %d\n\tseq_num = %d\n\t------------\n",
		pkt->pkt_len(),
		pkt->pkt_seq_num());
	
	for (int i = 0; i < pkt->count; i++) {
		print_olsr_msg(out, pkt->msg(i));
		fprintf(out, "\t------------\n");
	}
	fprintf(out, "\n");
}

///
/// \brief Prints a given %OLSR message into a given file.
/// \warning This function is actually not used.
/// \param out	File where the %OLSR message is going to be written.
/// \param msg	%OLSR message to be written.
///
void
OLSR_printer::print_olsr_msg(FILE* out, OLSR_msg& msg) {
	char *msg_type;
	
	switch (msg.msg_type()) {
		case OLSR_HELLO_MSG:
			msg_type = "HELLO";
			break;
		case OLSR_TC_MSG:
			msg_type = "TC";
			break;
		case OLSR_MID_MSG:
			msg_type = "MID";
			break;
		default:
			msg_type = "UNKNOWN (likely a bug!)";
	}
	
	fprintf(out,
		"\ttype = %s\n\tvtime = %.2f\n\tmsg_size = %d\n\t"
		"orig_addr = %d\n\tttl = %d\n\thop_count = %d\n\t"
		"msg_seq_num = %d\n",
		msg_type,
		OLSR::emf_to_seconds(msg.vtime()),
		msg.msg_size(),
		msg.orig_addr(),
		msg.ttl(),
		msg.hop_count(),
		msg.msg_seq_num());
	
	if (msg.msg_type() == OLSR_HELLO_MSG)
		print_olsr_hello(out, msg.hello());
	else if (msg.msg_type() == OLSR_TC_MSG)
		print_olsr_tc(out, msg.tc());
	else if (msg.msg_type() == OLSR_MID_MSG)
		print_olsr_mid(out, msg.mid());
}

///
/// \brief Prints a given %OLSR HELLO message into a given file.
/// \warning This function is actually not used.
/// \param out		File where the %OLSR HELLO message is going to be written.
/// \param hello	%OLSR HELLO message to be written.
///
void
OLSR_printer::print_olsr_hello(FILE* out, OLSR_hello& hello) {
	
	fprintf(out, "\thtime = %.2f\n\twillingness = %d\n",
		OLSR::emf_to_seconds(hello.htime()),
		hello.willingness());
	
	for (int i = 0; i < hello.count; i++) {
		char *nt, *lt;
		OLSR_hello_msg msg = hello.hello_msg(i);
		
		u_int8_t nb_type = msg.link_code() >> 2;
		u_int8_t link_type = msg.link_code() & 0x03;
		if (nb_type == OLSR_NOT_NEIGH)
			nt = "NOT_NEIGH";
		else if (nb_type == OLSR_SYM_NEIGH)
			nt = "SYM_NEIGH";
		else if (nb_type == OLSR_MPR_NEIGH)
			nt = "MPR_NEIGH";
		else
			nt = "UNKNOWN (likely a bug!)";
		
		if (link_type == OLSR_UNSPEC_LINK)
			lt = "UNSPEC_LINK";
		else if (link_type == OLSR_ASYM_LINK)
			lt = "ASYM_LINK";
		else if (link_type == OLSR_SYM_LINK)
			lt = "SYM_LINK";
		else if (link_type == OLSR_LOST_LINK)
			lt = "LOST_LINK";
		else
			lt = "UNKNOWN (likely a bug!)";
		
		fprintf(out, "\tlink_code = %s - %s\n\tlink_msg_size = %d\n",
			nt,
			lt,
			msg.link_msg_size());
	
		for (int j = 0; j < msg.count; j++)
			fprintf(out, "\tnb_iface_addr = %d\n", msg.nb_iface_addr(j));
	}
}

///
/// \brief Prints a given %OLSR TC message into a given file.
/// \warning This function is actually not used.
/// \param out	File where the %OLSR TC message is going to be written.
/// \param tc	%OLSR TC message to be written.
///
void
OLSR_printer::print_olsr_tc(FILE* out, OLSR_tc& tc) {
	fprintf(out, "\tansn = %d\n\treserved = %d\n",
		tc.ansn(),
		tc.reserved());
	for (int i = 0; i < tc.count; i++)
		fprintf(out, "\taddr = %d\n", tc.nb_main_addr(i));
}

///
/// \brief Prints a given %OLSR MID message into a given file.
/// \warning This function is actually not used.
/// \param out	File where the %OLSR MID message is going to be written.
/// \param mid	%OLSR MID message to be written.
///
void
OLSR_printer::print_olsr_mid(FILE* out, OLSR_mid& mid) {
	for (int i = 0; i < mid.count; i++)
		fprintf(out, "\tiface = %d\n", mid.iface_addr(i));
}

/********** Repositories printing functions **********/

///
/// \brief Prints a given Link Set into a given trace file.
///
/// \param out		Trace where the Link Set is going to be written.
/// \param linkset	Link Set to be written.
///
void
OLSR_printer::print_linkset(Trace* out, linkset_t& linkset) {
	sprintf(out->pt_->buffer(), "P\tlocal\tnb\tsym\t\tasym\t\tlost\t\ttime");
	out->pt_->dump();
	for (linkset_t::iterator it = linkset.begin(); it != linkset.end(); it++) {
		OLSR_link_tuple* tuple = *it;
		sprintf(out->pt_->buffer(), "P\t%d\t%d\t%f\t%f\t%f\t%f",
			OLSR::node_id(tuple->local_iface_addr()),
			OLSR::node_id(tuple->nb_iface_addr()),
			tuple->sym_time(),
			tuple->asym_time(),
			tuple->lost_time(),
			tuple->time());
		out->pt_->dump();
	}
}

///
/// \brief Prints a given Neighbor Set into a given trace file.
///
/// \param out		Trace where the Neighbor Set is going to be written.
/// \param nbset	Neighbor Set to be written.
///
void
OLSR_printer::print_nbset(Trace* out, nbset_t& nbset) {
	sprintf(out->pt_->buffer(), "P\tnb\tstatus\twillingness");
	out->pt_->dump();
	for (nbset_t::iterator it = nbset.begin(); it != nbset.end(); it++) {
		OLSR_nb_tuple* tuple = *it;
		sprintf(out->pt_->buffer(), "P\t%d\t%d\t%d",
			OLSR::node_id(tuple->nb_main_addr()),
			tuple->status(),
			tuple->willingness());
		out->pt_->dump();
	}
}

///
/// \brief Prints a given 2-hop Neighbor Set into a given trace file.
///
/// \param out		Trace where the 2-hop Neighbor Set is going to be written.
/// \param nb2hopset	2-hop Neighbor Set to be written.
///
void
OLSR_printer::print_nb2hopset(Trace* out, nb2hopset_t& nb2hopset) {
	sprintf(out->pt_->buffer(), "P\tnb\tnb2hop\ttime");
	out->pt_->dump();
	for (nb2hopset_t::iterator it = nb2hopset.begin(); it != nb2hopset.end(); it++) {
		OLSR_nb2hop_tuple* tuple = *it;
		sprintf(out->pt_->buffer(), "P\t%d\t%d\t%f",
			OLSR::node_id(tuple->nb_main_addr()),
			OLSR::node_id(tuple->nb2hop_addr()),
			tuple->time());
		out->pt_->dump();
	}
}

///
/// \brief Prints a given MPR Set into a given trace file.
///
/// \param out		Trace where the MPR Set is going to be written.
/// \param mprset	MPR Set to be written.
///
void
OLSR_printer::print_mprset(Trace* out, mprset_t& mprset) {
	sprintf(out->pt_->buffer(), "P\tnb");
	out->pt_->dump();
	for (mprset_t::iterator it = mprset.begin(); it != mprset.end(); it++) {
		sprintf(out->pt_->buffer(), "P\t%d", OLSR::node_id(*it));
		out->pt_->dump();
	}
}

///
/// \brief Prints a given MPR Selector Set into a given trace file.
///
/// \param out		Trace where the MPR Selector Set is going to be written.
/// \param mprselset	MPR Selector Set to be written.
///
void
OLSR_printer::print_mprselset(Trace* out, mprselset_t& mprselset) {
	sprintf(out->pt_->buffer(), "P\tnb\ttime");
	out->pt_->dump();
	for (mprselset_t::iterator it = mprselset.begin(); it != mprselset.end(); it++) {
		OLSR_mprsel_tuple* mprsel_tuple = *it;
		sprintf(out->pt_->buffer(), "P\t%d\t%f",
			OLSR::node_id(mprsel_tuple->main_addr()),
			mprsel_tuple->time());
		out->pt_->dump();
	}
}

///
/// \brief Prints a given Topology Set into a given trace file.
///
/// \param out		Trace where the Topology Set is going to be written.
/// \param topologyset	Topology Set to be written.
///
void
OLSR_printer::print_topologyset(Trace* out, topologyset_t& topologyset) {
	sprintf(out->pt_->buffer(), "P\tdest\tlast\tseq\ttime");
	out->pt_->dump();
	for (topologyset_t::iterator it = topologyset.begin(); it != topologyset.end(); it++) {
		OLSR_topology_tuple* topology_tuple = *it;
		sprintf(out->pt_->buffer(), "P\t%d\t%d\t%d\t%f",
			OLSR::node_id(topology_tuple->dest_addr()),
			OLSR::node_id(topology_tuple->last_addr()),
			topology_tuple->seq(),
			topology_tuple->time());
		out->pt_->dump();
	}
}
