BEGIN{
prev_time=-1
}
{
if ($2 != "-t") {
	event = $1
	time = $2
	if (event == "+" || event == "-") node_id = $3
	if (event == "r" || event == "d") node_id = $4
	flow_id = $8
	pkt_id = $12
	pkt_size = $6
	flow_t = $5
	level = "AGT"
	}
# Store packets send time
if (level == "AGT" && flow_id == flow && node_id == src &&sendTime[pkt_id] == 0 && (event == "+" || event == "s") && pkt_size >= pkt) {
	if (time < startTime) {
		startTime = time
		}
	sendTime[pkt_id] = time
	this_flow = flow_t
	}

# Update total received packets' size and store packets arrival time
	if (level == "AGT" && flow_id == flow && node_id == dst &&event == "r" && pkt_size >= pkt) {
		num_recv++
		if (time > stopTime) {
			stopTime = time
			}
		# Store received packet's size
		recvdSize += pkt_size
		# Store packet's reception time
		recvTime[pkt_id] = time

	}
inst_delay=recvTime[pkt_id]-sendTime[pkt_id]
printf("%g   %g\n", time, inst_delay*1000)

}
END{

}
