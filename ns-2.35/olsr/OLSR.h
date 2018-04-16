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
/// \file	OLSR.h
/// \brief	Header file for OLSR agent and related classes.
///
/// Here are defined all timers used by OLSR, including those for managing internal
/// state and those for sending messages. Class OLSR is also defined, therefore this
/// file has signatures for the most important methods. Lots of constants are also
/// defined.
///

#ifndef __OLSR_h__
#define __OLSR_h__

#include <olsr/OLSR_pkt.h>
#include <olsr/OLSR_state.h>
#include <olsr/OLSR_rtable.h>
#include <olsr/OLSR_repositories.h>
#include <trace.h>
#include <classifier-port.h>
#include <agent.h>
#include <packet.h>
#include <timer-handler.h>
#include <random.h>
#include <vector>

/********** Useful macros **********/

/// Returns maximum of two numbers.
#ifndef MAX
#define	MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

/// Returns minimum of two numbers.
#ifndef MIN
#define	MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

/// Defines NULL as zero, used for pointers.
#ifndef NULL
#define NULL 0
#endif

/// Gets current time from the scheduler.
#define CURRENT_TIME	Scheduler::instance().clock()

///
/// \brief Gets the delay between a given time and the current time.
///
/// If given time is previous to the current one, then this macro returns
/// a number close to 0. This is used for scheduling events at a certain moment.
///
#define DELAY(time) (((time) < (CURRENT_TIME)) ? (0.000001) : \
	(time - CURRENT_TIME + 0.000001))

/// Scaling factor used in RFC 3626.
#define OLSR_C		0.0625


/********** Intervals **********/

/// HELLO messages emission interval.
#define OLSR_HELLO_INTERVAL	2

/// TC messages emission interval.
#define OLSR_TC_INTERVAL	5

/// MID messages emission interval.
#define OLSR_MID_INTERVAL	OLSR_TC_INTERVAL

///
/// \brief Period at which a node must cite every link and every neighbor.
///
/// We only use this value in order to define OLSR_NEIGHB_HOLD_TIME.
///
#define OLSR_REFRESH_INTERVAL	2


/********** Holding times **********/

/// Neighbor holding time.
#define OLSR_NEIGHB_HOLD_TIME	3*OLSR_REFRESH_INTERVAL
/// Top holding time.
#define OLSR_TOP_HOLD_TIME	3*OLSR_TC_INTERVAL
/// Dup holding time.
#define OLSR_DUP_HOLD_TIME	30
/// MID holding time.
#define OLSR_MID_HOLD_TIME	3*OLSR_MID_INTERVAL


/********** Link types **********/

/// Unspecified link type.
#define OLSR_UNSPEC_LINK	0
/// Asymmetric link type.
#define OLSR_ASYM_LINK		1
/// Symmetric link type.
#define OLSR_SYM_LINK		2
/// Lost link type.
#define OLSR_LOST_LINK		3

/********** Neighbor types **********/

/// Not neighbor type.
#define OLSR_NOT_NEIGH		0
/// Symmetric neighbor type.
#define OLSR_SYM_NEIGH		1
/// Asymmetric neighbor type.
#define OLSR_MPR_NEIGH		2


/********** Willingness **********/

/// Willingness for forwarding packets from other nodes: never.
#define OLSR_WILL_NEVER		0
/// Willingness for forwarding packets from other nodes: low.
#define OLSR_WILL_LOW		1
/// Willingness for forwarding packets from other nodes: medium.
#define OLSR_WILL_DEFAULT	3
/// Willingness for forwarding packets from other nodes: high.
#define OLSR_WILL_HIGH		6
/// Willingness for forwarding packets from other nodes: always.
#define OLSR_WILL_ALWAYS	7


/********** Miscellaneous constants **********/

/// Maximum allowed jitter.
#define OLSR_MAXJITTER		OLSR_HELLO_INTERVAL/4
/// Maximum allowed sequence number.
#define OLSR_MAX_SEQ_NUM	65535
/// Used to set status of an OLSR_nb_tuple as "not symmetric".
#define OLSR_STATUS_NOT_SYM	0
/// Used to set status of an OLSR_nb_tuple as "symmetric".
#define OLSR_STATUS_SYM		1
/// Random number between [0-OLSR_MAXJITTER] used to jitter OLSR packet transmission.
#define JITTER			(Random::uniform()*OLSR_MAXJITTER)


class OLSR;			// forward declaration


/********** Timers **********/


/// Timer for sending an enqued message.
class OLSR_MsgTimer : public TimerHandler {
public:
	OLSR_MsgTimer(OLSR* agent) : TimerHandler() {
		agent_	= agent;
	}
protected:
	OLSR*	agent_;			///< OLSR agent which created the timer.
	virtual void expire(Event* e);
};

/// Timer for sending HELLO messages.
class OLSR_HelloTimer : public TimerHandler {
public:
	OLSR_HelloTimer(OLSR* agent) : TimerHandler() { agent_ = agent; }
protected:
	OLSR*	agent_;			///< OLSR agent which created the timer.
	virtual void expire(Event* e);
};


/// Timer for sending TC messages.
class OLSR_TcTimer : public TimerHandler {
public:
	OLSR_TcTimer(OLSR* agent) : TimerHandler() { agent_ = agent; }
protected:
	OLSR*	agent_;			///< OLSR agent which created the timer.
	virtual void expire(Event* e);
};


/// Timer for sending MID messages.
class OLSR_MidTimer : public TimerHandler {
public:
	OLSR_MidTimer(OLSR* agent) : TimerHandler() { agent_ = agent; }
protected:
	OLSR*	agent_;			///< OLSR agent which created the timer.
	virtual void expire(Event* e);
};


/// Timer for removing duplicate tuples: OLSR_dup_tuple.
class OLSR_DupTupleTimer : public TimerHandler {
public:
	OLSR_DupTupleTimer(OLSR* agent, OLSR_dup_tuple* tuple) : TimerHandler() {
		agent_ = agent;
		tuple_ = tuple;
	}
protected:
	OLSR*		agent_;	///< OLSR agent which created the timer.
	OLSR_dup_tuple*	tuple_;	///< OLSR_dup_tuple which must be removed.
	
	virtual void expire(Event* e);
};


/// Timer for removing link tuples: OLSR_link_tuple.
class OLSR_LinkTupleTimer : public TimerHandler {
	///
	/// \brief A flag which tells if the timer has expired (at least) once or not.
	///
	/// When a link tuple has been just created, its sym_time is expired but this
	/// does not mean a neighbor loss. Thus, we use this flag in order to be able
	/// to distinguish this situation.
	///
	bool			first_time_;
public:
	OLSR_LinkTupleTimer(OLSR* agent, OLSR_link_tuple* tuple) : TimerHandler() {
		agent_		= agent;
		tuple_		= tuple;
		first_time_	= true;
	}
protected:
	OLSR*			agent_;	///< OLSR agent which created the timer.
	OLSR_link_tuple*	tuple_;	///< OLSR_link_tuple which must be removed.
	
	virtual void expire(Event* e);
};


/// Timer for removing nb2hop tuples: OLSR_nb2hop_tuple.
class OLSR_Nb2hopTupleTimer : public TimerHandler {
public:
	OLSR_Nb2hopTupleTimer(OLSR* agent, OLSR_nb2hop_tuple* tuple) : TimerHandler() {
		agent_ = agent;
		tuple_ = tuple;
	}
protected:
	OLSR*			agent_;	///< OLSR agent which created the timer.
	OLSR_nb2hop_tuple*	tuple_;	///< OLSR_nb2hop_tuple which must be removed.
	
	virtual void expire(Event* e);
};


/// Timer for removing MPR selector tuples: OLSR_mprsel_tuple.
class OLSR_MprSelTupleTimer : public TimerHandler {
public:
	OLSR_MprSelTupleTimer(OLSR* agent, OLSR_mprsel_tuple* tuple) : TimerHandler() {
		agent_ = agent;
		tuple_ = tuple;
	}
protected:
	OLSR*			agent_;	///< OLSR agent which created the timer.
	OLSR_mprsel_tuple*	tuple_;	///< OLSR_mprsel_tuple which must be removed.
	
	virtual void expire(Event* e);
};


/// Timer for removing topology tuples: OLSR_topology_tuple.
class OLSR_TopologyTupleTimer : public TimerHandler {
public:
	OLSR_TopologyTupleTimer(OLSR* agent, OLSR_topology_tuple* tuple) : TimerHandler() {
		agent_ = agent;
		tuple_ = tuple;
	}
protected:
	OLSR*			agent_;	///< OLSR agent which created the timer.
	OLSR_topology_tuple*	tuple_;	///< OLSR_topology_tuple which must be removed.
	
	virtual void expire(Event* e);
};


/// Timer for removing interface association tuples: OLSR_iface_assoc_tuple.
class OLSR_IfaceAssocTupleTimer : public TimerHandler {
public:
	OLSR_IfaceAssocTupleTimer(OLSR* agent, OLSR_iface_assoc_tuple* tuple) : TimerHandler() {
		agent_ = agent;
		tuple_ = tuple;
	}
protected:
	OLSR*			agent_;	///< OLSR agent which created the timer.
	OLSR_iface_assoc_tuple*	tuple_;	///< OLSR_iface_assoc_tuple which must be removed.
	
	virtual void expire(Event* e);
};


/********** OLSR Agent **********/


///
/// \brief Routing agent which implements %OLSR protocol following RFC 3626.
///
/// Interacts with TCL interface through command() method. It implements all
/// functionalities related to sending and receiving packets and managing
/// internal state.
///
class OLSR : public Agent {
	// Makes some friends.
	friend class OLSR_HelloTimer;
	friend class OLSR_TcTimer;
	friend class OLSR_MidTimer;
	friend class OLSR_DupTupleTimer;
	friend class OLSR_LinkTupleTimer;
	friend class OLSR_Nb2hopTupleTimer;
	friend class OLSR_MprSelTupleTimer;
	friend class OLSR_TopologyTupleTimer;
	friend class OLSR_IfaceAssocTupleTimer;
	friend class OLSR_MsgTimer;
	
	/// Address of the routing agent.
	nsaddr_t	ra_addr_;
	
	/// Packets sequence number counter.
	u_int16_t	pkt_seq_;
	/// Messages sequence number counter.
	u_int16_t	msg_seq_;
	/// Advertised Neighbor Set sequence number.
	u_int16_t	ansn_;
	
	/// HELLO messages' emission interval.
	int		hello_ival_;
	/// TC messages' emission interval.
	int		tc_ival_;
	/// MID messages' emission interval.
	int		mid_ival_;
	/// Willingness for forwarding packets on behalf of other nodes.
	int		willingness_;
	/// Determines if layer 2 notifications are enabled or not.
	int		use_mac_;
	
	/// Routing table.
	OLSR_rtable		rtable_;
	/// Internal state with all needed data structs.
	OLSR_state		state_;
	/// A list of pending messages which are buffered awaiting for being sent.
	std::vector<OLSR_msg>	msgs_;
	
protected:
	PortClassifier*	dmux_;		///< For passing packets up to agents.
	Trace*		logtarget_;	///< For logging.
	
	OLSR_HelloTimer	hello_timer_;	///< Timer for sending HELLO messages.
	OLSR_TcTimer	tc_timer_;	///< Timer for sending TC messages.
	OLSR_MidTimer	mid_timer_;	///< Timer for sending MID messages.
	
	/// Increments packet sequence number and returns the new value.
	inline u_int16_t	pkt_seq() {
		pkt_seq_ = (pkt_seq_ + 1)%(OLSR_MAX_SEQ_NUM + 1);
		return pkt_seq_;
	}
	/// Increments message sequence number and returns the new value.
	inline u_int16_t	msg_seq() {
		msg_seq_ = (msg_seq_ + 1)%(OLSR_MAX_SEQ_NUM + 1);
		return msg_seq_;
	}
	
	inline nsaddr_t&	ra_addr()	{ return ra_addr_; }
	
	inline int&		hello_ival()	{ return hello_ival_; }
	inline int&		tc_ival()	{ return tc_ival_; }
	inline int&		mid_ival()	{ return mid_ival_; }
	inline int&		willingness()	{ return willingness_; }
	inline int&		use_mac()	{ return use_mac_; }
	
	inline linkset_t&	linkset()	{ return state_.linkset(); }
	inline mprset_t&	mprset()	{ return state_.mprset(); }
	inline mprselset_t&	mprselset()	{ return state_.mprselset(); }
	inline nbset_t&		nbset()		{ return state_.nbset(); }
	inline nb2hopset_t&	nb2hopset()	{ return state_.nb2hopset(); }
	inline topologyset_t&	topologyset()	{ return state_.topologyset(); }
	inline dupset_t&	dupset()	{ return state_.dupset(); }
	inline ifaceassocset_t&	ifaceassocset()	{ return state_.ifaceassocset(); }
	
	void		recv_olsr(Packet*);
	
	void		mpr_computation();
	void		rtable_computation();
	
	void		process_hello(OLSR_msg&, nsaddr_t, nsaddr_t);
	void		process_tc(OLSR_msg&, nsaddr_t);
	void		process_mid(OLSR_msg&, nsaddr_t);
	
	void		forward_default(Packet*, OLSR_msg&, OLSR_dup_tuple*, nsaddr_t);
	void		forward_data(Packet*);
	
	void		enque_msg(OLSR_msg&, double);
	void		send_hello();
	void		send_tc();
	void		send_mid();
	void		send_pkt();
	
	void		link_sensing(OLSR_msg&, nsaddr_t, nsaddr_t);
	void		populate_nbset(OLSR_msg&);
	void		populate_nb2hopset(OLSR_msg&);
	void		populate_mprselset(OLSR_msg&);

	void		set_hello_timer();
	void		set_tc_timer();
	void		set_mid_timer();

	void		nb_loss(OLSR_link_tuple*);
	void		add_dup_tuple(OLSR_dup_tuple*);
	void		rm_dup_tuple(OLSR_dup_tuple*);
	void		add_link_tuple(OLSR_link_tuple*, u_int8_t);
	void		rm_link_tuple(OLSR_link_tuple*);
	void		updated_link_tuple(OLSR_link_tuple*);
	void		add_nb_tuple(OLSR_nb_tuple*);
	void		rm_nb_tuple(OLSR_nb_tuple*);
	void		add_nb2hop_tuple(OLSR_nb2hop_tuple*);
	void		rm_nb2hop_tuple(OLSR_nb2hop_tuple*);
	void		add_mprsel_tuple(OLSR_mprsel_tuple*);
	void		rm_mprsel_tuple(OLSR_mprsel_tuple*);
	void		add_topology_tuple(OLSR_topology_tuple*);
	void		rm_topology_tuple(OLSR_topology_tuple*);
	void		add_ifaceassoc_tuple(OLSR_iface_assoc_tuple*);
	void		rm_ifaceassoc_tuple(OLSR_iface_assoc_tuple*);
	
	nsaddr_t	get_main_addr(nsaddr_t);
	int		degree(OLSR_nb_tuple*);

	static bool	seq_num_bigger_than(u_int16_t, u_int16_t);

public:
	OLSR(nsaddr_t);
	int	command(int, const char*const*);
	void	recv(Packet*, Handler*);
	void	mac_failed(Packet*);
	
	static double		emf_to_seconds(u_int8_t);
	static u_int8_t		seconds_to_emf(double);
	static int		node_id(nsaddr_t);
};

#endif
