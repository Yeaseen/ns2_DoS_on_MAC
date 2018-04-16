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
/// \file	OLSR_repositories.h
/// \brief	Here are defined all data structures needed by an OLSR node.
///

#ifndef __OLSR_repositories_h__
#define __OLSR_repositories_h__

#include <set>
#include <vector>
#include <config.h>

/// An %OLSR's routing table entry.
typedef struct OLSR_rt_entry {
	nsaddr_t	dest_addr_;	///< Address of the destination node.
	nsaddr_t	next_addr_;	///< Address of the next hop.
	nsaddr_t	iface_addr_;	///< Address of the local interface.
	u_int32_t	dist_;		///< Distance in hops to the destination.
	
	inline nsaddr_t&	dest_addr()	{ return dest_addr_; }
	inline nsaddr_t&	next_addr()	{ return next_addr_; }
	inline nsaddr_t&	iface_addr()	{ return iface_addr_; }
	inline u_int32_t&	dist()		{ return dist_; }
} OLSR_rt_entry;

/// An Interface Association Tuple.
typedef struct OLSR_iface_assoc_tuple {
	/// Interface address of a node.
	nsaddr_t	iface_addr_;
	/// Main address of the node.
	nsaddr_t	main_addr_;
	/// Time at which this tuple expires and must be removed.
	double		time_;
	
	inline nsaddr_t&	iface_addr()	{ return iface_addr_; }
	inline nsaddr_t&	main_addr()	{ return main_addr_; }
	inline double&		time()		{ return time_; }
} OLSR_iface_assoc_tuple;

/// A Link Tuple.
typedef struct OLSR_link_tuple {
	/// Interface address of the local node.
	nsaddr_t	local_iface_addr_;
	/// Interface address of the neighbor node.
	nsaddr_t	nb_iface_addr_;
	/// The link is considered bidirectional until this time.
	double		sym_time_;
	/// The link is considered unidirectional until this time.
	double		asym_time_;
	/// The link is considered lost until this time (used for link layer notification).
	double		lost_time_;
	/// Time at which this tuple expires and must be removed.
	double		time_;
	
	inline nsaddr_t&	local_iface_addr()	{ return local_iface_addr_; }
	inline nsaddr_t&	nb_iface_addr()		{ return nb_iface_addr_; }
	inline double&		sym_time()		{ return sym_time_; }
	inline double&		asym_time()		{ return asym_time_; }
	inline double&		lost_time()		{ return lost_time_; }
	inline double&		time()			{ return time_; }
} OLSR_link_tuple;

/// A Neighbor Tuple.
typedef struct OLSR_nb_tuple {
	/// Main address of a neighbor node.
	nsaddr_t nb_main_addr_;
	/// Neighbor Type and Link Type at the four less significative digits.
	u_int8_t status_;
	/// A value between 0 and 7 specifying the node's willingness to carry traffic on behalf of other nodes.
	u_int8_t willingness_;
	
	inline nsaddr_t&	nb_main_addr()	{ return nb_main_addr_; }
	inline u_int8_t&	status()	{ return status_; }
	inline u_int8_t&	willingness()	{ return willingness_; }
} OLSR_nb_tuple;

/// A 2-hop Tuple.
typedef struct OLSR_nb2hop_tuple {
	/// Main address of a neighbor.
	nsaddr_t	nb_main_addr_;
	/// Main address of a 2-hop neighbor with a symmetric link to nb_main_addr.
	nsaddr_t	nb2hop_addr_;
	/// Time at which this tuple expires and must be removed.
	double		time_;
	
	inline nsaddr_t&	nb_main_addr()	{ return nb_main_addr_; }
	inline nsaddr_t&	nb2hop_addr()	{ return nb2hop_addr_; }
	inline double&		time()		{ return time_; }
} OLSR_nb2hop_tuple;

/// An MPR-Selector Tuple.
typedef struct OLSR_mprsel_tuple {
	/// Main address of a node which have selected this node as a MPR.
	nsaddr_t	main_addr_;
	/// Time at which this tuple expires and must be removed.
	double		time_;
	
	inline nsaddr_t&	main_addr()	{ return main_addr_; }
	inline double&		time()		{ return time_; }
} OLSR_mprsel_tuple;

/// The type "list of interface addresses"
typedef std::vector<nsaddr_t> addr_list_t;

/// A Duplicate Tuple
typedef struct OLSR_dup_tuple {
	/// Originator address of the message.
	nsaddr_t	addr_;
	/// Message sequence number.
	u_int16_t	seq_num_;
	/// Indicates whether the message has been retransmitted or not.
	bool		retransmitted_;
	/// List of interfaces which the message has been received on.
	addr_list_t	iface_list_;
	/// Time at which this tuple expires and must be removed.
	double		time_;
	
	inline nsaddr_t&	addr()		{ return addr_; }
	inline u_int16_t&	seq_num()	{ return seq_num_; }
	inline bool&		retransmitted()	{ return retransmitted_; }
	inline addr_list_t&	iface_list()	{ return iface_list_; }
	inline double&		time()		{ return time_; }
} OLSR_dup_tuple;

/// A Topology Tuple
typedef struct OLSR_topology_tuple {
	/// Main address of the destination.
	nsaddr_t	dest_addr_;
	/// Main address of a node which is a neighbor of the destination.
	nsaddr_t	last_addr_;
	/// Sequence number.
	u_int16_t	seq_;
	/// Time at which this tuple expires and must be removed.
	double		time_;
	
	inline nsaddr_t&	dest_addr()	{ return dest_addr_; }
	inline nsaddr_t&	last_addr()	{ return last_addr_; }
	inline u_int16_t&	seq()		{ return seq_; }
	inline double&		time()		{ return time_; }
} OLSR_topology_tuple;


typedef std::set<nsaddr_t>			mprset_t;	///< MPR Set type.
typedef std::vector<OLSR_mprsel_tuple*>		mprselset_t;	///< MPR Selector Set type.
typedef std::vector<OLSR_link_tuple*>		linkset_t;	///< Link Set type.
typedef std::vector<OLSR_nb_tuple*>		nbset_t;	///< Neighbor Set type.
typedef std::vector<OLSR_nb2hop_tuple*>		nb2hopset_t;	///< 2-hop Neighbor Set type.
typedef std::vector<OLSR_topology_tuple*>	topologyset_t;	///< Topology Set type.
typedef std::vector<OLSR_dup_tuple*>		dupset_t;	///< Duplicate Set type.
typedef std::vector<OLSR_iface_assoc_tuple*>	ifaceassocset_t;///< Interface Association Set type.

#endif
