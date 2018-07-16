BEGIN {
	num_recv = 0
	
}

{
	# Trace line format: normal
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
	# Trace line format: new
	if ($2 == "-t") {
		event = $1
		time = $3
		node_id = $5
		flow_id = $39
		pkt_id = $41
		pkt_size = $37
		flow_t = $45
		level = $19
	}
		
	# Store packets send time
	if (level == "AGT" && flow_id == flow && node_id == src &&
	    sendTime[pkt_id] == 0 && (event == "+" || event == "s") && pkt_size >= pkt) {
		sendTime[pkt_id] = time
	}

	# Store packets arrival time
	if (level == "AGT" && flow_id == flow && node_id == dst &&
	    event == "r" && pkt_size >= pkt) {
		recvTime[pkt_id] = time
		num_recv ++
	}
}

END {
	# Compute average jitter
	jitter1 = jitter2 = jitter3 = jitter4 = tmp_recv = 0
	prev_time = delay = prev_delay = processed = currTime = 0
	prev_delay = -1
	for (i=0; processed<num_recv; i++) {
		if(recvTime[i] != 0) {
		        tmp_recv++
			if(prev_time != 0) {
				delay = recvTime[i] - prev_time
				e2eDelay = recvTime[i] - sendTime[i]
				if(delay < 0) delay = 0
				if(prev_delay != -1) {
					jitter1 += abs(e2eDelay - prev_e2eDelay)
					jitter2 += abs(delay-prev_delay)
					jitter3 += (abs(e2eDelay-prev_e2eDelay) - jitter3) / 16
					jitter4 += (abs(delay-prev_delay) - jitter4) / 16
				}
				currTime += delay
				if (currTime >= tic) {
					printf("%g %g\n",recvTime[i],jitter1*1000/tmp_recv)
					jitter1 = jitter2 = jitter3 = jitter4 = 0
					currTime = 0
					tmp_recv = 0
				}
				prev_delay = delay
				prev_e2eDelay = e2eDelay
			}
			prev_time = recvTime[i]
			processed++
		}
	}
}

END {
	printf("\n\n")
}

function abs(value) {
	if (value < 0) value = 0-value
	return value
}
