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
/// \file	OLSR_rtable.cc
/// \brief	Implementation of our routing table.
///

#include <olsr/OLSR.h>
#include <olsr/OLSR_rtable.h>
#include <olsr/OLSR_repositories.h>

///
/// \brief Creates a new empty routing table.
///
OLSR_rtable::OLSR_rtable() {
}

///
/// \brief Destroys the routing table and all its entries.
///
OLSR_rtable::~OLSR_rtable() {
	// Iterates over the routing table deleting each OLSR_rt_entry*.
	for (rtable_t::iterator it = rt_.begin(); it != rt_.end(); it++)
		delete (*it).second;
}

///
/// \brief Clears the routing table and frees the memory assigned to each one of its entries.
///
void
OLSR_rtable::clear() {
	// Iterates over the routing table deleting each OLSR_rt_entry*.
	for (rtable_t::iterator it = rt_.begin(); it != rt_.end(); it++)
		delete (*it).second;

	// Cleans routing table.
	rt_.clear();
}

///
/// \brief Deletes the entry whose destination address is given.
/// \param dest	address of the destination node.
///
void
OLSR_rtable::rm_entry(nsaddr_t dest) {
	// Remove the pair whose key is dest
	rt_.erase(dest);
}

///
/// \brief Looks up an entry for the specified destination address.
/// \param dest	destination address.
/// \return	the routing table entry for that destination address, or NULL
///		if such an entry does not exist
///
OLSR_rt_entry*
OLSR_rtable::lookup(nsaddr_t dest) {
	// Get the iterator at "dest" position
	rtable_t::iterator it = rt_.find(dest);
	// If there is no route to "dest", return NULL
	if (it == rt_.end())
		return NULL;
	
	// Returns the rt entry (second element of the pair)
	return (*it).second;
}

///
/// \brief	Finds the appropiate entry which must be used in order to forward
///		a data packet to a next hop (given a destination).
///
/// Imagine a routing table like this: [A,B] [B,C] [C,C]; being each pair of the
/// form [dest addr,next-hop addr]. In this case, if this function is invoked with
/// [A,B] then pair [C,C] is returned because C is the next hop that must be used
/// to forward a data packet destined to A. That is, C is a neighbor of this node,
/// but B isn't. This function finds the appropiate neighbor for forwarding a packet.
///
/// \param entry	the routing table entry which indicates the destination node
///			we are interested in.
/// \return		the appropiate routing table entry which indicates the next
///			hop which must be used for forwarding a data packet, or NULL
///			if there is no such entry.
///
OLSR_rt_entry*
OLSR_rtable::find_send_entry(OLSR_rt_entry* entry) {
	OLSR_rt_entry* e = entry;
	while (e != NULL && e->dest_addr() != e->next_addr())
		e = lookup(e->next_addr());
	return e;
}

///
/// \brief Adds a new entry into the routing table.
///
/// If an entry for the given destination existed, it is deleted and freed.
///
/// \param dest		address of the destination node.
/// \param next		address of the next hop node.
/// \param iface	address of the local interface.
/// \param dist		distance to the destination node.
/// \return		the routing table entry which has been added.
///
OLSR_rt_entry*
OLSR_rtable::add_entry(nsaddr_t dest, nsaddr_t next, nsaddr_t iface, u_int32_t dist) {
	// Creates a new rt entry with specified values
	OLSR_rt_entry* entry = new OLSR_rt_entry();
	entry->dest_addr()	= dest;
	entry->next_addr()	= next;
	entry->iface_addr()	= iface;
	entry->dist()		= dist;
	
	// Inserts the new entry
	rtable_t::iterator it = rt_.find(dest);
	if (it != rt_.end())
		delete (*it).second;
	rt_[dest] = entry;
	
	// Returns the new rt entry
	return entry;
}

///
/// \brief Returns the number of entries in the routing table.
/// \return the number of entries in the routing table.
///
u_int32_t
OLSR_rtable::size() {
	return rt_.size();
}

///
/// \brief Prints out the content of the routing table to a given trace file.
///
/// Content is represented as a table in which each line is preceeded by a 'P'.
/// First line contains the name of every column (dest, next, iface, dist)
/// and the following ones are the values of each entry.
///
/// \param out the Trace where the routing table must be written into.
///
void
OLSR_rtable::print(Trace* out) {	
	sprintf(out->pt_->buffer(), "P\tdest\tnext\tiface\tdist");
	out->pt_->dump();
	for (rtable_t::iterator it = rt_.begin(); it != rt_.end(); it++) {
		OLSR_rt_entry* entry = (*it).second;
		sprintf(out->pt_->buffer(), "P\t%d\t%d\t%d\t%d",
			OLSR::node_id(entry->dest_addr()),
			OLSR::node_id(entry->next_addr()),
			OLSR::node_id(entry->iface_addr()),
			entry->dist());
		out->pt_->dump();
	}
}
